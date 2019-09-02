#include "acc_image.h"
#include "md.h"
#include "nblist.h"

// MAYBE_UNUSED static const int GRID_DIM = 32;
// MAYBE_UNUSED static const int GRID_DIM = 64;
MAYBE_UNUSED static const int GRID_DIM = 128;
// MAYBE_UNUSED static const int GRID_DIM = 256;
// MAYBE_UNUSED static const int BLOCK_DIM = 32;
MAYBE_UNUSED static const int BLOCK_DIM = 64;
// MAYBE_UNUSED static const int BLOCK_DIM = 128;

#if defined(TINKER_CUDART)
TINKER_NAMESPACE_BEGIN
#  define m_swap_(a, b)                                                        \
    {                                                                          \
      auto tmp = b;                                                            \
      b = a;                                                                   \
      a = tmp;                                                                 \
    }
#  define m_max_heap_(arr, start, end)                                         \
    {                                                                          \
      int dad = start;                                                         \
      int son = dad * 2 + 1;                                                   \
      while (son <= end) {                                                     \
        if (son + 1 <= end && arr[son] < arr[son + 1])                         \
          ++son;                                                               \
        if (arr[dad] > arr[son])                                               \
          return;                                                              \
        else {                                                                 \
          m_swap_(arr[dad], arr[son]);                                         \
          dad = son;                                                           \
          son = dad * 2 + 1;                                                   \
        }                                                                      \
      }                                                                        \
    }

#pragma acc routine seq
inline void sort_v1_(int* arr, int len) {
  // heapsort
  for (int i = len / 2 - 1; i >= 0; --i) {
    m_max_heap_(arr, i, len - 1);
  }

  for (int i = len - 1; i > 0; --i) {
    m_swap_(arr[0], arr[i]);
    m_max_heap_(arr, 0, i - 1);
  }
}
#  undef m_swap_
#  undef m_max_heap_
TINKER_NAMESPACE_END
#else
#  include <algorithm>
TINKER_NAMESPACE_BEGIN
inline void sort_v1_(int* arr, int len) { std::sort(arr, arr + len); }
TINKER_NAMESPACE_END
#endif

TINKER_NAMESPACE_BEGIN

//====================================================================//
// double loop

inline void build_double_loop_(NBListUnit nu) {
  auto* lst = nu.deviceptr();
  #pragma acc parallel loop independent deviceptr(lst)
  for (int i = 0; i < n; ++i) {
    lst->nlst[i] = n - i - 1;
    lst->lst[i] = i + 1;
  }
}

// inline void update_double_loop_() {}

//====================================================================//
// version 1
// see also nblist.f

inline void build_v1_(NBListUnit nu) {
  auto& st = *nu;
  const int maxnlst = st.maxnlst;
  const real buf2 = REAL_SQ(st.cutoff + st.buffer);

  const auto* __restrict__ lx = st.x;
  const auto* __restrict__ ly = st.y;
  const auto* __restrict__ lz = st.z;
  auto* __restrict__ xo = st.xold;
  auto* __restrict__ yo = st.yold;
  auto* __restrict__ zo = st.zold;
  auto* __restrict__ nlst = st.nlst;
  auto* __restrict__ lst = st.lst;

  #pragma acc parallel num_gangs(GRID_DIM) vector_length(BLOCK_DIM)\
              deviceptr(box,lx,ly,lz,xo,yo,zo,nlst,lst)
  #pragma acc loop gang independent
  for (int i = 0; i < n; ++i) {
    real xi = lx[i];
    real yi = ly[i];
    real zi = lz[i];
    xo[i] = xi;
    yo[i] = yi;
    zo[i] = zi;

    int ilst = 0;
    #pragma acc loop vector independent
    for (int k = i + 1; k < n; ++k) {
      real xr = xi - lx[k];
      real yr = yi - ly[k];
      real zr = zi - lz[k];
      image(xr, yr, zr, box);
      real r2 = xr * xr + yr * yr + zr * zr;
      if (r2 <= buf2) {
        int j;
        #pragma acc atomic capture
        {
          j = ilst;
          ilst += 1;
        }
        lst[i * maxnlst + j] = k;
      }
    }
    nlst[i] = ilst;
  }

#ifdef TINKER_CUDA_ALGO
  constexpr int warp_size = st.warp_size;
  int max_warps = (n + warp_size - 1) / warp_size;

  device_array::ptr<int>::type nwnbs, wnbs; // neighbors of the same warp tile
  device_array::ptr<int>::type nwty, wty;   // all ty in a warp tile
  device_array::allocate(&nwnbs, max_warps);
  device_array::allocate(&nwty, max_warps);
  device_array::allocate(&wnbs, max_warps * warp_size * maxnlst);
  device_array::allocate(&wty, max_warps * maxnlst);

  // copy all neighbors of atoms [i0, i0 + warp_size) and sort them
  #pragma acc parallel deviceptr(nlst,lst,nwnbs,wnbs)
  #pragma acc loop independent
  for (int tx = 0; tx < max_warps; ++tx) {
    int iwnbs = 0;
    #pragma acc loop seq
    for (int ilane = 0, i = tx * warp_size; ilane < warp_size && i < n;
         ++ilane, ++i) {
      for (int kk = 0; kk < nlst[i]; ++kk) {
        int k = lst[i * maxnlst + kk];
        wnbs[tx * warp_size * maxnlst + iwnbs] = k;
        ++iwnbs;
      }
    }
    sort_v1_(&wnbs[tx * warp_size * maxnlst], iwnbs);
    nwnbs[tx] = iwnbs;
  }

  #pragma acc parallel deviceptr(nwnbs,wnbs,nwty,wty)
  #pragma acc loop independent
  for (int tx = 0; tx < max_warps; ++tx) {
    int iwty = 0;
    #pragma acc loop seq
    for (int kk = 0; kk < nwnbs[tx]; ++kk) {
      int k = wnbs[tx * warp_size * maxnlst + kk];
      int ty = k / warp_size;
      if (iwty == 0 || wty[tx * maxnlst + iwty - 1] < ty) {
        wty[tx * maxnlst + iwty] = ty;
        ++iwty;
      }
    }
    nwty[tx] = iwty;
  }

  auto* __restrict__ itile = st.itile;
  int ntile = 0;
  #pragma acc serial deviceptr(nwty,wty,itile) copy(ntile)
  for (int tx = 0; tx < max_warps; ++tx) {
    int iwty = nwty[tx];
    for (int kk = 0; kk < iwty; ++kk) {
      int ty = wty[tx * maxnlst + kk];
      itile[ntile + kk][0] = tx;
      itile[ntile + kk][1] = ty;
    }
    ntile += iwty;
  }
  st.ntile = ntile;
  nu.init_deviceptr(st);

  device_array::deallocate(nwnbs, wnbs, nwty, wty);
#endif
}

