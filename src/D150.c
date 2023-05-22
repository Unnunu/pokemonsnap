#include "common.h"

#define LOWER_BOUND -1.7014117e38
#define PROCESSED_DEFAULT -1.1342745e38
#define FLOAT_MAYBE_SCALE_MIN -1.1342745e38f

union AnimCmd {
    u32 w;
    f32 f;
    void* ptr;
};

AObj* om_dobj_add_aobj(DObj* dobj, u8 index);
void om_dobj_reset_aobj_list(DObj* dobj);
void func_8001FCA4(Vec3f*, void*, f32);
OMMtx* om_dobj_append_mtx(DObj* dobj, u8 kind, u8 arg2);
AObj* func_80009AB0(DObj* obj, u8 index);
AObj* om_mobj_add_aobj(MObj* mobj, u8 index);

#ifdef NON_MATCHING
DObj* func_8000C550(DObj* obj) {
    DObj* child;
    DObj* next;
    DObj* parent;

    child = obj->childList;
    if (obj->childList != NULL) {
        return child;
    }
    next = obj->next;
    if (obj->next != NULL) {
        return next;
    }
    while (TRUE) {
        parent = obj->parent;
        if ((uintptr_t)parent == 1) {
            return NULL;
        }
        if (parent->next != NULL) {
            return parent->next;
        }
        obj = parent;
    }
    return obj;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/D150/func_8000C550.s")
DObj* func_8000C550(DObj* obj);
#endif

void func_8000C5B4(GObjCommon* obj, f32 arg1) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        dobj->unk78 = arg1;
        dobj = func_8000C550(dobj);
    }
}

void func_8000C5FC(GObjCommon* obj, f32 arg1) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        MObj* mobj = dobj->mobjList;
        dobj->unk78 = arg1;
        while (mobj != NULL) {
            mobj->unk9C = arg1;
            mobj = mobj->next;
        }
        dobj = func_8000C550(dobj);
    }
}

void func_8000C660(GObjCommon* obj, f32 arg1) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        MObj* mobj = dobj->mobjList;
        while (mobj != NULL) {
            mobj->unk9C = arg1;
            mobj = mobj->next;
        }
        dobj = func_8000C550(dobj);
    }
}

void func_8000C6C0(GObjCommon* obj) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        om_dobj_reset_aobj_list(dobj);
        dobj = func_8000C550(dobj);
    }
}

void func_8000C704(GObjCommon* obj) {
    DObj* dobj = obj->children;
    MObj* mobj;

    while (dobj != NULL) {
        om_dobj_reset_aobj_list(dobj);
        mobj = dobj->mobjList;
        while (mobj != NULL) {
            om_mobj_reset_aobj_list(mobj);
            mobj = mobj->next;
        }

        dobj = func_8000C550(dobj);
    }
}

void func_8000C76C(GObjCommon* obj) {
    DObj* dobj = obj->children;
    MObj* mobj;

    while (dobj != NULL) {
        mobj = dobj->mobjList;
        while (mobj != NULL) {
            om_mobj_reset_aobj_list(mobj);
            mobj = mobj->next;
        }

        dobj = func_8000C550(dobj);
    }
}

void func_8000C7CC(DObj* arg0, union AnimCmd* arg1, f32 arg2) {
    AObj* aobj = arg0->aobjList;

    while (aobj != NULL) {
        aobj->unk05 = 0;
        aobj = aobj->next;
    }

    arg0->unk70 = arg1;
    arg0->unk74 = LOWER_BOUND;
    arg0->unk7C = arg2;
}

void func_8000C804(MObj* arg0, union AnimCmd* arg1, f32 arg2) {
    AObj* aobj = arg0->aobjList;

    while (aobj != NULL) {
        aobj->unk05 = 0;
        aobj = aobj->next;
    }

    arg0->unk94 = arg1;
    arg0->unk98 = LOWER_BOUND;
    arg0->unkA0 = arg2;
}

void func_8000C83C(GObjCommon* arg0, union AnimCmd** arg1, f32 arg2) {
    DObj* dobj;
    u8 s2 = 1;

    dobj = arg0->children;
    arg0->unk_4C = arg2;
    while (dobj != NULL) {
        if (*arg1 != NULL) {
            func_8000C7CC(dobj, *arg1, arg2);
            dobj->unk55 = s2;
            s2 = 0;
        } else {
            dobj->unk74 = FLOAT_NEG_MAX;
            dobj->unk55 = 0;
        }

        arg1++;
        dobj = func_8000C550(dobj);
    }
}

void func_8000C8D8(GObjCommon* arg0, union AnimCmd*** arg1, f32 arg2) {
    DObj* dobj;
    MObj* mobj;
    union AnimCmd** cmdlist;

    dobj = arg0->children;
    arg0->unk_4C = arg2;
    while (dobj != NULL) {
        if (arg1 != NULL) {
            if (*arg1 != NULL) {
                cmdlist = *arg1;
                mobj = dobj->mobjList;
                while (mobj != NULL) {
                    if (*cmdlist != NULL) {
                        func_8000C804(mobj, *cmdlist, arg2);
                    }
                    cmdlist++;
                    mobj = mobj->next;
                }
            }
            arg1++;
        }
        dobj = func_8000C550(dobj);
    }
}

void func_8000C988(GObjCommon* arg0, union AnimCmd** arg1, union AnimCmd*** arg2, f32 arg3) {
    DObj* dobj = arg0->children;
    MObj* mobj;
    union AnimCmd** cmdlist;
    u8 s5 = 1;

    arg0->unk_4C = arg3;
    while (dobj != NULL) {
        if (arg1 != NULL) {
            if (*arg1 != NULL) {
                func_8000C7CC(dobj, *arg1, arg3);
                dobj->unk55 = s5;
                s5 = 0;
            } else {
                dobj->unk74 = FLOAT_NEG_MAX;
                dobj->unk55 = 0;
            }
            arg1++;
        }
        if (arg2 != NULL) {
            if (*arg2 != NULL) {
                cmdlist = *arg2;
                mobj = dobj->mobjList;
                while (mobj != NULL) {
                    if (*cmdlist != NULL) {
                        func_8000C804(mobj, *cmdlist, arg3);
                    }
                    cmdlist++;
                    mobj = mobj->next;
                }
            }
            arg2++;
        }
        dobj = func_8000C550(dobj);
    }
}

