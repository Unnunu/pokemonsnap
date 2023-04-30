#include "sys/om.h"
#include "sys/gtl.h"
#include "sys/crash.h"
#include "macros.h"

void func_80007CBC(Vp*);
// TODO: header
void func_8000BE5C(GObjCommon*);
void func_8000B740(s32*);
void func_80015448(void);
void func_80018CD0(s32);
void func_8000B998(void);

typedef struct ThreadStackList {
    /* 0x00 */ struct ThreadStackList* next;
    /* 0x04 */ ThreadStackNode* stack;
    /* 0x08 */ u32 size;
} ThreadStackList; // size == 0x0C

struct Unk80046A88 {
    /* 0x00 */ u8 unk00;
    /* 0x04 */ Gfx* unk04[4];
}; // sizeof == 0x14

GObjThread* omFreeThreadList;
s32 omActiveThreads;
s32 omActiveStacks;
u32 omDefaultStackSize;
s32 omD_8004A9A0;
ThreadStackList* omFreeStackList;
void (*omD_8004A9A8)(GObjProcess*);
GObjProcess* omFreeProcessList;
GObjProcess* omGProcessList[12]; // indexed by priority
s32 omActiveProcesses;
GObjCommon* omGObjListHead[32]; // indexed by link
GObjCommon* omGObjListTail[32];
GObjCommon* omFreeGObjList;
GObjCommon* omGObjListDlHead[33]; // indexed by dlLink
GObjCommon* omGObjListDlTail[33];
s32 omActiveObjects;
u16 omGObjSize;
s16 omMaxObjects;
OMMtx* omFreeMtxList;
s32 omActiveMatrices;
void (*omD_8004AC0C)(void*);
AObj* omFreeAObjList;
s32 omActiveAObj;
MObj* omD_8004AC18;
s32 omD_8004AC1C;
DObj* omD_8004AC20;
s32 omD_8004AC24;
u16 omD_8004AC28;
SObj* omD_8004AC2C;
s32 omD_8004AC30;
u16 omD_8004AC34;
OMCamera* omD_8004AC38;
s32 omD_8004AC3C;
u16 omD_8004AC40;
GObjCommon* omCurrentObject;
void* omD_8004AC48;
void* omD_8004AC4C;
GObjProcess* omCurrentProcess;
s32 omD_8004AC54;
OSMesg omD_8004AC58[1];
OSMesgQueue omD_8004AC60;
struct Unk80046A88 omD_8004AC78[32];

// data
s32 omThreadCounter = 10000000;
struct Mtx6Float omD_80040E84 = { NULL, { 0.0, 30.0, 4.0 / 3.0, 100.0, 12800.0, 1.0 } };
struct Mtx7Float omD_80040EA0 = { NULL, { -160.0, 160.0, -120.0, 120.0, 100.0, 12800.0, 1.0 } };
struct Mtx3x3Float omD_80040EC0 = { NULL, { { 1500.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 } } };
union Mtx3fi omD_80040EE8 = { { NULL, { 0.0, 0.0, 0.0 } } };
struct Mtx4Float omD_80040EF8 = { NULL, { 0.0, 0.0, 0.0, 1.0 } };
struct Mtx4Float omD_80040F0C = { NULL, { 0.0, 0.0, 0.0, 0.0 } };
struct Mtx3Float omD_80040F20 = { NULL, { 1.0, 1.0, 1.0 } };

GObjThread* om_get_obj_thread(void) {
    GObjThread* ret;

    if (omFreeThreadList == NULL) {
        omFreeThreadList = hal_alloc(sizeof(GObjThread), 8);
    }

    if (omFreeThreadList == NULL) {
        fatal_printf("om : couldn't get GObjThread\n");
        while (TRUE) {}
    }

    ret = omFreeThreadList;
    omFreeThreadList = omFreeThreadList->next;
    omActiveThreads++;

    return ret;
}

void om_free_obj_thread(GObjThread* t) {
    t->next = omFreeThreadList;
    omFreeThreadList = t;
    omActiveThreads--;
}

