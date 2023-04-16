#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "ultra64.h"
#include "common_structs.h"
#include "unknown_structs.h"
#include "sys/om.h"

void loadCompressedData(u32 rom, u32 ram);

GObjThread *get_obj_thread(void);
void return_obj_thread(GObjThread*);
ThreadStackNode* get_stack_of_size(u32);
ThreadStackNode *get_default_stack(void);

GObj* runGObjProcess(GObj*, gfxFunc func, s8 kind, u32 priority);
void endGObjProcess(GObj*);

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
void spawnAnimalUsingDeltaHeight(s32 gObjID, u16 id, roomGFX* roomA, roomGFX* roomB, objectSpawn* spawn, animalInitData* initData);

void func_80004860(void);
void func_80005448(s32);
void func_800067DC(void);
void func_800073AC(Struct_800073AC*);
void func_80007A34(s16, s16, s16, s16);
void func_80007BC4(s32*);

GObj* func_8000A410(s32, s32, s32, s32);
void func_8000ADA0(void);
void func_8000AFFC(void);
void func_8000BC84(void);
void func_8000BCA8(s32);
Struct_8000C37C* func_8000C37C(s32, void*, s32, s32, void*, s32, s32, s32, s32*, s32, void (*)(Struct_8000C37C *), s32);
Struct_8000C37C* func_8000C3FC(s32, void*, s32, s32, void*, s32, s32, s32, s32, s32, s32, s32, s32);
void func_8000C4B0(s32, s32, s32, s32, s32);

void func_80017768(void);
void func_8001977C(void);
void func_80022374(s32, s32);
void func_80022454(s32, s32);
void func_800224DC(s32, s32, s32);
void func_800228E4(s32);
void func_80022A58(s32, s32, s32, f32, s32);

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

void func_8036A3F8(s32*, s32);
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