void func_8000CA98(DObj* dobj) {
    AObj* sp80[10];
    AObj* aobj;
    s32 i;
    u32 cmd;
    u32 subcmd;
    f32 payload;

    if (dobj->unk74 != (f32)FLOAT_NEG_MAX) {
        if (dobj->unk74 == (f32)LOWER_BOUND) {
            dobj->unk74 = -dobj->unk7C;
        } else {
            dobj->unk74 -= dobj->unk78;
            dobj->unk7C += dobj->unk78;
            dobj->obj->unk_4C = dobj->unk7C;

            if (dobj->unk74 > 0.0f) {
                return;
            }
        }

        for (i = 0; i < ARRAY_COUNT(sp80); i++) {
            sp80[i] = NULL;
        }

        aobj = dobj->aobjList;
        while (aobj != NULL) {
            if (aobj->unk04 > 0 && aobj->unk04 < ARRAY_COUNT(sp80) + 1) {
                sp80[aobj->unk04 - 1] = aobj;
            }
            aobj = aobj->next;
        }

        do {
            if (dobj->unk70 == NULL) {
                aobj = dobj->aobjList;
                while (aobj != NULL) {
                    if (aobj->unk05) {
                        aobj->unk0C += dobj->unk78 + dobj->unk74;
                    }
                    aobj = aobj->next;
                }
                dobj->unk7C = dobj->unk74;
                dobj->obj->unk_4C = dobj->unk74;
                dobj->unk74 = PROCESSED_DEFAULT;
                return;
            }

            cmd = dobj->unk70->w >> 25;
            switch (cmd) {
                case 8:
                case 9: {
                    payload = (f32)(dobj->unk70->w & 0x7FFF);
                    subcmd = (dobj->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = om_dobj_add_aobj(dobj, i + 1);
                            }

                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = dobj->unk70->f;
                            dobj->unk70++;
                            sp80[i]->unk18 = sp80[i]->unk1C;
                            sp80[i]->unk1C = 0.0f;
                            sp80[i]->unk05 = 3;
                            if (payload != 0.0f) {
                                sp80[i]->unk08 = 1.0f / payload;
                            }
                            sp80[i]->unk0C = -dobj->unk74 - dobj->unk78;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 8) {
                        dobj->unk74 += payload;
                    }
                    break;
                }
                case 3:
                case 4: {
                    payload = (f32)(dobj->unk70->w & 0x7FFF);
                    subcmd = (dobj->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = om_dobj_add_aobj(dobj, i + 1);
                            }
                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = dobj->unk70->f;
                            dobj->unk70++;
                            sp80[i]->unk05 = 2;
                            if (payload != 0.0f) {
                                sp80[i]->unk18 = (sp80[i]->unk14 - sp80[i]->unk10) / payload;
                            }
                            sp80[i]->unk0C = -dobj->unk74 - dobj->unk78;
                            sp80[i]->unk1C = 0.0f;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 3) {
                        dobj->unk74 += payload;
                    }
                    break;
                }
                case 5:
                case 6: {
                    payload = (f32)(dobj->unk70->w & 0x7FFF);
                    subcmd = (dobj->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = om_dobj_add_aobj(dobj, i + 1);
                            }

                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = dobj->unk70->f;
                            dobj->unk70++;
                            sp80[i]->unk18 = sp80[i]->unk1C;
                            sp80[i]->unk1C = dobj->unk70->f;
                            dobj->unk70++;
                            sp80[i]->unk05 = 3;
                            if (payload != 0.0f) {
                                sp80[i]->unk08 = 1.0f / payload;
                            }
                            sp80[i]->unk0C = -dobj->unk74 - dobj->unk78;
                        }

                        subcmd >>= 1;
                    }

                    if (cmd == 5) {
                        dobj->unk74 += payload;
                    }
                    break;
                }
                case 7: {
                    subcmd = (dobj->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = om_dobj_add_aobj(dobj, i + 1);
                            }

                            sp80[i]->unk1C = dobj->unk70->f;
                            dobj->unk70++;
                        }
                        subcmd >>= 1;
                    }
                    break;
                }
                case 2: {
                    dobj->unk74 += (f32)(dobj->unk70++->w & 0x7FFF);
                    break;
                }
                case 10:
                case 11: {
                    payload = (f32)(dobj->unk70->w & 0x7FFF);
                    subcmd = (dobj->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = om_dobj_add_aobj(dobj, i + 1);
                            }

                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = dobj->unk70->f;
                            dobj->unk70++;
                            sp80[i]->unk05 = 1;
                            sp80[i]->unk08 = payload;
                            sp80[i]->unk0C = -dobj->unk74 - dobj->unk78;
                            sp80[i]->unk1C = 0.0f;
                        }

                        subcmd >>= 1;
                    }

                    if (cmd == 10) {
                        dobj->unk74 += payload;
                    }
                    break;
                }
                case 14: {
                    dobj->unk70++;
                    dobj->unk70 = dobj->unk70->ptr;
                    dobj->unk7C = -dobj->unk74;
                    dobj->obj->unk_4C = -dobj->unk74;

                    if (dobj->unk55 != 0 && dobj->obj->unk_54 != NULL) {
                        dobj->obj->unk_54(dobj, -2, 0);
                    }
                    break;
                }
                case 1: {
                    dobj->unk70++;
                    dobj->unk70 = dobj->unk70->ptr;

                    if (dobj->unk55 != 0 && dobj->obj->unk_54 != NULL) {
                        dobj->obj->unk_54(dobj, -2, 0);
                    }
                    break;
                }
                case 12: {
                    payload = (f32)(dobj->unk70->w & 0x7FFF);
                    subcmd = (dobj->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = om_dobj_add_aobj(dobj, i + 1);
                            }

                            sp80[i]->unk0C += payload;
                        }
                        subcmd >>= 1;
                    }

                    break;
                }
                case 13: {
                    dobj->unk70++;
                    if (sp80[3] == NULL) {
                        sp80[3] = om_dobj_add_aobj(dobj, 3 + 1);
                    }
                    sp80[3]->unk20 = dobj->unk70->ptr;
                    dobj->unk70++;
                    break;
                }
                case 0: {
                    aobj = dobj->aobjList;
                    while (aobj != NULL) {
                        if (aobj->unk05 != 0) {
                            aobj->unk0C += dobj->unk78 + dobj->unk74;
                        }
                        aobj = aobj->next;
                    }

                    dobj->obj->unk_4C = dobj->unk7C;
                    dobj->unk74 = PROCESSED_DEFAULT;
                    if (dobj->unk55 != 0 && dobj->obj->unk_54 != NULL) {
                        dobj->obj->unk_54(dobj, -1, 0);
                    }
                    return; // not break
                }
                case 15: {
                    dobj->unk54 = (dobj->unk70->w << 7) >> 22;
                    dobj->unk74 += (f32)(dobj->unk70++->w & 0x7FFF);
                    break;
                }
                case 16: {
                    if (dobj->obj->unk_54 != NULL) {
                        // only seems to match when spelled out...
                        dobj->obj->unk_54(dobj, ((dobj->unk70->w << 7) >> 22) >> 8,
                                          ((dobj->unk70->w << 7) >> 22) & 0xFF);
                    }

                    dobj->unk74 += (f32)(dobj->unk70++->w & 0x7FFF);
                    ;
                    break;
                }
                case 17: {
                    subcmd = (dobj->unk70->w << 7) >> 22;
                    dobj->unk74 += (f32)(dobj->unk70++->w & 0x7FFF);

                    for (i = 4; i < 14; i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (dobj->obj->unk_54 != NULL) {
                                dobj->obj->unk_54(dobj, i, dobj->unk70->f);
                            }
                            dobj->unk70++;
                        }
                        subcmd >>= 1;
                    }
                    break;
                }
                // empty, but necessary
                default: {
                }
            }
        } while (dobj->unk74 <= 0.0f);
    }
}

f32 func_8000D4D4(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 temp_f10;
    f32 temp_f16;
    f32 temp_f18; // arg0^2
    f32 temp_f2;  // arg1^2

    temp_f2 = SQ(arg1);
    temp_f18 = SQ(arg0);
    temp_f16 = temp_f2 * arg1 * temp_f18; // arg1^3 * arg0^2
    temp_f10 = 2.0f * temp_f16 * arg0;    // 2.0f * arg1^3 * arg0^3
    sp14 = 3.0f * temp_f2 * temp_f18;     // 3 * arg1^2 * arg0^2
    sp18 = temp_f2 * arg0;                // arg0^3
    sp10 = temp_f16 - sp18;               // arg1^3 * arg0^2 - arg0^3

    return ((temp_f10 - sp14) + 1.0f) * arg2 + arg3 * (sp14 - temp_f10) + arg4 * ((sp10 - sp18) + arg1) + arg5 * sp10;
}

f32 func_8000D588(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 temp_f18;
    f32 temp_f16;
    f32 temp_f2;

    temp_f2 = 2.0f * arg1 * arg0;
    temp_f16 = 3.0f * arg1 * arg1 * arg0 * arg0;
    temp_f18 = 6.0f * arg1;

    return (((temp_f18 * arg1 * arg0 * arg0 * arg0) - (temp_f18 * arg0 * arg0)) * (arg2 - arg3)) +
           (arg4 * ((temp_f16 - (2.0f * temp_f2)) + 1.0f)) + (arg5 * (temp_f16 - temp_f2));
}

f32 func_8000D640(AObj* aobj) {
    switch (aobj->unk05) {
        case 2:
            return aobj->unk10 + (aobj->unk0C * aobj->unk18);
        case 3:
            return func_8000D4D4(aobj->unk08, aobj->unk0C, aobj->unk10, aobj->unk14, aobj->unk18, aobj->unk1C);
        case 1:
            return aobj->unk08 <= aobj->unk0C ? aobj->unk14 : aobj->unk10;
    }
}

f32 func_8000D6EC(AObj* aobj) {
    switch (aobj->unk05) {
        case 2:
            return aobj->unk18;
        case 3:
            return func_8000D588(aobj->unk08, aobj->unk0C, aobj->unk10, aobj->unk14, aobj->unk18, aobj->unk1C);
        case 1:
            return 0.0f;
    }
}