ThreadStackNode* om_get_stack_of_size(u32 size) {
    ThreadStackList* curr;
    ThreadStackList* prev;
    ThreadStackNode* ret;

    curr = omFreeStackList;
    prev = NULL;
    while (curr != NULL) {
        if (curr->size == size) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    // stack of size 'size' not found
    if (curr == NULL) {
        curr = hal_alloc(sizeof(ThreadStackList), 4);
        curr->next = NULL;
        curr->stack = NULL;
        curr->size = size;

        // add it to the end of the list
        if (prev != NULL) {
            prev->next = curr;
        } else {
            omFreeStackList = curr;
        }
    }

    if (curr->stack != NULL) {
        ret = curr->stack;
        curr->stack = curr->stack->next;
    } else {
        ret = hal_alloc(size + 8 /*offsetof(ThreadStackNode, stack)*/, 8);
        ret->stackSize = size;
    }

    ret->next = NULL;
    omActiveStacks++;
    return ret;
}

ThreadStackNode* om_get_default_stack(void) {
    return om_get_stack_of_size(omDefaultStackSize);
}

void om_free_stack(ThreadStackNode* node) {
    ThreadStackList* parent;

    parent = omFreeStackList;
    while (parent != NULL) {
        if (parent->size == node->stackSize) {
            break;
        }
        parent = parent->next;
    }

    if (parent == NULL) {
        fatal_printf("om : Illegal GObjThreadStack Link\n");
        while (TRUE) {}
    }

    node->next = parent->stack;
    parent->stack = node;
    omActiveStacks--;
}

GObjProcess* om_get_obj_process(void) {
    GObjProcess* ret;

    if (omFreeProcessList == NULL) {
        omFreeProcessList = hal_alloc(sizeof(GObjProcess), 4);
    }

    if (omFreeProcessList == NULL) {
        fatal_printf("om : couldn't get GObjProcess\n");
        while (TRUE) {}
    }

    ret = omFreeProcessList;
    omFreeProcessList = omFreeProcessList->next;
    omActiveProcesses++;

    return ret;
}

void om_link_process(GObjProcess* proc) {
    GObjCommon* obj = proc->object;
    GObjCommon* curObj = proc->object;
    s32 curLink = curObj->link;
    curObj = curObj; // required to match

    // find place in priority list
    // here it is assumed that objects with lower link and priority have processes with lower priority
    while (TRUE) {
        while (curObj != NULL) {
            GObjProcess* curProc = curObj->processListTail;
            while (curProc != NULL) {
                if (curProc->priority == proc->priority) {
                    proc->nextInPriorityList = curProc->nextInPriorityList;
                    curProc->nextInPriorityList = proc;
                    proc->prevInPriorityList = curProc;
                    goto END;
                }
                curProc = curProc->prev;
            }
            curObj = curObj->prev;
        }
        if (curLink != 0) {
            curObj = omGObjListTail[--curLink];
        } else {
            // curLink == 0
            proc->nextInPriorityList = omGProcessList[proc->priority];
            omGProcessList[proc->priority] = proc;
            proc->prevInPriorityList = NULL;
            goto END;
        }
    }
END:
    if (proc->nextInPriorityList != NULL) {
        proc->nextInPriorityList->prevInPriorityList = proc;
    }

    // add to object process list
    if (obj->processListTail != NULL) {
        obj->processListTail->next = proc;
    } else {
        obj->processListHead = proc;
    }
    
    proc->prev = obj->processListTail;
    proc->next = NULL;
    obj->processListTail = proc;
}

void om_free_process(GObjProcess* proc) {
    proc->next = omFreeProcessList;
    omFreeProcessList = proc;
    omActiveProcesses--;
}

void om_unlink_process_from_priority_list(GObjProcess* proc) {
    if (proc->prevInPriorityList != NULL) {
        proc->prevInPriorityList->nextInPriorityList = proc->nextInPriorityList;
    } else {
        omGProcessList[proc->priority] = proc->nextInPriorityList;
    }

    if (proc->nextInPriorityList != NULL) {
        proc->nextInPriorityList->prevInPriorityList = proc->prevInPriorityList;
    }
}

void om_unlink_process(GObjProcess* proc) {
    GObjCommon* obj = proc->object;

    om_unlink_process_from_priority_list(proc);
    if (proc->prev != NULL) {
        proc->prev->next = proc->next;
    } else {
        obj->processListHead = proc->next;
    }

    if (proc->next != NULL) {
        proc->next->prev = proc->prev;
    } else {
        obj->processListTail = proc->prev;
    }
}

GObjProcess* om_get_default_process(void) {
    return omCurrentProcess;
}

u64* om_get_process_stack(GObjProcess* proc) {
    if (proc == NULL) {
        proc = omCurrentProcess;
    }

    if (proc != NULL && proc->kind == 0) {
        return proc->unk1C.thread->osStack;
    }

    return NULL;
}

s32 om_get_process_stack_size(GObjProcess* arg0) {
    if (arg0 == NULL) {
        arg0 = omCurrentProcess;
    }

    if (arg0 != NULL && arg0->kind == 0) {
        return arg0->unk1C.thread->stackSize;
    }

    return NULL;
}

void func_80008354(void (*arg0)(GObjProcess*)) {
    omD_8004A9A8 = arg0;
}

s32 om_get_total_gobj_number(void) {
    GObjCommon* curr = omFreeGObjList;
    s32 i = 0;

    while (curr != NULL) {
        i++;
        curr = curr->next;
    }

    return i + omActiveObjects;
}

GObjCommon* om_get_gobj(void) {
    GObjCommon* ret;

    if (omMaxObjects == -1 || omActiveObjects < omMaxObjects) {
        ret = omFreeGObjList;
        if (ret == NULL) {
            omFreeGObjList = hal_alloc(omGObjSize, 8);
            ret = omFreeGObjList;
        }
    } else {
        fatal_printf("om : limit over get GObj\n");
        while (TRUE) {}
    }

    if (ret == NULL) {
        fatal_printf("om : couldn't get GObj\n");
        while (TRUE) {}
    }
    if (ret == NULL) {
        return NULL;
    }
    omFreeGObjList = ret->next;
    omActiveObjects++;

    return ret;
}

void om_free_gobj(GObjCommon* obj) {
    obj->next = omFreeGObjList;
    omFreeGObjList = obj;
    omActiveObjects--;
}

void om_link_gobj_after(GObjCommon* obj, GObjCommon* prevObj) {
    obj->prev = prevObj;
    if (prevObj != NULL) {
        obj->next = prevObj->next;
        prevObj->next = obj;
    } else {
        obj->next = omGObjListHead[obj->link];
        omGObjListHead[obj->link] = obj;
    }

    if (obj->next != NULL) {
        obj->next->prev = obj;
    } else {
        omGObjListTail[obj->link] = obj;
    }
}

void om_link_gobj_after_same_priority(GObjCommon* obj) {
    GObjCommon* curr;

    curr = omGObjListTail[obj->link];
    while (curr != NULL && curr->priority < obj->priority) {
        curr = curr->prev;
    }

    om_link_gobj_after(obj, curr);
}

void om_link_gobj_before_same_priority(GObjCommon* obj) {
    GObjCommon* curr;
    GObjCommon* beforeCurr;

    curr = omGObjListHead[obj->link];
    while (curr != NULL && obj->priority < curr->priority) {
        curr = curr->next;
    }

    if (curr != NULL) {
        beforeCurr = curr->prev;
    } else {
        beforeCurr = omGObjListTail[obj->link];
    }

    om_link_gobj_after(obj, beforeCurr);
}

void om_unlink_gobj(GObjCommon* obj) {
    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    } else {
        omGObjListHead[obj->link] = obj->next;
    }

    if (obj->next != NULL) {
        obj->next->prev = obj->prev;
    } else {
        omGObjListTail[obj->link] = obj->prev;
    }
}

void om_link_gobj_dl(GObjCommon* obj, GObjCommon* prevObj) {
    obj->prevDl = prevObj;

    if (prevObj != NULL) {
        obj->nextDl = prevObj->nextDl;
        prevObj->nextDl = obj;
    } else {
        obj->nextDl = omGObjListDlHead[obj->dlLink];
        omGObjListDlHead[obj->dlLink] = obj;
    }

    if (obj->nextDl != NULL) {
        obj->nextDl->prevDl = obj;
    } else {
        omGObjListDlTail[obj->dlLink] = obj;
    }
}

void om_link_gobj_dl_after_same_priority(GObjCommon* obj) {
    GObjCommon* csr;

    csr = omGObjListDlTail[obj->dlLink];
    while (csr != NULL && csr->dlPriority < obj->dlPriority) {
        csr = csr->prevDl;
    }
    om_link_gobj_dl(obj, csr);
}

void om_link_gobj_dl_before_same_priority(GObjCommon* obj) {
    GObjCommon* curr;
    GObjCommon* found;

    curr = omGObjListDlHead[obj->dlLink];
    while (curr != NULL && obj->dlPriority < curr->dlPriority) {
        curr = curr->nextDl;
    }

    if (curr != NULL) {
        found = curr->prevDl;
    } else {
        found = omGObjListDlTail[obj->dlLink];
    }

    om_link_gobj_dl(obj, found);
}

void om_unlink_gobj_dl(GObjCommon* obj) {
    if (obj->prevDl != NULL) {
        obj->prevDl->nextDl = obj->nextDl;
    } else {
        omGObjListDlHead[obj->dlLink] = obj->nextDl;
    }

    if (obj->nextDl != NULL) {
        obj->nextDl->prevDl = obj->prevDl;
    } else {
        omGObjListDlTail[obj->dlLink] = obj->prevDl;
    }
}

OMMtx* om_get_mtx(void) {
    OMMtx* ret;

    if (omFreeMtxList == NULL) {
        omFreeMtxList = hal_alloc(sizeof(OMMtx), 8);
    }

    if (omFreeMtxList == NULL) {
        fatal_printf("om : couldn't get OMMtx\n");
        while (TRUE) {}
    }

    ret = omFreeMtxList;
    omFreeMtxList = omFreeMtxList->next;
    omActiveMatrices++;
    return ret;
}

void om_free_mtx(OMMtx* mtx) {
    mtx->next = omFreeMtxList;
    omFreeMtxList = mtx;
    omActiveMatrices--;
}

