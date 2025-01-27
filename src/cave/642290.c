#include "common.h"
#include "world/world.h"
#include "app_level/app_level.h"
#include "cave.h"

extern AnimationHeader D_802C6BA4_649054;
extern AnimationHeader D_802C6BB8_649068;
extern AnimationHeader D_802C6BCC_64907C;
extern InteractionHandler D_802C6BE0_649090[];
extern InteractionHandler D_802C6C10_6490C0[];
extern InteractionHandler D_802C6C30_6490E0[];
extern PokemonInitData D_802C6C64_649114;

void func_802BFEF0_6423A0(GObj*);
void func_802BFF74_642424(GObj*);
void func_802C0058_642508(GObj*);

void func_802BFDE0_642290(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    HIDE_POKEMON();
    pokemon->transitionGraph = D_802C6C10_6490C0;
    Pokemon_WaitForFlag(obj, 0);
    Pokemon_SetState(obj, func_802BFDE0_642290);
}

void func_802BFE34_6422E4(GObj* obj) {
    Pokemon* pokemon = GET_POKEMON(obj);

    if (Pokemon_GetDistance(obj, pokemon->interactionTarget) > 500.0f) {
        Pokemon_SetState(obj, func_802BFDE0_642290);
    }
    pokemon->counter = 1, pokemon->processFlags &= ~POKEMON_PROCESS_WAIT_ENDED;
    pokemon->transitionGraph = NULL;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_WAIT_ENDED);
    cmdSendCommand(pokemon->interactionTarget, 0x1E, obj);
    pokemon->transitionGraph = D_802C6C30_6490E0;
    Pokemon_WaitForFlag(obj, 0);
    Pokemon_SetState(obj, func_802BFEF0_6423A0);
}

void func_802BFEF0_6423A0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->flags &= ~POKEMON_PROCESS_WAIT_ENDED;
    Pokemon_SetAnimation(obj, &D_802C6BB8_649068);
    Pokemon_StartPathProc(obj, func_802BFF74_642424);
    SHOW_POKEMON();
    pokemon->transitionGraph = NULL;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_PATH_ENDED);
    Pokemon_SetState(obj, func_802C0058_642508);
}

POKEMON_FUNC(func_802BFF74_642424)
    Mtx3Float* someScale;

    position->v.y = GET_TRANSFORM(pokemon->interactionTarget->data.dobj)->pos.v.y - 100.0f;
    ohWait(64);
    pokemon->flags |= POKEMON_FLAG_4;

    someScale = &(GET_TRANSFORM(func_8035E1D4_4FE5E4(obj)->data.dobj)->scale);
    someScale->v.x *= 2.0f;
    someScale->v.y *= 2.0f;
    someScale->v.z *= 2.0f;

    pokemon->hSpeed = 0.0f;
    pokemon->jumpVel = 700.0f;
    Pokemon_FallDownOnGround(obj, -9.8f, 1);

    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

void func_802C0058_642508(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C6BA4_649054);
    pokemon->transitionGraph = D_802C6BE0_649090;
    Pokemon_WaitForFlag(obj, 0);
    Pokemon_SetState(obj, NULL);
}

void func_802C00B0_642560(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C6BCC_64907C);
    pokemon->transitionGraph = NULL;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802C0058_642508);
}

GObj* func_802C0104_6425B4(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData) {
    return Pokemon_SpawnOnGround(objID, id, block, blockB, spawn, &D_802C6C64_649114);
}