void func_8000D768(DObj* dobj) {
    f32 f26;
    AObj* aobj;

    if (dobj->unk74 != (f32)FLOAT_NEG_MAX) {
        aobj = dobj->aobjList;

        while (aobj != NULL) {
            if (aobj->unk05 != 0) {
                if (dobj->unk74 != FLOAT_MAYBE_SCALE_MIN) {
                    aobj->unk0C += dobj->unk78;
                }

                if (!(dobj->obj->unk_50 & 2)) {
                    switch (aobj->unk05) {
                        case 2:
                            f26 = aobj->unk10 + (aobj->unk0C * aobj->unk18);
                            break;
                        case 3: {
                            f32 temp_f16 = SQ(aobj->unk08);
                            f32 temp_f12 = SQ(aobj->unk0C);
                            f32 temp_f18 = aobj->unk08 * temp_f12;
                            f32 temp_f14 = aobj->unk0C * temp_f12 * temp_f16;
                            f32 temp_f20 = 2.0f * temp_f14 * aobj->unk08;
                            f32 temp_f22 = 3.0f * temp_f12 * temp_f16;
                            f32 temp_f24 = temp_f14 - temp_f18;

                            f26 = (aobj->unk10 * ((temp_f20 - temp_f22) + 1.0f)) +
                                  (aobj->unk14 * (temp_f22 - temp_f20)) +
                                  (aobj->unk18 * ((temp_f24 - temp_f18) + aobj->unk0C)) + (aobj->unk1C * temp_f24);
                            break;
                        }
                        case 1:
                            f26 = aobj->unk08 <= aobj->unk0C ? aobj->unk14 : aobj->unk10;
                            break;
                        default: {
                        }
                    }

                    switch (aobj->unk04) {
                        case 1:
                            dobj->unk28.f[1] = f26;
                            break;
                        case 2:
                            dobj->unk28.f[2] = f26;
                            break;
                        case 3:
                            dobj->unk28.f[3] = f26;
                            break;
                        case 4:
                            if (f26 < 0.0f) {
                                f26 = 0.0f;
                            } else {
                                if (f26 > 1.0f) {
                                    f26 = 1.0f;
                                }
                            }
                            func_8001FCA4(&dobj->unk18.f.v, aobj->unk20, f26);
                            break;
                        case 5:
                            dobj->unk18.f.v.x = f26;
                            break;
                        case 6:
                            dobj->unk18.f.v.y = f26;
                            break;
                        case 7:
                            dobj->unk18.f.v.z = f26;
                            break;
                        case 8:
                            dobj->unk3C.v.x = f26;
                            break;
                        case 9:
                            dobj->unk3C.v.y = f26;
                            break;
                        case 10:
                            dobj->unk3C.v.z = f26;
                            break;
                    }
                }
            }
            aobj = aobj->next;
        }

        if (dobj->unk74 == FLOAT_MAYBE_SCALE_MIN) {
            dobj->unk74 = FLOAT_NEG_MAX;
        }
    }
}

void func_8000DA18(MObj* mobj) {
    AObj* sp90[10];
    AObj* sp7C[5];
    AObj* aobj;
    s32 i;
    u32 cmd;
    u32 subcmd;
    f32 payload;

    if (mobj->unk98 != (f32)FLOAT_NEG_MAX) {
        if (mobj->unk98 == (f32)LOWER_BOUND) {
            mobj->unk98 = -mobj->unkA0;
        } else {
            mobj->unk98 -= mobj->unk9C;
            mobj->unkA0 += mobj->unk9C;

            if (mobj->unk98 > 0.0f) {
                return;
            }
        }

        // clang-format off
        for (i = 0; i < ARRAY_COUNT(sp90); i++) { 
            sp90[i] = NULL; 
        }

        for (i = 0; i < ARRAY_COUNT(sp7C); i++) { 
            sp7C[i] = NULL; 
        }
        // clang-format on

        aobj = mobj->aobjList;
        while (aobj != NULL) {
            if (aobj->unk04 >= 13 && aobj->unk04 < 23) {
                sp90[aobj->unk04 - 13] = aobj;
            }

            if (aobj->unk04 >= 37 && aobj->unk04 < 42) {
                sp7C[aobj->unk04 - 37] = aobj;
            }

            aobj = aobj->next;
        }

        do {
            if (mobj->unk94 == NULL) {
                aobj = mobj->aobjList;
                while (aobj != NULL) {
                    if (aobj->unk05 != 0) {
                        aobj->unk0C += mobj->unk9C + mobj->unk98;
                    }
                    aobj = aobj->next;
                }
                mobj->unkA0 = mobj->unk98;
                mobj->unk98 = PROCESSED_DEFAULT;

                break; // or return?
            }

            cmd = mobj->unk94->w >> 25;
            switch (cmd) {
                case 8:
                case 9: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp90); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp90[i] == NULL) {
                                sp90[i] = om_mobj_add_aobj(mobj, i + 13);
                            }
                            sp90[i]->unk10 = sp90[i]->unk14;
                            sp90[i]->unk14 = mobj->unk94->f;
                            mobj->unk94++;
                            sp90[i]->unk18 = sp90[i]->unk1C;
                            sp90[i]->unk1C = 0.0f;
                            sp90[i]->unk05 = 3;

                            if (payload != 0.0f) {
                                sp90[i]->unk08 = 1.0f / payload;
                            }
                            sp90[i]->unk0C = -mobj->unk98 - mobj->unk9C;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 8) {
                        mobj->unk98 += payload;
                    }
                    break;
                }
                case 3:
                case 4: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp90); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp90[i] == NULL) {
                                sp90[i] = om_mobj_add_aobj(mobj, i + 13);
                            }

                            sp90[i]->unk10 = sp90[i]->unk14;
                            sp90[i]->unk14 = mobj->unk94->f;
                            mobj->unk94++;
                            sp90[i]->unk05 = 2;
                            if (payload != 0.0f) {
                                sp90[i]->unk18 = (sp90[i]->unk14 - sp90[i]->unk10) / payload;
                            }
                            sp90[i]->unk0C = -mobj->unk98 - mobj->unk9C;
                            sp90[i]->unk1C = 0.0f;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 3) {
                        mobj->unk98 += payload;
                    }
                    break;
                }
                case 5:
                case 6: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp90); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp90[i] == NULL) {
                                sp90[i] = om_mobj_add_aobj(mobj, i + 13);
                            }
                            sp90[i]->unk10 = sp90[i]->unk14;
                            sp90[i]->unk14 = mobj->unk94->f;
                            mobj->unk94++;
                            sp90[i]->unk18 = sp90[i]->unk1C;
                            sp90[i]->unk1C = mobj->unk94->f;
                            mobj->unk94++;

                            sp90[i]->unk05 = 3;
                            if (payload != 0.0f) {
                                sp90[i]->unk08 = 1.0f / payload;
                            }
                            sp90[i]->unk0C = -mobj->unk98 - mobj->unk9C;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 5) {
                        mobj->unk98 += payload;
                    }
                    break;
                }
                case 7: {
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp90); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp90[i] == NULL) {
                                sp90[i] = om_mobj_add_aobj(mobj, i + 13);
                            }
                            sp90[i]->unk1C = mobj->unk94->f;
                            mobj->unk94++;
                        }
                        subcmd >>= 1;
                    }
                    break;
                }
                case 2: {
                    mobj->unk98 += (f32)(mobj->unk94->w & 0x7FFF);
                    mobj->unk94++;

                    break;
                }
                case 10:
                case 11: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp90); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp90[i] == NULL) {
                                sp90[i] = om_mobj_add_aobj(mobj, i + 13);
                            }
                            sp90[i]->unk10 = sp90[i]->unk14;
                            sp90[i]->unk14 = mobj->unk94->f;
                            mobj->unk94++;

                            sp90[i]->unk05 = 1;
                            sp90[i]->unk08 = payload;
                            sp90[i]->unk0C = -mobj->unk98 - mobj->unk9C;
                            sp90[i]->unk1C = 0.0f;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 10) {
                        mobj->unk98 += payload;
                    }
                    break;
                }
                case 14: {
                    mobj->unk94++;
                    mobj->unk94 = mobj->unk94->ptr;
                    mobj->unkA0 = -mobj->unk98;
                    break;
                }
                case 1: {
                    mobj->unk94++;
                    mobj->unk94 = mobj->unk94->ptr;
                    break;
                }
                case 12: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp90); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp90[i] == NULL) {
                                sp90[i] = om_mobj_add_aobj(mobj, i + 13);
                            }
                            sp90[i]->unk0C += payload;
                        }
                        subcmd >>= 1;
                    }

                    break;
                }
                case 0: {
                    aobj = mobj->aobjList;
                    while (aobj != NULL) {
                        if (aobj->unk05 != 0) {
                            aobj->unk0C += mobj->unk9C + mobj->unk98;
                        }
                        aobj = aobj->next;
                    }

                    mobj->unk98 = PROCESSED_DEFAULT;
                    return; // not break
                }
                case 18:
                case 19: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp7C); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp7C[i] == NULL) {
                                sp7C[i] = om_mobj_add_aobj(mobj, i + 37);
                            }
                            sp7C[i]->unk10 = sp7C[i]->unk14;
                            sp7C[i]->unk14 = mobj->unk94->f;
                            mobj->unk94++;

                            sp7C[i]->unk05 = 1;
                            sp7C[i]->unk08 = payload;
                            sp7C[i]->unk0C = -mobj->unk98 - mobj->unk9C;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 18) {
                        mobj->unk98 += payload;
                    }
                    break;
                }
                case 20:
                case 21: {
                    payload = (f32)(mobj->unk94->w & 0x7FFF);
                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp7C); i++) {
                        if (subcmd == 0) {
                            break;
                        }

                        if (subcmd & 1) {
                            if (sp7C[i] == NULL) {
                                sp7C[i] = om_mobj_add_aobj(mobj, i + 37);
                            }
                            sp7C[i]->unk10 = sp7C[i]->unk14;
                            sp7C[i]->unk14 = mobj->unk94->f;
                            mobj->unk94++;

                            sp7C[i]->unk05 = 2;
                            if (payload != 0.0f) {
                                sp7C[i]->unk08 = 1.0f / payload;
                            }
                            sp7C[i]->unk0C = -mobj->unk98 - mobj->unk9C;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 20) {
                        mobj->unk98 += payload;
                    }
                    break;
                }
                case 22: {
                    mobj->unk98 = (f32)(mobj->unk94->w & 0x7FFF);

                    subcmd = (mobj->unk94++->w << 7) >> 22;

                    if (subcmd & 0x01) {
                        mobj->unk08.unk4C = mobj->unk94->w;
                        mobj->unk94++;
                    }
                    if (subcmd & 0x02) {
                        mobj->unk08.unk6C = mobj->unk94->w;
                        mobj->unk94++;
                    }
                    if (subcmd & 0x04) {
                        mobj->unk08.unk68 = mobj->unk94->w;
                        mobj->unk94++;
                    }
                    if (subcmd & 0x08) {
                        mobj->unk08.unk74 = mobj->unk94->w;
                        mobj->unk94++;
                    }
                    if (subcmd & 0x10) {
                        mobj->unk08.unk70 = mobj->unk94->w;
                        mobj->unk94++;
                    }
                    break;
                }
                default: {
                }
            }
        } while (mobj->unk98 <= 0.0f);
    }
}