AObj* om_get_aobj(void) {
    AObj* ret;

    if (omFreeAObjList == NULL) {
        omFreeAObjList = hal_alloc(sizeof(AObj), 4);
    }

    if (omFreeAObjList == NULL) {
        fatal_printf("om : couldn't get AObj\n");
        while (TRUE) {}
    }

    ret = omFreeAObjList;
    omFreeAObjList = omFreeAObjList->next;
    omActiveAObj++;
    return ret;
}

void om_dobj_attach_aobj(DObj* dobj, AObj* aobj) {
    aobj->next = dobj->aobjList;
    dobj->aobjList = aobj;
}

void om_mobj_attacj_aobj(MObj* mobj, AObj* aobj) {
    aobj->next = mobj->aobjList;
    mobj->aobjList = aobj;
}

// `arg0` could be another object type? SObj maybe
// copy of `append_aobj_to_dobj`
void func_80008950(DObj* arg0, AObj* arg1) {
    arg1->next = arg0->aobjList;
    arg0->aobjList = arg1;
}

void func_80008960(AObj* aobj) {
    aobj->next = omFreeAObjList;
    omFreeAObjList = aobj;
    omActiveAObj--;
}

MObj* func_8000898C(void) {
    MObj* ret;

    if (omD_8004AC18 == NULL) {
        omD_8004AC18 = hal_alloc(sizeof(MObj), 4);
    }

    if (omD_8004AC18 == NULL) {
        fatal_printf("om : couldn't get MObj\n");
        while (TRUE) {}
    }

    ret = omD_8004AC18;
    omD_8004AC18 = omD_8004AC18->next;
    omD_8004AC1C++;

    return ret;
}

void func_80008A04(MObj* obj) {
    obj->next = omD_8004AC18;
    omD_8004AC18 = obj;
    omD_8004AC1C--;
}

DObj* func_80008A30(void) {
    DObj* ret;

    if (omD_8004AC20 == NULL) {
        omD_8004AC20 = hal_alloc(omD_8004AC28, 8);
    }

    if (omD_8004AC20 == NULL) {
        fatal_printf("om : couldn't get DObj\n");
        while (TRUE) {}
    }

    ret = omD_8004AC20;
    omD_8004AC20 = omD_8004AC20->unk0;
    omD_8004AC24++;

    return ret;
}

void func_80008AAC(MObj* obj) {
    obj->next = omD_8004AC20;
    omD_8004AC20 = obj;
    omD_8004AC24--;
}

SObj* func_80008AD8(void) {
    SObj* ret;

    if (omD_8004AC2C == NULL) {
        omD_8004AC2C = hal_alloc(omD_8004AC34, 8);
    }

    if (omD_8004AC2C == NULL) {
        fatal_printf("om : couldn't get SObj\n");
        while (TRUE) {}
    }

    ret = omD_8004AC2C;
    omD_8004AC2C = omD_8004AC2C->next;
    omD_8004AC30++;

    return ret;
}

void func_80008B54(SObj* obj) {
    obj->next = omD_8004AC2C;
    omD_8004AC2C = obj;
    omD_8004AC30--;
}

OMCamera* func_80008B80(void) {
    OMCamera* ret;

    if (omD_8004AC38 == NULL) {
        omD_8004AC38 = hal_alloc(omD_8004AC40, 8);
    }

    if (omD_8004AC38 == NULL) {
        fatal_printf("om : couldn't get Camera\n");
        while (TRUE) {}
    }

    ret = omD_8004AC38;
    omD_8004AC38 = omD_8004AC38->next;
    omD_8004AC3C++;

    return ret;
}

void func_80008BFC(OMCamera* obj) {
    obj->next = omD_8004AC38;
    omD_8004AC38 = obj;
    omD_8004AC3C--;
}

GObjProcess* om_create_process(GObjCommon* com, void* func, u8 kind, u32 pri) {
    ThreadStackNode* stackNode;
    GObjThread* thread;
    GObjProcess* process;

    if (com == NULL) {
        com = omCurrentObject;
    }

    process = om_get_obj_process();
    if (pri > 11) {
        fatal_printf("om : GObjProcess's priority is bad value\n");
        while (TRUE) {}
    }
    process->priority = pri;
    process->kind = kind;
    process->unk15 = 0;
    process->object = com;
    process->function = func;

    switch (kind) {
        case 0:
            thread = om_get_obj_thread();
            process->unk1C.thread = thread;

            stackNode = om_get_default_stack();
            thread->osStack = stackNode->stack;
            thread->stackSize = omDefaultStackSize;
            osCreateThread(&thread->osThread, omThreadCounter++, func, com, &thread->osStack[omDefaultStackSize / 8],
                           51);
            thread->osStack[7] = 0xFEDCBA98;

            if (omThreadCounter >= 20000000) {
                omThreadCounter = 10000000;
            }
            break;
        case 1:
            process->unk1C.cb = func;
            break;
        default:
            fatal_printf("om : GObjProcess's kind is bad value\n");
            while (TRUE) {}
    }

    om_link_process(process);
    return process;
}

GObjProcess* om_create_process_threaded(GObjCommon* obj, void (*entry)(void*), u32 pri, s32 threadId, u32 stackSize) {
    GObjProcess* process;
    GObjThread* thread;
    ThreadStackNode* stackNode;
    OSId tid;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    process = om_get_obj_process();

    if (pri > 11) {
        fatal_printf("om : GObjProcess's priority is bad value\n");
        while (TRUE) {}
    }

    process->priority = pri;
    process->unk15 = 0;
    process->object = obj;
    process->function = entry;

    process->unk1C.thread = thread = om_get_obj_thread();
    process->kind = 0;

    stackNode = stackSize == 0 ? om_get_default_stack() : om_get_stack_of_size(stackSize);
    thread->osStack = stackNode->stack;
    thread->stackSize = stackSize == 0 ? omDefaultStackSize : stackSize;
    tid = threadId != -1 ? threadId : omThreadCounter++;

    osCreateThread(&thread->osThread, tid, entry, obj, &thread->osStack[thread->stackSize / 8], 51);
    thread->osStack[7] = 0xFEDCBA98;
    if (omThreadCounter >= 20000000) {
        omThreadCounter = 10000000;
    }

    om_link_process(process);
    return process;
}

void om_end_process(GObjProcess* proc) {
    ThreadStackNode* tnode;

    if (proc == NULL || proc == omCurrentProcess) {
        omD_8004AC54 = 1;
        if (omCurrentProcess->kind == 0) {
            func_8000BCA8(1);
        }
        return;
    }

    if (omD_8004A9A8 != NULL) {
        omD_8004A9A8(proc);
    }

    switch (proc->kind) {
        case 0:
            osDestroyThread(&proc->unk1C.thread->osThread);
            // cast from stack pointer back to stack node
            tnode = (void*)((uintptr_t)(proc->unk1C.thread->osStack) - 8); /*offsetof(struct ThreadStackNode, stack));*/
            om_free_stack(tnode);
            om_free_obj_thread(proc->unk1C.thread);
            break;
        case 1:
            break;
    }

    om_unlink_process(proc);
    om_free_process(proc);
}

