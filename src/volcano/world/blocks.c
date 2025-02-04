#include "world/world.h"

extern SkyBox volcano_skybox;

extern WorldBlockGFX volcano_block0_gfx;
extern WorldBlockGFX volcano_block1_gfx;
extern WorldBlockGFX volcano_block2_gfx;
extern WorldBlockGFX volcano_block3_gfx;
extern WorldBlockGFX volcano_block4_gfx;
extern WorldBlockGFX volcano_block5_gfx;
extern WorldBlockGFX volcano_block6_gfx;

Vec3f volcano_smoke_spawner_path_points[] = {
    { 26.570312, -9.117575, 10.786863 },
    { 26.570312, 2.192231, 10.743836 },
    { 26.570312, 13.502036, 10.700809 },
    { 26.570312, 24.811842, 10.657783 },
};

f32 volcano_smoke_spawner_path_timestamps[] = {
    0.000000,
    1.000000,
};

FittingParams volcano_smoke_spawner_path_speedData[] = {
    { 0.000000, 0.000000, 0.000043, -0.000022, 127.913544 },
};

InterpData volcano_smoke_spawner_path = {
    HAL_INTERP_BEZIER,
    2,
    0,
    volcano_smoke_spawner_path_points,
    11.309887,
    volcano_smoke_spawner_path_timestamps,
    volcano_smoke_spawner_path_speedData
};

Vec3f volcano_rapidash_path1_points[] = {
    { -11.275351, -3.375000, 14.931845 },
    { -2.738348, -1.582031, 22.819855 },
    { 5.798654, 0.210938, 30.707865 },
    { 9.798126, 1.109375, 34.708885 },
    { 18.152578, 1.554688, 31.241335 },
    { 18.863596, 2.003906, 22.972561 },
    { 14.578974, 2.671875, 18.029030 },
    { 8.752455, 3.339844, 14.698301 },
    { 0.785582, 3.339844, 8.869528 },
    { -5.158001, 3.339844, -0.067636 },
    { -5.482355, 3.339844, -13.338756 },
    { 4.763733, 3.339844, -18.194044 },
    { 16.088356, 3.339844, -20.136160 },
    { 27.412979, 3.339844, -22.078276 },
};

f32 volcano_rapidash_path1_timestamps[] = {
    0.000000,
    0.108264,
    0.175515,
    0.248948,
    0.322116,
    0.388057,
    0.460389,
    0.555081,
    0.662737,
    0.777656,
    0.885085,
    1.000000,
};

FittingParams volcano_rapidash_path1_speedData[] = {
    { 9.124516, 0.000002, -71.001389, -0.000008, 138.315857 },
    { 23.024820, -26.823320, 71.541656, -105.504715, 76.438980 },
    { 37.821396, -72.381233, 1.855537, 49.207951, 38.677422 },
    { 18.274109, -59.254395, 45.884281, -12.939106, 55.181057 },
    { 3.727162, -22.995789, 41.080803, -25.837297, 47.145939 },
    { 4.425850, -5.707226, 24.565048, 2.245590, 43.120819 },
    { 4.538657, -7.833181, -14.429534, 51.957405, 68.650085 },
    { 3.608197, 11.085161, -2.206482, 17.753416, 102.883423 },
    { 46.767895, -27.435005, -106.733482, 61.028725, 133.123718 },
    { 30.093887, -146.643097, 185.344070, -47.671669, 106.751862 },
    { 2.412452, -9.649809, 7.918426, 3.462763, 127.875076 },
};

InterpData volcano_rapidash_path1 = {
    HAL_INTERP_BEZIER,
    12,
    0,
    volcano_rapidash_path1_points,
    99.33809,
    volcano_rapidash_path1_timestamps,
    volcano_rapidash_path1_speedData
};

Vec3f volcano_rapidash_path2_points[] = {
    { 14.154409, -0.882812, 37.187485 },
    { 15.724806, 0.683594, 26.340075 },
    { 17.295202, 2.250000, 15.492662 },
    { 10.200638, 2.445312, 9.817454 },
    { 8.146284, 2.640625, 4.043466 },
    { -0.853740, 3.035156, 1.205404 },
    { -5.255925, 3.820312, -0.751880 },
    { -11.321159, 4.269531, -7.602375 },
    { -6.138896, 4.937500, -19.892223 },
    { 6.424281, 5.273438, -26.411892 },
    { 17.047106, 5.437256, -28.123287 },
    { 18.215345, 5.605469, -22.706944 },
    { 19.383585, 5.773682, -17.290600 },
};

