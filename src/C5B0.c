#include "common.h"

s32 func_8000B880(void (*handler)(GObjCommon*, s32));
OMMtx* om_dobj_append_mtx(DObj* dobj, u8 kind, u8 arg2);
OMMtx* om_camera_add_mtx(OMCamera* arg0, u8 kind, u8 arg2);
void om_dobj_remove_all_mobj(DObj* obj);
void om_link_gobj_dl(GObjCommon* obj, void (*arg1)(GObjCommon*), u8 dlLink, s32 dlPriority, s32 cameraTag);
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

void func_8000BC84(UNUSED GObjCommon* arg0) {
    func_8000B880(NULL);
}

void func_8000BCA8(s32 numFrames) {
    // todo: main.h STACK_PROBE_MAGIC
    if (omCurrentProcess->unk1C.thread->osStack[7] != 0xFEDCBA98) {
        fatal_printf("gobjthread stack over  gobjid = %d\n", omCurrentProcess->object->id);
    }

    while (numFrames) {
        osSendMesg(&omProcessWaitQueue, (OSMesg)1, OS_MESG_NOBLOCK);
        osStopThread(NULL);
        numFrames--;
    }
}

void func_8000BD44(GObjCommon* obj) {
    GObjProcess* proc;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    proc = obj->processListHead;
    while (proc != NULL) {
        proc->paused = 1;
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
        proc->paused = 0;
        proc = proc->next;
    }
}

void func_8000BDAC(GObjProcess* proc) {
    if (proc == NULL) {
        proc = omCurrentProcess;
    }

    proc->paused = 1;
}

void func_8000BDC4(GObjProcess* proc) {
    if (proc == NULL) {
        proc = omCurrentProcess;
    }

    proc->paused = 0;
}