OMMtx* func_80008FFC(DObj* arg0, u8 arg1, u8 arg2, s32 arg3) {
    uintptr_t csr;
    union Mtx3fi* t2;
    struct Mtx4Float* t1;
    struct Mtx3Float* t4;
    s32 i;
    OMMtx* mtx;
    s32 t5 = arg0->unk56;

    if (arg0->unk56 == 5) {
        fatal_printf("om : couldn\'t add OMMtx for DObj\n");
        while (TRUE) {}
    }

    if (arg0->unk4C != NULL) {
        csr = (uintptr_t)arg0->unk4C->data;
        for (i = 0; i < 3; i++) {
            switch (arg0->unk4C->kinds[i]) {
                case 0:
                    break;
                case 1:
                    t2 = (void*)csr;
                    csr += sizeof(union Mtx3fi);
                    break;
                case 2:
                    t1 = (void*)csr;
                    csr += sizeof(struct Mtx4Float);
                    break;
                case 3:
                    t4 = (void*)csr;
                    csr += sizeof(struct Mtx3Float);
                    break;
            }
        }
    }

    for (; t5 > arg3; t5--) {
        arg0->unk58[t5] = arg0->unk58[t5 - 1];
    }
    arg0->unk56 += 1;

    arg0->unk58[arg3] = mtx = om_get_mtx();
    mtx->unk04 = arg1;

    switch (arg1) {
        case 18:
        case 34:
        case 36:
        case 38:
        case 40:
        case 55:
            arg0->unk18 = omD_80040EE8;
            arg0->unk18.f.mtx = mtx;
            break;
        case 19:
        case 23:
            arg0->unk28 = omD_80040EF8;
            arg0->unk28.mtx = mtx;
            break;
        case 20:
        case 24:
            arg0->unk18 = omD_80040EE8;
            arg0->unk28 = omD_80040EF8;
            arg0->unk18.f.mtx = mtx;
            arg0->unk28.mtx = mtx;
            break;
        case 21:
        case 26:
        case 29:
            arg0->unk28 = omD_80040F0C;
            arg0->unk28.mtx = mtx;
            break;
        case 22:
        case 27:
        case 30:
        case 51:
        case 52:
            arg0->unk18 = omD_80040EE8;
            arg0->unk28 = omD_80040F0C;
            arg0->unk18.f.mtx = mtx;
            arg0->unk28.mtx = mtx;
            break;
        case 25:
            arg0->unk18 = omD_80040EE8;
            arg0->unk28 = omD_80040EF8;
            arg0->unk3C = omD_80040F20;
            arg0->unk18.f.mtx = mtx;
            arg0->unk28.mtx = mtx;
            arg0->unk3C.mtx = mtx;
            break;
        case 28:
        case 31:
        case 54:
            arg0->unk18 = omD_80040EE8;
            arg0->unk28 = omD_80040F0C;
            arg0->unk3C = omD_80040F20;
            arg0->unk18.f.mtx = mtx;
            arg0->unk28.mtx = mtx;
            arg0->unk3C.mtx = mtx;
            break;
        case 32:
        case 43:
        case 44:
        case 47:
        case 48:
        case 49:
        case 50:
        case 53:
            arg0->unk3C = omD_80040F20;
            arg0->unk3C.mtx = mtx;
            break;
        case 45:
        case 46:
            arg0->unk28 = omD_80040EF8;
            arg0->unk3C = omD_80040F20;
            arg0->unk28.mtx = mtx;
            arg0->unk3C.mtx = mtx;
            break;
        case 56:
            *t2 = omD_80040EE8;
            t2->f.mtx = mtx;
            break;
        case 57:
            *t1 = omD_80040EF8;
            t1->mtx = mtx;
            break;
        case 58:
            *t1 = omD_80040F0C;
            t1->mtx = mtx;
            break;
        case 59:
            *t4 = omD_80040F20;
            t4->mtx = mtx;
            break;
        case 60:
            *t2 = omD_80040EE8;
            *t1 = omD_80040EF8;
            t2->f.mtx = t1->mtx = mtx;
            break;
        case 61:
            *t2 = omD_80040EE8;
            *t1 = omD_80040EF8;
            *t4 = omD_80040F20;
            t2->f.mtx = t1->mtx = t4->mtx = mtx;
            break;
        case 62:
            *t2 = omD_80040EE8;
            *t1 = omD_80040F0C;
            t2->f.mtx = t1->mtx = mtx;
            break;
        case 63:
            *t2 = omD_80040EE8;
            *t1 = omD_80040F0C;
            *t4 = omD_80040F20;
            t2->f.mtx = t1->mtx = t4->mtx = mtx;
            break;
        case 1:
        case 17:
            // empty branch?
            // could be for any and all cases between 1 and 17
            break;
    }

    mtx->unk05 = arg2;
    return mtx;
}

void* func_80009760(DObj* arg0, u8 arg1, u8 arg2) {
    return func_80008FFC(arg0, arg1, arg2, arg0->unk56);
}

OMMtx* func_80009790(OMCamera* arg0, u8 arg1, u8 arg2) {
    OMMtx* mtx;

    if (arg0->unk60 == 2) {
        fatal_printf("om : couldn't add OMMtx for Camera\n");
        while (TRUE) {}
    }

    mtx = om_get_mtx();

    arg0->unk64[arg0->unk60] = mtx;
    arg0->unk60++;
    mtx->unk04 = arg1;

    switch (arg1) {
        case 3:
        case 4:
            arg0->unk18.f6 = omD_80040E84;
            arg0->unk18.f6.mtx = mtx;
            break;
        case 5:
            arg0->unk18.f7 = omD_80040EA0;
            arg0->unk18.f7.mtx = mtx;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            arg0->unk38 = omD_80040EC0;
            arg0->unk38.mtx = mtx;
            break;
        case 1:
        case 2:
            break;
    }

    mtx->unk05 = arg2;
    return mtx;
}

AObj* func_80009918(DObj* dobj, u8 index) {
    AObj* aobj = om_get_aobj();

    aobj->unk04 = index;
    aobj->unk05 = 0;
    aobj->unk20 = NULL;
    aobj->unk1C = 0.0;
    aobj->unk18 = 0.0;
    aobj->unk14 = 0.0;
    aobj->unk10 = 0.0;
    aobj->unk0C = 0.0;
    aobj->unk08 = 1.0;

    om_dobj_attach_aobj(dobj, aobj);

    return aobj;
}

void func_80009984(DObj* arg0) {
    AObj* curr;
    AObj* origNext;

    curr = arg0->aobjList;
    while (curr != NULL) {
        origNext = curr->next;
        func_80008960(curr);
        curr = origNext;
    }
    arg0->aobjList = NULL;
    arg0->unk74 = FLOAT_NEG_MAX;
}

AObj* func_800099E4(MObj* mobj, u8 index) {
    AObj* aobj = om_get_aobj();

    aobj->unk04 = index;
    aobj->unk05 = 0;
    aobj->unk20 = NULL;
    aobj->unk1C = 0.0;
    aobj->unk18 = 0.0;
    aobj->unk14 = 0.0;
    aobj->unk10 = 0.0;
    aobj->unk0C = 0.0;
    aobj->unk08 = 1.0;

    om_mobj_attacj_aobj(mobj, aobj);

    return aobj;
}

void func_80009A50(MObj* mobj) {
    AObj* curr;
    AObj* origNext;

    curr = mobj->aobjList;
    while (curr != NULL) {
        origNext = curr->next;
        func_80008960(curr);
        curr = origNext;
    }
    mobj->aobjList = NULL;
    mobj->unk98 = FLOAT_NEG_MAX;
}