inline void displace_v1_(NBListUnit nu) {
  auto& st = *nu;
  auto* lst = nu.deviceptr();
  const real lbuf2 = REAL_SQ(0.5f * st.buffer);
  #pragma acc parallel loop independent deviceptr(lst,box)
  for (int i = 0; i < n; ++i) {
    real xi = lst->x[i];
    real yi = lst->y[i];
    real zi = lst->z[i];
    real xr = xi - lst->xold[i];
    real yr = yi - lst->yold[i];
    real zr = zi - lst->zold[i];
    imagen(xr, yr, zr, box);
    lst->update[i] = 0;
    real r2 = xr * xr + yr * yr + zr * zr;
    if (r2 >= lbuf2) {
      lst->update[i] = 1;
      lst->xold[i] = xi;
      lst->yold[i] = yi;
      lst->zold[i] = zi;
    }
  }
}

inline void update_v1_(NBListUnit nu) {

  // test sites for displacement exceeding half the buffer

  displace_v1_(nu);

  // rebuild the higher numbered neighbors for updated sites

  auto& st = *nu;
  auto* lst = nu.deviceptr();
  const int maxnlst = st.maxnlst;
  const real buf2 = REAL_SQ(st.cutoff + st.buffer);
  const real bufx = REAL_SQ(st.cutoff + 2 * st.buffer);

  #pragma acc kernels deviceptr(lst,box)
  {
    #pragma acc loop independent
    for (int i = 0; i < n; ++i) {
      if (lst->update[i]) {
        real xi = lst->xold[i];
        real yi = lst->yold[i];
        real zi = lst->zold[i];
        lst->nlst[i] = 0;
        #pragma acc loop seq
        for (int k = i + 1; k < n; ++k) {
          real xr = xi - lst->xold[k];
          real yr = yi - lst->yold[k];
          real zr = zi - lst->zold[k];
          imagen(xr, yr, zr, box);
          real r2 = xr * xr + yr * yr + zr * zr;
          if (r2 <= buf2) {
            int current = lst->nlst[i];
            lst->lst[i * maxnlst + current] = k;
            lst->nlst[i] += 1;
          }
        }
      }
    }

    #pragma acc loop seq
    for (int i = 0; i < n; ++i) {
      if (lst->update[i]) {
        real xi = lst->xold[i];
        real yi = lst->yold[i];
        real zi = lst->zold[i];
        #pragma acc loop seq
        for (int k = 0; k < i - 1; ++k) {
          if (!lst->update[k]) {
            real xr = xi - lst->xold[k];
            real yr = yi - lst->yold[k];
            real zr = zi - lst->zold[k];
            imagen(xr, yr, zr, box);
            real r2 = xr * xr + yr * yr + zr * zr;
            if (r2 <= buf2) {
              for (int j = 0; j < lst->nlst[k]; ++j) {
                if (lst->lst[k * maxnlst + j] == i)
                  goto label_20;
              }
              lst->lst[k * maxnlst + lst->nlst[k]] = i;
              lst->nlst[k] += 1;
            label_20:;
            } else if (r2 <= bufx) {
              for (int j = 0; j < lst->nlst[k]; ++j) {
                if (lst->lst[k * maxnlst + j] == i) {
                  lst->nlst[k] -= 1;
                  lst->lst[k * maxnlst + j] =
                      lst->lst[k * maxnlst + lst->nlst[k]];
                  goto label_30;
                }
              }
            label_30:;
            }
          }
        }
        sort_v1_(&lst->lst[i * maxnlst], lst->nlst[i]);
      }
    }
  }
}

//====================================================================//

void nblist_build_acc_impl_(NBListUnit nu) {
  if (nu->maxnlst == 1) {
    build_double_loop_(nu);
  } else {
    build_v1_(nu);
  }
}

// #define TINKER_DEFAULT_NBLIST_UPDATE_(nu) build_v1_(nu)
#define TINKER_DEFAULT_NBLIST_UPDATE_(nu) update_v1_(nu)
void nblist_update_acc_impl_(NBListUnit nu) {
  if (nu->maxnlst == 1) {
    // update_double_loop_();
  } else {
    TINKER_DEFAULT_NBLIST_UPDATE_(nu);
  }
}
#undef TINKER_DEFAULT_NBLIST_UPDATE_
TINKER_NAMESPACE_END
