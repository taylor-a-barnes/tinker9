#pragma once
#include "macro.h"


namespace tinker {
namespace gpu {
TINKER_EXTERN int q0;
TINKER_EXTERN int q1;
TINKER_EXTERN int qpme;
}
namespace g = gpu;
TINKER_EXTERN bool use_pme_stream;
}
