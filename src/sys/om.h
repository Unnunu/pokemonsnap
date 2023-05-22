#ifndef SYS_OM_H
#define SYS_OM_H

#include "PR/os.h"
#include "PR/mbi.h"
#include "PR/sp.h"
#include "types.h"
#include "common_structs.h"

// Object Manager (OM) Objects

typedef struct GObjThread {
    /* 0x000 */ struct GObjThread* next;
    /* 0x008 */ OSThread osThread;
    /* 0x1B8 */ u64* osStack;
    /* 0x1BC */ u32 stackSize;
} GObjThread; // size == 0x1C0

/// This is the node for the allocated stack for a `struct GObjThread`
typedef struct ThreadStackNode {
    /* 0x00 */ struct ThreadStackNode* next;
    /* 0x04 */ u32 stackSize;
    /* 0x08 */ u64 stack[1];
} ThreadStackNode; // size == 0x08 + VLA

typedef struct GObjProcess {
    /* 0x00 */ struct GObjProcess* next; // Points to next process in free or object process list
    /* 0x04 */ struct GObjProcess* prev; // Points to previous process in object process list
    /* 0x08 */ struct GObjProcess* nextInPriorityList;
    /* 0x08 */ struct GObjProcess* prevInPriorityList;
    /* 0x10 */ s32 priority;
    /* 0x14 */ u8 kind;
    /* 0x15 */ u8 frozen;
    /* 0x18 */ struct GObjCommon* object;
    // following two fields are typed via kind
    /* 0x1C */ union {
        struct GObjThread* thread;
        void (*cb)(struct GObjCommon*);
    } unk1C;
    /* 0x20 */ void (*function)(struct GObjCommon*);
} GObjProcess; // size == 0x24

struct MaybeCommonLink {
    struct GObjCommon* unk00;
    s32 unk04;
}; // size == 8

struct DObj;
struct Struct82F0;
struct GObjCommon;

typedef struct GObjCmd {
    /* 0x00 */ struct GObjCmd* next;
    /* 0x04 */ struct GObjCommon* obj;
    /* 0x08 */ s32 cmd;
} GObjCmd; // size = 0xC

typedef struct GObj_Sub3CList {
    /* 0x00 */ GObjCmd* head;
    /* 0x04 */ GObjCmd* tail;
    /* 0x08 */ s32 count;
} GObj_Sub3CList; // size = 0xC

typedef struct GObjCommon {
    /* 0x00 */ u32 id;
    /* 0x04 */ struct GObjCommon* next;
    /* 0x08 */ struct GObjCommon* prev;
    /* 0x0C */ u8 link;
    /* 0x0D */ u8 dlLink;
    /* 0x0E */ u8 unk0E;
    /* 0x0F */ u8 type;                              // ptr kind?
    /* 0x10 */ u32 priority;
    /* 0x14 */ void (*fnUpdate)(struct GObjCommon*); // takes a self?
    /* 0x18 */ struct GObjProcess* processListHead;
    /* 0x1C */ struct GObjProcess* processListTail;
    /* 0x20 */ struct GObjCommon* nextDl;
    /* 0x24 */ struct GObjCommon* prevDl;
    /* 0x28 */ u32 dlPriority;
    /* 0x2C */ void (*render)(struct GObjCommon*);
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ GObj_Sub3CList sub3C;
    // typed based on unk0F?
    // 0 : NULL
    // 1 : DObj
    // 2 : SObj
    // 3 : OMCamera
    /* 0x48 */ void* children;

    f32 unk_4C;
    u32 unk_50;
    void (*unk_54)(struct DObj*, u32, f32);
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    ///* 0x48 */ struct MaybeCommonLink unk48[5];
    // active/len of unk48
    /* 0x70 */ s32 unk70;
    // typed based on unk0F?
    // 0 : NULL
    // 1 : DObj
    // 2 : SObj
    // 3 : OMCamera
    /* 0x74 */ void* unk74_old;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ u32 unk7C_old;
    /* 0x80 */ void (*unk80)(struct DObj*, u32, f32);
    /* 0x84 */ void* unk84;
} GObjCommon; // size >= 0x88

