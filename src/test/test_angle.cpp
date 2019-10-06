#include "files.h"
#include "test.h"
#include "test_rt.h"

using namespace TINKER_NAMESPACE;

static const char* angleterm_only = R"**(
angleterm  only
)**";

static int usage = calc::xyz | calc::vmask;

static const double ref_g_angle_trpcage[][3] = {
   {-3.7764, 0.1402, -1.4677},    {4.3577, -15.9432, -6.3659},
   {-4.0588, 16.3061, 12.5717},   {-8.9235, -3.3856, -7.8413},
   {2.7129, 2.6169, 3.1148},      {3.3460, -0.5464, -0.0532},
   {0.8164, -0.0204, -1.9399},    {-1.7843, -0.9406, 0.5760},
   {-0.7626, 11.7508, -2.1019},   {-3.4659, -0.5135, 5.0874},
   {0.3831, -1.1630, -3.7437},    {-1.0774, -1.9238, -5.8700},
   {0.9678, -1.9050, 4.6786},     {-0.3516, -3.3363, -0.8679},
   {-0.1117, 2.2760, 5.4699},     {0.2613, -0.4470, -0.0859},
   {8.8155, -8.6811, -11.3522},   {-9.4715, 12.6502, 5.0815},
   {12.2300, -10.3798, -2.4741},  {-2.2025, 1.6991, -6.6856},
   {5.3198, 1.7288, 5.5562},      {0.0117, 0.3859, -0.9776},
   {5.9246, -8.5359, 6.3219},     {-8.7008, 10.4478, -2.9514},
   {-3.5115, -4.7639, 5.6742},    {1.8479, -5.4120, -8.2108},
   {-0.2596, 0.7536, -2.9346},    {-1.9385, -1.6327, -0.1518},
   {2.7075, 1.1457, -1.5297},     {0.9524, 0.5527, -2.7896},
   {2.9118, 0.8820, -1.6633},     {2.5696, 1.2637, -0.3830},
   {-1.1210, 0.3119, 2.4245},     {1.5519, 3.0548, 3.1337},
   {0.2153, 0.6873, 3.6844},      {-15.1151, 5.0771, 3.3535},
   {12.5700, -8.9394, -7.1714},   {-12.1815, 6.4387, 10.9571},
   {-3.1924, -1.3757, -3.6182},   {2.3820, -1.3127, 6.0485},
   {-0.7293, -0.5491, -0.1838},   {-1.0774, 7.1823, 3.5997},
   {-0.1657, -3.0350, -2.7181},   {1.8502, -0.8877, -1.7403},
   {-2.9269, 1.6677, 0.4285},     {-0.5156, 0.3875, -0.0357},
   {-0.2741, 0.4493, -1.7562},    {1.8028, -1.3843, 2.4729},
   {0.5923, -0.0115, -3.7632},    {-1.5986, -0.8394, -0.6650},
   {0.6841, -2.1587, -1.0176},    {-0.7395, 0.1663, 2.0603},
   {0.8856, -0.7268, 1.4212},     {-0.1100, 0.0846, -0.1678},
   {0.2570, -0.0659, -0.6942},    {-1.1295, 0.2243, 3.8322},
   {13.3057, -5.3644, -11.1923},  {-7.9607, 13.6633, 7.3336},
   {14.6470, -8.4745, -8.0195},   {-1.1707, 0.4404, -2.9578},
   {3.1874, 1.4080, 3.9112},      {-2.0195, -1.2186, -2.0370},
   {-1.9441, -6.6171, 12.7441},   {-7.6824, 3.7751, -8.3561},
   {6.6210, -3.3027, -6.8167},    {0.9457, -5.2277, 3.7445},
   {0.1580, 0.3303, -0.0021},     {2.2796, 1.7854, -0.6630},
   {2.5092, -0.1921, 0.0657},     {-0.8825, 2.9199, 0.9373},
   {-3.2022, 1.1508, 0.4294},     {-2.3039, 1.5689, -0.3011},
   {0.1031, 1.4296, -0.7740},     {2.3121, 1.4262, -0.7118},
   {1.2373, 0.9780, -0.0562},     {-17.6828, 2.1006, 7.1638},
   {14.3922, -9.2597, -7.9998},   {-14.6253, 6.7237, 9.7803},
   {-1.4571, -0.3936, -1.6663},   {1.8819, -0.8178, 4.2778},
   {-1.3263, -0.2390, -1.4154},   {4.5550, 9.4019, 5.0450},
   {-8.2565, -4.2460, -10.0485},  {9.1521, 3.0918, -2.0684},
   {-0.2890, -1.4066, -0.2684},   {-2.6935, -1.1843, 3.4706},
   {-0.2361, 0.0811, -0.3085},    {-0.3138, 1.8139, 0.7274},
   {-1.2857, -2.7956, 2.5933},    {-1.0682, -0.8006, 3.1003},
   {-0.8020, -3.4250, -0.7400},   {-0.6744, 0.8031, 1.4633},
   {12.7910, -1.2733, -12.5812},  {-12.0072, 5.3188, 10.9903},
   {15.6452, -3.3986, -9.0696},   {-1.4282, 0.5667, -2.7930},
   {3.3835, 1.5152, 3.8862},      {-0.5896, 0.2202, -1.4984},
   {4.5857, -7.2439, -1.2613},    {-4.9477, -5.0561, -17.2958},
   {12.6934, 8.8913, 12.9943},    {-18.3771, 2.1531, 10.1162},
   {-19.5064, -4.1702, -2.4443},  {28.4465, -3.3037, -12.9867},
   {3.4127, 4.6922, 7.5510},      {-2.5258, 4.8809, 8.2000},
   {6.1128, -0.6870, -2.8856},    {-7.0236, -3.8452, -4.3058},
   {-0.5578, 0.4180, -1.9131},    {-0.3332, 1.4111, 0.1808},
   {0.1338, 0.0107, 0.0310},      {-0.1052, -0.3848, -0.6064},
   {-0.2804, 0.9065, 1.5812},     {-0.0702, 0.2469, 0.4176},
   {0.1757, 0.1507, 0.0813},      {0.0733, -0.0491, 0.0005},
   {-16.3413, -0.2554, 9.6812},   {12.6699, -6.8112, -8.7081},
   {-15.7169, 5.1180, 9.3531},    {-0.5566, -0.1041, -0.5555},
   {1.7466, -0.6474, 2.9988},     {-2.1178, -0.1741, -2.1006},
   {3.2538, 7.1078, 10.6233},     {1.2107, -8.4711, -10.2952},
   {6.0744, 5.5895, 0.7493},      {-8.2974, 4.5187, 0.0455},
   {-2.5579, -0.3628, -2.2267},   {0.4844, 1.7618, -1.7704},
   {-1.9815, -1.4666, 1.4547},    {-2.7653, -1.0202, -0.3954},
   {-2.4883, -1.2249, 1.4225},    {-1.2515, -2.0065, 1.6233},
   {2.5810, -0.3447, -0.1688},    {2.5174, -2.9159, 1.8193},
   {2.8449, -0.3604, 1.3557},     {14.2374, -1.5802, -12.2429},
   {-12.2076, 11.0651, 8.1942},   {11.8623, -11.0201, -3.6720},
   {-2.4093, 1.2343, -5.0666},    {2.4595, 1.0758, 2.4747},
   {-1.5306, -0.2363, -2.6598},   {10.0866, -6.4920, 7.8354},
   {-10.9571, 5.9422, -0.1155},   {13.0751, 1.3486, -7.3746},
   {-11.6991, -0.7708, 1.7113},   {8.2555, 13.4225, -2.4218},
   {-1.6898, -1.2516, -2.4989},   {-0.5529, -1.5777, 0.1488},
   {1.1228, -0.4710, 0.3992},     {0.4909, -0.7219, -2.8567},
   {-0.7394, -1.1922, -1.2740},   {-1.5588, -0.2003, 1.6685},
   {0.9923, -0.7467, -0.2054},    {0.2760, 0.2798, 3.9151},
   {-1.0583, -3.5858, 3.5502},    {1.5307, -3.6043, -4.5116},
   {-1.6528, -1.8231, 2.9305},    {-11.0006, 0.7702, 8.4640},
   {13.2071, 7.7532, -11.8030},   {-12.6497, -27.6390, 8.1031},
   {0.2820, 0.8127, 1.3158},      {0.4638, -0.1950, 0.7965},
   {0.1182, -1.3045, -0.7790},    {-13.1899, 6.8382, 2.7789},
   {7.8686, -4.3545, -15.5756},   {-3.1270, -0.4801, 8.8937},
   {3.0731, -1.0299, 4.2003},     {0.8154, -1.5128, 0.7474},
   {1.5305, -1.6459, 2.2850},     {6.4734, 37.3178, -12.4937},
   {-3.7026, -12.0782, 10.2941},  {1.5500, -7.5359, -13.1613},
   {-1.6875, 2.3889, -2.1361},    {0.4452, -1.3821, -2.1787},
   {1.0659, 1.6628, 4.1779},      {0.5079, 0.1599, 1.6619},
   {-5.2337, 9.3415, 10.7930},    {9.0841, 9.1567, -7.2075},
   {-9.2100, -11.4195, 8.0021},   {1.8077, 1.0195, 2.4051},
   {3.0600, -5.5183, 5.1571},     {-3.8965, -4.4651, -3.9790},
   {0.6701, 0.1819, -0.1975},     {2.3672, 5.8930, -4.1028},
   {-6.6018, -11.5528, -3.8377},  {2.2314, 17.9034, 2.0532},
   {0.6799, -0.1853, 0.2090},     {-1.7612, 0.9014, 0.2577},
   {1.6257, 2.6743, -1.6382},     {-4.0601, -1.1891, 0.8334},
   {8.2619, 1.2671, 1.5406},      {0.7317, -2.5673, 2.3472},
   {0.9438, 0.0669, 0.2145},      {0.0418, 0.1925, 0.4914},
   {-0.5467, 3.3472, -0.5859},    {-0.8977, 0.4812, 1.6207},
   {0.5632, -0.9958, -1.2111},    {-4.9362, -17.3191, 1.1716},
   {6.5715, 15.8514, -6.1352},    {-4.7091, -20.2953, -1.0959},
   {0.4028, -0.0117, 0.4607},     {3.6221, -0.8336, 0.4808},
   {-2.1476, 0.0071, -0.7933},    {1.4252, -0.0506, 4.1795},
   {-2.7816, -0.3629, -3.4158},   {-1.4285, -0.4920, -1.4961},
   {-0.1000, -0.2448, 0.3784},    {1.4873, -1.2724, 0.8441},
   {0.9587, 21.5760, 3.0792},     {-0.3582, -16.8505, 2.1886},
   {-2.5158, 18.9808, 3.0902},    {1.3949, -0.3277, 0.4666},
   {2.0824, 0.0191, 1.0087},      {-1.5946, 0.0709, -1.0136},
   {4.7669, -1.1000, -3.9276},    {-5.0908, 1.1919, 1.8902},
   {-1.9041, 0.5485, 0.1069},     {0.6335, 1.1062, 0.8117},
   {1.7215, 1.0355, 0.4009},      {0.4728, -19.6702, -8.8448},
   {3.9853, 12.5013, 8.6048},     {-5.6379, -16.4063, -9.9325},
   {3.9233, -0.2985, 2.8373},     {1.7725, -0.3755, 0.6727},
   {-2.2113, -0.3015, -2.0390},   {-1.5965, 0.0678, -0.7656},
   {0.9915, 17.8486, 4.3329},     {-3.7686, -21.9498, -0.6717},
   {5.0224, 14.9342, 1.2681},     {-1.9198, 0.2224, 0.4288},
   {2.5844, -0.2292, 1.6954},     {-5.2025, 1.5233, 1.2692},
   {14.4161, 10.9087, -2.0823},   {-16.6595, -8.1462, 7.1652},
   {11.9678, 5.0525, -6.1403},    {-13.4171, 5.5035, 13.1741},
   {8.6419, -3.8175, -4.1843},    {-8.2109, 2.2167, -0.1779},
   {5.9038, -2.0894, -0.6146},    {-1.8043, 0.1520, -3.3962},
   {-1.0312, -1.3419, -0.1944},   {2.7160, 1.4453, -2.4928},
   {0.4291, 0.2710, 0.9398},      {2.7484, -0.9186, 1.7399},
   {-2.7201, -3.1615, -2.9880},   {-2.2087, -0.9265, -2.2636},
   {5.8689, -2.2236, -1.0712},    {2.1397, 0.5529, 2.0138},
   {-3.3960, 1.2916, 0.4582},     {-0.5612, 1.2738, 2.0472},
   {1.1989, -3.9460, -3.3209},    {-9.0662, 5.2976, 11.4574},
   {10.5184, -13.1820, -14.4523}, {-0.9018, 3.6069, -2.2287},
   {-0.1846, 5.5580, -3.7477},    {-1.4062, -1.0201, -3.7543},
   {-3.3043, 2.3840, 1.4868},     {2.5490, -6.7574, 0.3698},
   {1.4723, 0.0458, 4.0601},      {2.1060, -0.6489, -0.0972},
   {1.1915, 0.6036, -0.0555},     {1.0717, -0.1199, -3.3736},
   {1.6860, 2.9811, 0.2210},      {-0.8037, -0.3120, 0.1190},
   {-4.8875, -2.7738, 4.4206},    {10.5506, 15.1289, -2.1539},
   {-8.1794, -14.0810, 9.0152},   {-0.4559, 0.1113, -1.1937},
   {-1.4181, -0.6919, -5.3137},   {-3.1808, 0.0229, -2.4606},
   {0.8186, 2.1821, -2.6661},     {-1.4077, -1.3876, 6.3252},
   {4.0946, 0.2135, 1.5015},      {0.2134, -1.1680, 1.9431},
   {0.3286, -1.0529, -0.3441},    {-3.0270, -1.8147, -0.5583},
   {0.8190, -1.9143, -1.3561},    {0.2154, 1.2863, 0.9746},
   {0.5293, 2.0614, -2.1026},     {-2.2254, -11.3404, -8.5283},
   {8.8222, 14.8267, 6.4268},     {-10.6363, -1.1147, -0.9311},
   {-1.6318, -0.0294, 1.6172},    {-3.8396, 6.5155, 2.0931},
   {-2.1942, -2.3533, -3.9228},   {4.8792, 4.2252, 4.1574},
   {3.3735, -3.3986, 1.2671},     {1.7058, 0.6121, 0.2612},
   {0.1720, -0.0772, 1.6068},     {-1.4946, 2.0880, 0.7370},
   {-2.0542, -1.8341, 2.0385},    {1.5382, 0.1246, -1.0857},
   {-6.3464, -19.0409, -5.2728},  {-2.8373, 14.5406, -0.8630},
   {2.0486, -3.7756, -0.3413},    {-0.5960, -0.4023, -0.2732},
   {9.9328, 1.5360, 1.6128},      {-0.1190, 0.0754, 0.1621},
   {3.0840, 5.1936, 11.2318},     {6.2831, -8.3562, -7.9840},
   {-0.4297, -0.9594, -1.5695},   {1.8260, -0.4375, -1.0835},
   {-7.2834, 3.2321, 0.9319},     {-0.1681, 2.8839, 1.5875}};
