#ifndef SYS_OM_H
#define SYS_OM_H

#include "PR/os.h"

// Object Manager (OM) Objects

typedef struct {
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

#endif /* SYS_OM_H */