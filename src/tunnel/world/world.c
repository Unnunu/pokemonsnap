#include <common.h>
#include "world/world.h"

extern WorldBlockSetup tunnel_blocksSetup[];
extern Gfx computer_draw[];
extern Gfx pkmn_1016_draw[];
extern Gfx pkmn_1017_draw[];

UnkVioletMarlin D_8011CB60[] = {
    { PokemonID_COMPUTER, func_800E30B0_60860, computer_draw },
    { PokemonID_1016, func_800E30B0_60860, pkmn_1016_draw },
    { PokemonID_1017, func_800E30B0_60860, pkmn_1017_draw },
    { -1, NULL, NULL }
};

#define ZERO_MATRIX \
{ \
    { 0.0, 0.0, 0.0, 0.0 }, \
    { 0.0, 0.0, 0.0, 0.0 }, \
    { 0.0, 0.0, 0.0, 0.0 }, \
    { 0.0, 0.0, 0.0, 0.0 }, \
}

#define DEFAULT_HITBOX_TYPE_1(name) HitBoxType1 name = { 1, ZERO_MATRIX, ZERO_MATRIX, 1.0f }
#define DEFAULT_HITBOX_TYPE_3(name) HitBoxType3 name = { 3, ZERO_MATRIX, ZERO_MATRIX, 1.0f, 1.0f }
#define DEFAULT_HITBOX_TYPE_4(name) HitBoxType4 name = { 4, ZERO_MATRIX, ZERO_MATRIX, 0.5f, 0.5f, 0.5f }

DEFAULT_HITBOX_TYPE_3(D_8011CF54_hitbox_0);
DEFAULT_HITBOX_TYPE_1(D_8011CF54_hitbox_1);
DEFAULT_HITBOX_TYPE_1(D_8011CF54_hitbox_2);
DEFAULT_HITBOX_TYPE_1(D_8011CF54_hitbox_3);
DEFAULT_HITBOX_TYPE_3(D_8011CF54_hitbox_4);
DEFAULT_HITBOX_TYPE_3(D_8011CF54_hitbox_5);
DEFAULT_HITBOX_TYPE_1(D_8011CF54_hitbox_6);

Collider D_8011CF54[] = {
    {
        0,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        3,
        (HitBox*)&D_8011CF54_hitbox_0,
        { 0.0, -0.404, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 2.491113, 1.418955, 2.491113 },
    },
    {
        3,
        (HitBox*)&D_8011CF54_hitbox_1,
        { 0.0, -1.179999, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.447898, 3.49149, 1.447898 },
    },
    {
        3,
        (HitBox*)&D_8011CF54_hitbox_2,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 2.0, 2.0, 2.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        3,
        (HitBox*)&D_8011CF54_hitbox_3,
        { -2.2, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.84098, 2.159545, 2.382806 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        3,
        (HitBox*)&D_8011CF54_hitbox_4,
        { 0.0, -4.56, -2.279999 },
        { -0.07, 0.0, 0.0 },
        { 0.2937, 6.309075, 0.2848 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        3,
        (HitBox*)&D_8011CF54_hitbox_5,
        { 1.38, -3.859999, 0.82 },
        { 0.11, 0.0, -0.18 },
        { 0.344401, 7.713232, 0.344401 },
    },
    {
        2,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        1,
        (HitBox*)&D_8011CF54_hitbox_6,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 4.0, 4.0, 4.0 },
    },
    {
        18,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
    },
};

DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_0);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_1);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_2);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_3);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_4);
DEFAULT_HITBOX_TYPE_3(D_8011DBF8_hitbox_5);
DEFAULT_HITBOX_TYPE_3(D_8011DBF8_hitbox_6);
DEFAULT_HITBOX_TYPE_1(D_8011DBF8_hitbox_7);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_8);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_9);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_10);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_11);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_12);
DEFAULT_HITBOX_TYPE_4(D_8011DBF8_hitbox_13);
DEFAULT_HITBOX_TYPE_1(D_8011DBF8_hitbox_14);