TEST_CASE ("Angle-Trpcage", "[ff][eangle][trpcage]")
{
   const char* k = "test_trpcage.key";
   const char* x1 = "test_trpcage.xyz";
   const char* p = "amoebapro13.prm";

   std::string k0 = trpcage_key;
   k0 += angleterm_only;
   TestFile fke (k, k0);

   TestFile fx1 (x1, trpcage_xyz);
   TestFile fpr (p, commit_291a85c1::amoebapro13_prm);

   const char* argv[] = {"dummy", x1};
   int argc = 2;
   test_begin_with_args (argc, argv);
   rc_flag = usage;
   initialize ();

   const double eps_e = 0.0001;
   const double ref_e = 72.8274;
   const int ref_count = 565;
   const double eps_g = test_get_eps (0.001, 0.0001);
   const double eps_v = test_get_eps (0.003, 0.001);
   const double ref_v[][3] = {{-0.464, 37.932, 190.883},
                              {37.932, -71.780, -18.911},
                              {190.883, -18.911, 72.244}};

   COMPARE_BONDED_FORCE (eangle, nangle, ea_handle, ref_e, eps_e, ref_count, gx,
                         gy, gz, ref_g_angle_trpcage, eps_g, ref_v, eps_v);

   finish ();
   test_end ();
}
