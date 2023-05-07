#include "common.h"

s32 func_8000B880(void (*handler)(GObjCommon*, s32));
OMMtx* om_dobj_append_mtx(DObj* dobj, u8 kind, u8 arg2);
OMMtx* om_camera_add_mtx(OMCamera* arg0, u8 kind, u8 arg2);
void func_80009C94(DObj* obj);
void om_link_gobj_dl(GObjCommon* obj, void (*arg1)(GObjCommon*), u8 dlLink, s32 dlPriority, s32 arg4);
SObj* om_gobj_add_sprite(GObjCommon* arg0, Sprite* sprite);

DObj* func_8000C550(DObj* obj);

void func_8000B9B0(s32 link, void (*cb)(GObjCommon*, void*), void* param) {
    GObjCommon* curr;
    GObjCommon* next;

    curr = omGObjListHead[link];
    while (curr != NULL) {
        next = curr->next;
        cb(curr, param);
        curr = next;
    }
}

void func_8000BA18(void (*cb)(GObjCommon*, void*), void* param) {
    GObjCommon* curr;
    GObjCommon* next;
    s32 link;

    for (link = 0; link < 32; link++) {
        curr = omGObjListHead[link];
        while (curr != NULL) {
            next = curr->next;
            cb(curr, param);
            curr = next;
        }
    }
}

s32 func_8000BAA4(s32 link, s32 (*cb)(GObjCommon*, void*), void* param, s32 getFirst) {
    GObjCommon* curr;
    GObjCommon* next;
    s32 ret = 0;

    curr = omGObjListHead[link];
    while (curr != NULL) {
        s32 retVal;

        next = curr->next;
        retVal = cb(curr, param);

        if (retVal != 0) {
            ret = retVal;
            if (getFirst == TRUE) {
                return ret;
            }
        }
        curr = next;
    }

    return ret;
}

s32 func_8000BB4C(s32 (*cb)(GObjCommon*, void*), void* param, s32 getFirst) {
    GObjCommon* curr;
    GObjCommon* next;
    s32 link;
    s32 ret = 0;

    for (link = 0; link < 32; link++) {
        curr = omGObjListHead[link];
        while (curr != NULL) {
            s32 retVal;

            next = curr->next;
            retVal = cb(curr, param);

            if (retVal != 0) {
                ret = retVal;
                if (getFirst == TRUE) {
                    return ret;
                }
            }
            curr = next;
        }
    }

    return ret;
}

GObjCommon* func_8000BC0C(GObjCommon* obj, void* id) {
    return obj->id == (u32)id ? obj : NULL;
}

GObjCommon* func_8000BC2C(s32 link, u32 id) {
    return func_8000BAA4(link, func_8000BC0C, (void*)id, TRUE);
}

GObjCommon* func_8000BC58(u32 id) {
    return func_8000BB4C(func_8000BC0C, id, TRUE);
}

void func_8000BC84(UNUSED u32 arg0) {
    func_8000B880(NULL);
}

void func_8000BCA8(s32 timesToStop) {
    // todo: main.h STACK_PROBE_MAGIC
    if (omCurrentProcess->unk1C.thread->osStack[7] != 0xFEDCBA98) {
        fatal_printf("gobjthread stack over  gobjid = %d\n", omCurrentProcess->object->id);
    }

    while (timesToStop) {
        osSendMesg(&omProcessWaitQueue, (OSMesg)1, OS_MESG_NOBLOCK);
        osStopThread(NULL);
        timesToStop--;
    }
}

void func_8000BD44(GObjCommon* obj) {
    GObjProcess* proc;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    proc = obj->processListHead;
    while (proc != NULL) {
        proc->frozen = 1;
        proc = proc->next;
    }
}

void func_8000BD78(GObjCommon* obj) {
    GObjProcess* proc;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    proc = obj->processListHead;
    while (proc != NULL) {
        proc->frozen = 0;
        proc = proc->next;
    }
}

void func_8000BDAC(GObjProcess* proc) {
    if (proc == NULL) {
        proc = omCurrentProcess;
    }

    proc->frozen = 1;
}

void func_8000BDC4(GObjProcess* proc) {
    if (proc == NULL) {
        proc = omCurrentProcess;
    }

    proc->frozen = 0;
}

void func_8000BDDC(GObjCommon* obj, void (*function)(GObjCommon*)) {
    GObjProcess* proc;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    proc = obj->processListHead;
    while (proc != NULL) {
        if (proc->function == function) {
            proc->frozen = 1;
        }
        proc = proc->next;
    }
}

void func_8000BE1C(GObjCommon* obj, void (*function)(GObjCommon*)) {
    GObjProcess* proc;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    proc = obj->processListHead;
    while (proc != NULL) {
        if (proc->function == function) {
            proc->frozen = 0;
        }
        proc = proc->next;
    }
}

void omh_end_all_object_processes(GObjCommon* obj) {
    GObjProcess *curr, *next;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    curr = obj->processListHead;
    while (curr != NULL) {
        next = curr->next;
        om_end_process(curr);
        curr = next;
    }
}

void func_8000BEAC(DObj* dobj) {
    om_dobj_append_mtx(dobj, 18, 0);
    om_dobj_append_mtx(dobj, 21, 0);
    om_dobj_append_mtx(dobj, 32, 0);
}

void func_8000BEF4(DObj* dobj) {
    om_dobj_append_mtx(dobj, 18, 0);
    om_dobj_append_mtx(dobj, 26, 0);
    om_dobj_append_mtx(dobj, 32, 0);
}

void func_8000BF3C(OMCamera* camera) {
    om_camera_add_mtx(camera, 3, 0);
    om_camera_add_mtx(camera, 6, 0);
}

void func_8000BF74(GObjCommon* obj) {
    DObj* curr;

    curr = obj->children;
    while (curr != NULL) {
        func_80009C94(curr);
        curr = func_8000C550(curr);
    }
}

DObj* func_8000BFB8(GObjCommon* obj, void* arg1) {
    DObj* ret;

    ret = func_80009D70(obj, arg1);
    func_8000BEAC(ret);

    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000BFE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C018.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C0A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C0D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C1CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C220.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C274.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C2E4.s")

GObjCommon* func_8000C37C(s32 objId, void (*objFnUpdate)(GObjCommon*), s32 objLink, s32 objPriority,
                          void (*fnRender)(GObjCommon*), u8 dlLink, s32 dlPriority, s32 dlArg, Sprite* sprite, u8 procKind,
                          void (*procFunc)(GObjCommon*), s32 procPriority) {
    GObjCommon* obj;

    obj = om_add_gobj(objId, objFnUpdate, objLink, objPriority);
    if (obj == NULL) {
        return NULL;
    }

    om_link_gobj_dl(obj, fnRender, dlLink, dlPriority, dlArg);
    om_gobj_add_sprite(obj, sprite);
    if (procFunc != 0) {
        om_create_process(obj, procFunc, procKind, procPriority);
    }
    return obj;
}

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C3FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C4B0.s")
