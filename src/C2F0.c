#include "sys/gtl.h"
#include "sys/om.h"

s32 func_8000BAA4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8000BD44(s32);
void func_8000BD78(s32);

s32 padding1[4];
GObjCmd* _D_8004AF10;

GObjCmd* get_gobj_cmd(void) {
    GObjCmd* ret;

    if (_D_8004AF10 != NULL) {
        ret = _D_8004AF10;
        _D_8004AF10 = _D_8004AF10->next;
    } else {
        ret = hal_alloc(sizeof(GObjCmd), 4);
    }
    return ret;
}

void func_8000B740(GObj_Sub3CList* arg0) {
    GObjCmd* last = arg0->tail;
    if (last != NULL) {
        last->next = _D_8004AF10;
        _D_8004AF10 = arg0->head;
        arg0->head = NULL;
        arg0->tail = NULL;
    }
}

s32 func_8000B774(GObjCommon* obj, s32 cmd, GObjCommon* arg2) {
    GObjCmd* temp_v0;

    if (obj == NULL) {
        obj = omCurrentObject;
    }
    if (arg2 == NULL) {
        arg2 = omCurrentObject;
    }
    temp_v0 = get_gobj_cmd();
    temp_v0->cmd = cmd;
    temp_v0->obj = arg2;

    if (obj->sub3C.head == NULL) {
        obj->sub3C.tail = temp_v0;
        obj->sub3C.head = temp_v0;
    } else {
        obj->sub3C.tail->next = temp_v0;
        obj->sub3C.tail = temp_v0;
    }
    temp_v0->next = NULL;
    obj->sub3C.count++;
    return 0;
}

struct SignalParam {
    GObjCommon* obj;
    s32 cmd; 
};

void func_8000B808(GObjCommon* obj, struct SignalParam* param) {
    func_8000B774(obj, param->cmd, param->obj);
}

void sendSignalToLink(s32 arg0, s32 cmd, GObjCommon* arg2) {
    struct SignalParam param;

    if (arg2 != 0) {
        param.obj = arg2;
    } else {
        param.obj = omCurrentObject;
    }
    param.cmd = cmd;
    func_8000BAA4(arg0, func_8000B808, &param, FALSE);
}

#ifdef NON_MATCHING
s32 func_8000B880(void (*handler)(GObjCommon*, s32)) {
    GObj_Sub3CList* temp_t7 = &omCurrentObject->sub3C;
    GObjCmd* cur = temp_t7->head;

    while (cur != NULL) {
        switch (cur->cmd) {
            case -1:
                func_8000A52C(NULL);
                return 1;
            case -2:
                func_8000BD44(NULL);
                break;
            case -3:
                func_8000BD78(NULL);
                break;
            case -4:
                omCurrentObject->flags |= GOBJ_FLAG_1;
                break;
            case -5:
                omCurrentObject->flags &= ~GOBJ_FLAG_1;
                break;
            default:
                if (handler != NULL) {
                    handler(cur->obj, cur->cmd);
                }
                break;
        }
        cur = cur->next;
    }

    func_8000B740(temp_t7);
    temp_t7->count = 0;
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/C2F0/func_8000B880.s")
s32 func_8000B880(void (*handler)(GObjCommon*, s32));
#endif

void func_8000B998(void) {
    _D_8004AF10 = NULL;
}