// might be another type? SObj; matches `func_80008950`
AObj* func_80009AB0(DObj* obj, u8 index) {
    AObj* aobj = om_get_aobj();

    aobj->unk04 = index;
    aobj->unk05 = 0;
    aobj->unk20 = NULL;
    aobj->unk1C = 0.0;
    aobj->unk18 = 0.0;
    aobj->unk14 = 0.0;
    aobj->unk10 = 0.0;
    aobj->unk0C = 0.0;
    aobj->unk08 = 1.0;

    func_80008950(obj, aobj);

    return aobj;
}

// could be dobj, but maybe it's another type; probably matches `func_80008950`
void func_80009B1C(DObj* dobj) {
    AObj* curr;
    AObj* origNext;

    curr = dobj->aobjList;
    while (curr != NULL) {
        origNext = curr->next;
        func_80008960(curr);
        curr = origNext;
    }
    dobj->aobjList = NULL;
    dobj->unk74 = FLOAT_NEG_MAX;
}

// nonmatching: regalloc and ordering for the final set of initialization statements
MObj* func_80009B7C(DObj* arg0, MObjSub* arg1) {
    MObj* mobj; // a1, v0?

    mobj = func_8000898C();

    if (arg0->unk80 != NULL) {
        MObj* curr = arg0->unk80->next;
        MObj* prior = arg0->unk80;
        while (curr != NULL) {
            prior = curr;
            curr = curr->next;
        }
        prior->next = mobj;
    } else {
        arg0->unk80 = mobj;
    }

    mobj->next = NULL;
    mobj->unk84 = (f32)arg1->unk54 / 255.0f;
    mobj->unk08 = *arg1;

    mobj->unk08.unk24 = arg1->unk14;
    mobj->unk08.unk28 = arg1->unk1C;
    mobj->unk80 = 0;
    mobj->unk82 = 0;
    mobj->unk88 = 0.f;
    mobj->aobjList = NULL;
    mobj->unk94 = NULL;
    mobj->unk98 = FLOAT_NEG_MAX;
    mobj->unk9C = 1.0f;
    mobj->unkA0 = 0.0f;

    return mobj;
}

void func_80009C94(DObj* obj) {
    MObj* currM;
    MObj* nextM;
    AObj* currA;
    AObj* nextA;

    currM = obj->unk80;
    while (currM != NULL) {
        currA = currM->aobjList;
        while (currA != NULL) {
            nextA = currA->next;
            func_80008960(currA);
            currA = nextA;
        }
        nextM = currM->next;
        func_80008A04(currM);
        currM = nextM;
    }
    obj->unk80 = NULL;
}

void func_80009D0C(DObj* arg0) {
    s32 i;

    arg0->unk4C = 0;
    arg0->unk54 = 0;
    arg0->unk55 = 0;
    arg0->unk56 = 0;

    for (i = 0; i < 5; i++) {
        arg0->unk58[i] = NULL;
    }
    arg0->aobjList = NULL;
    arg0->unk70 = NULL;
    arg0->unk74 = FLOAT_NEG_MAX;
    arg0->unk78 = 1.0;
    arg0->unk7C = 0.0;
    arg0->unk80 = 0;
    arg0->unk84 = 0;
}

DObj* func_80009D70(GObjCommon* arg0, void* arg1) {
    DObj* newObj;
    DObj* curr;

    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }

    newObj = func_80008A30();
    if (arg0->unk74 != NULL) {
        curr = arg0->unk74;

        while (curr->unk8 != NULL) {
            curr = curr->unk8;
        }

        curr->unk8 = newObj;
        newObj->unkC = curr;
    } else {
        arg0->unk0F = 1;
        arg0->unk74 = newObj;
        newObj->unkC = NULL;
    }

    newObj->unk4 = arg0;
    newObj->unk14 = (void*)1;
    newObj->unk8 = NULL;
    newObj->unk10 = NULL;
    newObj->unk50 = arg1;

    func_80009D0C(newObj);

    return newObj;
}

DObj* func_80009E20(DObj* arg0, void* arg1) {
    DObj* newObj;

    newObj = func_80008A30();
    if (arg0->unk8 != NULL) {
        arg0->unk8->unkC = newObj;
    }
    newObj->unkC = arg0;
    newObj->unk8 = arg0->unk8;
    arg0->unk8 = newObj;
    newObj->unk4 = arg0->unk4;
    newObj->unk14 = arg0->unk14;

    newObj->unk10 = NULL;
    newObj->unk50 = arg1;
    func_80009D0C(newObj);

    return newObj;
}

DObj* func_80009E94(DObj* arg0, void* arg1) {
    DObj* newObj;
    DObj* curr;

    newObj = func_80008A30();
    if (arg0->unk10 != NULL) {
        curr = arg0->unk10;

        while (curr->unk8 != NULL) {
            curr = curr->unk8;
        }

        curr->unk8 = newObj;
        newObj->unkC = curr;
    } else {
        arg0->unk10 = newObj;
        newObj->unkC = NULL;
    }

    newObj->unk4 = arg0->unk4;
    newObj->unk14 = arg0;
    newObj->unk10 = NULL;
    newObj->unk8 = NULL;
    newObj->unk50 = arg1;

    func_80009D0C(newObj);

    return newObj;
}

void func_80009F2C(DObj* arg0) {
    s32 i;
    AObj* currA;
    AObj* nextA;
    MObj* currM;
    MObj* nextM;

    while (arg0->unk10 != NULL) {
        func_80009F2C(arg0->unk10);
    }

    if ((uintptr_t)arg0->unk14 == 1) {
        if (arg0 == arg0->unk4->unk74) {
            arg0->unk4->unk74 = arg0->unk8;
            if (arg0->unk4->unk74 == NULL) {
                arg0->unk4->unk0F = 0;
            }
        }
    } else if (arg0 == arg0->unk14->unk10) {
        arg0->unk14->unk10 = arg0->unk8;
    }

    if (arg0->unkC != NULL) {
        arg0->unkC->unk8 = arg0->unk8;
    }

    if (arg0->unk8 != NULL) {
        arg0->unk8->unkC = arg0->unkC;
    }

    for (i = 0; i < ARRAY_COUNT(arg0->unk58); i++) {
        if (arg0->unk58[i] != NULL) {
            om_free_mtx(arg0->unk58[i]);
        }
    }

    if (arg0->unk4C != NULL && omD_8004AC0C != NULL) {
        omD_8004AC0C(arg0->unk4C);
    }

    currA = arg0->aobjList;
    while (currA != NULL) {
        nextA = currA->next;
        func_80008960(currA);
        currA = nextA;
    }

    // why not just call func_80009C94?
    currM = arg0->unk80;
    while (currM != NULL) {
        currA = currM->aobjList;
        while (currA != NULL) {
            nextA = currA->next;
            func_80008960(currA);
            currA = nextA;
        }
        nextM = currM->next;
        func_80008A04(currM);
        currM = nextM;
    }

    func_80008AAC(arg0);
}