#ifdef NON_MATCHING
union Bytes4 {
    u8 b[4];
    u32 i;
};

union Bytes2 {
    u8 b[2];
    u16 h;
};

void func_8000E4E4(MObj* mobj) {
    AObj* aobj;
    f32 temp_f26;
    union Bytes4 color;
    s32 i;

    if (mobj->unk98 != (f32)FLOAT_NEG_MAX) {
        aobj = mobj->aobjList;
        while (aobj != NULL) {
            if (aobj->unk05 != 0) {
                if (mobj->unk98 != FLOAT_MAYBE_SCALE_MIN) {
                    aobj->unk0C += mobj->unk9C;
                }
                if (aobj->unk04 < 36) {
                    switch (aobj->unk05) {
                        case 2:
                            temp_f26 = aobj->unk10 + (aobj->unk0C * aobj->unk18);
                            break;
                        case 3: {
                            f32 temp_f16 = SQ(aobj->unk08);
                            f32 temp_f12 = SQ(aobj->unk0C);
                            f32 temp_f18 = aobj->unk08 * temp_f12;
                            f32 temp_f14 = aobj->unk0C * temp_f12 * temp_f16;
                            f32 temp_f20 = 2.0f * temp_f14 * aobj->unk08;
                            f32 temp_f22 = 3.0f * temp_f12 * temp_f16;
                            f32 temp_f24 = temp_f14 - temp_f18;

                            temp_f26 = (aobj->unk10 * ((temp_f20 - temp_f22) + 1.0f)) +
                                       (aobj->unk14 * (temp_f22 - temp_f20)) +
                                       (aobj->unk18 * ((temp_f24 - temp_f18) + aobj->unk0C)) + (aobj->unk1C * temp_f24);
                            break;
                        }
                        case 1:
                            temp_f26 = aobj->unk08 <= aobj->unk0C ? aobj->unk14 : aobj->unk10;
                            break;
                        default:
                            break;
                    }
                    switch (aobj->unk04) {
                        case 13:
                            mobj->unk80 = temp_f26;
                            break;
                        case 14:
                            mobj->unk08.unk14 = temp_f26;
                            break;
                        case 15:
                            mobj->unk08.unk18 = temp_f26;
                            break;
                        case 16:
                            mobj->unk08.unk1C = temp_f26;
                            break;
                        case 17:
                            mobj->unk08.unk20 = temp_f26;
                            break;
                        case 18:
                            mobj->unk82 = temp_f26;
                            break;
                        case 19:
                            mobj->unk08.unk3C = temp_f26;
                            break;
                        case 20:
                            mobj->unk08.unk40 = temp_f26;
                            break;
                        case 21:
                            mobj->unk84 = temp_f26;
                            break;
                        case 22:
                            mobj->unk88 = temp_f26;
                            break;
                        default:
                            break;
                    }
                } else {
                    if (aobj->unk05 != 1) {
                        if (aobj->unk05 == 2) {
                            s32 v1;

                            v1 = (s32)(aobj->unk0C * aobj->unk08 * 256.0f);
                            if (v1 < 0) {
                                v1 = 0;
                            }
                            if (v1 > 256) {
                                v1 = 256;
                            }

                            for (i = 0; i < 4; i++) {
                                color.b[i] = ((256 - v1) * aobj->unk10_color[i] + aobj->unk14_color[i] * v1) / 256;
                            }
                        }
                    } else {
                        color.i = *(u32*)(aobj->unk08 <= aobj->unk0C ? aobj->unk14_color : aobj->unk10_color);
                    }

                    switch (aobj->unk04) {
                        case 37:
                            mobj->unk08.unk50 = color.i;
                            break;
                        case 38:
                            mobj->unk08.unk58 = color.i;
                            break;
                        case 39:
                            mobj->unk08.unk5C = color.i;
                            break;
                        case 40:
                            mobj->unk08.unk60 = color.i;
                            break;
                        case 41:
                            mobj->unk08.unk64 = color.i;
                            break;
                    }
                }
            }
            aobj = aobj->next;
        }

        if (mobj->unk98 == FLOAT_MAYBE_SCALE_MIN) {
            mobj->unk98 = FLOAT_NEG_MAX;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/D150/func_8000E4E4.s")
#endif

void func_8000E9D8(GObjCommon* arg0) {
    DObj* dobj;
    MObj* mobj;

    dobj = arg0->children;
    while (dobj != NULL) {
        func_8000CA98(dobj);
        func_8000D768(dobj);

        mobj = dobj->mobjList;
        while (mobj != NULL) {
            func_8000DA18(mobj);
            func_8000E4E4(mobj);

            mobj = mobj->next;
        }

        if (dobj->childList != NULL) {
            dobj = dobj->childList;
        } else if (dobj->next != NULL) {
            dobj = dobj->next;
        } else {
            while (TRUE) {
                if ((uintptr_t)dobj->parent == 1) {
                    dobj = NULL;
                    break;
                }

                if (dobj->parent->next != NULL) {
                    dobj = dobj->parent->next;
                    break;
                }

                dobj = dobj->parent;
            }
        }
    }
}

AObj* func_8000EAAC(AObj* aobj, u8 key) {
    while (aobj != NULL) {
        if (aobj->unk04 == key) {
            return aobj;
        }
        aobj = aobj->next;
    }

    return NULL;
}

void func_8000EAE8(DObj* dobj, f32 arg1) {
    AObj* aobj;

    aobj = dobj->aobjList;
    dobj->unk74 = dobj->unk78 + arg1;
    while (aobj != NULL) {
        aobj->unk08 = 1.0f / arg1;
        aobj = aobj->next;
    }
}

f32 func_8000EB28(DObj* dobj, s32 idx) {
    switch (idx) {
        case 1:
            return dobj->unk28.f[1];
        case 2:
            return dobj->unk28.f[2];
        case 3:
            return dobj->unk28.f[3];
        case 5:
            return dobj->unk18.f.v.x;
        case 6:
            return dobj->unk18.f.v.y;
        case 7:
            return dobj->unk18.f.v.z;
        case 8:
            return dobj->unk3C.v.x;
        case 9:
            return dobj->unk3C.v.y;
        case 10:
            return dobj->unk3C.v.z;
    }
}

struct UnkEC64Arg3 {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ void* unk04;
    /* 0x08 */ struct Vec3f unk08;
    /* 0x14 */ struct Vec3f unk14;
    /* 0x20 */ struct Vec3f unk20;
}; // size == 0x2C

f32 func_8000EB98(struct UnkEC64Arg3* arg0, s32 idx) {
    switch (idx) {
        case 1:
            return arg0->unk14.x;
        case 2:
            return arg0->unk14.y;
        case 3:
            return arg0->unk14.z;
        case 5:
            return arg0->unk08.x;
        case 6:
            return arg0->unk08.y;
        case 7:
            return arg0->unk08.z;
        case 8:
            return arg0->unk20.x;
        case 9:
            return arg0->unk20.y;
        case 10:
            return arg0->unk20.z;
    }
}

s32 func_8000EC08(s32 arg0, DObj* arg1, f32* arg2, f32* arg3, AObj* arg4, struct UnkEC64Arg3* arg5, s32 arg6, s32 arg7,
                  struct Vec3f* arg8, s32* arg9) {
    AObj* aobj; // sp24

    aobj = func_8000EAAC(arg4, arg6);

    if (aobj != NULL && aobj->unk05 != 0) {
        if (arg0 == 0 && arg1->unk74 != (f32)PROCESSED_DEFAULT) {
            aobj->unk0C += arg1->unk78;
        }
        // L8000E1DC
        *arg2 = func_8000D640(aobj);
        if (arg7 != 0) {
            *arg3 = func_8000D6EC(aobj);
        }
    } else {
        // L8000E20C
        if (arg6 == 5 || arg6 == 6 || arg6 == 7) {
            if (*arg9 != 0) {
                switch (arg6) {
                    case 5:
                        *arg2 = arg8->x;
                        break;
                    case 6:
                        *arg2 = arg8->y;
                        break;
                    case 7:
                        *arg2 = arg8->z;
                        break;
                }
            } else {
                // L8000E284
                aobj = func_8000EAAC(arg4, 4);
                if (aobj != NULL && aobj->unk05 != 0) {
                    if (arg0 == 0 && arg1->unk74 != (f32)PROCESSED_DEFAULT) {
                        aobj->unk0C += arg1->unk78;
                    }
                    // L8000E2DC
                    *arg2 = func_8000D640(aobj);
                    if (*arg2 < 0.0f) {
                        *arg2 = 0.0f;
                    } else if (*arg2 > 1.0f) {
                        *arg2 = 1.0f;
                    }
                    // L8000E330
                    func_8001FCA4(arg8, aobj->unk20, *arg2);
                    switch (arg6) {
                        case 5:
                            *arg2 = arg8->x;
                            break;
                        case 6:
                            *arg2 = arg8->y;
                            break;
                        case 7:
                            *arg2 = arg8->z;
                            break;
                    }

                    *arg9 = 1;
                } else {
                    // L8000E39C
                    if (arg0 == 0) {
                        if (arg5 == 0) {
                            return 1;
                        }
                        *arg2 = func_8000EB98(arg5, arg6);
                    } else {
                        *arg2 = func_8000EB28(arg1, arg6);
                    }
                }
            }
        } else {
            if (arg0 == 0) {
                if (arg5 == 0) {
                    return 1;
                }
                *arg2 = func_8000EB98(arg5, arg6);
            } else {
                *arg2 = func_8000EB28(arg1, arg6);
            }
        }
    }
    return 0;
}

#ifdef NON_MATCHING
void func_8000EECC(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, struct AObj* arg5) {
    f32 sp3C;
    f32 sp38;

    f32 sp2C;
    f32 sp28;
    f32 sp24;

    f32 sp20;
    f32 sp1C;

    f32 temp;

    switch (arg0) {
        case 1:
        case 2:
        case 3:
            sp2C = arg2;
            break;
        case 5:
        case 6:
        case 7:
            sp2C = arg1;
            break;
        case 8:
        case 9:
        case 10:
            sp2C = arg3;
            break;
    }
    if (!sp2C) {
        return;
    }

    sp3C = 2.0f * arg5->unk18 + arg5->unk1C;
    sp28 = -6.0f * sp2C;
    sp38 = sp28 * (arg5->unk14 - arg5->unk10);

    sp24 = SQ(sp3C);
    sp20 = sp24 - sp38;
    sp1C = -sp3C;

    if (sp38 < sp24) {
        temp = (sqrtf(sp20) + sp1C) / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
        temp = (sp1C - sqrtf(sp20)) / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    } else if (sp24 - sp38 == 0.0f) {
        temp = -sp3C / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    }

    sp20 = sp24 + sp38;
    sp1C = -sp3C;

    if (sp20 > 0.0f) {
        temp = (sqrtf(sp20) + sp1C) / -sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
        temp = (sp1C - sqrtf(sp20)) / sp24;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    } else if (sp20 == 0.0f) {
        temp = -sp3C / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    }

    sp3C = -(arg5->unk18 + 2.0f * arg5->unk1C);
    sp38 = sp28 * (arg5->unk10 - arg5->unk14);

    sp24 = SQ(sp3C);
    sp20 = sp24 - sp38;
    sp1C = -sp3C;

    if (sp38 < sp24) {
        temp = (sqrtf(sp20) + sp1C) / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
        temp = (sp1C - sqrtf(sp20)) / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    } else if (sp24 - sp38 == 0.0f) {
        temp = -sp3C / sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    }

    sp20 = sp24 + sp38;
    sp1C = -sp3C;
    sp24 = -sp2C;

    if (sp20 > 0.0f) {
        temp = (sqrtf(sp20) + sp1C) / sp24;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
        temp = (sp1C - sqrtf(sp20)) / sp24;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    } else if (sp20 == 0.0f) {
        temp = -sp3C / -sp2C;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/D150/func_8000EECC.s")
void func_8000EECC(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, struct AObj* arg5);
#endif

f32 func_8000F34C(DObj* arg0, union AnimCmd** arg1, f32 arg2, struct UnkEC64Arg3* arg3, s32 arg4, f32 arg5, f32 arg6,
                  f32 arg7, f32 arg8) {
    AObj* spC4;
    AObj* spC0;
    AObj* newAObj;
    AObj* origAObj;

    f32 spB4;
    f32 spB0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    s32 i;
    s32 sp9C;
    s32 sp98;
    Vec3f sp8C;
    Vec3f sp80;

    spC0 = NULL;
    sp9C = 0;
    sp98 = 0;
    spA4 = 0.0f;

    if (arg1 == NULL || *arg1 == NULL) {
        if (arg3 == NULL) {
            om_dobj_reset_aobj_list(arg0);
            return 0;
        }
    }

    spC4 = arg0->aobjList;
    arg0->aobjList = NULL;
    if (arg1 != NULL && *arg1 != NULL) {
        arg0->unk70 = *arg1;
        arg0->unk74 = LOWER_BOUND;
        arg0->unk7C = arg2;
        func_8000CA98(arg0);
        spC0 = arg0->aobjList;
        arg0->aobjList = NULL;
    }

    for (i = 1; i <= 10; i++) {
        if (i == 4) {
            continue;
        }
        spA8 = 0.0f;
        spAC = 0.0f;

        if (func_8000EC08(0, arg0, &spB0, &spA8, spC0, arg3, i, arg4, &sp80, &sp98) != 0) {
            continue;
        }

        func_8000EC08(1, arg0, &spB4, &spAC, spC4, arg3, i, arg4, &sp8C, &sp9C);

        if (spB0 != spB4 || spA8 != spAC) {
            newAObj = om_dobj_add_aobj(arg0, i);

            if (i == 1 || i == 2 || i == 3) {
                if (ABS(spB0 - spB4) > ABS(spB0 - (spB4 + TAU))) {
                    spB4 += TAU;
                }

                if (ABS(spB0 - spB4) > ABS(spB0 - (spB4 - TAU))) {
                    spB4 -= TAU;
                }
            }

            switch (arg4) {
                case 0:
                    newAObj->unk10 = spB4;
                    newAObj->unk14 = spB0;
                    newAObj->unk05 = 2;
                    newAObj->unk08 = 1.0f / arg5;
                    newAObj->unk0C = -arg0->unk78;
                    newAObj->unk18 = (newAObj->unk14 - newAObj->unk10) / arg5;
                    newAObj->unk1C = 0.0f;
                    break;
                case 1:
                case 2:
                    newAObj->unk10 = spB4;
                    newAObj->unk14 = spB0;
                    newAObj->unk05 = 3;
                    newAObj->unk08 = 1.0f / arg5;
                    newAObj->unk0C = -arg0->unk78;
                    newAObj->unk18 = spAC;
                    newAObj->unk1C = spA8;

                    if (arg4 == 2) {
                        func_8000EECC(i, arg6, arg7, arg8, &spA4, newAObj);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    origAObj = arg0->aobjList;
    arg0->aobjList = spC4;
    om_dobj_reset_aobj_list(arg0);
    arg0->aobjList = spC0;
    om_dobj_reset_aobj_list(arg0);

    arg0->aobjList = origAObj;
    arg0->unk70 = NULL;
    arg0->unk74 = arg0->unk78 + arg5;
    arg0->unk7C = -arg0->unk78;

    return spA4;
}

f32 func_8000F708(GObjCommon* arg0, union AnimCmd** arg1, f32 arg2, struct UnkEC64Arg3* arg3, s32 arg4, f32 arg5,
                  f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 ret;
    UNUSED f32 tmp;
    DObj* dobj;

    dobj = arg0->children;
    arg0->unk_4C = arg2;
    if (arg4 == 2) {
        f32 sp68 = arg5;
        arg5 = 0.0f;
        while (dobj != NULL) {
            ret = func_8000F34C(dobj, arg1, arg2, arg3, arg4, arg5, arg7, arg8, arg9);
            if (arg5 < ret) {
                arg5 = ret;
            }

            if (arg1 != NULL) {
                arg1++;
            }

            if (arg3 != NULL) {
                arg3++;
            }

            dobj = func_8000C550(dobj);
        }

        dobj = arg0->children;
        if (arg5 < sp68) {
            arg5 = sp68;
        } else if (arg5 > arg6) {
            arg5 = arg6;
        }

        while (dobj != NULL) {
            func_8000EAE8(dobj, arg5);
            dobj = func_8000C550(dobj);
        }
    } else {
        while (dobj != NULL) {
            func_8000F34C(dobj, arg1, arg2, arg3, arg4, arg5, arg7, arg8, arg9);

            if (arg1 != NULL) {
                arg1++;
            }

            if (arg3 != NULL) {
                arg3++;
            }

            dobj = func_8000C550(dobj);
        }
    }

    arg0->unk_4C = 0.0f;
    return arg5;
}

#ifdef NON_MATCHING
void func_8000F8E4(GObjCommon* arg0, union AnimCmd** arg1, f32 arg2, struct UnkEC64Arg3* arg3) {
    f32 sp84;
    DObj* dobj;
    s32 i;
    s32 sp7C;
    Vec3f sp70;
    s32 sp6C;

    dobj = arg0->children;
    sp7C = 0;
    sp6C = 1;
    arg0->unk_4C = arg2;

    while (dobj != NULL) {
        if (*arg1 != NULL) {
            func_8000C7CC(dobj, *arg1, arg2);
            dobj->unk55 = sp6C;
            sp6C = 0;

            for (i = 1; i <= 10; i++) {
                if (i == 4) {
                    continue;
                }

                func_8000EC08(0, dobj, &sp84, NULL, dobj->aobjList, arg3, i, 0, &sp70, &sp7C);

                switch (i) {
                    case 1:
                        dobj->unk28.f[1] = sp84;
                        break;
                    case 2:
                        dobj->unk28.f[2] = sp84;
                        break;
                    case 3:
                        dobj->unk28.f[3] = sp84;
                        break;
                    case 5:
                        dobj->unk18.f.v.x = sp84;
                        break;
                    case 6:
                        dobj->unk18.f.v.y = sp84;
                        break;
                    case 7:
                        dobj->unk18.f.v.z = sp84;
                        break;
                    case 8:
                        dobj->unk3C.v.x = sp84;
                        break;
                    case 9:
                        dobj->unk3C.v.y = sp84;
                        break;
                    case 10:
                        dobj->unk3C.v.z = sp84;
                        break;
                }
            }
        } else {
            dobj->unk74 = (f32)FLOAT_NEG_MAX;
            dobj->unk55 = 0;
            if (arg3 != NULL) {
                dobj->unk18.f.v = arg3->unk08;
                *((struct Vec3f*)&dobj->unk28.f[1]) = arg3->unk14;
                dobj->unk3C.v = arg3->unk20;
            }
        }

        arg1++;

        if (arg3 != NULL) {
            arg3++;
        }

        dobj = func_8000C550(dobj);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/D150/func_8000F8E4.s")
void func_8000F8E4(GObjCommon* arg0, union AnimCmd** arg1, f32 arg2, struct UnkEC64Arg3* arg3);
#endif

void func_8000FAFC(struct DObj* dobj) {
    om_dobj_append_mtx(dobj, 27, 0);
    om_dobj_append_mtx(dobj, 32, 0);
}

DObj* func_8000FB34(GObjCommon* arg0, void* arg1) {
    DObj* dobj;

    dobj = om_gobj_add_dobj(arg0, arg1);
    func_8000FAFC(dobj);

    return dobj;
}

DObj* func_8000FB64(DObj* arg0, void* arg1) {
    DObj* dobj;

    dobj = om_dobj_add_sibling(arg0, arg1);
    func_8000FAFC(dobj);

    return dobj;
}

DObj* func_8000FB94(DObj* arg0, void* arg1) {
    DObj* dobj;

    dobj = om_dobj_add_child(arg0, arg1);
    func_8000FAFC(dobj);

    return dobj;
}

void func_8000FBC4(GObjCommon* arg0, struct UnkEC64Arg3* arg1, DObj** arg2) {
    int i;
    DObj* dobj;
    s32 trunc;
    DObj* sp44[18];

    // clang-format off
    for (i = 0; i < ARRAY_COUNT(sp44); i++) {
        sp44[i] = NULL;
    }
    // clang-format on

    while (arg1->unk00 != 18) {
        // L8000F18C
        trunc = arg1->unk00 & 0xFFF;
        if (trunc != 0) {
            dobj = sp44[trunc] = om_dobj_add_child(sp44[trunc - 1], arg1->unk04);
        } else {
            dobj = sp44[0] = om_gobj_add_dobj(arg0, arg1->unk04);
        }
        // L8000F1C4
        if (arg1->unk00 & 0xF000) {
            om_dobj_append_mtx(dobj, 18, 0);
        }

        if (arg1->unk00 & 0x8000) {
            om_dobj_append_mtx(dobj, 44, 0);
        } else if (arg1->unk00 & 0x4000) {
            om_dobj_append_mtx(dobj, 46, 0);
        } else if (arg1->unk00 & 0x2000) {
            om_dobj_append_mtx(dobj, 48, 0);
        } else if (arg1->unk00 & 0x1000) {
            om_dobj_append_mtx(dobj, 50, 0);
        } else {
            func_8000FAFC(dobj);
        }
        dobj->unk18.f.v = arg1->unk08;
        *((struct Vec3f*)&dobj->unk28.f[1]) = arg1->unk14;
        dobj->unk3C.v = arg1->unk20;

        if (arg2 != NULL) {
            *arg2 = dobj;
            arg2++;
        }

        arg1++;
    }
}

void func_8000FDA0(DObj* arg0, u8 arg1, u8 arg2, u8 arg3) {
    if (arg1 != 0) {
        om_dobj_append_mtx(arg0, arg1, 0);
    }
    if (arg2 != 0) {
        om_dobj_append_mtx(arg0, arg2, 0);
    }
    if (arg3 != 0) {
        om_dobj_append_mtx(arg0, arg3, 0);
    }
}

void func_8000FE08(struct DObj* arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    s32 phi_t0 = 0;
    s32 phi_t1 = 0;
    s32 phi_v1 = 0;
    s32 phi_t2 = 0;

    switch (arg1) {
        case 18:
            phi_t0 = 1;
            break;
        case 26:
            phi_v1 = 1;
            break;
        case 27:
            phi_v1 = 1;
            phi_t0 = 1;
            break;
        case 28:
            phi_t1 = 1;
            phi_v1 = 1;
            phi_t0 = 1;
            break;
        case 29:
            phi_v1 = 2;
            break;
        case 30:
            phi_v1 = 2;
            phi_t0 = 1;
            break;
        case 31:
            phi_v1 = 2;
            phi_t1 = 1;
            phi_t0 = 1;
            break;
        case 32:
            phi_t1 = 1;
            break;
        case 51:
            phi_t2 = 1;
            phi_t1 = 1;
            phi_v1 = 1;
            phi_t0 = 1;
            break;
        case 52:
            phi_v1 = 2;
            phi_t2 = 1;
            phi_t1 = 1;
            phi_t0 = 1;
            break;
        case 54:
            phi_t1 = 1;
            phi_v1 = 1;
            phi_t0 = 1;
            break;
    }

    switch (arg2) {
        case 26:
            phi_v1 = 1;
            break;
        case 29:
            phi_v1 = 2;
            break;
        case 32:
            phi_t1 = 1;
            break;
    }

    if (arg3 == 32) {
        phi_t1 = 1;
    }

    if (phi_t0 != 0) {
        if (phi_t2 != 0) {
            om_dobj_append_mtx(arg0, 55, 0);
        } else {
            om_dobj_append_mtx(arg0, 18, 0);
        }
    }

    if (arg4 & 0x4000) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, 46, 0);
        } else {
            om_dobj_append_mtx(arg0, 45, 0);
        }
    } else if (arg4 & 0x2000) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, 48, 0);
        } else {
            om_dobj_append_mtx(arg0, 47, 0);
        }
    } else if (arg4 & 0x1000) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, 50, 0);
        } else {
            om_dobj_append_mtx(arg0, 49, 0);
        }
    } else if (phi_t1 != 0) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, 44, 0);
        } else {
            om_dobj_append_mtx(arg0, 43, 0);
        }
    } else {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, 42, 0);
        } else {
            om_dobj_append_mtx(arg0, 41, 0);
        }
    }
}

