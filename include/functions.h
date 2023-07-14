#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "ultra64.h"
#include "common_structs.h"
#include "unknown_structs.h"
#include "sys/om.h"

void loadCompressedData(u32 rom, u32 ram);

GObj* om_create_process(GObj*, gfxFunc func, u8 kind, u32 priority);
void om_end_process(GObjProcess*);

void sendSignalToLink(s32 llIndex, s32, GObj*);

void osStopThread(OSThread* thread);
s32 osRecvMesg(OSMesgQueue* mq, OSMesg* msg, s32 flag);
void __osEnqueueAndYield(OSThread**);
void __osEnqueueThread(OSThread** queue, OSThread* thread);
void __osDispatchThread();
OSThread* __osPopThread(OSThread**);
void __osDequeueThread(OSThread** queue, OSThread* thread);
void __osSetSR(u32);
u32 __osGetSR();
f32 sqrtf(f32 f);
void osSetThreadPri(OSThread* thread, OSPri pri);
OSPri osGetThreadPri(OSThread* thread);
void __osSetCompare(u32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);
void osYieldThread(void);
void osStartThread(OSThread*);

roomGFX* getCurrentRoom(void);
GObj* animalAddOne(roomGFX* roomA, roomGFX* roomB, objectSpawn* spawn, animalDef* def);

s32 getLevelId();
void setLevelId(s32 levelID);
s8* getLevelName(s32 levelIdx);

void func_800AAED0(s32);
s32 func_800AAEE8(s32);
void func_800AAF10(void);

s32 inRange_DEBUG(u32, s32, s32, s32);
s32 bool_DEBUG(s32, s32);

void spawnStaryuAtGeo(GObj*);
void spawnStarmieAtGeo(GObj*);
void spawnKoffingSmoke(s32 gObjID, u16 id, roomGFX* roomA, roomGFX* roomB, objectSpawn* spawn);

void updateAnimalState(GObj* obj, gfxFunc state);
void weightedRandomStaightTransition(GObj* obj, randomTransition* nextStates);
void runPathProcess(GObj* obj, gfxFunc func);
void setAnimalAnimation(GObj*, animationHeader*);
void forceAnimalAnimation(GObj*, animationHeader*);
void animalUVStuff(GObj*, animationHeader*, f32 start, s32 forceUpdate);
void runInteractionsAndWaitForFlags(GObj* obj, u32 flags);
void runAnimalCleanup(GObj* obj);
GObj* addAnimalAtGeo(GObj* obj, u16 AnimalID, animalDef* def);
roomGFX* setNodePosToNegRoom(GObj*);
void animalPathLoop(GObj* obj, f32 start, f32 end, f32 dt, f32 yawStep, u32 flags);
void spawnAnimalUsingDeltaHeight(s32 gObjID, u16 id, roomGFX* roomA, roomGFX* roomB, objectSpawn* spawn,
                                 animalInitData* initData);

void contUpdate(void);
void func_80005448(s32);
void func_800067DC(void);
void om_setup_scene(Struct_800073AC*);

GObjCommon* om_add_gobj(u32 id, void (*fnUpdate)(GObjCommon*), u8 link, u32 priority);
void om_draw_all(void);
void om_update_all(void);
void func_8000BC84(GObjCommon* arg0);
void func_8000BCA8(s32);
GObjCommon* create_sprite(s32 objId, void (*objFnUpdate)(GObjCommon*), s32 objLink, s32 objPriority,
                          void (*fnRender)(GObjCommon*), u8 dlLink, s32 dlPriority, s32 dlArg, Sprite* sprite,
                          u8 procKind, void (*procFunc)(GObjCommon*), s32 procPriority);
GObjCommon* create_camera(s32 objId, void (*objFnUpdate)(GObjCommon*), s32 objLink, s32 objPriority,
                  void (*fnRender)(GObjCommon*), s32 dlPriority, s32 dlLinkBitMask, s32 cameraTag, s32 defaultMatrices,
                  u8 procKind, void (*procFunc)(GObjCommon*), s32 procPriority, s32 defaultFlags);
GObjCommon* func_8000C4B0(s32 link, s32 priority, s32 dlPriority, s32 arg3, s32 arg4);

void draw_sprite(GObjCommon*);
void func_800191D8(GObjCommon*);
void sprite_camera_render(GObjCommon*);
void auPlaySong(s32, s32);
void func_80022454(s32, s32);
void func_800224DC(s32, u32, u32);
void auPlaySound(s32);
void auPlaySoundWithParams(s32, s32, s32, f32, s32);

void func_800A1A50(void);
void func_800A7470(s32, s32, s32);
void func_800A7860(s32, f32);
void func_800A7F68(s32, s32);
s32* func_800AA38C(s32);
void func_800AA85C(s32, s32);
void func_800AA870(s32);
void func_800AAE28(void);
void func_800BF44C_5C2EC(u8*);
void func_800BFC18_5CAB8(s32, s32);
void func_800BFEBC_5CD5C(s32, s32);

void mem_create_heap(u8*, u32);
void* mem_alloc(u32 size);

void func_8036A8E4(s32);
s32 func_8036AC6C(s32 posX, s32 posY, s32 width, s32 height, s32);
void func_8036B734(s32);
void func_8036B870(s32, s32, s32, s32, s32, s32);
void func_8036B9EC(s32, s32, s32);
void func_8036C898(s32, char*);
void func_8036CB58(s32, s32);
void func_8036CBA0(s32, s32, s32);
void func_8036D1A4(s32, s32);
void func_8036D248(s32, s32, s32, s32, s32);
void func_8036D344(s32);
void func_8036D3E8(s32, s32);
void func_8036D448(s32);
void func_8036D4A0(s32);
void func_8036D4B4(s32, s32);
s32 func_8036D4F0(u8*);
void func_8036EB98(void);
void func_8036F0A0(s32, s32);
void func_8036F0DC(s32, s32);
void func_8036F1F4(s32, s32, s32);
void func_8036F684(s32, s32);
void func_8036F738(s32, ColorRGB*);
s32 func_8036F78C(void);
void func_8036FE54(s32, s32);
void func_8036FFE0(s32, s32);
void func_80370038(s32, s32);
void func_803700A4(s32);
void func_80370134(void);
void func_80370428(void);

#endif