f32 volcano_rapidash_path2_timestamps[] = {
    0.000000,
    0.116911,
    0.211292,
    0.288144,
    0.377905,
    0.449859,
    0.544523,
    0.678666,
    0.828225,
    0.938594,
    1.000000,
};

FittingParams volcano_rapidash_path2_speedData[] = {
    { 25.928291, -0.000010, -71.860161, 0.000041, 122.586105 },
    { 54.373737, -147.897278, 110.611290, -40.007149, 76.654274 },
    { 38.227619, -60.711136, 62.908833, -44.981518, 53.734848 },
    { 34.378220, -86.173050, 2.004505, 51.613213, 49.178650 },
    { 18.266384, -34.155960, 77.438919, -65.384010, 51.001556 },
    { 41.822041, -18.984806, -38.000462, 60.091473, 47.166882 },
    { 35.226871, -104.582520, 50.262871, 94.424278, 92.095116 },
    { 21.959200, -74.402908, 14.304886, 22.109932, 167.426605 },
    { 15.468693, 25.702497, -69.700165, -84.652214, 151.397705 },
    { 35.048466, -140.193848, 182.728973, -85.070244, 38.216518 },
};

InterpData volcano_rapidash_path2 = {
    HAL_INTERP_BEZIER,
    11,
    0,
    volcano_rapidash_path2_points,
    86.92006,
    volcano_rapidash_path2_timestamps,
    volcano_rapidash_path2_speedData
};

Vec3f volcano_rapidash_path3_points[] = {
    { -9.690323, -3.375000, 10.567410 },
    { -3.457331, -1.582031, 19.481787 },
    { 2.775661, 0.210938, 28.396164 },
    { 9.622644, 1.109375, 36.247311 },
    { 18.973976, 1.554688, 32.067440 },
    { 19.674084, 2.003906, 25.208771 },
    { 13.872190, 2.671875, 16.574720 },
    { 8.174212, 3.339844, 13.870787 },
    { 2.476232, 3.339844, 7.878285 },
    { 0.357752, 3.339844, 5.539748 },
    { -1.398007, 3.339844, 1.796920 },
    { -5.937281, 3.339844, -3.708968 },
    { -5.051979, 3.339844, -12.645925 },
    { 10.884015, 3.339844, -22.307499 },
    { 26.820009, 3.339844, -31.969074 },
};

f32 volcano_rapidash_path3_timestamps[] = {
    0.000000,
    0.109233,
    0.205115,
    0.288094,
    0.359822,
    0.448051,
    0.519577,
    0.589989,
    0.631508,
    0.675878,
    0.742286,
    0.834721,
    1.000000,
};

FittingParams volcano_rapidash_path3_speedData[] = {
    { 0.576907, 0.000000, -7.254888, 0.000006, 121.531052 },
    { 31.015152, 12.427073, -76.675789, -12.202147, 114.853058 },
    { 53.029922, -140.661240, 78.346069, -4.211842, 69.417336 },
    { 1.370371, -21.012648, 87.930939, -57.383732, 55.920246 },
    { 25.765167, -56.679714, -31.186890, 60.921680, 66.825172 },
    { 21.360279, -54.678669, 87.589050, -68.430565, 65.645416 },
    { 15.364446, -23.277172, -39.099625, 28.152639, 51.485512 },
    { 8.983406, -29.997150, 59.806450, -58.420334, 32.625809 },
    { 2.506878, -0.637389, 9.289132, 7.134742, 12.998182 },
    { 17.538746, -19.906536, -7.265285, 33.828354, 31.291550 },
    { 24.996750, 42.931580, 4.067051, 29.733152, 55.486816 },
    { 56.762131, -227.048462, 93.723892, 266.648987, 157.215363 },
};

InterpData volcano_rapidash_path3 = {
    HAL_INTERP_BEZIER,
    13,
    0,
    volcano_rapidash_path3_points,
    99.9589,
    volcano_rapidash_path3_timestamps,
    volcano_rapidash_path3_speedData
};

Vec3f volcano_vulpix_path1_points[] = {
    { 12.083276, 0.449219, 37.877117 },
    { -34.292488, 0.449219, 21.257080 },
};

f32 volcano_vulpix_path1_timestamps[] = {
    0.000000,
    1.000000,
};