void func_800100A0(struct GObjCommon* arg0, struct UnkEC64Arg3* arg1, struct DObj** arg2, u8 arg3, u8 arg4, u8 arg5) {
    int i;
    struct DObj* dobj;
    s32 trunc;
    struct DObj* sp54[18];

    for (i = 0; i < ARRAY_COUNT(sp54); i++) {
        sp54[i] = NULL;
    }

    while (arg1->unk00 != 18) {
        trunc = arg1->unk00 & 0xFFF;
        if (trunc) {
            dobj = sp54[trunc] = om_dobj_add_child(sp54[trunc - 1], arg1->unk04);
        } else {
            dobj = sp54[0] = om_gobj_add_dobj(arg0, arg1->unk04);
        }

        if (arg1->unk00 & 0xF000) {
            func_8000FE08(dobj, arg3, arg4, arg5, arg1->unk00 & 0xF000);
        } else {
            func_8000FDA0(dobj, arg3, arg4, arg5);
        }

        dobj->unk18.f.v = arg1->unk08;
        *((struct Vec3f*)&dobj->unk28.f[1]) = arg1->unk14;
        dobj->unk3C.v = arg1->unk20;

        if (arg2 != NULL) {
            *arg2 = dobj;
            arg2++;
        }
        arg1++;
    }
}

