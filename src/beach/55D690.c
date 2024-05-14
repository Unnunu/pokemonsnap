#include "common.h"
#include "beach.h"

void func_beach_802C5620(GObj* arg0) {
    UNUSED s32 pad[4];
    s32 sp34;
    f32 sp30;

    while (TRUE) {
        func_80357384_4F7794(&sp34, &sp30);
        if (sp34 > 3 || (sp34 == 3 && sp30 >= 0.3)) {
            break;
        }
        ohWait(1);
    }
    runPokemonCleanup(arg0);
    func_8035EDC8_4FF1D8(arg0);
}

void func_beach_802C56C8(GObj* obj) {
    func_8035ED90_4FF1A0(obj, func_beach_802C5620);
    updatePokemonState(obj, beach_snorlax_state1);
}

void beach_snorlax_state1(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags |= 8;
    setPokemonAnimation(obj, &D_beach_802CC10C);
    pokemon->transitionGraph = D_beach_802CC1D4;
    runInteractionsAndWaitForFlags(obj, 1);
    weightedRandomStaightTransition(obj, &D_beach_802CC2C4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/beach/55D690/func_beach_802C5768.s")

void beach_snorlax_state2(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    forcePokemonAnimation(obj, &D_beach_802CC134);
    pokemon->transitionGraph = D_beach_802CC1D4;
    runInteractionsAndWaitForFlags(obj, 1);
    updatePokemonState(obj, beach_snorlax_state1);
}

void beach_snorlax_handleCmd9(GObj* obj) {
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags |= 8;
    updatePokemonState(obj, func_beach_802C5768);
}

void beach_snorlax_handleCmd13(GObj* obj) {
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags |= 8;
    updatePokemonState(obj, beach_snorlax_state2);
}

void func_beach_802C58EC(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags &= ~8;
    setPokemonAnimation(obj, &D_beach_802CC148);
    pokemon->transitionGraph = NULL;
    runInteractionsAndWaitForFlags(obj, 1);
    updatePokemonState(obj, func_beach_802C594C);
}

void func_beach_802C594C(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    setPokemonAnimation(obj, &D_beach_802CC170);
    runPathProcess(obj, func_beach_802C5B68);
    pokemon->transitionGraph = D_beach_802CC234;
    runInteractionsAndWaitForFlags(obj, 2);
    updatePokemonState(obj, func_beach_802C5BD4);
}

void func_beach_802C59C0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags &= ~8;
    setPokemonAnimation(obj, &D_beach_802CC148);
    pokemon->transitionGraph = NULL;
    runInteractionsAndWaitForFlags(obj, 1);
    updatePokemonState(obj, func_beach_802C5A20);
}

void func_beach_802C5A20(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    setPokemonAnimation(obj, &D_beach_802CC184);
    runPathProcess(obj, func_beach_802C5B68);
    pokemon->transitionGraph = D_beach_802CC264;
    runInteractionsAndWaitForFlags(obj, 2);
    updatePokemonState(obj, func_beach_802C5BD4);
}

void func_beach_802C5A94(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags &= ~8;
    setPokemonAnimation(obj, &D_beach_802CC148);
    pokemon->transitionGraph = NULL;
    runInteractionsAndWaitForFlags(obj, 1);
    updatePokemonState(obj, func_beach_802C5AF4);
}

void func_beach_802C5AF4(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    setPokemonAnimation(obj, &D_beach_802CC198);
    runPathProcess(obj, func_beach_802C5B68);
    pokemon->transitionGraph = D_beach_802CC294;
    runInteractionsAndWaitForFlags(obj, 2);
    updatePokemonState(obj, func_beach_802C5BD4);
}

void func_beach_802C5B68(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    while (1) {
        if (!func_80361440_501850(obj)) {
            break;
        }

        ohWait(1);
    }

    pokemon->processFlags |= 2;
    pokemon->pathProcess = NULL;
    omEndProcess(NULL);
}

void func_beach_802C5BD4(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    setPokemonAnimation(obj, &D_beach_802CC15C);
    pokemon->transitionGraph = NULL;
    runInteractionsAndWaitForFlags(obj, 1);
    updatePokemonState(obj, beach_snorlax_state1);
}

s32 D_beach_802CC0F0[] = { 294 };
s32 D_beach_802CC0F4[] = { 293 };
s32 D_beach_802CC0F8[] = { 295 };
s32 D_beach_802CC0FC[] = { 355 };
s32 D_beach_802CC100[] = { 297 };
s32 D_beach_802CC104[] = { 296, 125 };

AnimationHeader D_beach_802CC10C = {
    0.1f,
    25.0f,
    0x8017EBA0,
    0x80182230,
    D_beach_802CC0F0
};

AnimationHeader D_beach_802CC120 = {
    0.2f,
    8.0f,
    0x8017E740,
    0x801821C0,
    NULL
};

AnimationHeader D_beach_802CC134 = {
    0.15f,
    40.0f,
    0x8017F8B0,
    0x80182310,
    D_beach_802CC0F4
};

AnimationHeader D_beach_802CC148 = {
    0.23f,
    24.0f,
    0x8017FEF0,
    NULL,
    D_beach_802CC0F8
};

AnimationHeader D_beach_802CC15C = {
    0.2f,
    20.0f,
    0x8017F080,
    NULL,
    D_beach_802CC0FC
};

AnimationHeader D_beach_802CC170 = {
    0.2f,
    36.0f,
    0x80181880,
    NULL,
    NULL
};

AnimationHeader D_beach_802CC184 = {
    0.42f,
    71.0f,
    0x80180910,
    0x801823C0,
    D_beach_802CC100
};

AnimationHeader D_beach_802CC198 = {
    0.2f,
    71.0f,
    0x8017BB20,
    NULL,
    NULL
};

AnimationHeader D_beach_802CC1AC = {
    0.2f,
    19.0f,
    0x8017DA60,
    0x801820B0,
    D_beach_802CC104
};

AnimationHeader D_beach_802CC1C0 = {
    0.2f,
    8.0f,
    0x8017E2E0,
    0x80182150,
    NULL
};

idFuncStruct D_beach_802CC1D4[] = {
    {
        POKEMON_CMD_9,
        beach_snorlax_handleCmd9,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_13,
        beach_snorlax_handleCmd13,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_5,
        func_beach_802C58EC,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_6,
        func_beach_802C59C0,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_7,
        func_beach_802C5A94,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_58,
        NULL,
        0.0f,
        NULL
    },
};

idFuncStruct D_beach_802CC234[] = {
    {
        POKEMON_CMD_6,
        func_beach_802C5A20,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_7,
        func_beach_802C5AF4,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_58,
        NULL,
        0.0f,
        NULL
    },
};

idFuncStruct D_beach_802CC264[] = {
    {
        POKEMON_CMD_5,
        func_beach_802C594C,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_7,
        func_beach_802C5AF4,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_58,
        NULL,
        0.0f,
        NULL
    },
};

idFuncStruct D_beach_802CC294[] = {
    {
        POKEMON_CMD_5,
        func_beach_802C594C,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_6,
        func_beach_802C5A20,
        0.0f,
        NULL
    },
    {
        POKEMON_CMD_58,
        NULL,
        0.0f,
        NULL
    },
};

randomTransition D_beach_802CC2C4[] = {
    { 100, beach_snorlax_state1 },
    { 100, beach_snorlax_state2 },
    { 0, NULL }
};

PokemonAnimationSetup D_beach_802CC2DC = {
    &D_beach_802CC10C,
    func_beach_802C56C8,
    0,
    { 0, 0, 0 },
    NULL,
    NULL
};

PokemonInitData beach_snorlax_initData = {
    0x803300D0,
    0x8032B8F8,
    func_80359484_4F9894,
    &D_beach_802CC2DC,
    { 1.5f, 1.5f, 1.5f },
    { 0.0f, 82.0f, 0.0f },
    105.0f,
    0x35,
    0x36, 0, 0,
    { 0, 0, 0},
    0
};

GObj* beach_spawnSnorlax(s32 gObjID, u16 id, WorldBlock* spawnBlock, WorldBlock* currentBlock, ObjectSpawn* spawn) {
    return spawnPokemonOnGround(gObjID, id, spawnBlock, currentBlock, spawn, &beach_snorlax_initData);
}
