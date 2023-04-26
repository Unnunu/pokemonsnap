#ifndef SYS_OM_H
#define SYS_OM_H

#include "PR/os.h"
#include "PR/mbi.h"

// Object Manager (OM) Objects

typedef struct GObjThread {
    /* 0x000 */ struct GObjThread *next;
    /* 0x008 */ OSThread osThread;
    /* 0x1B8 */ u64 *osStack;
    /* 0x1BC */ u32 stackSize;
} GObjThread; // size == 0x1C0

/// This is the node for the allocated stack for a `struct GObjThread`
typedef struct {
    /* 0x00 */ struct ThreadStackNode *next;
    /* 0x04 */ u32 stackSize;
    /* 0x08 */ u64 stack[1];
} ThreadStackNode; // size == 0x08 + VLA

typedef struct GObjProcess {
    /* 0x00 */ struct GObjProcess *unk00;
    /* 0x04 */ struct GObjProcess *unk04;
    /* 0x08 */ struct GObjProcess *unk08;
    /* 0x08 */ struct GObjProcess *unk0C;
    /* 0x10 */ s32 unk10; // priority
    /* 0x14 */ u8 unk14;  // kind
    /* 0x15 */ u8 unk15;
    /* 0x18 */ struct GObjCommon *unk18;
    // following two fields are typed via unk14 / kind
    /* 0x1C */ union {
        struct GObjThread *thread;
        void (*cb)(struct GObjCommon *);
    } unk1C;
    /* 0x20 */ void *unk20;
} GObjProcess; // size == 0x24

typedef struct OMMtx {
    /* 0x00 */ struct OMMtx *next;
    /* 0x04 */ u8 unk04; // kind
    /* 0x05 */ u8 unk05;
    /* 0x08 */ Mtx unk08;
    ///* 0x08 */ f32 unk08[4][4];
    ///* 0x08 */ f32 (*unk08)[4][4];
    ///* 0x0C */ u8 pad0C[0x48 - 0xc];
} OMMtx; // size == 0x48

typedef struct AObj {
    /* 0x00 */ struct AObj *next;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    // interpolation control struct?
    /* 0x20 */ void *unk20;
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
    /* 0x2C */ void **unk2C; // image pointers?
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
    /* 0x50 */ u8 unk50; // primitive color r?
    /* 0x51 */ u8 unk51; // g?
    /* 0x51 */ u8 unk52; // b?
    /* 0x51 */ u8 unk53; // a?
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 pad56;
    /* 0x57 */ u8 pad57;
    /* 0x58 */ u8 unk58;  // env color r?
    /* 0x59 */ u8 unk59;  // g?
    /* 0x5A */ u8 unk5A;  // b?
    /* 0x5B */ u8 unk5B;  // a?
    /* 0x5C */ u8 unk5C;  // blend color r?
    /* 0x5D */ u8 unk5D;  // g?
    /* 0x5E */ u8 unk5E;  // b?
    /* 0x5F */ u8 unk5F;  // a?
    /* 0x60 */ s32 unk60; // light 1 color?
    /* 0x64 */ s32 unk64; // light 2 color?
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
} MObjSub; // size == 0x78

typedef struct MObj {
    /* 0x00 */ struct MObj *next;
    /* 0x04 */ u32 pad04;
    /* 0x08 */ struct MObjSub unk08;
    /* 0x80 */ u16 unk80;
    /* 0x82 */ u16 unk82; // frame number?
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ u32 pad8C;
    /* 0x90 */ struct AObj *unk90;
    /* 0x94 */ union AnimCmd *unk94;
    /* 0x98 */ f32 unk98;
    /* 0x9C */ f32 unk9C;
    /* 0xA0 */ f32 unkA0;
    /* 0xA4 */ u32 padA4;
} MObj; // size = 0xA8

typedef struct {
    /* 0x00 */ struct GObjThread *threads;
    /* 0x04 */ s32 numThreads;
    /* 0x08 */ u32 threadStackSize;
    /* 0x0C */ struct ThreadStackNode *stacks;
    /* 0x10 */ u32 numStacks;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ GObjProcess *processes;
    /* 0x1C */ s32 numProcesses;
    /* 0x20 */ struct GObjCommon *commons;
    /* 0x24 */ s32 numCommons;
    /* 0x28 */ s32 commonSize;
    /* 0x2C */ OMMtx *matrices;
    /* 0x30 */ s32 numMatrices;
    /* 0x34 */ void *cleanupFn; // void(*)(struct DObjDynamicStore *)
    /* 0x38 */ struct AObj *aobjs;
    /* 0x3C */ s32 numAObjs;
    /* 0x40 */ struct MObj *mobjs;
    /* 0x44 */ s32 numMObjs;
    /* 0x48 */ struct DObj *dobjs;
    /* 0x4C */ s32 numDObjs;
    /* 0x50 */ s32 dobjSize;
    /* 0x54 */ struct SObj *sobjs;
    /* 0x58 */ s32 numSObjs;
    /* 0x5C */ s32 sobjSize;
    /* 0x60 */ struct OMCamera *cameras;
    /* 0x64 */ s32 numCameras;
    /* 0x68 */ s32 cameraSize;
} OMSetup; // size == 0x6C

#endif /* SYS_OM_H */
