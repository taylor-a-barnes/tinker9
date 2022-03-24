#include "mod/elecpchg.h"
#include "test.h"
#include "testrt.h"

using namespace tinker;

TEST_CASE("PartialCharge-Trpcage", "[ff][echarge][ewald][nonewald][trpcage]")
{
   rc_flag = calc::xyz | calc::vmask;

   const char* kname = "test_pchg.key";
   std::string k0 = "CHARGETERM ONLY\n";
   const char* xname = "test_pchg.xyz";

   const double eps_e = 0.0001;
   const double eps_g = 0.0001;
   const double eps_v = 0.001;
   const char* argv[] = {"dummy", xname};
   int argc = 2;

   SECTION("  - ec -- no pbc, no cutoff, non-ewald, taper")
   {
      TestFile fxy(TINKER9_DIRSTR "/src/test/file/trpcage/trp_charmm.xyz", xname);
      TestFile fke(TINKER9_DIRSTR "/src/test/file/trpcage/trp_charmm.key", kname, k0);
      TestFile fpr(TINKER9_DIRSTR "/src/test/file/commit_11e84c69/charmm19.prm");

      const double ref_e = -769.0436;
      const double ref_v[][3] = {
         {303.611, -2.245, 26.499}, {-2.245, 283.515, 15.988}, {26.499, 15.988, 181.918}};
      const int ref_count = 10981;
      const double ref_g[][3] = {

         {-6.4279, -2.6636, -0.1138}, {4.8265, -0.0786, -0.8306}, {-0.5366, -0.2937, 7.2453},
         {-3.6253, -5.1800, -10.0274}, {2.9238, 1.0779, -0.5114}, {2.5454, 0.3447, -2.9386},
         {5.0265, 4.4962, -0.2644}, {0.0000, 0.0000, 0.0000}, {8.1101, 6.2277, 11.2244},
         {-12.8708, -11.1896, -1.8769}, {-8.3554, -1.0171, -6.8423}, {3.5008, 0.4725, 1.8222},
         {1.9419, 1.2708, -0.5312}, {7.6220, 1.5904, -8.1158}, {-0.8293, -0.7061, 1.1479},
         {-5.2358, -0.3469, -0.8156}, {8.2055, 6.4708, -6.6939}, {-6.2864, -0.9883, 4.7035},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {9.2727, 2.8723, 2.8082}, {-1.3143, -1.0917, 0.0435},
         {-2.0164, 0.8414, 1.8553}, {2.4090, 5.6702, 8.7642}, {-7.3943, -0.5631, -0.6113},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {-0.7269, -0.8203, -0.6299}, {0.8616, 1.7762, 1.2453}, {-0.2037, -1.3542, -0.8094},
         {0.7188, -3.9843, 1.9971}, {-0.3432, -0.2439, -0.7434}, {-3.9999, -2.1085, 0.4165},
         {-6.4885, 0.2237, 2.3965}, {-4.6859, 5.1749, 1.2863}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0565, -0.5239, -2.8780}, {0.4727, 0.1388, -0.0472}, {-4.8393, 1.1412, -5.6594},
         {7.3240, -5.3388, -5.3866}, {0.2232, -1.1785, -0.1125}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {-1.5884, -7.1477, -0.4596}, {0.1086, 11.7717, 6.8557},
         {-0.3100, 3.5516, -2.5031}, {-1.0361, 1.2206, 0.8726}, {-1.4192, 0.7814, -0.3770},
         {4.0993, 1.6785, -0.8630}, {0.1758, -0.1921, 0.9284}, {2.9339, 0.9500, 2.7841},
         {4.6315, 4.9816, -7.0889}, {-1.8281, -1.4931, 2.2339}, {0.0000, 0.0000, 0.0000},
         {0.0565, -0.3426, -0.0518}, {-0.5589, 0.0798, 0.0709}, {-0.2063, 0.2779, 0.0349},
         {4.7166, -0.7235, -1.8397}, {0.2972, -0.0917, -0.2640}, {0.0227, -0.0305, 0.1308},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {-8.7536, 1.6696, 0.3441}, {-0.8154, 2.2109, 3.0495}, {-0.0442, -1.2877, 0.3060},
         {1.1498, -2.0714, 4.1046}, {-4.8726, 0.9427, 8.4983}, {1.3015, -3.6945, -2.1791},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {-3.9635, -0.1312, -0.3559}, {-0.2799, -0.6847, -0.7132},
         {-7.5172, -2.4123, -1.6676}, {1.3678, -2.8127, -0.3364}, {3.1582, -3.1539, 0.2860},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {-4.4787, -0.9334, -0.8269}, {1.8855, 2.3708, 2.4020}, {-2.8815, -1.9580, -2.0139},
         {-1.9101, -7.3754, -2.4310}, {-1.6006, -0.7209, -4.5325}, {3.6773, 1.2911, -8.3075},
         {1.3354, -0.2456, 0.7053}, {10.0090, 0.6499, -0.4110}, {-14.2286, -9.4404, 2.9877},
         {-2.6741, 1.0165, 3.1755}, {0.0547, -0.7288, 1.4185}, {-1.8686, 1.6261, -2.8387},
         {6.5499, 0.5197, 4.6656}, {-3.7349, -3.1707, 2.3303}, {-2.9505, -2.9123, 1.7481},
         {1.2299, 0.6531, 1.3285}, {3.4242, -1.1392, 6.0439}, {-6.9009, -9.1866, -0.4076},
         {3.6133, 0.4048, -2.4884}, {-0.9822, 2.3871, -1.9808}, {0.8461, 0.8109, 0.5105},
         {1.1212, 3.3914, -3.1059}, {-6.2856, 6.9164, 0.1813}, {-0.8246, -1.2252, 2.1069},
         {-0.1915, 0.8884, 1.0338}, {1.0492, -0.4090, -0.0922}, {7.8321, 3.0621, -1.9837},
         {2.5450, 4.9519, 6.6188}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {-0.1088, 0.4829, 0.6343}, {-4.3940, -3.8247, 2.3274}, {0.6073, -0.7230, -0.2212},
         {2.5501, -3.9518, 2.3019}, {-1.2724, -5.4263, 1.3710}, {2.9934, 5.1797, -0.0901},
         {0.1276, 0.5936, 2.0010}, {0.8924, -5.6468, -3.7423}, {-0.3405, 1.2967, 0.5004},
         {-3.3642, -0.3251, 0.7246}, {0.3073, 1.0945, -1.7268}, {-1.9353, 2.9274, -3.5721},
         {-4.6739, -8.2919, 1.5550}, {2.0285, -0.0297, -0.2782}, {1.0935, 3.1026, -1.3981},
         {-5.4441, -10.3120, 8.3558}, {17.3005, 13.7145, -11.5859}, {2.4100, -2.8242, -0.7771},
         {0.2475, 1.1331, -0.1834}, {3.9566, 2.9133, -1.6750}, {3.9955, -2.2457, -1.4903},
         {0.1098, 0.8271, 0.3450}, {-1.0017, 1.6302, 1.3598}, {-0.9227, 0.7842, 0.2745},
         {-0.2680, -6.6256, 2.1382}, {4.6284, 0.3668, 0.0704}, {0.6743, -2.6521, -0.6317},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {1.2360, 1.2232, -1.6867},
         {-5.0858, -7.4676, 5.9540}, {2.4954, 7.2601, -4.5299}, {0.2441, 1.3798, 1.8088},
         {-3.5409, -8.7325, 5.0448}, {5.7958, 5.5862, -6.4407}, {-0.2970, -0.3232, -0.6513},
         {-0.7921, -5.6264, -1.3412}, {3.8782, 6.1407, -2.6779}, {2.2469, 8.7753, -3.5060},
         {-0.7000, 2.1916, -0.0801}, {-0.3512, 0.3278, 0.1391}, {0.9804, -2.3150, -3.0192},
         {0.6637, 1.9817, 1.5391}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.8429, 0.1315, -0.4831}, {-0.5834, 1.5354, 0.2094}, {-0.7459, 0.3796, 0.5623},
         {1.8486, -0.7686, 2.8920}, {0.1212, 2.3203, 0.2081}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.4486, -0.6991, 1.0195}, {-0.5256, 1.4621, 0.1606},
         {-0.9216, -0.4703, -0.3217}, {-4.4296, -13.7448, 1.3884}, {-0.6412, 12.1345, -1.3906},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {-0.9236, -1.5982, -0.3806},
         {-0.8604, 4.4898, -2.8181}, {0.4564, 0.6070, 0.3996}, {0.1529, 0.0661, -0.4518},
         {-0.0634, 5.0636, 1.3903}, {2.6545, -4.4121, -2.9500}, {3.0941, -0.3088, -0.0172},
         {-7.3074, 3.9036, 0.7184}, {4.7207, -0.7016, 2.1016}, {1.3439, 1.5807, 2.4695}};

      testBeginWithArgs(argc, argv);
      initialize();

      energy(calc::v0);
      COMPARE_REALS(esum, ref_e, eps_e);

      energy(calc::v1);
      COMPARE_REALS(esum, ref_e, eps_e);
      COMPARE_GRADIENT(ref_g, eps_g);
      for (int i = 0; i < 3; ++i)
         for (int j = 0; j < 3; ++j)
            COMPARE_REALS(vir[i * 3 + j], ref_v[i][j], eps_v);

      energy(calc::v3);
      COMPARE_REALS(esum, ref_e, eps_e);
      COMPARE_INTS(count_reduce(nec), ref_count);

      energy(calc::v4);
      COMPARE_REALS(esum, ref_e, eps_e);
      COMPARE_GRADIENT(ref_g, eps_g);

      energy(calc::v5);
      COMPARE_GRADIENT(ref_g, eps_g);

      energy(calc::v6);
      COMPARE_GRADIENT(ref_g, eps_g);
      for (int i = 0; i < 3; ++i)
         for (int j = 0; j < 3; ++j)
            COMPARE_REALS(vir[i * 3 + j], ref_v[i][j], eps_v);

      finish();
      testEnd();
   }

   SECTION("  - ec -- pbc, cutoff, ewald")
   {
      std::string k1 = k0 +
         "EWALD\n"
         "NEIGHBOR-LIST\n"
         "CUTOFF           9.0\n"
         "LIST-BUFFER      0.5\n"
         "A-AXIS            25\n"
         "B-AXIS            30\n"
         "C-AXIS            20\n";

      TestFile fxy(TINKER9_DIRSTR "/src/test/file/trpcage/trp_charmm.xyz", xname);
      TestFile fke(TINKER9_DIRSTR "/src/test/file/trpcage/trp_charmm.key", kname, k1);
      TestFile fpr(TINKER9_DIRSTR "/src/test/file/commit_11e84c69/charmm19.prm");

      const double ref_e = -784.1710;
      const double ref_v[][3] = {
         {308.950, -10.859, 21.517}, {-10.859, 307.362, 18.690}, {21.517, 18.690, 167.439}};
      const int ref_count = 5039;
      const double ref_g[][3] = {{-6.3995, -2.5833, -0.4125}, {4.8138, -0.0886, -0.6376},
         {-0.4733, -0.3038, 7.6132}, {-3.7105, -5.2366, -10.4420}, {2.9116, 0.9762, -0.0820},
         {2.5393, 0.2261, -2.6482}, {4.9295, 4.3764, 0.1128}, {0.0000, 0.0000, 0.0000},
         {7.9166, 6.3644, 11.5815}, {-12.5198, -11.3178, -2.5188}, {-8.0233, -1.2761, -7.0385},
         {3.3719, 0.5245, 1.7613}, {1.5926, 1.5385, -0.2954}, {7.5590, 1.6277, -8.3062},
         {-0.7940, -0.7155, 1.2007}, {-4.9722, -0.3685, -0.3767}, {7.8776, 6.4253, -7.1049},
         {-6.2501, -1.0322, 4.8204}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {9.1237, 2.9152, 2.4268},
         {-1.2671, -1.1015, 0.1807}, {-1.8125, 0.8945, 2.6889}, {2.1684, 5.5381, 7.8823},
         {-7.3062, -0.6177, -0.3326}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {-0.5846, -0.9480, -0.1569}, {0.3568, 2.1530, 0.0337},
         {0.2321, -1.6145, -0.1575}, {0.6402, -4.0318, 1.4799}, {-0.3288, -0.2108, -0.5910},
         {-3.8690, -1.8491, 1.0620}, {-6.6213, -0.1331, 1.7188}, {-4.6376, 5.1875, 1.6221},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {-0.0511, -0.6701, -3.1683}, {0.5121, 0.1900, 0.0070},
         {-4.4823, 1.4666, -5.3065}, {6.9142, -5.7499, -5.6787}, {0.2923, -1.1038, 0.0840},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {-1.5637, -6.8193, -0.6247},
         {0.0976, 11.3942, 6.9627}, {-0.2747, 3.2293, -2.1280}, {-1.0628, 1.3523, 0.6487},
         {-1.4490, 0.9570, -0.6072}, {3.8208, 1.5057, -1.1500}, {0.2855, -0.1338, 1.0165},
         {3.4808, 1.3800, 3.3817}, {3.9844, 4.4358, -7.6436}, {-1.6525, -1.4045, 2.4543},
         {0.0000, 0.0000, 0.0000}, {0.0075, -0.3554, -0.0803}, {-0.4333, 0.1108, 0.1165},
         {-0.0452, 0.3195, 0.1511}, {3.8550, -0.9420, -2.1175}, {0.2142, -0.1123, -0.3052},
         {-0.0148, -0.0403, 0.0865}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {-7.9068, 1.8916, 0.5282}, {-1.0578, 1.9530, 2.5683},
         {0.0066, -1.1928, 0.4695}, {1.3910, -1.4207, 4.9049}, {-5.1371, 0.1549, 7.7066},
         {1.4474, -3.5537, -1.8195}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {-4.0835, -0.5189, -0.7965},
         {-0.2576, -0.5587, -0.6093}, {-7.2808, -1.6207, -1.2389}, {1.1893, -3.7299, -0.6938},
         {3.2413, -2.9208, 0.5973}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {-4.4284, -0.6934, -0.8147}, {1.8955, 2.0950, 2.4551},
         {-2.9244, -1.6344, -2.0763}, {-1.9404, -7.0835, -2.4914}, {-1.6184, -0.3879, -4.6515},
         {3.4175, 0.8141, -8.5550}, {1.4410, -0.0972, 0.7599}, {10.7054, 1.5758, -0.0515},
         {-15.0805, -10.4150, 2.7023}, {-2.4804, 1.3168, 3.3689}, {-0.1563, -0.9410, 1.3428},
         {-1.4301, 2.0798, -2.7313}, {6.0244, -0.1864, 4.5232}, {-4.6297, -3.9630, 2.1922},
         {-3.3431, -3.5105, 1.4298}, {1.3562, 0.8398, 1.4307}, {4.1985, -0.1834, 6.6826},
         {-7.7565, -10.2163, -1.0849}, {3.8325, 0.8058, -2.2282}, {-1.4705, 1.8695, -2.4027},
         {1.0036, 0.9403, 0.6444}, {2.2299, 4.1667, -2.4451}, {-7.5370, 6.0983, -0.3211},
         {-0.5059, -0.8801, 2.3900}, {-0.6146, 0.5961, 0.7436}, {1.2896, -0.2464, 0.0368},
         {9.1679, 4.1583, -1.4404}, {1.0512, 3.7415, 6.2578}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {0.0883, 0.6264, 0.8320}, {-5.1512, -4.5436, 1.9619},
         {0.8223, -0.5077, -0.1401}, {3.8039, -2.8057, 2.6117}, {-2.5083, -6.5460, 1.1178},
         {3.4904, 5.6774, 0.2184}, {0.6150, 1.1295, 2.2331}, {-0.3188, -7.1464, -4.4932},
         {0.4050, 2.2333, 0.9317}, {-4.2253, -1.0363, 0.5454}, {0.5630, 1.2925, -1.6899},
         {-0.2761, 4.1152, -3.4465}, {-6.3940, -9.4469, 1.4356}, {2.6575, 0.4637, -0.1292},
         {1.6660, 3.5426, -1.2804}, {-6.8394, -11.4315, 8.1088}, {18.0634, 14.4005, -11.4197},
         {1.2513, -3.6800, -0.8143}, {0.6414, 1.4201, -0.1941}, {6.4236, 4.4419, -1.7383},
         {1.1339, -4.0587, -1.4444}, {0.8867, 1.4394, 0.3845}, {-2.4835, 0.8507, 1.3614},
         {-0.4655, 0.9618, 0.2780}, {2.1198, -5.9372, 2.1574}, {2.6309, -0.2625, -0.0906},
         {1.6112, -2.1380, -0.6004}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {1.4974, 1.3409, -1.6476}, {-6.1073, -8.0325, 5.8321}, {3.8686, 8.0199, -4.4192},
         {-1.1480, 0.7414, 1.7233}, {-4.7097, -9.4909, 4.9567}, {6.4894, 6.0295, -6.3395},
         {0.8391, 0.1031, -0.5582}, {0.3237, -5.0984, -1.3056}, {4.7073, 6.7249, -2.5960},
         {3.1942, 9.4073, -3.4511}, {-1.6957, 2.0285, -0.0429}, {0.0992, 0.3493, 0.1318},
         {2.9244, -2.3350, -2.8422}, {-1.1640, 1.9673, 1.3126}, {0.0000, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}, {1.4881, 0.1910, -0.5277}, {-1.1867, 1.5882, 0.1095},
         {-0.5110, 0.3450, 0.6354}, {3.0902, -1.0725, 3.2449}, {-1.2584, 2.7590, -0.0643},
         {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000}, {0.7556, -0.7434, 1.0678},
         {-0.8954, 1.5556, 0.0158}, {-0.7570, -0.5264, -0.2553}, {-3.6869, -14.1074, 1.7782},
         {-1.2395, 12.4818, -1.8606}, {0.0000, 0.0000, 0.0000}, {0.0000, 0.0000, 0.0000},
         {-0.7676, -1.6245, -0.2954}, {-1.3451, 4.7630, -3.0331}, {0.5659, 0.5278, 0.4619},
         {0.2571, -0.0193, -0.3632}, {-0.4900, 5.3899, 1.0783}, {3.0613, -4.6257, -2.8146},
         {3.4054, -0.5440, 0.1161}, {-8.3359, 4.6215, 0.4214}, {5.3244, -1.1341, 2.2995},
         {1.0532, 1.8806, 2.0643}};

      testBeginWithArgs(argc, argv);
      initialize();

      energy(calc::v0);
      COMPARE_REALS(esum, ref_e, eps_e);

      energy(calc::v1);
      COMPARE_REALS(esum, ref_e, eps_e);
      COMPARE_GRADIENT(ref_g, eps_g);
      for (int i = 0; i < 3; ++i)
         for (int j = 0; j < 3; ++j)
            COMPARE_REALS(vir[i * 3 + j], ref_v[i][j], eps_v);

      energy(calc::v3);
      COMPARE_REALS(esum, ref_e, eps_e);
      COMPARE_INTS(count_reduce(nec), ref_count);

      energy(calc::v4);
      COMPARE_REALS(esum, ref_e, eps_e);
      COMPARE_GRADIENT(ref_g, eps_g);

      energy(calc::v5);
      COMPARE_GRADIENT(ref_g, eps_g);

      energy(calc::v6);
      COMPARE_GRADIENT(ref_g, eps_g);
      for (int i = 0; i < 3; ++i)
         for (int j = 0; j < 3; ++j)
            COMPARE_REALS(vir[i * 3 + j], ref_v[i][j], eps_v);

      finish();
      testEnd();
   }
}