void func_80010230(struct GObjCommon* arg0, struct UnkEC64Arg3* arg1, struct MObjSub*** arg2, struct DObj** arg3,
                   u8 arg4, u8 arg5, u8 arg6) {
    int i;
    struct DObj* dobj;
    s32 trunc;
    struct DObj* sp5C[18];
    struct MObjSub** csr;
    struct MObjSub* msub;

    // clang-format off
    for (i = 0; i < ARRAY_COUNT(sp5C); i++) {
        sp5C[i] = NULL;
    }
    // clang-format on

    while (arg1->unk00 != 18) {
        // L8000F7A0
        trunc = arg1->unk00 & 0xFFF;
        if (trunc) {
            dobj = sp5C[trunc] = om_dobj_add_child(sp5C[trunc - 1], arg1->unk04);
        } else {
            dobj = sp5C[0] = om_gobj_add_dobj(arg0, arg1->unk04);
        }
        // L8000F7DC
        if (arg1->unk00 & 0xF000) {
            func_8000FE08(dobj, arg4, arg5, arg6, arg1->unk00 & 0xF000);
        } else {
            func_8000FDA0(dobj, arg4, arg5, arg6);
        }
        // L8000F820
        dobj->unk18.f.v = arg1->unk08;
        *((struct Vec3f*)&dobj->unk28.f[1]) = arg1->unk14;
        dobj->unk3C.v = arg1->unk20;

        if (arg2 != NULL) {
            if (*arg2 != NULL) {
                csr = *arg2;
                msub = *csr;
                while (msub != NULL) {
                    om_dobj_add_mobj(dobj, msub);
                    csr++;
                    msub = *csr;
                }
            }

            arg2++;
        }
        // L8000F8A0
        if (arg3 != NULL) {
            *arg3 = dobj;
            arg3++;
        }
        // L8000F8B4
        arg1++;
    }
    // L8000F8C4
}