SObj* func_8000A0B4(GObjCommon* arg0, Sprite* sprite) {
    SObj* newObj;

    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }

    newObj = func_80008AD8();

    if (arg0->unk74 != NULL) {
        SObj* tail = arg0->unk74;

        while (tail->unk08 != NULL) {
            tail = tail->unk08;
        }

        tail->unk08 = newObj;
        newObj->unk0C = tail;
    } else {
        arg0->unk0F = 2;
        arg0->unk74 = newObj;
        newObj->unk0C = NULL;
    }

    newObj->unk04 = arg0;
    newObj->unk08 = NULL;

    if (sprite != NULL) {
        newObj->sp = *sprite;
    }

    newObj->unk54 = 0;
    return newObj;
}

void func_8000A18C(SObj* obj) {
    if (obj == obj->unk04->unk74) {
        obj->unk04->unk74 = (void*)obj->unk08;
        if (obj->unk04->unk74 == NULL) {
            obj->unk04->unk0F = 0;
        }
    }

    if (obj->unk0C != NULL) {
        obj->unk0C->unk08 = obj->unk08;
    }

    if (obj->unk08 != NULL) {
        obj->unk08->unk0C = obj->unk0C;
    }

    func_80008B54(obj);
}

OMCamera* func_8000A200(GObjCommon* arg0) {
    s32 i;
    OMCamera* newCam;

    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    arg0->unk0F = 3;

    newCam = func_80008B80();
    arg0->unk74 = newCam;
    newCam->unk04 = arg0;
    func_80007CBC(&newCam->unk08);
    if (FALSE) {} // required to match
    newCam->unk60 = 0;
    for (i = 0; i < ARRAY_COUNT(newCam->unk64); i++) {
        newCam->unk64[i] = NULL;
    }
    newCam->unk80 = 0;
    newCam->unk84 = 0;
    newCam->unk88 = NULL;
    newCam->unk8C = 0;
    newCam->unk6C = NULL;
    newCam->unk70 = 0;
    newCam->unk74 = FLOAT_NEG_MAX;
    newCam->unk78 = 1.0;
    newCam->unk7C = 0.0;

    return newCam;
}

void func_8000A2B0(OMCamera* cam) {
    GObjCommon* ctx;
    s32 i;
    AObj* curr;
    AObj* next;

    ctx = cam->unk04;
    ctx->unk0F = 0;
    ctx->unk74 = NULL;

    for (i = 0; i < ARRAY_COUNT(cam->unk64); i++) {
        if (cam->unk64[i] != NULL) {
            om_free_mtx(cam->unk64[i]);
        }
    }

    curr = cam->unk6C;
    while (curr != NULL) {
        next = curr->next;
        func_80008960(curr);
        curr = next;
    }

    func_80008BFC(cam);
}

GObjCommon* om_add_gobj_common(u32 id, void (*arg1)(GObjCommon*), u8 link, u32 priority) {
    GObjCommon* obj;

    if (link >= 32) {
        fatal_printf("omGAddCommon() : link num over : link = %d : id = %d\n", link, id);
        while (TRUE) {}
    }

    obj = om_get_gobj();
    if (obj == NULL) {
        return NULL;
    }

    obj->id = id;
    obj->link = link;
    obj->priority = priority;
    obj->unk14 = arg1;
    obj->processListHead = NULL;
    obj->processListTail = NULL;
    obj->unk44 = 0;
    obj->pad3C = 0;
    obj->unk40 = 0;
    obj->unk_4C = 0.0f;
    obj->unk_50 = 0;
    obj->unk0F = 0;
    obj->unk74 = NULL;
    obj->dlLink = 33;

    if (FALSE) {} // required to match

    obj->unk_54 = 0;
    obj->unk_58 = 0;
    return obj;
}

GObjCommon* om_add_gobj(u32 id, void (*arg1)(GObjCommon*), u8 link, u32 priority) {
    GObjCommon* com = om_add_gobj_common(id, arg1, link, priority);

    if (com == NULL) {
        return NULL;
    }

    om_link_gobj_after_same_priority(com);

    return com;
}

GObjCommon* om_add_gobj_before_same_priority(u32 id, void (*arg1)(GObjCommon*), u8 link, u32 priority) {
    GObjCommon* com = om_add_gobj_common(id, arg1, link, priority);

    if (com == NULL) {
        return NULL;
    }

    om_link_gobj_before_same_priority(com);

    return com;
}

GObjCommon* om_add_gobj_after(u32 id, void (*arg1)(GObjCommon*), GObjCommon* arg2) {
    GObjCommon* com = om_add_gobj_common(id, arg1, arg2->link, arg2->priority);

    if (com == NULL) {
        return NULL;
    }

    om_link_gobj_after(com, arg2);

    return com;
}

GObjCommon* om_add_gobj_before(u32 id, void (*arg1)(GObjCommon*), GObjCommon* arg2) {
    GObjCommon* com = om_add_gobj_common(id, arg1, arg2->link, arg2->priority);

    if (com == NULL) {
        return NULL;
    }

    om_link_gobj_after(com, arg2->prev);

    return com;
}

void func_8000A52C(GObjCommon* obj) {
    if (obj == NULL || obj == omCurrentObject) {
        omD_8004AC54 = 2;
        return;
    }

    func_8000BE5C(obj);
    func_8000B740(&obj->pad3C);
    switch (obj->unk0F) {
        case 1:
            func_8000C1CC(obj);
            break;
        case 2:
            func_8000C220(obj);
            break;
        case 3:
            func_8000A2B0(obj->unk74);
            break;
    }

    if (obj->dlLink != 33) {
        om_unlink_gobj_dl(obj);
    }
    om_unlink_gobj(obj);
    om_free_gobj(obj);
}

void om_move_gobj_common(s32 moveType, GObjCommon* obj, u8 link, u32 priority, GObjCommon* refObj) {
    GObjProcess* csr;
    GObjProcess* procList;
    GObjProcess* next;

    if (link >= 32) {
        fatal_printf("omGMoveCommon() : link num over : link = %d : id = %d\n", link, obj->id);

        while (TRUE) {}
    }

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    procList = obj->processListHead;
    obj->processListHead = NULL;
    obj->processListTail = NULL;

    csr = procList;
    while (csr != NULL) {
        om_unlink_process_from_priority_list(csr);
        csr = csr->next;
    }
    om_unlink_gobj(obj);
    obj->link = link;
    obj->priority = priority;

    switch (moveType) {
        case 0:
            om_link_gobj_after_same_priority(obj);
            break;
        case 1:
            om_link_gobj_before_same_priority(obj);
            break;
        case 2:
            om_link_gobj_after(obj, refObj);
            break;
        case 3:
            om_link_gobj_after(obj, refObj->prev);
            break;
    }

    csr = procList;
    while (csr != NULL) {
        next = csr->next;
        om_link_process(csr);
        csr = next;
    }
}

void func_8000A740(GObjCommon* obj, u8 link, u32 priority) {
    om_move_gobj_common(0, obj, link, priority, NULL);
}

void func_8000A778(GObjCommon* arg0, u8 link, u32 arg2) {
    om_move_gobj_common(1, arg0, link, arg2, NULL);
}

void func_8000A7B0(GObjCommon* arg0, GObjCommon* arg1) {
    om_move_gobj_common(2, arg0, arg1->link, arg1->priority, arg1);
}

void func_8000A7EC(GObjCommon* arg0, GObjCommon* arg1) {
    om_move_gobj_common(3, arg0, arg1->link, arg1->priority, arg1);
}

