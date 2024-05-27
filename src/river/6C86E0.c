#include "common.h"
#include "world/world.h"
#include "app_level/app_level.h"

void func_802E142C_6C920C(GObj*);
void func_802E1488_6C9268(GObj*);
void func_802E1594_6C9374(GObj*);
void func_802E1110_6C8EF0(GObj*);
void func_802E0E20_6C8C00(GObj*);
void func_802E0C3C_6C8A1C(GObj*);

extern AnimationHeader D_802E4720_6CC500;
extern InteractionHandler D_802E4938_6CC718;
extern InteractionHandler D_802E4908_6CC6E8;
extern AnimationHeader D_802E470C_6CC4EC;
extern AnimationHeader D_802E46D0_6CC4B0;
extern AnimationHeader D_802E46E4_6CC4C4;
extern AnimationHeader D_802E46F8_6CC4D8;
extern InteractionHandler D_802E4838_6CC618[];
extern RandomState D_802E4978_6CC758[];
extern PokemonInitData D_802E4AA8_6CC888;

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E0900_6C86E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E09A8_6C8788.s")

void func_802E0A48_6C8828(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E46D0_6CC4B0);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E4838_6CC618;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetStateRandom(obj, D_802E4978_6CC758);
}

void func_802E0AB8_6C8898(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ForceAnimation(obj, &D_802E46E4_6CC4C4);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E4838_6CC618;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetStateRandom(obj, D_802E4978_6CC758);
}

void func_802E0B28_6C8908(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ForceAnimation(obj, &D_802E46F8_6CC4D8);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E4838_6CC618;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetStateRandom(obj, D_802E4978_6CC758);
}

void func_802E0B98_6C8978(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E470C_6CC4EC);
    pokemon->pokemonLoopTarget = 5;
    Pokemon_StartPathProc(obj, func_802E0C3C_6C8A1C);
    pokemon->transitionGraph = D_802E4838_6CC618;
    Pokemon_WaitForFlag(obj, 3);
    if (pokemon->processFlags & 2) {
        Pokemon_SetState(obj, func_802E0A48_6C8828);
    }
    Pokemon_SetStateRandom(obj, D_802E4978_6CC758);
}

void func_802E0C3C_6C8A1C(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->hSpeed = 20.0f;
    Pokemon_RunInCircles(obj, 500.0f, 0.1f, 1);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E0C98_6C8A78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E0D44_6C8B24.s")

void func_802E0DAC_6C8B8C(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E4720_6CC500);
    Pokemon_StartPathProc(obj, func_802E0E20_6C8C00);
    pokemon->transitionGraph = &D_802E4938_6CC718;
    Pokemon_WaitForFlag(obj, 2);
    Pokemon_SetState(obj, func_802E0A48_6C8828);
}

void func_802E0E20_6C8C00(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->hSpeed = 80.0f;
    Pokemon_RunAwayFromTarget(obj, 300.0f, 0.1f, 1);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E0E7C_6C8C5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E0F04_6C8CE4.s")

void func_802E1030_6C8E10(GObj*);
extern AnimationHeader D_802E4784_6CC564;
extern InteractionHandler D_802E48B8_6CC698;

void func_802E0F9C_6C8D7C(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E4784_6CC564);
    Pokemon_StartPathProc(obj, func_802E1030_6C8E10);
    pokemon->transitionGraph = &D_802E48B8_6CC698;
    Pokemon_WaitForFlag(obj, 1);
    if (pokemon->interactionTarget != NULL) {
        Pokemon_SetState(obj, func_802E0F9C_6C8D7C);
    }
    Pokemon_SetStateRandom(obj, D_802E4978_6CC758);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E1030_6C8E10.s")

void func_802E1094_6C8E74(void) {
}

void func_802E109C_6C8E7C(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E470C_6CC4EC);
    Pokemon_StartPathProc(obj, func_802E1110_6C8EF0);
    pokemon->transitionGraph = &D_802E4908_6CC6E8;
    Pokemon_WaitForFlag(obj, 2);
    Pokemon_SetState(obj, func_802E0A48_6C8828);
}

void func_802E1110_6C8EF0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_TurnToTarget(obj, 0.1f, 0x2E);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= 2;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E115C_6C8F3C.s")

extern InteractionHandler D_802E49E0_6CC7C0;

void func_802E1194_6C8F74(GObj* obj) {
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->transitionGraph = &D_802E49E0_6CC7C0;
    Pokemon_WaitForFlag(obj, 0);
    Pokemon_SetState(obj, NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E11D4_6C8FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E12D8_6C90B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E1374_6C9154.s")

void func_802E13F4_6C91D4(GObj* arg0) {
    Pokemon_StartPathProc(arg0, func_802E1488_6C9268);
    Pokemon_SetState(arg0, func_802E142C_6C920C);
}

void func_802E13F4_6C91D4(GObj*);
extern AnimationHeader D_802E4720_6CC500;
extern InteractionHandler D_802E49A0_6CC780;

void func_802E142C_6C920C(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E4720_6CC500);
    pokemon->transitionGraph = &D_802E49A0_6CC780;
    Pokemon_WaitForFlag(obj, 2);
    Pokemon_SetState(obj, func_802E13F4_6C91D4);
}

void func_802E1488_6C9268(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ResetPathPos(obj);
    Pokemon_FollowPath(obj, 0, 1, 0.05f, 0.0f, MOVEMENT_FLAG_UPDATE_TARGET_POS);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E14F4_6C92D4.s")

void func_802E1570_6C9350(GObj* arg0) {
    Pokemon_SetState(arg0, func_802E1594_6C9374);
}

void func_802E1604_6C93E4(GObj*);
extern AnimationHeader D_802E46D0_6CC4B0;
extern InteractionHandler D_802E4A10_6CC7F0;

void func_802E1594_6C9374(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E46D0_6CC4B0);
    Pokemon_StartPathProc(obj, func_802E1604_6C93E4);
    pokemon->transitionGraph = &D_802E4A10_6CC7F0;
    Pokemon_WaitForFlag(obj, 0);
    Pokemon_SetState(obj, NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E1604_6C93E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E1678_6C9458.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E171C_6C94FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/river/6C86E0/func_802E17D4_6C95B4.s")

GObj* func_802E1864_6C9644(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData) {
    return Pokemon_SpawnOnGround(objID, id, block, blockB, spawn, &D_802E4AA8_6CC888);
}