void func_80010404(struct GObjCommon* arg0, struct MObjSub*** arg1) {
    struct DObj* dobj;
    struct MObjSub** csr;
    struct MObjSub* msub;

    dobj = arg0->children;
    while (dobj != NULL) {
        if (arg1 != NULL) {
            if (*arg1 != NULL) {
                csr = *arg1;
                msub = *csr;
                while (msub != NULL) {
                    om_dobj_add_mobj(dobj, msub);
                    csr++;
                    msub = *csr;
                }
            }
            arg1++;
        }
        // L8000F95C
        dobj = func_8000C550(dobj);
    }
}

void func_80010498(GObjCommon* arg0, struct UnkEC64Arg3* arg1) {
    DObj* dobj;

    dobj = arg0->children;
    while (dobj != NULL && arg1->unk00 != 18) {
        dobj->unk18.f.v = arg1->unk08;
        *((struct Vec3f*)&dobj->unk28.f[1]) = arg1->unk14;
        dobj->unk3C.v = arg1->unk20;

        arg1++;
        dobj = func_8000C550(dobj);
    }
}

void func_8001054C(DObj* dobj, union AnimCmd* arg1, f32 arg2) {
    AObj* aobj;

    aobj = dobj->aobjList;
    while (aobj != NULL) {
        aobj->unk05 = 0;
        aobj = aobj->next;
    }
    dobj->unk70 = arg1;
    dobj->unk74 = LOWER_BOUND;
    dobj->unk7C = arg2;
}

// maybe this takes an `SObj *`...?
void func_80010584(struct DObj* arg) {
    struct AObj* sp80[10];
    struct AObj* aobj;
    s32 i;
    u32 cmd;
    u32 subcmd;
    f32 payload;

    if (arg->unk74 != (f32)FLOAT_NEG_MAX) {
        if (arg->unk74 == (f32)LOWER_BOUND) {
            arg->unk74 = -arg->unk7C;
        } else {
            arg->unk74 -= arg->unk78;
            arg->unk7C += arg->unk78;
            arg->obj->unk_4C = arg->unk7C;

            if (arg->unk74 > 0.0f) {
                return;
            }
        }
        // L8000FB44
        // clang-format off
        for (i = 0; i < ARRAY_COUNT(sp80); i++) {
            sp80[i] = NULL;
        }
        // clang-format on
        aobj = arg->aobjList;
        while (aobj != NULL) {
            if (aobj->unk04 >= 25 && aobj->unk04 < 35) {
                sp80[aobj->unk04 - 25] = aobj;
            }
            aobj = aobj->next;
        }
        // L8000FBA0
        do {
            // L8000FBAC
            if (arg->unk70 == NULL) {
                aobj = arg->aobjList;
                while (aobj != NULL) {
                    if (aobj->unk05) {
                        aobj->unk0C += arg->unk78 + arg->unk74;
                    }
                    aobj = aobj->next;
                }
                arg->unk7C = arg->unk74;
                arg->unk74 = PROCESSED_DEFAULT;
                return;
            }
            // L8000FC0C
            cmd = arg->unk70->w >> 25;
            switch (cmd) {
                case 8:
                case 9: {
                    payload = (f32)(arg->unk70->w & 0x7FFF);
                    subcmd = (arg->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }
                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = func_80009AB0(arg, i + 25);
                            }
                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = arg->unk70->f;
                            arg->unk70++;
                            sp80[i]->unk18 = sp80[i]->unk1C;
                            sp80[i]->unk1C = 0.0f;
                            sp80[i]->unk05 = 3;

                            if (payload != 0.0f) {
                                sp80[i]->unk08 = 1.0f / payload;
                            }
                            sp80[i]->unk0C = -arg->unk74 - arg->unk78;
                        }
                        subcmd >>= 1;
                    }
                    // L8000FD18
                    if (cmd == 8) {
                        arg->unk74 += payload;
                    }
                    break;
                }
                case 3:
                case 4: {
                    payload = (f32)(arg->unk70->w & 0x7FFF);
                    subcmd = (arg->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }
                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = func_80009AB0(arg, i + 25);
                            }
                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = arg->unk70->f;
                            arg->unk70++;
                            sp80[i]->unk05 = 2;

                            if (payload != 0.0f) {
                                sp80[i]->unk18 = (sp80[i]->unk14 - sp80[i]->unk10) / payload;
                            }
                            sp80[i]->unk0C = -arg->unk74 - arg->unk78;
                            sp80[i]->unk1C = 0.0f;
                        }
                        subcmd >>= 1;
                    }
                    // L8000FE20
                    if (cmd == 3) {
                        arg->unk74 += payload;
                    }
                    break;
                }
                case 5:
                case 6: {
                    payload = (f32)(arg->unk70->w & 0x7FFF);
                    subcmd = (arg->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }
                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = func_80009AB0(arg, i + 25);
                            }

                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = arg->unk70->f;
                            arg->unk70++;
                            sp80[i]->unk18 = sp80[i]->unk1C;
                            sp80[i]->unk1C = arg->unk70->f;
                            arg->unk70++;
                            sp80[i]->unk05 = 3;
                            if (payload != 0.0f) {
                                sp80[i]->unk08 = 1.0f / payload;
                            }
                            sp80[i]->unk0C = -arg->unk74 - arg->unk78;
                        }

                        subcmd >>= 1;
                    }

                    if (cmd == 5) {
                        arg->unk74 += payload;
                    }
                    break;
                }
                case 7: {
                    subcmd = (arg->unk70++->w << 7) >> 22;
                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }
                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = func_80009AB0(arg, i + 25);
                            }

                            sp80[i]->unk1C = arg->unk70->f;
                            arg->unk70++;
                        }
                        subcmd >>= 1;
                    }
                    break;
                }
                case 2: {
                    arg->unk74 += (f32)(arg->unk70++->w & 0x7FFF);
                    break;
                }
                case 10:
                case 11: {
                    payload = (f32)(arg->unk70->w & 0x7FFF);
                    subcmd = (arg->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }
                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = func_80009AB0(arg, i + 25);
                            }

                            sp80[i]->unk10 = sp80[i]->unk14;
                            sp80[i]->unk14 = arg->unk70->f;
                            arg->unk70++;
                            sp80[i]->unk05 = 1;
                            sp80[i]->unk08 = payload;
                            sp80[i]->unk0C = -arg->unk74 - arg->unk78;
                            sp80[i]->unk1C = 0.0f;
                        }
                        subcmd >>= 1;
                    }

                    if (cmd == 10) {
                        arg->unk74 += payload;
                    }

                    break;
                }
                case 14: {
                    arg->unk70++;
                    arg->unk70 = arg->unk70->ptr;
                    arg->unk7C = -arg->unk74;
                    arg->obj->unk_4C = -arg->unk74;
                    break;
                }
                case 1: {
                    arg->unk70++;
                    arg->unk70 = arg->unk70->ptr;
                    break;
                }
                case 12: {
                    payload = (f32)(arg->unk70->w & 0x7FFF);
                    subcmd = (arg->unk70++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(sp80); i++) {
                        if (subcmd == 0) {
                            break;
                        }
                        if (subcmd & 1) {
                            if (sp80[i] == NULL) {
                                sp80[i] = func_80009AB0(arg, i + 25);
                            }

                            sp80[i]->unk0C += payload;
                        }
                        subcmd >>= 1;
                    }
                    break;
                }
                case 13: {
                    subcmd = (arg->unk70++->w << 7) >> 22;

                    if (subcmd & 0x08) {
                        if (sp80[3] == NULL) {
                            sp80[3] = func_80009AB0(arg, 3 + 25);
                        }

                        sp80[3]->unk20 = arg->unk70->ptr;
                        arg->unk70++;
                    }
                    if (subcmd & 0x80) {
                        if (sp80[7] == NULL) {
                            sp80[7] = func_80009AB0(arg, 7 + 25);
                        }

                        sp80[7]->unk20 = arg->unk70->ptr;
                        arg->unk70++;
                    }
                    break;
                }
                case 0: {
                    aobj = arg->aobjList;
                    while (aobj != NULL) {
                        if (aobj->unk05 != 0) {
                            aobj->unk0C += arg->unk78 + arg->unk74;
                        }
                        aobj = aobj->next;
                    }
                    arg->unk7C = arg->unk74;
                    arg->unk74 = PROCESSED_DEFAULT;
                    return; // not break
                }
                case 23: {
                    arg->unk74 += (f32)(arg->unk70++->w & 0x7FFF);
                    arg->unk70 += 2;
                    break;
                }
                default: {
                }
            }
            // L800102F4
        } while (arg->unk74 <= 0.0f);
    }
    // L80010308
}