InterpData volcano_vulpix_path1 = {
    HAL_INTERP_LINEAR,
    2,
    0,
    volcano_vulpix_path1_points,
    49.263954,
    volcano_vulpix_path1_timestamps,
    NULL
};

Vec3f volcano_charmander_path_points[] = {
    { -48.974190, 0.000000, 33.725616 },
    { -73.852776, 0.000000, 33.123596 },
};

f32 volcano_charmander_path_timestamps[] = {
    0.000000,
    1.000000,
};

InterpData volcano_charmander_path = {
    HAL_INTERP_LINEAR,
    2,
    0,
    volcano_charmander_path_points,
    24.885868,
    volcano_charmander_path_timestamps,
    NULL
};

Vec3f volcano_vulpix_path2_points[] = {
    { -4.278325, 2.171875, 16.094757 },
    { -32.001377, 2.172269, 17.779566 },
};

f32 volcano_vulpix_path2_timestamps[] = {
    0.000000,
    1.000000,
};

InterpData volcano_vulpix_path2 = {
    HAL_INTERP_LINEAR,
    2,
    0,
    volcano_vulpix_path2_points,
    27.7742,
    volcano_vulpix_path2_timestamps,
    NULL
};

Vec3f volcano_vulpix_path3_points[] = {
    { -0.313998, 1.578125, 23.235800 },
    { -35.087563, 1.578125, 18.267374 },
};

f32 volcano_vulpix_path3_timestamps[] = {
    0.000000,
    1.000000,
};

InterpData volcano_vulpix_path3 = {
    HAL_INTERP_LINEAR,
    2,
    0,
    volcano_vulpix_path3_points,
    35.126713,
    volcano_vulpix_path3_timestamps,
    NULL
};

Vec3f volcano_moltres_path_points[] = {
    { -56.841537, -9.473475, 13.161154 },
    { -65.412956, -2.234375, 5.118658 },
    { -73.984375, 5.004725, -2.923837 },
    { -82.558594, 6.324929, -8.906443 },
    { -76.683594, 8.173213, -2.132022 },
    { -75.438118, 13.718750, -0.325507 },
    { -77.534218, 15.855469, 11.158135 },
    { -103.558594, 17.767563, 19.852798 },
    { -104.269730, 17.511719, 30.150768 },
    { -99.324219, 11.388056, 37.963593 },
    { -95.371094, 5.679582, 30.064022 },
    { -86.855469, 9.474386, 13.063178 },
    { -75.960938, 14.761468, 9.313456 },
    { -69.449219, 13.605528, -20.316500 },
    { -56.675781, 16.840895, -27.769026 },
    { -37.886719, 19.372921, -32.549892 },
    { -19.097656, 21.904947, -37.330757 },
};

f32 volcano_moltres_path_timestamps[] = {
    0.000000,
    0.071020,
    0.113779,
    0.142894,
    0.177608,
    0.240811,
    0.350346,
    0.411393,
    0.459641,
    0.513404,
    0.599425,
    0.679850,
    0.805309,
    0.898636,
    1.000000,
};

FittingParams volcano_moltres_path_speedData[] = {
    { 9.819123, 0.000000, -59.390144, -0.000004, 190.555542 },
    { 91.213280, -16.164539, -132.041428, -79.503799, 140.984497 },
    { 172.053818, -500.116699, 395.552643, -27.227173, 4.488017 },
    { 66.658012, -104.990662, 100.931198, -48.256874, 44.750557 },
    { 147.338776, -62.698380, 54.581375, 105.265549, 59.092224 },
    { 611.948975, -1190.078125, -71.575806, 615.688171, 303.579498 },
    { 104.196548, -496.106140, 868.906372, -649.902283, 269.562744 },
    { 64.661491, -41.607140, -81.291603, 16.378193, 96.657227 },
    { 39.289272, -105.615875, 218.699493, -12.380460, 54.798176 },
    { 142.143417, -289.528656, -90.278152, 265.328003, 194.790604 },
    { 409.988220, -546.461548, 487.820435, -215.240662, 222.455200 },
    { 635.056763, -1360.202148, 43.453369, 760.968506, 358.561707 },
    { 101.622765, -456.501038, 904.322388, -692.504517, 437.837921 },
    { 10.955049, -43.820190, -32.743973, 153.128281, 294.777527 },
};