Collider D_8011DBF8[] = {
    {
        0,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_0,
        { 0.0, 0.518986, 0.0 },
        { 0.47, 0.0, 0.0 },
        { 1.16688, 0.599151, 0.969181 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_1,
        { 0.0, 0.288326, -0.010485 },
        { 0.0, 0.0, 0.0 },
        { 1.1648, 0.591203, 1.113209 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_2,
        { 0.0, 0.508502, -0.418407 },
        { 0.0, 0.0, 0.0 },
        { 1.1837, 1.02, 0.28012 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_3,
        { 0.0, 3.5081, -12.1697 },
        { 0.0, 0.0, 0.0 },
        { 6.9444, 7.0148, 0.89 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_4,
        { 0.7287, 0.4374, -4.7663 },
        { 0.0, 0.0, 0.0 },
        { 0.49, 0.88, 0.49 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_5,
        { 0.029551, 0.253557, -3.042824 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 0.497178, 0.73 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_6,
        { 0.070922, 0.854085, -3.044008 },
        { 0.0, 0.0, 0.0 },
        { 0.5712, 0.684001, 0.5712 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_7,
        { 0.01773, 1.199083, -3.053416 },
        { 0.0, 0.0, 0.0 },
        { 0.572001, 0.576, 0.572001 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_8,
        { 0.0, 0.9693, -3.0516 },
        { 0.0, 0.0, 0.0 },
        { 0.78, 1.0605, 0.79 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_9,
        { 0.358, 1.3019, -5.755 },
        { -0.145862, 0.813669, -0.116419 },
        { 0.4166, 2.6734, 0.3226 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_10,
        { 2.3855, 1.0974, -0.4196 },
        { 0.0, 0.0, 0.0 },
        { 0.4998, 2.1644, 0.192 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_11,
        { 2.8792, 0.2364, -3.8809 },
        { 0.0, 0.0, 0.0 },
        { 0.6586, 0.4606, 0.68 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_12,
        { 3.7554, 1.2519, -6.4244 },
        { -0.419999, 0.0, 0.0 },
        { 0.3654, 2.8589, 0.22 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011DBF8_hitbox_13,
        { -1.9025, 0.3014, -7.3397 },
        { 0.0, 0.164, 0.0 },
        { 0.72, 0.594, 0.72 },
    },
    {
        1,
        (HitBox*)&D_8011DBF8_hitbox_14,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 14.0, 14.0, 14.0 },
    },
    {
        18,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
    },
};

DEFAULT_HITBOX_TYPE_4(computer_hitbox_0);
DEFAULT_HITBOX_TYPE_1(computer_hitbox_1);

Collider computer_collision_model[] = {
    {
        0,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&computer_hitbox_0,
        { 0.0, 1.4, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 2.0, 2.8, 1.5 },
    },
    {
        1,
        (HitBox*)&computer_hitbox_1,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.4, 1.4, 1.4 },
    },
    {
        18,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
    },
};

DEFAULT_HITBOX_TYPE_4(D_8011E3A8_hitbox_0);
DEFAULT_HITBOX_TYPE_1(D_8011E3A8_hitbox_1);

Collider D_8011E3A8[] = {
    {
        0,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011E3A8_hitbox_0,
        { 0.0, 2.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 2.0, 4.0, 2.0 },
    },
    {
        1,
        (HitBox*)&D_8011E3A8_hitbox_1,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 2.0, 2.0, 2.0 },
    },
    {
        18,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
    },
};

DEFAULT_HITBOX_TYPE_4(D_8011E59C_hitbox_0);
DEFAULT_HITBOX_TYPE_1(D_8011E59C_hitbox_1);

Collider D_8011E59C[] = {
    {
        0,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        1,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
    },
    {
        2,
        (HitBox*)&D_8011E59C_hitbox_0,
        { 0.0, 0.699999988079071, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.2, 1.4, 0.2 },
    },
    {
        1,
        (HitBox*)&D_8011E59C_hitbox_1,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.4, 0.4, 0.4 },
    },
    {
        18,
        NULL,
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
    },
};

CollisionModel tunnel_CollisionModels[] = {
    { PokemonID_GATE, D_800EDF78, 1.0 },
    { PokemonID_1008, D_8011CF54, 1.0 },
    { PokemonID_1010, D_8011DBF8, 1.0 },
    { PokemonID_COMPUTER, computer_collision_model, -1.0 },
    { PokemonID_1016, D_8011E3A8, -1.0 },
    { PokemonID_1017, D_8011E59C, -1.0 },
    { -1, NULL, 0 }
};

WorldSetup tunnel_WorldSetup = {
    tunnel_blocksSetup,
    D_8011CB60,
    0,
    tunnel_CollisionModels,
    0.2f,
    996,
    1000,
    18,
    81,
    92,
    90,
    140,
    200
};

void tunnel_stub() {
}