// could take an SObj ...?
// But it seems that the game only has SObj sized at 0x6C
struct TempUnkObj {
    /* 0x00 */ u32 pad00;
    /* 0x04 */ struct GObjCommon* unk04;
    /* 0x08 */ u8 pad08[0x20 - 0x08];
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x3C - 0x24];
    /* 0x3C */ struct Vec3f unk3C;
    /* 0x48 */ struct Vec3f unk48;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u8 pad58[0x6c - 0x58];
    /* 0x6C */ struct AObj* unk6C;
    /* 0x70 */ u32 pad70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
}; // size >= 0x7C

void func_80010E54(struct TempUnkObj* arg) {
    AObj* aobj;
    f32 tmp;

    if (arg->unk74 != (f32)FLOAT_NEG_MAX) {
        aobj = arg->unk6C;
        while (aobj != NULL) {
            if (aobj->unk05 != 0) {
                if (arg->unk74 != (f32)FLOAT_MAYBE_SCALE_MIN) {
                    aobj->unk0C += arg->unk78;
                }

                if (!(arg->unk04->unk_50 & 2)) {
                    switch (aobj->unk04) {
                        case 25:
                            arg->unk3C.x = func_8000D640(aobj);
                            break;
                        case 26:
                            arg->unk3C.y = func_8000D640(aobj);
                            break;
                        case 27:
                            arg->unk3C.z = func_8000D640(aobj);
                            break;
                        case 28:
                            tmp = func_8000D640(aobj);
                            if (tmp < 0.0f) {
                                tmp = 0.0f;
                            } else if (tmp > 1.0f) {
                                tmp = 1.0f;
                            }
                            func_8001FCA4(&arg->unk3C, aobj->unk20, tmp);
                            break;
                        case 29:
                            arg->unk48.x = func_8000D640(aobj);
                            break;
                        case 30:
                            arg->unk48.y = func_8000D640(aobj);
                            break;
                        case 31:
                            arg->unk48.z = func_8000D640(aobj);
                            break;
                        case 32:
                            tmp = func_8000D640(aobj);
                            if (tmp < 0.0f) {
                                tmp = 0.0f;
                            } else if (tmp > 1.0f) {
                                tmp = 1.0f;
                            }
                            func_8001FCA4(&arg->unk48, aobj->unk20, tmp);
                            break;
                        case 33:
                            arg->unk54 = func_8000D640(aobj);
                            break;
                        case 34:
                            arg->unk20 = func_8000D640(aobj);
                            break;
                    }
                }
            }

            aobj = aobj->next;
        }

        if (arg->unk74 == FLOAT_MAYBE_SCALE_MIN) {
            arg->unk74 = FLOAT_NEG_MAX;
        }
    }
    // L80010564
}

void func_80011090(GObjCommon* obj) {
    struct DObj* dobj; // could be SObj?
    dobj = obj->children;
    func_80010584(dobj);
    func_80010E54((void*)dobj);
}

s32 func_800110BC(union AnimCmd** arg0) {
    union AnimCmd* list; // a0
    u32 subcmd;          // v0
    s32 v1 = 0;
    s32 i;               // a2
    u32 cmd;             // a1
    u32 id;

    while (*arg0 == NULL) {
        arg0++;
    }

    list = *arg0;

    cmd = list->w;
    id = cmd >> 25;

    while (TRUE) {
        switch (id) {
            case 3:
            case 8:
            case 10:
                v1 += cmd & 0x7FFF;
                /* fallthrough */
            case 4:
            case 7:
            case 9:
            case 11:
                subcmd = (cmd << 7) >> 22;
                list++;
                for (i = 0; i < 10; i++) {
                    if (subcmd == 0) {
                        break;
                    }
                    if (subcmd & 1) {
                        list++;
                    }

                    subcmd >>= 1;
                }
                cmd = list->w;
                id = cmd >> 25;
                break;
            case 5:
                v1 += cmd & 0x7FFF;
                /* fallthrough */
            case 6:
                subcmd = (cmd << 7) >> 22;
                list++;
                for (i = 0; i < 10; i++) {
                    if (subcmd == 0) {
                        break;
                    }
                    if (subcmd & 1) {
                        list += 2;
                    }

                    subcmd >>= 1;
                }
                cmd = list->w;
                id = cmd >> 25;
                break;
            case 2:
            case 15:
                v1 += cmd & 0x7FFF;
                list++;
                cmd = list->w;
                id = cmd >> 25;
                break;
            case 12:
                list++;
                cmd = list->w;
                id = cmd >> 25;
                break;
            case 13:
                list += 2;
                cmd = list->w;
                id = cmd >> 25;
                break;
            case 17:
                v1 += cmd & 0x7FFF;
                subcmd = (cmd << 7) >> 22;
                list++;
                for (i = 4; i < 14; i++) {
                    if (subcmd == 0) {
                        break;
                    }
                    if (subcmd & 1) {
                        list++;
                    }

                    subcmd >>= 1;
                }
                cmd = list->w;
                id = cmd >> 25;
                break;
            case 0:
                return v1;
            case 1:
            case 14:
                return -v1;
            case 16:
                break;
        }
    }
}