void func_8000A828(GObjCommon* obj, void (*arg1)(GObjCommon*), u8 dlLink, s32 arg3, s32 arg4) {
    if (dlLink >= 32) {
        fatal_printf("omGLinkObjDLCommon() : dl_link num over : dl_link = %d : id = %d\n", dlLink, obj->id);
        while (TRUE) {}
    }

    obj->dlLink = dlLink;
    obj->dlPriority = arg3;
    obj->unk2C = arg1;
    obj->unk34 = arg4;
    obj->unk0E = gtlD_80040CF8 - 1;
}

void func_8000A8A4(GObjCommon* arg0, void (*arg1)(GObjCommon*), u8 dlLink, s32 arg3, s32 arg4) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A828(arg0, arg1, dlLink, arg3, arg4);
    om_link_gobj_dl_after_same_priority(arg0);
}

void func_8000A8E8(GObjCommon* arg0, void (*arg1)(GObjCommon*), u8 dlLink, s32 arg3, s32 arg4) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A828(arg0, arg1, dlLink, arg3, arg4);
    om_link_gobj_dl_before_same_priority(arg0);
}

void func_8000A92C(GObjCommon* arg0, void (*arg1)(GObjCommon*), s32 arg2, GObjCommon* arg3) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A828(arg0, arg1, arg3->dlLink, arg3->dlPriority, arg2);
    om_link_gobj_dl(arg0, arg3);
}

void func_8000A980(GObjCommon* arg0, void (*arg1)(struct GObjCommon*), s32 arg2, GObjCommon* arg3) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A828(arg0, arg1, arg3->dlLink, arg3->dlPriority, arg2);
    om_link_gobj_dl(arg0, arg3->prev);
}

void func_8000A9D8(GObjCommon* arg0, void (*arg1)(GObjCommon*), u32 arg2, s32 arg3, s32 arg4) {
    arg0->dlLink = 32;
    arg0->dlPriority = arg2;
    arg0->unk2C = arg1;
    arg0->unk30 = arg3;
    arg0->unk34 = arg4;
    arg0->unk38 = 0;
    arg0->unk0E = gtlD_80040CF8 - 1;
}

void func_8000AA0C(GObjCommon* arg0, void (*arg1)(GObjCommon*), u32 arg2, s32 arg3, s32 arg4) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A9D8(arg0, arg1, arg2, arg3, arg4);
    om_link_gobj_dl_after_same_priority(arg0);
}

void func_8000AA48(GObjCommon* arg0, void (*arg1)(GObjCommon*), u32 arg2, s32 arg3, s32 arg4) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A9D8(arg0, arg1, arg2, arg3, arg4);
    om_link_gobj_dl_before_same_priority(arg0);
}

void func_8000AA84(GObjCommon* arg0, void (*arg1)(GObjCommon*), s32 arg2, s32 arg3, GObjCommon* arg4) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A9D8(arg0, arg1, arg4->dlPriority, arg2, arg3);
    om_link_gobj_dl(arg0, arg4);
}

void func_8000AAD8(GObjCommon* arg0, void (*arg1)(GObjCommon*), s32 arg2, s32 arg3, GObjCommon* arg4) {
    if (arg0 == NULL) {
        arg0 = omCurrentObject;
    }
    func_8000A9D8(arg0, arg1, arg4->dlPriority, arg2, arg3);
    om_link_gobj_dl(arg0, arg4->prev);
}

void func_8000AB30(GObjCommon* arg0, u8 dlLink, u32 arg2) {
    if (dlLink >= 32) {
        fatal_printf("omGMoveObjDL() : dl_link num over : dl_link = %d : id = %d\n", dlLink, arg0->id);
        while (TRUE) {}
    }

    om_unlink_gobj_dl(arg0);
    arg0->dlLink = dlLink;
    arg0->dlPriority = arg2;
    om_link_gobj_dl_after_same_priority(arg0);
}

void func_8000ABAC(GObjCommon* arg0, u8 dlLink, u32 arg2) {
    if (dlLink >= 32) {
        fatal_printf("omGMoveObjDLHead() : dl_link num over : dl_link = %d : id = %d\n", dlLink, arg0->id);
        while (TRUE) {}
    }
    om_unlink_gobj_dl(arg0);
    arg0->dlLink = dlLink;
    arg0->dlPriority = arg2;
    om_link_gobj_dl_before_same_priority(arg0);
}

void func_8000AC28(GObjCommon* arg0, GObjCommon* arg1) {
    om_unlink_gobj_dl(arg0);
    arg0->dlLink = arg1->dlLink;
    arg0->dlPriority = arg1->dlPriority;
    om_link_gobj_dl(arg0, arg1);
}

void func_8000AC68(GObjCommon* arg0, GObjCommon* arg1) {
    om_unlink_gobj_dl(arg0);
    arg0->dlLink = arg1->dlLink;
    arg0->dlPriority = arg1->dlPriority;
    om_link_gobj_dl(arg0, arg1->prevDl);
}

void func_8000ACAC(GObjCommon* arg0, u32 arg1) {
    om_unlink_gobj_dl(arg0);
    arg0->dlPriority = arg1;
    om_link_gobj_dl_after_same_priority(arg0);
}

void func_8000ACE0(GObjCommon* arg0, u32 arg1) {
    om_unlink_gobj_dl(arg0);
    arg0->dlPriority = arg1;
    om_link_gobj_dl_before_same_priority(arg0);
}

void func_8000AD14(GObjCommon* arg0, GObjCommon* arg1) {
    om_unlink_gobj_dl(arg0);
    arg0->dlPriority = arg1->dlPriority;
    om_link_gobj_dl(arg0, arg1);
}

void func_8000AD4C(GObjCommon* arg0, GObjCommon* arg1) {
    om_unlink_gobj_dl(arg0);
    arg0->dlPriority = arg1->dlPriority;
    om_link_gobj_dl(arg0, arg1->prev);
}

void func_8000AD88(s32 n) {
    omMaxObjects = n;
}

s16 func_8000AD94(void) {
    return omMaxObjects;
}

void func_8000ADA0(void) {
    s32 i;
    GObjCommon* s0;

    omD_8004AC48 = NULL;
    omD_8004AC4C = NULL;

    for (i = 0; i < ARRAY_COUNT(omD_8004AC78); i++) {
        omD_8004AC78[i].unk00 = gtlD_80040CF8 - 1;
    }

    s0 = omGObjListDlHead[32];
    while (s0 != NULL) {
        if (!(s0->unk_50 & 1)) {
            omD_8004AC48 = s0;
            s0->unk2C(s0);
        }
        s0 = s0->nextDl;
    }
}

GObjCommon* func_8000AE4C(GObjCommon* arg0) {
    GObjCommon* ret;

    omCurrentObject = arg0;
    arg0->unk14(arg0);
    ret = arg0->next;
    omCurrentObject = NULL;

    switch (omD_8004AC54) {
        case 0:
            break;
        case 2:
            omD_8004AC54 = 0;
            func_8000A52C(arg0);
            break;
        default:
            omD_8004AC54 = 0;
            break;
    }

    return ret;
}

