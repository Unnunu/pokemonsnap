#ifndef APP_LEVEL_H
#define APP_LEVEL_H
#include "common.h"
#include "world/world.h"

#define GET_ITEM(x) ((Item*) ((x)->userData))
#define GET_POKEMON(obj) ((Pokemon*) ((obj)->userData))
#define GET_TRANSFORM(p) ((PokemonTransform*) ((p)->unk_4C->data))
#define GET_TRANSFORM_BASE(p) ((PokemonTransformBase*) ((p)->unk_4C))

#define POKEMON_FUNC(name)                                \
    void name(GObj* obj) {                                \
        DObj* model = obj->data.dobj;                     \
        Mtx3Float* position = &GET_TRANSFORM(model)->pos; \
        Mtx4Float* rotation = &GET_TRANSFORM(model)->rot; \
        Pokemon* pokemon = GET_POKEMON(obj);

#define HIDE_POKEMON()         \
    pokemon->tangible = false; \
    obj->flags |= GOBJ_FLAG_HIDDEN | GOBJ_FLAG_2;
#define SHOW_POKEMON()        \
    pokemon->tangible = true; \
    obj->flags &= 0;

enum ItemIds {
    ITEM_ID_POKEFLUTE = 161,
    ITEM_ID_PESTER_BALL = 162,
    ITEM_ID_APPLE = 163
};

enum ItemStates {
    ITEM_STATE_INVALID = 0,
    ITEM_STATE_FLYING = 1,
    ITEM_STATE_STILL = 2
};

enum EndLevelReasons {
    END_LEVEL_REASON_QUIT = 1,
    END_LEVEL_REASON_FINISH = 2,
    END_LEVEL_REASON_3 = 3,
    END_LEVEL_REASON_OUT_OF_FILM = 4,
    END_LEVEL_REASON_RETRY = 6,
    END_LEVEL_REASON_IDLE_FINISHED = 7,
    END_LEVEL_REASON_IDLE_INTERRUPTED = 8
};

enum PokemonCommands {
    POKEMON_CMD_5 = 5,
    POKEMON_CMD_6 = 6,
    POKEMON_CMD_7 = 7,
    POKEMON_CMD_8 = 8,
    POKEMON_CMD_9 = 9,
    POKEMON_CMD_10 = 10,
    POKEMON_CMD_12 = 12,
    POKEMON_CMD_13 = 13,
    POKEMON_CMD_14 = 14,
    POKEMON_CMD_15 = 15,
    POKEMON_CMD_16 = 16,
    POKEMON_CMD_17 = 17,
    POKEMON_CMD_18 = 18,
    POKEMON_CMD_19 = 19,
    POKEMON_CMD_20 = 20,
    POKEMON_CMD_21 = 21,
    POKEMON_CMD_22 = 22,
    POKEMON_CMD_23 = 23,
    POKEMON_CMD_24 = 24,
    POKEMON_CMD_25 = 25,
    POKEMON_CMD_26 = 26,
    POKEMON_CMD_27 = 27,
    // command below are probably specific to level
    POKEMON_CMD_28 = 28,
    POKEMON_CMD_29 = 29,
    POKEMON_CMD_30 = 30,
    POKEMON_CMD_31 = 31,
    POKEMON_CMD_32 = 32,
    POKEMON_CMD_33 = 33,
    POKEMON_CMD_34 = 34,
    POKEMON_CMD_35 = 35,
    POKEMON_CMD_36 = 36,
    POKEMON_CMD_37 = 37,
    POKEMON_CMD_38 = 38,
    POKEMON_CMD_39 = 39,
    POKEMON_CMD_40 = 40,
    POKEMON_CMD_41 = 41,
    POKEMON_CMD_42 = 42,
    POKEMON_CMD_43 = 43,
    POKEMON_CMD_44 = 44,
    POKEMON_CMD_45 = 45,
    POKEMON_CMD_46 = 46,
    POKEMON_CMD_47 = 47,
    POKEMON_CMD_48 = 48,
    POKEMON_CMD_49 = 49,
    POKEMON_CMD_50 = 50,
    POKEMON_CMD_51 = 51,
    POKEMON_CMD_52 = 52,
    POKEMON_CMD_53 = 53,
    POKEMON_CMD_54 = 54,
    POKEMON_CMD_55 = 55,
    POKEMON_CMD_56 = 56,
    POKEMON_CMD_57 = 57,
    POKEMON_CMD_58 = 58
};

enum PlayerCommands {
    PLAYER_CMD_SHAKE_CAMERA = 3,
    PLAYER_CMD_BUMP = 4,
    PLAYER_CMD_5 = 5,
    PLAYER_CMD_FINISH = 6,
    PLAYER_CMD_7 = 7,
    PLAYER_CMD_8 = 8,
    PLAYER_CMD_9 = 9,
    PLAYER_CMD_10 = 10
};

enum MovementFlags {
    MOVEMENT_FLAG_ON_GROUND = 0x01,
    MOVEMENT_FLAG_UPDATE_TARGET_POS = 0x02,
    MOVEMENT_FLAG_STOP_WHEN_FLUTE_STOPPED_PLAYING = 0x04,
    MOVEMENT_FLAG_STOP_WHEN_TURN_COMPLETED = 0x08,
    MOVEMENT_FLAG_FIXED_HEIGHT = 0x10,
    MOVEMENT_FLAG_TURN_TO_PLAYER = 0x20,
    MOVEMENT_FLAG_TURN_AWAY = 0x40,
    MOVEMENT_FLAG_TURN_GRADUALLY = 0x80 // affects only movement along path
};

enum PitchModifiers {
    PITCH_MOD_0 = 0,
    PITCH_MOD_1 = 1,
    PITCH_MOD_2 = 2,
    PITCH_MOD_3 = 3,
    PITCH_MOD_4 = 4
};

#define ITEM_CMD_REMOVE 100

#define CAMERA_CMD_BLINK 1

typedef struct Item {
    /* 0x00 */ u8 itemID;
    /* 0x01 */ u8 state;
    /* 0x02 */ u8 entryIndex;
    /* 0x03 */ u8 flags;
    /* 0x04 */ f32 restTimer;
    /* 0x08 */ Vec3f velocity;
    /* 0x14 */ Vec3f collisionVelocity;
    /* 0x20 */ Vec3f prevPos;
} Item;

typedef struct EnvSound {
    /* 0x00 */ struct EnvSound* next;
    /* 0x04 */ GObj* source;
    /* 0x08 */ s32 soundHandle;
    /* 0x0C */ u16 soundId;
    /* 0x0E */ u8 hearingRange;
    /* 0x0F */ u32 category : 2;
    /* 0x0F */ u32 fixedParams : 1;
} EnvSound; // size 0x10

typedef struct EnvSoundData {
    /* 0x00 */ u16 soundID;
    /* 0x02 */ u8 pitchModifier;
    /* 0x03 */ u8 hearingRange;
} EnvSoundData; // size 0x4

// todo this is dumb, figure out a better type
typedef struct UnkBlueShark {
    /* 0x0 */ u16 unk_0;
    /* 0x2 */ u16 unk2;
} UnkBlueShark; // size: 0x4

extern f32 gCamTargetX;
extern f32 gCamTargetY;
extern f32 gCamTargetZ;
extern DObj* gPlayerDObj;
extern GObj* gObjPlayer;
extern s32 gDirectionIndex;
extern OMCamera* gMainCamera;
extern MovementState gMovementState;
extern u32 sTimerInFocus;

extern s32 LastItemId;

extern s32 Icons_NumItemsAvailable;

void Items_Init(void);
void Items_SpawnPesterBall(Vec3f*, Vec3f*);
void Items_SpawnApple(Vec3f*, Vec3f*);
void Items_PlayPokeFlute(void);
void Items_StopPokeFlute(void);
void Items_SetCustomFunctions(GObjFunc, u8, void (*)(GObj*, GroundResult*));
void Items_RemoveFlyingItems(void);
s32 Items_GetPokeFluteState(void);
void Items_func_8035CA1C(s32 (*arg0)(Vec3f*, Vec3f*, Vec3f*, Vec3f*));
void Items_Pause(void);
void Items_UnPause(void);
void Items_InitIterator(void);
GObj* Items_NextValidItem(void);
s32 Items_GetPokeFluteCmd(void);
GObj* Items_CheckObjectExists(GObj* arg0);
void Items_DeleteItem(GObj*);
void Items_DisplaceAllItems(f32 deltaX, f32 deltaY, f32 deltaZ);

u32 getProgressFlags(void);
void setPitchLimits(f32 minValue, f32 maxValue);
void mainCameraSetScissor(Gfx** gfxPtr);
int func_80353D68_4F4178(void);
void func_803570B0_4F74C0(void);
void func_80357120_4F7530(GObj*);
void func_80357150_4F7560(GObj*);
void func_80357170_4F7580(void);
void func_803571C4_4F75D4(void);
void func_803572B0_4F76C0(void);
void func_8035E174_4FE584(GObj* obj, Vec3f*);
GObj* func_8035E238_4FE648(GObj* obj);
void func_8035E264_4FE674(GObj* obj, Vec3f*);
void func_8035E298_4FE6A8(GObj* obj);

void Icons_SetDashEngineEnabled(s32 enabled);
void Icons_Init(void);
void Icons_ProcessButtonPress(s32 eventID);
void Icons_ProcessZoom(s32 zoomEnabled);
void Icons_Freeze(void);
void Icons_UnFreeze(void);
void Icons_Hide(void);
void Icons_Show(void);
void Icons_ProcessTakePhotoPressed(void);

void PokemonDetector_CleanupPokemon(GObj* pokemonObj);
void func_80357428_4F7838(GObj* arg0);

void renderPokemonModelTypeI(GObj* arg0);
void renderPokemonModelTypeIFogged(GObj* arg0);
void renderPokemonModelTypeJFogged(GObj* arg0);
void renderPokemonModelTypeBFogged(GObj* arg0);
void renderPokemonModelTypeDFogged(GObj* arg0);

s32 Pokemon_GetFlag100(GObj*);
void Pokemon_RunAwayFromTarget(GObj* obj, f32 targetDistance, f32 turnSpeed, u32 flags);
void Pokemon_StartAuxProc(GObj*, void (*)(GObj*));
void Pokemon_WaitForFlagNoInteraction(GObj*, u32);
void Pokemon_RemovePokemons(u16*);
void Pokemon_EatApple(GObj*);
void Pokemon_SetScale(GObj*, f32);
void Pokemon_KickItem(GObj* obj, f32 speed);
void Pokemon_Jump(GObj*, f32, f32, f32, f32);
void Pokemon_JumpToHeight(GObj* obj, f32 maxHeight, f32 acceleration, f32 minJumpSpeed, f32 maxJumpSpeed);
void Pokemon_Fall(GObj* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
void Pokemon_FallToHeight(GObj* obj, f32 minHeight, f32 acceleration, f32 minFallSpeed, f32 maxFallSpeed);
void Pokemon_JumpAndBounceFromGround(GObj* obj, f32 speed, f32 jumpBackwards, f32 arg3);
void Pokemon_FallDownOnGround(GObj* obj, f32 acceleration, s32);
bool Pokemon_StepWalkInDirectionFacing(GObj*, u32);
s32 Pokemon_StepToTargetPos(GObj* obj, f32 turnSpeed, u32 flags);
bool Pokemon_Turn(DObj* model, f32 targetYaw, f32 turnSpeed);
void Pokemon_RunInCirclesDefault(GObj* obj, f32 radius);
void Pokemon_RunAwayDefault(GObj* obj, f32 minDistance);
s32 Pokemon_HearsPokeFlute(GObj*);
void Pokemon_ForceAnimationAtTime(GObj* obj, AnimationHeader* header, f32 time);
void Pokemon_SetTargetPos(GObj*, f32, f32);
void Pokemon_RunToTargetPos(GObj*, f32);
void Pokemon_StartAuxProc(GObj* obj, GObjFunc state);
void Pokemon_SetState(GObj* obj, GObjFunc state);
void Pokemon_SetStateRandom(GObj* obj, RandomState* nextStates);
void Pokemon_StartPathProc(GObj* obj, GObjFunc func);
void Pokemon_SetAnimation(GObj*, AnimationHeader*);
void Pokemon_ForceAnimation(GObj*, AnimationHeader*);
void Pokemon_SetAnimationAtTime(GObj* obj, AnimationHeader* header, f32 time);
void Pokemon_SetAnimationCommon(GObj*, AnimationHeader*, f32 start, s32 forceUpdate);
f32 Pokemon_GetDistance(GObj* obj1, GObj* obj2);
void Pokemon_WaitForFlag(GObj* obj, s32 flags);
void Pokemon_RunCleanup(GObj* obj);
GObj* Pokemon_AddAtGeo(GObj* obj, u16 PokemonID, PokemonDef* def);
void Pokemon_ResetPathPos(GObj*);
void Pokemon_FollowPath(GObj* obj, f32 start, f32 end, f32 dt, f32 yawStep, u32 flags);
void func_80362414_502824(Pokemon* obj, EggStruct* egg, f32 arg2);
GObj* Pokemon_Spawn(s32 gObjID, u16 id, struct WorldBlock* roomA, struct WorldBlock* roomB, ObjectSpawn* spawn, PokemonInitData* initData);
void Pokemon_StopAuxProc(GObj*);
void Pokemon_SetFlag100(GObj*, s32);
void Pokemon_RunInCircles(GObj*, f32, f32, UNK_TYPE);
void Pokemon_TurnToTarget(GObj*, f32, u32);
void Pokemon_TurnToModelNode(GObj* obj, DObj* modelNode, f32 turnSpeed, u32 flags);
void Pokemon_RunToTarget(GObj*, f32, f32, u32);
GObj* Pokemon_SpawnOnGround(s32 gObjID, u16 id, struct WorldBlock* roomA, struct WorldBlock* roomB, ObjectSpawn* spawn, PokemonInitData* initData);
GObj* Pokemon_SpawnDlLink4(s32 gObjID, u16 id, struct WorldBlock* roomA, struct WorldBlock* roomB, ObjectSpawn* spawn, PokemonInitData* initData);
GObj* Pokemon_SpawnOnGroundDlLink4(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData);
void Pokemon_ChangeBlockAndRemove(GObj* obj, f32 prevBlockX, f32 prevBlockY, f32 prevBlockZ, f32 currBlockX, f32 currBlockY, f32 currBlockZ);
void Pokemons_Init(void);

void EnvSound_Init(EnvSoundData* data, s32 numEntries);

GObj* initUI(void (*exitBlockCB)(struct WorldBlock*), void (*updateMovementCB)(s32, f32), GObjFunc fnUpdateItems, u8 fnUpdateItemsKind, void (*fnCollide)(GObj*, GroundResult*));
GObj* createMainCameras(s32 bgColor);

void setEndLevelCallback(void (*cb)(s32));
void setPauseCallback(void (*cb)(s32));
void setIdleScript(struct IdleScript*);
s32 func_8009A8E4(void);
void PokemonDetector_Create(void);
void PokemonDetector_Enable(void);
void PokemonDetector_Disable(void);

GObj* PlayerModel_Init(void);
void PlayerModel_SetAnimation(AnimCmd** modelAnim, AnimCmd*** matAnim, f32 startTime, f32 speed);
void PlayerModel_SetAnimationSpeed(f32 speed);
void PlayerModel_ChangeAnimation(AnimCmd** modelAnim, AnimCmd*** matAnim, f32 startTime);

void pokemonChangeBlock(GObj* obj, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void pokemonChangeBlockOnGround(GObj* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void pokemonRemoveOne(GObj* obj);

void resetMainCameraSettings(void);

void Camera_StartCutScene(GObj* pokemon, AnimCmd* camAnim, f32 time);

void Msg_ShowMessage(u8* message, s32 posX, s32 posY, u8 colorR, u8 colorG, u8 colorB, u8 colorA, s32 delayBetweenChars, u8 alignment);
void Msg_Reset(void);
void Msg_SetSpecialHandler(void (*handler)(u8));
void Msg_SetNonInteractive(void);
void Msg_SetInteractive(void);
bool Msg_IsMessagePrinted(void);

void EnvSound_PlaySound(GObj* obj, u8 category, s32 soundID);
void EnvSound_StopSoundByCategory(GObj*, u8);

#endif