void func_8000BDDC(GObjCommon* obj, void (*function)(GObjCommon*)) {
    GObjProcess* proc;

    if (obj == NULL) {
        obj = omCurrentObject;
    }

    proc = obj->processListHead;
    while (proc != NULL) {
        if (proc->function == function) {
            proc->paused = TRUE;
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
            proc->paused = FALSE;
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
    om_dobj_append_mtx(dobj, MTX_TYPE_TRANSLATE, 0);
    om_dobj_append_mtx(dobj, MTX_TYPE_ROTATE_RPY_DEG, 0);
    om_dobj_append_mtx(dobj, MTX_TYPE_SCALE, 0);
}

void func_8000BEF4(DObj* dobj) {
    om_dobj_append_mtx(dobj, MTX_TYPE_TRANSLATE, 0);
    om_dobj_append_mtx(dobj, MTX_TYPE_ROTATE_RPY, 0);
    om_dobj_append_mtx(dobj, MTX_TYPE_SCALE, 0);
}

void func_8000BF3C(OMCamera* camera) {
    om_camera_add_mtx(camera, MTX_TYPE_PERSP_FAST, 0);
    om_camera_add_mtx(camera, MTX_TYPE_LOOKAT, 0);
}

void func_8000BF74(GObjCommon* obj) {
    DObj* curr;

    curr = obj->children;
    while (curr != NULL) {
        om_dobj_remove_all_mobj(curr);
        curr = func_8000C550(curr);
    }
}

DObj* func_8000BFB8(GObjCommon* obj, void* arg1) {
    DObj* ret = om_gobj_add_dobj(obj, arg1);
    func_8000BEAC(ret);
    return ret;
}

s32 func_8000BFE8(DObj* dobj, void* arg1) {
    DObj* ret = om_dobj_add_sibling(dobj, arg1);
    func_8000BEAC(ret);
    return ret;
}

s32 func_8000C018(DObj* dobj, void* arg1) {
    DObj* ret = om_dobj_add_child(dobj, arg1);
    func_8000BEAC(ret);
    return ret;
}

DObj* func_8000C048(GObjCommon* obj, void* arg1) {
    DObj* ret = om_gobj_add_dobj(obj, arg1);
    func_8000BEF4(ret);
    return ret;
}

s32 func_8000C078(DObj* dobj, void* arg1) {
    DObj* ret = om_dobj_add_sibling(dobj, arg1);
    func_8000BEF4(ret);
    return ret;
}

s32 func_8000C0A8(DObj* dobj, void* arg1) {
    DObj* ret = om_dobj_add_child(dobj, arg1);
    func_8000BEF4(ret);
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/C5B0/func_8000C0D8.s")

void func_8000C1CC(GObjCommon* obj) {
    if (obj == NULL) {
        obj = omCurrentObject;
    }
    while (obj->children != NULL) {
        om_dobj_remove(obj->children);
    }
}

void func_8000C220(GObjCommon* obj) {
    if (obj == NULL) {
        obj = omCurrentObject;
    }
    while (obj->children != NULL) {
        om_gobj_remove_sprite(obj->children);
    }
}

void func_8000C274(void) {
    GObjCommon* curr;
    GObjCommon* next;
    s32 i;

    for (i = 0; i < 32; i++) {
        curr = omGObjListHead[i];
        while (curr != NULL) {
            next = curr->next;
            func_8000A52C(curr);
            curr = next;
        }
    }
}

s32 func_8000C2E4(s32 objId, void (*objFnUpdate)(GObjCommon*), s32 objLink, s32 objPriority,
                  void (*fnRender)(GObjCommon*), u8 dlLink, s32 dlPriority, s32 dlArg, void* dobjBP, s32 arg9,
                  u8 procKind, void (*procFunc)(GObjCommon*), s32 procPriority) {
    GObjCommon* obj;
    DObj* dobj;

    obj = om_add_gobj(objId, objFnUpdate, objLink, objPriority);
    if (obj == NULL) {
        return NULL;
    }

    om_link_gobj_dl(obj, fnRender, dlLink, dlPriority, dlArg);

    dobj = om_gobj_add_dobj(obj, dobjBP);
    if (arg9) {
        func_8000BEAC(dobj);
    }
    if (procFunc != NULL) {
        om_create_process(obj, procFunc, procKind, procPriority);
    }
    return obj;
}

GObjCommon* create_sprite(s32 objId, void (*objFnUpdate)(GObjCommon*), s32 objLink, s32 objPriority,
                          void (*fnRender)(GObjCommon*), u8 dlLink, s32 dlPriority, s32 cameraTag, Sprite* sprite,
                          u8 procKind, void (*procFunc)(GObjCommon*), s32 procPriority) {
    GObjCommon* obj;

    obj = om_add_gobj(objId, objFnUpdate, objLink, objPriority);
    if (obj == NULL) {
        return NULL;
    }

    om_link_gobj_dl(obj, fnRender, dlLink, dlPriority, cameraTag);
    om_gobj_add_sprite(obj, sprite);
    if (procFunc != NULL) {
        om_create_process(obj, procFunc, procKind, procPriority);
    }
    return obj;
}

GObjCommon* create_camera(s32 objId, void (*objFnUpdate)(GObjCommon*), s32 objLink, s32 objPriority,
                  void (*fnRender)(GObjCommon*), s32 dlPriority, s32 dlLinkBitMask, s32 cameraTag, s32 defaultMatrices,
                  u8 procKind, void (*procFunc)(GObjCommon*), s32 procPriority, s32 defaultFlags) {
    GObjCommon* obj;
    OMCamera* cam;

    obj = om_add_gobj(objId, objFnUpdate, objLink, objPriority);
    if (obj == NULL) {
        return NULL;
    }
    om_link_gobj_dl_camera(obj, fnRender, dlPriority, dlLinkBitMask, cameraTag);
    cam = om_gobj_set_camera(obj);
    if (defaultMatrices) {
        func_8000BF3C(cam);
    }
    if (procFunc != NULL) {
        om_create_process(obj, procFunc, procKind, procPriority);
    }
    if (defaultFlags) {
        cam->flags = 7;
        cam->unk84 = 0xFF;
    }
    return obj;
}

GObjCommon* func_8000C4B0(s32 link, s32 priority, s32 dlPriority, s32 flags, s32 bgColor) {
    GObjCommon* obj;
    OMCamera* cam;

    obj = create_camera(-1, func_8000BC84, link, priority, func_800191D8, dlPriority, 0, 0, FALSE, 0, NULL, 0, FALSE);
    if (obj == NULL) {
        return NULL;
    }
    cam = obj->children;
    cam->flags = flags;
    cam->unk84 = bgColor;
    return obj;
}