InterpData volcano_moltres_path = {
    HAL_INTERP_BEZIER,
    15,
    0,
    volcano_moltres_path_points,
    184.86777,
    volcano_moltres_path_timestamps,
    volcano_moltres_path_speedData
};

Vec3f volcano_charmeleon_path_points[] = {
    { -87.531021, 5.142990, 27.282108 },
    { -85.449486, 5.142990, 29.052105 },
    { -85.189293, 5.142990, 31.134449 },
    { -85.501526, 5.142990, 33.216793 },
    { -86.906563, 5.142990, 35.090908 },
    { -88.884026, 5.142990, 36.027966 },
    { -93.307300, 5.142990, 34.882671 },
    { -93.284302, 5.142990, 30.279819 },
    { -90.601295, 5.142990, 28.167103 },
    { -87.531021, 5.142990, 27.282108 },
    { -85.449486, 5.142990, 29.052105 },
    { -85.189293, 5.142990, 31.134449 },
};

f32 volcano_charmeleon_path_timestamps[] = {
    0.000000,
    0.084205,
    0.167915,
    0.256107,
    0.354526,
    0.500344,
    0.650739,
    0.784063,
    0.902156,
    1.000000,
};

FittingParams volcano_charmeleon_path_speedData[] = {
    { 0.414338, -2.372266, 4.275527, -3.061821, 5.081055 },
    { 0.078538, 0.297878, -0.092393, 0.029789, 4.336834 },
    { 0.200496, -0.416908, -0.651025, 1.052790, 4.650646 },
    { 1.205315, 2.145579, 2.764296, -0.698000, 4.835999 },
    { 12.347972, -13.993032, -11.595818, 16.088591, 10.253189 },
    { 9.641439, -28.506657, 18.559891, 0.309748, 13.100904 },
    { 1.689762, -9.189102, 14.440321, -9.524679, 13.105326 },
    { 0.982623, 1.219406, -4.440269, -1.452295, 10.521627 },
    { 1.545302, -5.396471, 4.845253, -2.744123, 6.831093 },
};

InterpData volcano_charmeleon_path = {
    HAL_INTERP_BEZIER,
    10,
    0,
    volcano_charmeleon_path_points,
    25.088354,
    volcano_charmeleon_path_timestamps,
    volcano_charmeleon_path_speedData
};