typedef struct OMMtx {
    /* 0x00 */ struct OMMtx* next;
    /* 0x04 */ u8 kind; // kind
    /* 0x05 */ u8 unk05;
    /* 0x08 */ Mtx unk08;
    ///* 0x08 */ f32 unk08[4][4];
    ///* 0x08 */ f32 (*unk08)[4][4];
    ///* 0x0C */ u8 pad0C[0x48 - 0xc];
} OMMtx; // size == 0x48

struct Mtx3Float {
    /* 0x00 */ struct OMMtx* mtx;
    /* 0x04 */ struct Vec3f v;
}; // size == 0x10

struct Mtx3Int {
    /* 0x00 */ struct OMMtx* mtx;
    /* 0x04 */ struct Vec3i v;
}; // size == 0x10

union Mtx3fi {
    struct Mtx3Float f;
    struct Mtx3Int i;
}; // size == 0x10

struct Mtx4Float {
    /* 0x00 */ struct OMMtx* mtx;
    /* 0x04 */ f32 f[4];
}; // size == 0x14

struct Mtx6Float {
    /* 0x00 */ struct OMMtx* mtx;
    /* 0x04 */ f32 f[6];
}; // size == 0x1C

struct Mtx7Float {
    /* 0x00 */ struct OMMtx* mtx;
    /* 0x04 */ f32 f[7];
}; // size == 0x20

struct Mtx3x3Float {
    struct OMMtx* mtx;
    f32 array[3][3];
}; // size == 0x28;

/// This stores up to 3 `Mtx3Int`/`Mtx3Float`/`Mtx4Float` structures in the VLA data
/// based on the kind id in the `kinds` arrays:
/// Kind 1 - `struct Mtx3Int` or `union Mtx3fi`
/// Kind 2 - `struct Mtx4Float`
/// Kind 3 - `struct Mtx3Float`
struct DObjDynamicStore {
    /* 0x00 */ u8 kinds[3];
    /* 0x03 */ u8 pad;
    /* 0x04 */ u8 data[1];
}; // size == 4 + VLA

typedef struct DObj {
    /* 0x00 */ struct DObj* nextFree;
    /* 0x04 */ struct GObjCommon* obj;
    /* 0x08 */ struct DObj* next;
    /* 0x0C */ struct DObj* prev;
    /* 0x10 */ struct DObj* childList;
    /* 0x14 */ struct DObj* parent;    // checked with `1` for a top level
    /* 0x18 */ union Mtx3fi unk18;     // position?
    /* 0x28 */ struct Mtx4Float unk28; // rotation?
    /* 0x3C */ struct Mtx3Float unk3C; // scale?
    /* 0x4C */ struct DObjDynamicStore* unk4C;
    // can be any of: DObj *, Gfx *, or the struct Unk50... above
    /* 0x50 */ void* unk50;
    // is this a union? WeirdBytewise...?
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 numMatrices;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ struct OMMtx* matrices[5];
    /* 0x6C */ struct AObj* aobjList;
    /* 0x70 */ union AnimCmd* unk70;
    // Vec3fi?
    /* 0x74 */ f32 unk74; // scale? only in OMAnimation
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ struct MObj* mobjList;
    /* 0x84 */ u32 unk84;
} DObj; // size == 0x88, but it's dynamic..?

typedef struct AObj {
    /* 0x00 */ struct AObj* next;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ union {
        f32 unk10;
        u8 unk10_color[4];
    };
    /* 0x14 */ union {
        f32 unk14;
        u8 unk14_color[4];
    };
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    // interpolation control struct?
    /* 0x20 */ void* unk20;
} AObj; // size == 0x24

typedef struct MObjSub {
    ///* 0x00 */ f32 unk00;
    /* 0x00 */ u16 pad00;
    /* 0x02 */ u8 unk02;  // SetTextureImage format?
    /* 0x03 */ u8 unk03;  // SetTextureImage size?
    /* 0x04 */ f32 unk04; // should this be a pointer to an array of images (sprite set)?
    /* 0x08 */ u16 unk08;
    /* 0x0A */ u16 unk0A;
    ///* 0x0C */ f32 unk0C;
    /* 0x0C */ u16 unk0C;
    /* 0x0E */ u16 unk0E;
    /* 0x10 */ s32 unk10; // could be f32??
    /* 0x14 */ f32 unk14;
    // next three part of vec3f?
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ void** unk2C; // image pointers?
    /* 0x30 */ u16 unk30;    // command flags?
    /* 0x32 */ u8 unk32;     // texture image format?
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ u16 unk38;
    /* 0x3A */ u16 unk3A;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ u8 pad44[0x4C - 0x48];
    /* 0x4C */ u32 unk4C;
    /* 0x50 */ u32 unk50; // primitive color ?
    /* 0x54 */ u32 unk54;
    /* 0x58 */ u32 unk58;  // env color ?
    /* 0x5C */ u32 unk5C;  // blend color ?
    /* 0x60 */ u32 unk60; // light 1 color?
    /* 0x64 */ u32 unk64; // light 2 color?
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
} MObjSub; // size == 0x78