GObjProcess* om_run_process(GObjProcess* proc) {
    GObjProcess* ret;

    omCurrentObject = proc->object;
    omCurrentProcess = proc;

    switch (proc->kind) {
        case 0: {
            osStartThread(&proc->unk1C.thread->osThread);
            osRecvMesg(&omD_8004AC60, NULL, OS_MESG_BLOCK);
            break;
        }
        case 1: {
            proc->unk1C.cb(proc->object);
            break;
        }
    }

    ret = proc->nextInPriorityList;

    omCurrentObject = NULL;
    omCurrentProcess = NULL;

    switch (omD_8004AC54) {
        case 2:
            omD_8004AC54 = 0;
            while (ret != NULL && ret->object == proc->object) {
                ret = ret->nextInPriorityList;
            }

            func_8000A52C(proc->object);
            break;
        case 1:
            omD_8004AC54 = 0;
            om_end_process(proc);
            break;
        case 0:
            break;
        default:
            omD_8004AC54 = 0;
            break;
    }

    return ret;
}

void func_8000AFFC(void) {
    s32 i;
    GObjCommon* obj;
    GObjProcess* proc;

    omD_8004AC54 = 0;
    omCurrentObject = NULL;
    omCurrentProcess = NULL;

    for (i = 0; i < 32; i++) {
        obj = omGObjListHead[i];
        while (obj != NULL) {
            if (!(obj->unk_50 & 0x40) && obj->unk14 != NULL) {
                obj = func_8000AE4C(obj);
            } else {
                obj = obj->next;
            }
        }
    }

    for (i = ARRAY_COUNT(omGProcessList) - 1; i >= 0; i--) {
        proc = omGProcessList[i];
        while (proc != NULL) {
            if (proc->unk15 == 0) {
                proc = om_run_process(proc);
            } else {
                proc = proc->nextInPriorityList;
            }
        }
    }
}

void om_create_objects(OMSetup* setup) {
    s32 i;

    omDefaultStackSize = setup->threadStackSize;
    omD_8004A9A0 = setup->unk14;

    if (setup->numThreads != 0) {
        GObjThread* csr;
        omFreeThreadList = csr = setup->threads;

        for (i = 0; i < setup->numThreads - 1; i++) {
            GObjThread* next = csr + 1;

            csr->next = next;
            csr = next;
        }
        csr->next = NULL;
    } else {
        omFreeThreadList = NULL;
    }

    if (setup->numStacks != 0 && setup->threadStackSize != NULL) {
        ThreadStackNode* csr;

        omFreeStackList = hal_alloc(sizeof(ThreadStackList), 4);
        omFreeStackList->next = NULL;
        omFreeStackList->size = omDefaultStackSize;
        omFreeStackList->stack = csr = setup->stacks;

        for (i = 0; (u32)i < setup->numStacks - 1; i++) {
            csr->next = (void*)((uintptr_t)csr + omDefaultStackSize + 8); // offsetof(struct ThreadStackNode, stack));
            csr->stackSize = omDefaultStackSize;
            csr = (void*)((uintptr_t)csr + omDefaultStackSize + 8); // offsetof(struct ThreadStackNode, stack));
        }

        csr->stackSize = omDefaultStackSize;
        csr->next = NULL;
    } else {
        omFreeStackList = NULL;
    }

    if (setup->numProcesses != 0) {
        GObjProcess* csr;
        omFreeProcessList = csr = setup->processes;

        for (i = 0; i < setup->numProcesses - 1; i++) {
            GObjProcess* next = csr + 1;

            csr->next = next;
            csr = next;
        }

        csr->next = NULL;
    } else {
        omFreeProcessList = NULL;
    }

    for (i = 0; i < ARRAY_COUNT(omGProcessList); i++) {
        omGProcessList[i] = NULL;
    }

    if (setup->numCommons != 0) {
        GObjCommon* csr;
        omFreeGObjList = csr = setup->commons;

        // todo: is this the purest form?
        for (i = 0; i < setup->numCommons - 1; i++) {
            csr->next = (void*)((uintptr_t)csr + setup->commonSize);
            csr = csr->next;
        }
        csr->next = NULL;

    } else {
        omFreeGObjList = NULL;
    }

    omGObjSize = setup->commonSize;
    omMaxObjects = -1;
    omD_8004AC0C = setup->cleanupFn;

    if (setup->numMatrices != 0) {
        OMMtx* csr;

        omFreeMtxList = csr = setup->matrices;

        for (i = 0; i < setup->numMatrices - 1; i++) {
            OMMtx* next = csr + 1;

            csr->next = next;
            csr = next;
        }
        csr->next = NULL;
    } else {
        omFreeMtxList = NULL;
    }

    if (setup->numAObjs != 0) {
        AObj* csr;

        omFreeAObjList = csr = setup->aobjs;

        for (i = 0; i < setup->numAObjs - 1; i++) {
            AObj* next = csr + 1;

            csr->next = next;
            csr = next;
        }
        csr->next = NULL;
    } else {
        omFreeAObjList = NULL;
    }

    if (setup->numMObjs != 0) {
        MObj* csr;

        omD_8004AC18 = csr = setup->mobjs;

        for (i = 0; i < setup->numMObjs - 1; i++) {
            MObj* next = csr + 1;

            csr->next = next;
            csr = next;
        }
        csr->next = NULL;

    } else {
        omD_8004AC18 = NULL;
    }

    if (setup->numDObjs != 0) {
        DObj* csr;
        omD_8004AC20 = csr = setup->dobjs;

        for (i = 0; i < setup->numDObjs - 1; i++) {
            csr->unk0 = (void*)((uintptr_t)csr + setup->dobjSize);
            csr = csr->unk0;
        }

        csr->unk0 = NULL;
    } else {
        omD_8004AC20 = NULL;
    }
    omD_8004AC28 = setup->dobjSize;

    if (setup->numSObjs != 0) {
        SObj* csr;
        omD_8004AC2C = csr = setup->sobjs;

        for (i = 0; i < setup->numSObjs - 1; i++) {
            csr->next = (void*)((uintptr_t)csr + setup->sobjSize);
            csr = csr->next;
        }

        csr->next = NULL;
    } else {
        omD_8004AC2C = NULL;
    }
    omD_8004AC34 = setup->sobjSize;

    if (setup->numCameras != 0) {
        OMCamera* csr;
        omD_8004AC38 = csr = setup->cameras;

        for (i = 0; i < setup->numCameras - 1; i++) {
            csr->next = (void*)((uintptr_t)csr + setup->cameraSize);
            csr = csr->next;
        }

        csr->next = NULL;
    } else {
        omD_8004AC38 = NULL;
    }
    omD_8004AC40 = setup->cameraSize;

    for (i = 0; i < 32; i++) {
        omGObjListTail[i] = NULL;
        omGObjListHead[i] = NULL;
    }

    for (i = 0; i < 33; i++) {
        omGObjListDlTail[i] = NULL;
        omGObjListDlHead[i] = NULL;
    }

    func_80015448();
    osCreateMesgQueue(&omD_8004AC60, omD_8004AC58, ARRAY_COUNT(omD_8004AC58));

    omActiveStacks = omActiveThreads = omActiveProcesses = omActiveObjects = omActiveMatrices = omActiveAObj = omD_8004AC24 =
        omD_8004AC30 = omD_8004AC3C = 0;

    omD_8004A9A8 = NULL;
    func_80018CD0(0);
    func_8000B998();
}