ObjectSpawn volcano_block0_spawn[] = {
    { PokemonID_SMOKE_SPAWNER, 0, { 26.5703, 2.19223, 10.7438 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_smoke_spawner_path },
    { PokemonID_RAPIDASH, 0, { 34.2705, 0.169739, 2.61987 }, { 0.0, 0.046, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_RAPIDASH, 0, { 19.1184, -0.345276, 4.54273 }, { 0.0, -0.038, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { -1, 0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, NULL }
};

ObjectSpawn volcano_block1_spawn[] = {
    { PokemonID_LAVA_SPLASH, 3, { 6.31607, 2.06901, -2.76967 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_LAVA_SPLASH, 3, { 5.20564, 2.06901, -4.77762 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_LAVA_SPLASH, 3, { 7.36789, 2.06901, -4.93974 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_RAPIDASH, 1, { -22.7383, -1.58203, 7.81985 }, { 0.0, 1.004, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_rapidash_path1 },
    { PokemonID_RAPIDASH, 1, { -4.27519, 0.683594, 11.3401 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_rapidash_path2 },
    { PokemonID_RAPIDASH, 1, { -23.4573, -1.58203, 4.48179 }, { 0.0, 1.004, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_rapidash_path3 },
    { -1, 0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, NULL }
};

ObjectSpawn volcano_block2_spawn[] = {
    { PokemonID_VULPIX, 1, { 2.08328, 0.449219, 2.87712 }, { 0.0, 2.794, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_vulpix_path1 },
    { -1, 0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, NULL }
};

ObjectSpawn volcano_block3_spawn[] = {
    { PokemonID_MAGMAR, 1, { -19.9109, 4.07471, 15.3729 }, { 0.0, -8.95802, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMANDER, 5, { -23.9742, 0.0, 13.7256 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_charmander_path },
    { PokemonID_VULPIX, 1, { 20.7217, 2.17188, -3.90524 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_vulpix_path2 },
    { PokemonID_VULPIX, 1, { 24.686, 1.57812, 3.2358 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_vulpix_path3 },
    { PokemonID_VOLCANO_SMOKE, 0, { -20.0, 25.0, -35.0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { -1, 0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, NULL }
};

ObjectSpawn volcano_block4_spawn[] = {
    { PokemonID_MAGMAR, 0, { -10.4394, 4.07471, -9.71333 }, { 0.0, -5.80801, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_MAGMAR, 0, { -6.34335, 3.71422, -12.5297 }, { 0.0, -5.606, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_MOLTRES_EGG, 0, { 4.74962, 1.5, 1.84203 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_MOLTRES, 0, { 4.58704, -2.23438, -4.88134 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_moltres_path },
    { PokemonID_CHARMANDER, 1, { 1.84956, 1.15484, 7.33554 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMANDER, 2, { -4.35545, 1.52299, 0.678629 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMANDER, 3, { -10.0561, 6.64637, 17.2347 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMANDER, 3, { -12.1999, 6.64637, 17.0698 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMANDER, 4, { -10.8806, 6.64637, 14.7611 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMANDER, 4, { -12.794, 6.64637, 14.7611 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { -1, 0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, NULL }
};

ObjectSpawn volcano_block5_spawn[] = {
    { PokemonID_GATE, 0, { 19.7892, 8.24216, 24.4197 }, { 0.0, -3.21359, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_GROWLITHE_SPAWNER, 0, { -10.3015, 2.61608, -6.84823 }, { 0.0, 1.3, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_GROWLITHE_SPAWNER, 0, { -5.66958, 2.83189, -20.8215 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_GROWLITHE_SPAWNER, 0, { -11.0348, 2.81804, -13.4768 }, { 0.0, 0.528, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { PokemonID_CHARMELEON, 0, { 14.247, 5.14299, -0.895836 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, &volcano_charmeleon_path },
    { PokemonID_EVOLUTION_CONTROLLER, 0, { 10.8459, 3.85002, 1.70108 }, { 0.0, -1.9, 0.0 }, { 1.0, 1.0, 1.0 }, NULL },
    { -1, 0, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, NULL }
};

StaticObject volcano_block5_staticObjects[] = {
    { PokemonID_GATE, { 19.789238, 8.242157, 24.419727 }, { 0.0, -3.213589, 0.0 }, { 1.0, 1.0, 1.0 } },
    { -1, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } }
};

WorldBlockDescriptor volcano_block0 = {
    &volcano_block0_gfx,
    { 0.0, 0.0, 0.0 },
    0.0,
    false,
    NULL,
    volcano_block0_spawn,
    NULL,
};

WorldBlockDescriptor volcano_block1 = {
    &volcano_block1_gfx,
    { 20.0, 0.0, 15.0 },
    0.0,
    false,
    NULL,
    volcano_block1_spawn,
    NULL,
};

WorldBlockDescriptor volcano_block2 = {
    &volcano_block2_gfx,
    { 10.0, 0.0, 35.0 },
    0.0,
    false,
    NULL,
    volcano_block2_spawn,
    NULL,
};

WorldBlockDescriptor volcano_block3 = {
    &volcano_block3_gfx,
    { -25.0, 0.0, 20.0 },
    0.0,
    false,
    NULL,
    volcano_block3_spawn,
    NULL,
};

WorldBlockDescriptor volcano_block4 = {
    &volcano_block4_gfx,
    { -70.0, 0.0, 10.0 },
    0.0,
    false,
    NULL,
    volcano_block4_spawn,
    NULL,
};

WorldBlockDescriptor volcano_block5 = {
    &volcano_block5_gfx,
    { -100.0, 0.0, 30.0 },
    0.0,
    false,
    NULL,
    volcano_block5_spawn,
    volcano_block5_staticObjects,
};

WorldBlockDescriptor volcano_block6 = {
    &volcano_block6_gfx,
    { -45.0, 0.0, -10.0 },
    0.0,
    false,
    NULL,
    NULL,
    NULL,
};

WorldBlockDescriptor* volcano_modelBlocks[] = {
    &volcano_block0,
    &volcano_block1,
    &volcano_block2,
    &volcano_block3,
    &volcano_block4,
    &volcano_block5,
    NULL,
};

WorldBlockDescriptor* volcano_uvScrollBlocks[] = {
    &volcano_block6,
    NULL,
};

WorldBlockSetup volcano_blocksSetup = {
    volcano_modelBlocks,
    volcano_uvScrollBlocks,
    &volcano_skybox,
};