typedef struct MObj {
    /* 0x00 */ struct MObj* next;
    /* 0x04 */ u32 pad04;
    /* 0x08 */ struct MObjSub unk08;
    /* 0x80 */ u16 unk80;
    /* 0x82 */ u16 unk82; // frame number?
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ u32 pad8C;
    /* 0x90 */ struct AObj* aobjList;
    /* 0x94 */ union AnimCmd* unk94;
    /* 0x98 */ f32 unk98;
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ u32 padA4;
} MObj; // size = 0xA8

typedef struct SObj {
    /* 0x00 */ struct SObj* nextFree;
    /* 0x04 */ struct GObjCommon* obj;
    /* 0x08 */ struct SObj* next;
    /* 0x0C */ struct SObj* prev;
    /* 0x10 */ Sprite sp;
    /* 0x54 */ s32 unk54;
} SObj; // size >= 0x58

typedef struct OMCamera {
    /* 0x00 */ struct OMCamera* nextFree;
    /* 0x04 */ struct GObjCommon* obj;
    /* 0x08 */ Vp vp;
    /* 0x18 */ union {
        struct Mtx6Float f6;
        struct Mtx7Float f7;
    } unk18;
    /* 0x38 */ struct Mtx3x3Float unk38;
    /* 0x60 */ s32 numMatrices; // len of unk64
    /* 0x64 */ struct OMMtx* matrices[2];
    /* 0x6C */ struct AObj* aobjs;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ s32 unk80; // attr flags?
    /* 0x84 */ s32 unk84; // color?
    /* 0x88 */ void (*unk88)(struct OMCamera*, s32);
    /* 0x8C */ s32 unk8C;
} OMCamera; // size >= 0x90

typedef struct {
    /* 0x00 */ struct GObjThread* threads;
    /* 0x04 */ s32 numThreads;
    /* 0x08 */ u32 threadStackSize;
    /* 0x0C */ struct ThreadStackNode* stacks;
    /* 0x10 */ u32 numStacks;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ GObjProcess* processes;
    /* 0x1C */ s32 numProcesses;
    /* 0x20 */ struct GObjCommon* commons;
    /* 0x24 */ s32 numObjects;
    /* 0x28 */ s32 objectSize;
    /* 0x2C */ OMMtx* matrices;
    /* 0x30 */ s32 numMatrices;
    /* 0x34 */ void* cleanupFn; // void(*)(struct DObjDynamicStore *)
    /* 0x38 */ struct AObj* aobjs;
    /* 0x3C */ s32 numAObjs;
    /* 0x40 */ struct MObj* mobjs;
    /* 0x44 */ s32 numMObjs;
    /* 0x48 */ struct DObj* dobjs;
    /* 0x4C */ s32 numDObjs;
    /* 0x50 */ s32 dobjSize;
    /* 0x54 */ struct SObj* sobjs;
    /* 0x58 */ s32 numSObjs;
    /* 0x5C */ s32 sobjSize;
    /* 0x60 */ struct OMCamera* cameras;
    /* 0x64 */ s32 numCameras;
    /* 0x68 */ s32 cameraSize;
} OMSetup; // size == 0x6C

void func_8000A52C(GObjCommon* obj);
void om_link_gobj_dl_special(GObjCommon* obj, void (*renderFunc)(GObjCommon*), u32 dlPriority, s32 arg3, s32 arg4);
OMCamera* om_gobj_set_camera(GObjCommon* obj);
DObj* om_gobj_add_dobj(GObjCommon* obj, void* arg1);

extern GObjCommon* omCurrentObject;
extern GObjCommon* omGObjListHead[];
extern OSMesgQueue omProcessWaitQueue;
extern GObjProcess* omCurrentProcess;

#endif /* SYS_OM_H */
