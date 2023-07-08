#include "common.h"

#define ANIMATION_DISABLED (FLOAT_NEG_MAX)
#define ANIMATION_CHANGED (FLOAT_NEG_MAX / 2)
#define ANIMATION_FINISHED (FLOAT_NEG_MAX / 3)

union AnimCmd {
    u32 w;
    f32 f;
    void* ptr;
};

struct UnkEC64Arg3 {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ void* unk04;
    /* 0x08 */ Vec3f position;
    /* 0x14 */ Vec3f rotation;
    /* 0x20 */ Vec3f scale;
}; // size == 0x2C

AObj* om_dobj_add_aobj(DObj* dobj, u8 index);
void om_dobj_reset_animation(DObj* dobj);
void func_8001FCA4(Vec3f*, void*, f32);
OMMtx* om_dobj_append_mtx(DObj* dobj, u8 kind, u8 arg2);
AObj* om_camera_add_aobj(OMCamera* obj, u8 index);
AObj* om_mobj_add_aobj(MObj* mobj, u8 index);

#ifdef NON_MATCHING
DObj* func_8000C550(DObj* obj) {
    DObj* child;
    DObj* next;
    DObj* parent;

    child = obj->firstChild;
    if (obj->firstChild != NULL) {
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

void set_model_animation_speed(GObjCommon* obj, f32 speed) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        dobj->animSpeed = speed;
        dobj = func_8000C550(dobj);
    }
}

void set_model_and_texture_animation_speed(GObjCommon* obj, f32 speed) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        MObj* mobj = dobj->mobjList;
        dobj->animSpeed = speed;
        while (mobj != NULL) {
            mobj->animSpeed = speed;
            mobj = mobj->next;
        }
        dobj = func_8000C550(dobj);
    }
}

void set_texture_animation_speed(GObjCommon* obj, f32 speed) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        MObj* mobj = dobj->mobjList;
        while (mobj != NULL) {
            mobj->animSpeed = speed;
            mobj = mobj->next;
        }
        dobj = func_8000C550(dobj);
    }
}

void reset_model_animation(GObjCommon* obj) {
    DObj* dobj = obj->children;

    while (dobj != NULL) {
        om_dobj_reset_animation(dobj);
        dobj = func_8000C550(dobj);
    }
}

void reset_model_and_texture_animation(GObjCommon* obj) {
    DObj* dobj = obj->children;
    MObj* mobj;

    while (dobj != NULL) {
        om_dobj_reset_animation(dobj);
        mobj = dobj->mobjList;
        while (mobj != NULL) {
            om_mobj_reset_aobj_list(mobj);
            mobj = mobj->next;
        }

        dobj = func_8000C550(dobj);
    }
}

void reset_texture_animation(GObjCommon* obj) {
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

void set_model_animation(DObj* dobj, union AnimCmd* animList, f32 skipFrames) {
    AObj* aobj = dobj->aobjList;

    while (aobj != NULL) {
        aobj->kind = ANIM_TYPE_NONE;
        aobj = aobj->next;
    }

    dobj->animList = animList;
    dobj->timeLeft = ANIMATION_CHANGED;
    dobj->timePassed = skipFrames;
}

void set_texture_animation(MObj* mobj, union AnimCmd* animList, f32 skipFrames) {
    AObj* aobj = mobj->aobjList;

    while (aobj != NULL) {
        aobj->kind = ANIM_TYPE_NONE;
        aobj = aobj->next;
    }

    mobj->animList = animList;
    mobj->timeLeft = ANIMATION_CHANGED;
    mobj->timePassed = skipFrames;
}

void set_model_tree_animation(GObjCommon* obj, union AnimCmd** animLists, f32 skipFrames) {
    DObj* dobj;
    u8 s2 = TRUE;

    dobj = obj->children;
    obj->animationTime = skipFrames;
    while (dobj != NULL) {
        if (*animLists != NULL) {
            set_model_animation(dobj, *animLists, skipFrames);
            dobj->animCBReceiver = s2;
            s2 = FALSE;
        } else {
            dobj->timeLeft = ANIMATION_DISABLED;
            dobj->animCBReceiver = FALSE;
        }

        animLists++;
        dobj = func_8000C550(dobj);
    }
}

void set_model_tree_texture_animation(GObjCommon* obj, union AnimCmd*** textureAnimLists, f32 skipFrames) {
    DObj* dobj;
    MObj* mobj;
    union AnimCmd** cmdlist;

    dobj = obj->children;
    obj->animationTime = skipFrames;
    while (dobj != NULL) {
        if (textureAnimLists != NULL) {
            if (*textureAnimLists != NULL) {
                cmdlist = *textureAnimLists;
                mobj = dobj->mobjList;
                while (mobj != NULL) {
                    if (*cmdlist != NULL) {
                        set_texture_animation(mobj, *cmdlist, skipFrames);
                    }
                    cmdlist++;
                    mobj = mobj->next;
                }
            }
            textureAnimLists++;
        }
        dobj = func_8000C550(dobj);
    }
}

void set_model_and_texture_tree_animation(GObjCommon* obj, union AnimCmd** modelAnimLists,
                                          union AnimCmd*** textureAnimLists, f32 skipFrames) {
    DObj* dobj = obj->children;
    MObj* mobj;
    union AnimCmd** cmdlist;
    u8 s5 = TRUE;

    obj->animationTime = skipFrames;
    while (dobj != NULL) {
        if (modelAnimLists != NULL) {
            if (*modelAnimLists != NULL) {
                set_model_animation(dobj, *modelAnimLists, skipFrames);
                dobj->animCBReceiver = s5;
                s5 = FALSE;
            } else {
                dobj->timeLeft = ANIMATION_DISABLED;
                dobj->animCBReceiver = FALSE;
            }
            modelAnimLists++;
        }
        if (textureAnimLists != NULL) {
            if (*textureAnimLists != NULL) {
                cmdlist = *textureAnimLists;
                mobj = dobj->mobjList;
                while (mobj != NULL) {
                    if (*cmdlist != NULL) {
                        set_texture_animation(mobj, *cmdlist, skipFrames);
                    }
                    cmdlist++;
                    mobj = mobj->next;
                }
            }
            textureAnimLists++;
        }
        dobj = func_8000C550(dobj);
    }
}

void process_model_animation(DObj* dobj) {
    AObj* aobjArray[ANIM_PARAM_MODEL_MAX - ANIM_PARAM_MODEL_MIN + 1];
    AObj* aobj;
    s32 i;
    u32 cmd;
    u32 bitMask;
    f32 duration;

    if (dobj->timeLeft == ANIMATION_DISABLED) {
        return;
    }

    if (dobj->timeLeft == ANIMATION_CHANGED) {
        dobj->timeLeft = -dobj->timePassed;
    } else {
        dobj->timeLeft -= dobj->animSpeed;
        dobj->timePassed += dobj->animSpeed;
        dobj->obj->animationTime = dobj->timePassed;

        if (dobj->timeLeft > 0.0f) {
            return;
        }
    }

    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
        aobjArray[i] = NULL;
    }

    aobj = dobj->aobjList;
    while (aobj != NULL) {
        if (aobj->paramID >= ANIM_PARAM_MODEL_MIN && aobj->paramID <= ANIM_PARAM_MODEL_MAX) {
            aobjArray[aobj->paramID - ANIM_PARAM_MODEL_MIN] = aobj;
        }
        aobj = aobj->next;
    }

    do {
        if (dobj->animList == NULL) {
            aobj = dobj->aobjList;
            while (aobj != NULL) {
                if (aobj->kind != ANIM_TYPE_NONE) {
                    aobj->time += dobj->animSpeed + dobj->timeLeft;
                }
                aobj = aobj->next;
            }
            dobj->timePassed = dobj->timeLeft;
            dobj->obj->animationTime = dobj->timeLeft;
            dobj->timeLeft = ANIMATION_FINISHED;
            return;
        }

        cmd = dobj->animList->w >> 25;
        switch (cmd) {
            case ANIM_CMD_SET_VALUE_ZERO_RATE_LAST:
            case ANIM_CMD_SET_VALUE_ZERO_RATE:
                duration = (f32)(dobj->animList->w & 0x7FFF);
                bitMask = (dobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_dobj_add_aobj(dobj, i + ANIM_PARAM_MODEL_MIN);
                        }

                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = dobj->animList->f;
                        dobj->animList++;
                        aobjArray[i]->rate = aobjArray[i]->targetRate;
                        aobjArray[i]->targetRate = 0.0f;
                        aobjArray[i]->kind = ANIM_TYPE_CUBIC;
                        if (duration != 0.0f) {
                            aobjArray[i]->invDuration = 1.0f / duration;
                        }
                        aobjArray[i]->time = -dobj->timeLeft - dobj->animSpeed;
                    }
                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_ZERO_RATE_LAST) {
                    dobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_VALUE_LAST:
            case ANIM_CMD_SET_VALUE:
                duration = (f32)(dobj->animList->w & 0x7FFF);
                bitMask = (dobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_dobj_add_aobj(dobj, i + ANIM_PARAM_MODEL_MIN);
                        }
                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = dobj->animList->f;
                        dobj->animList++;
                        aobjArray[i]->kind = ANIM_TYPE_LINEAR;
                        if (duration != 0.0f) {
                            aobjArray[i]->rate = (aobjArray[i]->targetValue - aobjArray[i]->initialValue) / duration;
                        }
                        aobjArray[i]->time = -dobj->timeLeft - dobj->animSpeed;
                        aobjArray[i]->targetRate = 0.0f;
                    }
                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_LAST) {
                    dobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_VALUE_WITH_RATE_LAST:
            case ANIM_CMD_SET_VALUE_WITH_RATE:
                duration = (f32)(dobj->animList->w & 0x7FFF);
                bitMask = (dobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_dobj_add_aobj(dobj, i + ANIM_PARAM_MODEL_MIN);
                        }

                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = dobj->animList->f;
                        dobj->animList++;
                        aobjArray[i]->rate = aobjArray[i]->targetRate;
                        aobjArray[i]->targetRate = dobj->animList->f;
                        dobj->animList++;
                        aobjArray[i]->kind = ANIM_TYPE_CUBIC;
                        if (duration != 0.0f) {
                            aobjArray[i]->invDuration = 1.0f / duration;
                        }
                        aobjArray[i]->time = -dobj->timeLeft - dobj->animSpeed;
                    }

                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_WITH_RATE_LAST) {
                    dobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_TARGET_RATE:
                bitMask = (dobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_dobj_add_aobj(dobj, i + ANIM_PARAM_MODEL_MIN);
                        }

                        aobjArray[i]->targetRate = dobj->animList->f;
                        dobj->animList++;
                    }
                    bitMask >>= 1;
                }
                break;
            case ANIM_CMD_WAIT:
                dobj->timeLeft += (f32)(dobj->animList++->w & 0x7FFF);
                break;
            case ANIM_CMD_SET_VALUE_AFTER_LAST:
            case ANIM_CMD_SET_VALUE_AFTER:
                duration = (f32)(dobj->animList->w & 0x7FFF);
                bitMask = (dobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_dobj_add_aobj(dobj, i + ANIM_PARAM_MODEL_MIN);
                        }

                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = dobj->animList->f;
                        dobj->animList++;
                        aobjArray[i]->kind = ANIM_TYPE_STEP;
                        aobjArray[i]->invDuration = duration;
                        aobjArray[i]->time = -dobj->timeLeft - dobj->animSpeed;
                        aobjArray[i]->targetRate = 0.0f;
                    }

                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_AFTER_LAST) {
                    dobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_ANIMATION:
                dobj->animList++;
                dobj->animList = dobj->animList->ptr;
                // reset animation timers
                dobj->timePassed = -dobj->timeLeft;
                dobj->obj->animationTime = -dobj->timeLeft;

                if (dobj->animCBReceiver && dobj->obj->fnAnimCallback != NULL) {
                    dobj->obj->fnAnimCallback(dobj, -2, 0);
                }
                break;
            case ANIM_CMD_JUMP:
                dobj->animList++;
                dobj->animList = dobj->animList->ptr;

                if (dobj->animCBReceiver && dobj->obj->fnAnimCallback != NULL) {
                    dobj->obj->fnAnimCallback(dobj, -2, 0);
                }
                break;
            case ANIM_CMD_12:
                duration = (f32)(dobj->animList->w & 0x7FFF);
                bitMask = (dobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_dobj_add_aobj(dobj, i + ANIM_PARAM_MODEL_MIN);
                        }

                        aobjArray[i]->time += duration;
                    }
                    bitMask >>= 1;
                }

                break;
            case ANIM_CMD_13:
                dobj->animList++;
                if (aobjArray[ANIM_PARAM_4 - ANIM_PARAM_MODEL_MIN] == NULL) {
                    aobjArray[ANIM_PARAM_4 - ANIM_PARAM_MODEL_MIN] = om_dobj_add_aobj(dobj, ANIM_PARAM_4);
                }
                aobjArray[ANIM_PARAM_4 - ANIM_PARAM_MODEL_MIN]->unk20 = dobj->animList->ptr;
                dobj->animList++;
                break;
            case ANIM_CMD_END:
                aobj = dobj->aobjList;
                while (aobj != NULL) {
                    if (aobj->kind != ANIM_TYPE_NONE) {
                        aobj->time += dobj->animSpeed + dobj->timeLeft;
                    }
                    aobj = aobj->next;
                }

                dobj->obj->animationTime = dobj->timePassed;
                dobj->timeLeft = ANIMATION_FINISHED;
                if (dobj->animCBReceiver && dobj->obj->fnAnimCallback != NULL) {
                    dobj->obj->fnAnimCallback(dobj, -1, 0);
                }
                return;
            case ANIM_CMD_SET_FLAGS:
                dobj->flags = (dobj->animList->w << 7) >> 22;
                dobj->timeLeft += (f32)(dobj->animList++->w & 0x7FFF);
                break;
            case ANIM_CMD_16:
                if (dobj->obj->fnAnimCallback != NULL) {
                    dobj->obj->fnAnimCallback(dobj, ((dobj->animList->w << 7) >> 22) >> 8,
                                              ((dobj->animList->w << 7) >> 22) & 0xFF);
                }

                dobj->timeLeft += (f32)(dobj->animList++->w & 0x7FFF);
                break;
            case ANIM_CMD_17:
                bitMask = (dobj->animList->w << 7) >> 22;
                dobj->timeLeft += (f32)(dobj->animList++->w & 0x7FFF);

                for (i = 4; i < 14; i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (dobj->obj->fnAnimCallback != NULL) {
                            dobj->obj->fnAnimCallback(dobj, i, dobj->animList->f);
                        }
                        dobj->animList++;
                    }
                    bitMask >>= 1;
                }
                break;
            default:
                break;
        }
    } while (dobj->timeLeft <= 0.0f);
}

f32 get_aobj_interp_value(f32 invDuration, f32 time, f32 initialValue, f32 targetValue, f32 initialRate,
                          f32 targetRate) {
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 temp_f10;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;

    temp_f2 = SQ(time);
    temp_f18 = SQ(invDuration);
    temp_f16 = temp_f2 * time * temp_f18;
    temp_f10 = 2.0f * temp_f16 * invDuration;
    sp14 = 3.0f * temp_f2 * temp_f18;
    sp18 = temp_f2 * invDuration;
    sp10 = temp_f16 - sp18;

    return ((temp_f10 - sp14) + 1.0f) * initialValue + targetValue * (sp14 - temp_f10) +
           initialRate * ((sp10 - sp18) + time) + targetRate * sp10;
}

f32 get_aobj_interp_rate(f32 invDuration, f32 time, f32 initialValue, f32 targetValue, f32 initialRate,
                         f32 targetRate) {
    f32 temp_f18;
    f32 temp_f16;
    f32 temp_f2;

    temp_f2 = 2.0f * time * invDuration;
    temp_f16 = 3.0f * time * time * invDuration * invDuration;
    temp_f18 = 6.0f * time;

    return (temp_f18 * time * invDuration * invDuration * invDuration - temp_f18 * invDuration * invDuration) *
               (initialValue - targetValue) +
           initialRate * (temp_f16 - 2.0f * temp_f2 + 1.0f) + targetRate * (temp_f16 - temp_f2);
}

f32 get_aobj_value(AObj* aobj) {
    switch (aobj->kind) {
        case ANIM_TYPE_LINEAR:
            return aobj->initialValue + (aobj->time * aobj->rate);
        case ANIM_TYPE_CUBIC:
            return get_aobj_interp_value(aobj->invDuration, aobj->time, aobj->initialValue, aobj->targetValue,
                                         aobj->rate, aobj->targetRate);
        case ANIM_TYPE_STEP:
            return aobj->invDuration <= aobj->time ? aobj->targetValue : aobj->initialValue;
    }
}

f32 get_aobj_rate(AObj* aobj) {
    switch (aobj->kind) {
        case ANIM_TYPE_LINEAR:
            return aobj->rate;
        case ANIM_TYPE_CUBIC:
            return get_aobj_interp_rate(aobj->invDuration, aobj->time, aobj->initialValue, aobj->targetValue,
                                        aobj->rate, aobj->targetRate);
        case ANIM_TYPE_STEP:
            return 0.0f;
    }
}

void update_model_animated_params(DObj* dobj) {
    f32 value;
    AObj* aobj;

    if (dobj->timeLeft != ANIMATION_DISABLED) {
        aobj = dobj->aobjList;

        while (aobj != NULL) {
            if (aobj->kind != ANIM_TYPE_NONE) {
                if (dobj->timeLeft != ANIMATION_FINISHED) {
                    aobj->time += dobj->animSpeed;
                }

                if (!(dobj->obj->flags & GOBJ_FLAG_2)) {
                    switch (aobj->kind) {
                        case ANIM_TYPE_LINEAR:
                            value = aobj->initialValue + aobj->time * aobj->rate;
                            break;
                        case ANIM_TYPE_CUBIC: {
                            f32 temp_f16 = SQ(aobj->invDuration);
                            f32 temp_f12 = SQ(aobj->time);
                            f32 temp_f18 = aobj->invDuration * temp_f12;
                            f32 temp_f14 = aobj->time * temp_f12 * temp_f16;
                            f32 temp_f20 = 2.0f * temp_f14 * aobj->invDuration;
                            f32 temp_f22 = 3.0f * temp_f12 * temp_f16;
                            f32 temp_f24 = temp_f14 - temp_f18;

                            value = (aobj->initialValue * ((temp_f20 - temp_f22) + 1.0f)) +
                                    (aobj->targetValue * (temp_f22 - temp_f20)) +
                                    (aobj->rate * ((temp_f24 - temp_f18) + aobj->time)) + (aobj->targetRate * temp_f24);
                            break;
                        }
                        case ANIM_TYPE_STEP:
                            value = aobj->invDuration <= aobj->time ? aobj->targetValue : aobj->initialValue;
                            break;
                        default:
                            break;
                    }

                    switch (aobj->paramID) {
                        case ANIM_PARAM_ROLL:
                            dobj->rotation.f[1] = value;
                            break;
                        case ANIM_PARAM_PITCH:
                            dobj->rotation.f[2] = value;
                            break;
                        case ANIM_PARAM_YAW:
                            dobj->rotation.f[3] = value;
                            break;
                        case ANIM_PARAM_4:
                            if (value < 0.0f) {
                                value = 0.0f;
                            } else if (value > 1.0f) {
                                value = 1.0f;
                            }
                            func_8001FCA4(&dobj->position.v, aobj->unk20, value);
                            break;
                        case ANIM_PARAM_POSITION_X:
                            dobj->position.v.x = value;
                            break;
                        case ANIM_PARAM_POSITION_Y:
                            dobj->position.v.y = value;
                            break;
                        case ANIM_PARAM_POSITION_Z:
                            dobj->position.v.z = value;
                            break;
                        case ANIM_PARAM_SCALE_X:
                            dobj->scale.v.x = value;
                            break;
                        case ANIM_PARAM_SCALE_Y:
                            dobj->scale.v.y = value;
                            break;
                        case ANIM_PARAM_SCALE_Z:
                            dobj->scale.v.z = value;
                            break;
                    }
                }
            }
            aobj = aobj->next;
        }

        if (dobj->timeLeft == ANIMATION_FINISHED) {
            dobj->timeLeft = ANIMATION_DISABLED;
        }
    }
}

void process_texture_animation(MObj* mobj) {
    AObj* aobjArray[10];
    AObj* extraAobjArray[5];
    AObj* aobj;
    s32 i;
    u32 cmd;
    u32 bitMask;
    f32 duration;

    if (mobj->timeLeft == ANIMATION_DISABLED) {
        return;
    }

    if (mobj->timeLeft == ANIMATION_CHANGED) {
        mobj->timeLeft = -mobj->timePassed;
    } else {
        mobj->timeLeft -= mobj->animSpeed;
        mobj->timePassed += mobj->animSpeed;

        if (mobj->timeLeft > 0.0f) {
            return;
        }
    }

    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
        aobjArray[i] = NULL;
    }

    for (i = 0; i < ARRAY_COUNT(extraAobjArray); i++) {
        extraAobjArray[i] = NULL;
    }

    aobj = mobj->aobjList;
    while (aobj != NULL) {
        if (aobj->paramID >= ANIM_PARAM_TEXTURE_MIN && aobj->paramID <= ANIM_PARAM_TEXTURE_MAX) {
            aobjArray[aobj->paramID - ANIM_PARAM_TEXTURE_MIN] = aobj;
        }

        if (aobj->paramID >= ANIM_PARAM_TEXTURE_EXTRA_MIN && aobj->paramID <= ANIM_PARAM_TEXTURE_EXTRA_MAX) {
            extraAobjArray[aobj->paramID - ANIM_PARAM_TEXTURE_EXTRA_MIN] = aobj;
        }

        aobj = aobj->next;
    }

    do {
        if (mobj->animList == NULL) {
            aobj = mobj->aobjList;
            while (aobj != NULL) {
                if (aobj->kind != ANIM_TYPE_NONE) {
                    aobj->time += mobj->animSpeed + mobj->timeLeft;
                }
                aobj = aobj->next;
            }
            mobj->timePassed = mobj->timeLeft;
            mobj->timeLeft = ANIMATION_FINISHED;
            return;
        }

        cmd = mobj->animList->w >> 25;
        switch (cmd) {
            case ANIM_CMD_SET_VALUE_ZERO_RATE_LAST:
            case ANIM_CMD_SET_VALUE_ZERO_RATE:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_MIN);
                        }
                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = mobj->animList->f;
                        mobj->animList++;
                        aobjArray[i]->rate = aobjArray[i]->targetRate;
                        aobjArray[i]->targetRate = 0.0f;
                        aobjArray[i]->kind = ANIM_TYPE_CUBIC;

                        if (duration != 0.0f) {
                            aobjArray[i]->invDuration = 1.0f / duration;
                        }
                        aobjArray[i]->time = -mobj->timeLeft - mobj->animSpeed;
                    }
                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_ZERO_RATE_LAST) {
                    mobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_VALUE_LAST:
            case ANIM_CMD_SET_VALUE:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_MIN);
                        }

                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = mobj->animList->f;
                        mobj->animList++;
                        aobjArray[i]->kind = ANIM_TYPE_LINEAR;
                        if (duration != 0.0f) {
                            aobjArray[i]->rate = (aobjArray[i]->targetValue - aobjArray[i]->initialValue) / duration;
                        }
                        aobjArray[i]->time = -mobj->timeLeft - mobj->animSpeed;
                        aobjArray[i]->targetRate = 0.0f;
                    }
                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_LAST) {
                    mobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_VALUE_WITH_RATE_LAST:
            case ANIM_CMD_SET_VALUE_WITH_RATE:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_MIN);
                        }
                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = mobj->animList->f;
                        mobj->animList++;
                        aobjArray[i]->rate = aobjArray[i]->targetRate;
                        aobjArray[i]->targetRate = mobj->animList->f;
                        mobj->animList++;

                        aobjArray[i]->kind = ANIM_TYPE_CUBIC;
                        if (duration != 0.0f) {
                            aobjArray[i]->invDuration = 1.0f / duration;
                        }
                        aobjArray[i]->time = -mobj->timeLeft - mobj->animSpeed;
                    }
                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_WITH_RATE_LAST) {
                    mobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_TARGET_RATE:
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_MIN);
                        }
                        aobjArray[i]->targetRate = mobj->animList->f;
                        mobj->animList++;
                    }
                    bitMask >>= 1;
                }
                break;
            case ANIM_CMD_WAIT:
                mobj->timeLeft += (f32)(mobj->animList->w & 0x7FFF);
                mobj->animList++;
                break;
            case ANIM_CMD_SET_VALUE_AFTER_LAST:
            case ANIM_CMD_SET_VALUE_AFTER:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_MIN);
                        }
                        aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                        aobjArray[i]->targetValue = mobj->animList->f;
                        mobj->animList++;

                        aobjArray[i]->kind = ANIM_TYPE_STEP;
                        aobjArray[i]->invDuration = duration;
                        aobjArray[i]->time = -mobj->timeLeft - mobj->animSpeed;
                        aobjArray[i]->targetRate = 0.0f;
                    }
                    bitMask >>= 1;
                }

                if (cmd == ANIM_CMD_SET_VALUE_AFTER_LAST) {
                    mobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_ANIMATION:
                mobj->animList++;
                mobj->animList = mobj->animList->ptr;
                mobj->timePassed = -mobj->timeLeft;
                break;
            case ANIM_CMD_JUMP:
                mobj->animList++;
                mobj->animList = mobj->animList->ptr;
                break;
            case ANIM_CMD_12:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (aobjArray[i] == NULL) {
                            aobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_MIN);
                        }
                        aobjArray[i]->time += duration;
                    }
                    bitMask >>= 1;
                }
                break;
            case ANIM_CMD_END:
                aobj = mobj->aobjList;
                while (aobj != NULL) {
                    if (aobj->kind != ANIM_TYPE_NONE) {
                        aobj->time += mobj->animSpeed + mobj->timeLeft;
                    }
                    aobj = aobj->next;
                }

                mobj->timeLeft = ANIMATION_FINISHED;
                return; // not break
            case ANIM_CMD_SET_EXTRA_VALUE_AFTER_LAST:
            case ANIM_CMD_SET_EXTRA_VALUE_AFTER:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(extraAobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (extraAobjArray[i] == NULL) {
                            extraAobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_EXTRA_MIN);
                        }
                        extraAobjArray[i]->initialValue = extraAobjArray[i]->targetValue;
                        extraAobjArray[i]->targetValue = mobj->animList->f;
                        mobj->animList++;

                        extraAobjArray[i]->kind = ANIM_TYPE_STEP;
                        extraAobjArray[i]->invDuration = duration;
                        extraAobjArray[i]->time = -mobj->timeLeft - mobj->animSpeed;
                    }
                    bitMask >>= 1;
                }
                if (cmd == ANIM_CMD_SET_EXTRA_VALUE_AFTER_LAST) {
                    mobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_SET_EXTRA_VALUE_LAST:
            case ANIM_CMD_SET_EXTRA_VALUE:
                duration = (f32)(mobj->animList->w & 0x7FFF);
                bitMask = (mobj->animList++->w << 7) >> 22;

                for (i = 0; i < ARRAY_COUNT(extraAobjArray); i++) {
                    if (bitMask == 0) {
                        break;
                    }

                    if (bitMask & 1) {
                        if (extraAobjArray[i] == NULL) {
                            extraAobjArray[i] = om_mobj_add_aobj(mobj, i + ANIM_PARAM_TEXTURE_EXTRA_MIN);
                        }
                        extraAobjArray[i]->initialValue = extraAobjArray[i]->targetValue;
                        extraAobjArray[i]->targetValue = mobj->animList->f;
                        mobj->animList++;

                        extraAobjArray[i]->kind = ANIM_TYPE_LINEAR;
                        if (duration != 0.0f) {
                            extraAobjArray[i]->invDuration = 1.0f / duration;
                        }
                        extraAobjArray[i]->time = -mobj->timeLeft - mobj->animSpeed;
                    }
                    bitMask >>= 1;
                }
                if (cmd == ANIM_CMD_SET_EXTRA_VALUE_LAST) {
                    mobj->timeLeft += duration;
                }
                break;
            case ANIM_CMD_22:
                mobj->timeLeft = (f32)(mobj->animList->w & 0x7FFF);

                bitMask = (mobj->animList++->w << 7) >> 22;

                if (bitMask & 0x01) {
                    mobj->texture.unk4C = mobj->animList->w;
                    mobj->animList++;
                }
                if (bitMask & 0x02) {
                    mobj->texture.unk6C = mobj->animList->w;
                    mobj->animList++;
                }
                if (bitMask & 0x04) {
                    mobj->texture.unk68 = mobj->animList->w;
                    mobj->animList++;
                }
                if (bitMask & 0x08) {
                    mobj->texture.unk74 = mobj->animList->w;
                    mobj->animList++;
                }
                if (bitMask & 0x10) {
                    mobj->texture.unk70 = mobj->animList->w;
                    mobj->animList++;
                }
                break;
            default:
                break;
        }
    } while (mobj->timeLeft <= 0.0f);
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
    f32 value;
    union Bytes4 color;
    s32 i;

    if (mobj->timeLeft != ANIMATION_DISABLED) {
        aobj = mobj->aobjList;
        while (aobj != NULL) {
            if (aobj->kind != ANIM_TYPE_NONE) {
                if (mobj->timeLeft != ANIMATION_FINISHED) {
                    aobj->time += mobj->animSpeed;
                }
                if (aobj->paramID < 36) {
                    switch (aobj->kind) {
                        case ANIM_TYPE_LINEAR:
                            value = aobj->initialValue + (aobj->time * aobj->rate);
                            break;
                        case ANIM_TYPE_CUBIC: {
                            f32 temp_f16 = SQ(aobj->invDuration);
                            f32 temp_f12 = SQ(aobj->time);
                            f32 temp_f18 = aobj->invDuration * temp_f12;
                            f32 temp_f14 = aobj->time * temp_f12 * temp_f16;
                            f32 temp_f20 = 2.0f * temp_f14 * aobj->invDuration;
                            f32 temp_f22 = 3.0f * temp_f12 * temp_f16;
                            f32 temp_f24 = temp_f14 - temp_f18;

                            value = (aobj->initialValue * ((temp_f20 - temp_f22) + 1.0f)) +
                                    (aobj->targetValue * (temp_f22 - temp_f20)) +
                                    (aobj->rate * ((temp_f24 - temp_f18) + aobj->time)) + (aobj->targetRate * temp_f24);
                            break;
                        }
                        case ANIM_TYPE_STEP:
                            value = aobj->invDuration <= aobj->time ? aobj->targetValue : aobj->initialValue;
                            break;
                        default:
                            break;
                    }
                    switch (aobj->paramID) {
                        case 13:
                            mobj->imageIndex = value;
                            break;
                        case 14:
                            mobj->texture.unk14 = value;
                            break;
                        case 15:
                            mobj->texture.unk18 = value;
                            break;
                        case 16:
                            mobj->texture.scaleS = value;
                            break;
                        case 17:
                            mobj->texture.scaleT = value;
                            break;
                        case 18:
                            mobj->nextImageIndex = value;
                            break;
                        case 19:
                            mobj->texture.unk3C = value;
                            break;
                        case 20:
                            mobj->texture.unk40 = value;
                            break;
                        case 21:
                            mobj->lodLevel = value;
                            break;
                        case 22:
                            mobj->paletteIndex = value;
                            break;
                        default:
                            break;
                    }
                } else {
                    if (aobj->kind != ANIM_TYPE_STEP) {
                        if (aobj->kind == ANIM_TYPE_LINEAR) {
                            s32 v1;

                            v1 = (s32)(aobj->time * aobj->invDuration * 256.0f);
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
                        color.i = *(u32*)(aobj->invDuration <= aobj->time ? aobj->unk14_color : aobj->unk10_color);
                    }

                    switch (aobj->paramID) {
                        case 37:
                            mobj->texture.primR = color.i;
                            break;
                        case 38:
                            mobj->texture.envR = color.i;
                            break;
                        case 39:
                            mobj->texture.blendR = color.i;
                            break;
                        case 40:
                            mobj->texture.lightColor1 = color.i;
                            break;
                        case 41:
                            mobj->texture.lightColor2 = color.i;
                            break;
                    }
                }
            }
            aobj = aobj->next;
        }

        if (mobj->timeLeft == ANIMATION_FINISHED) {
            mobj->timeLeft = ANIMATION_DISABLED;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/D150/func_8000E4E4.s")
void update_texture_animated_params(MObj* mobj);
#endif

void update_model_tree_animation(GObjCommon* obj) {
    DObj* dobj;
    MObj* mobj;

    dobj = obj->children;
    while (dobj != NULL) {
        process_model_animation(dobj);
        update_model_animated_params(dobj);

        mobj = dobj->mobjList;
        while (mobj != NULL) {
            process_texture_animation(mobj);
            func_8000E4E4(mobj);

            mobj = mobj->next;
        }

        if (dobj->firstChild != NULL) {
            dobj = dobj->firstChild;
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

AObj* get_aobj_by_param_id(AObj* aobj, u8 paramID) {
    while (aobj != NULL) {
        if (aobj->paramID == paramID) {
            return aobj;
        }
        aobj = aobj->next;
    }

    return NULL;
}

void func_8000EAE8(DObj* dobj, f32 duration) {
    AObj* aobj;

    aobj = dobj->aobjList;
    dobj->timeLeft = dobj->animSpeed + duration;
    while (aobj != NULL) {
        aobj->invDuration = 1.0f / duration;
        aobj = aobj->next;
    }
}

f32 get_dobj_param_value(DObj* dobj, s32 paramID) {
    switch (paramID) {
        case ANIM_PARAM_ROLL:
            return dobj->rotation.f[1];
        case ANIM_PARAM_PITCH:
            return dobj->rotation.f[2];
        case ANIM_PARAM_YAW:
            return dobj->rotation.f[3];
        case ANIM_PARAM_POSITION_X:
            return dobj->position.v.x;
        case ANIM_PARAM_POSITION_Y:
            return dobj->position.v.y;
        case ANIM_PARAM_POSITION_Z:
            return dobj->position.v.z;
        case ANIM_PARAM_SCALE_X:
            return dobj->scale.v.x;
        case ANIM_PARAM_SCALE_Y:
            return dobj->scale.v.y;
        case ANIM_PARAM_SCALE_Z:
            return dobj->scale.v.z;
    }
}

f32 func_8000EB98(struct UnkEC64Arg3* arg0, s32 paramID) {
    switch (paramID) {
        case ANIM_PARAM_ROLL:
            return arg0->rotation.x;
        case ANIM_PARAM_PITCH:
            return arg0->rotation.y;
        case ANIM_PARAM_YAW:
            return arg0->rotation.z;
        case ANIM_PARAM_POSITION_X:
            return arg0->position.x;
        case ANIM_PARAM_POSITION_Y:
            return arg0->position.y;
        case ANIM_PARAM_POSITION_Z:
            return arg0->position.z;
        case ANIM_PARAM_SCALE_X:
            return arg0->scale.x;
        case ANIM_PARAM_SCALE_Y:
            return arg0->scale.y;
        case ANIM_PARAM_SCALE_Z:
            return arg0->scale.z;
    }
}

s32 func_8000EC08(s32 arg0, DObj* dobj, f32* outValue, f32* outRate, AObj* aobjList, struct UnkEC64Arg3* arg5,
                  s32 paramID, s32 setRate, Vec3f* interpPos, s32* interpPosReady) {
    AObj* aobj;

    aobj = get_aobj_by_param_id(aobjList, paramID);

    if (aobj != NULL && aobj->kind != ANIM_TYPE_NONE) {
        if (arg0 == 0 && dobj->timeLeft != ANIMATION_FINISHED) {
            aobj->time += dobj->animSpeed;
        }
        *outValue = get_aobj_value(aobj);
        if (setRate) {
            *outRate = get_aobj_rate(aobj);
        }
    } else {
        if (paramID == ANIM_PARAM_POSITION_X || paramID == ANIM_PARAM_POSITION_Y || paramID == ANIM_PARAM_POSITION_Z) {
            if (*interpPosReady) {
                switch (paramID) {
                    case ANIM_PARAM_POSITION_X:
                        *outValue = interpPos->x;
                        break;
                    case ANIM_PARAM_POSITION_Y:
                        *outValue = interpPos->y;
                        break;
                    case ANIM_PARAM_POSITION_Z:
                        *outValue = interpPos->z;
                        break;
                }
            } else {
                aobj = get_aobj_by_param_id(aobjList, ANIM_PARAM_4);
                if (aobj != NULL && aobj->kind != ANIM_TYPE_NONE) {
                    if (arg0 == 0 && dobj->timeLeft != ANIMATION_FINISHED) {
                        aobj->time += dobj->animSpeed;
                    }
                    *outValue = get_aobj_value(aobj);
                    if (*outValue < 0.0f) {
                        *outValue = 0.0f;
                    } else if (*outValue > 1.0f) {
                        *outValue = 1.0f;
                    }
                    func_8001FCA4(interpPos, aobj->unk20, *outValue);
                    switch (paramID) {
                        case ANIM_PARAM_POSITION_X:
                            *outValue = interpPos->x;
                            break;
                        case ANIM_PARAM_POSITION_Y:
                            *outValue = interpPos->y;
                            break;
                        case ANIM_PARAM_POSITION_Z:
                            *outValue = interpPos->z;
                            break;
                    }

                    *interpPosReady = TRUE;
                } else {
                    if (arg0 == 0) {
                        if (arg5 == NULL) {
                            return TRUE;
                        }
                        *outValue = func_8000EB98(arg5, paramID);
                    } else {
                        *outValue = get_dobj_param_value(dobj, paramID);
                    }
                }
            }
        } else {
            if (arg0 == 0) {
                if (arg5 == NULL) {
                    return TRUE;
                }
                *outValue = func_8000EB98(arg5, paramID);
            } else {
                *outValue = get_dobj_param_value(dobj, paramID);
            }
        }
    }
    return FALSE;
}

#ifdef NON_MATCHING
void func_8000EECC(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, AObj* arg5) {
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

    sp3C = 2.0f * arg5->rate + arg5->targetRate;
    sp28 = -6.0f * sp2C;
    sp38 = sp28 * (arg5->targetValue - arg5->initialValue);

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

    sp3C = -(arg5->rate + 2.0f * arg5->targetRate);
    sp38 = sp28 * (arg5->initialValue - arg5->targetValue);

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
    t1 if (sp20 > 0.0f) {
        temp = (sqrtf(sp20) + sp1C) / sp24;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
        temp = (sp1C - sqrtf(sp20)) / sp24;
        if (*arg4 < temp) {
            *arg4 = temp;
        }
    }
    else if (sp20 == 0.0f) {
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

f32 func_8000F34C(DObj* dobj, union AnimCmd** animLists, f32 arg2, struct UnkEC64Arg3* arg3, s32 setRate, f32 duration,
                  f32 arg6, f32 arg7, f32 arg8) {
    AObj* origAobjList;
    AObj* spC0;
    AObj* newAObj;
    AObj* origAObj;

    f32 oldValue;
    f32 newValue;
    f32 oldRate;
    f32 newRate;
    f32 spA4;
    s32 i;
    s32 interpPosReady2;
    s32 interpPosReady;
    Vec3f interpPos2;
    Vec3f interpPos;

    spC0 = NULL;
    interpPosReady2 = 0;
    interpPosReady = 0;
    spA4 = 0.0f;

    if (animLists == NULL || *animLists == NULL) {
        if (arg3 == NULL) {
            om_dobj_reset_animation(dobj);
            return 0;
        }
    }

    origAobjList = dobj->aobjList;
    dobj->aobjList = NULL;
    if (animLists != NULL && *animLists != NULL) {
        dobj->animList = *animLists;
        dobj->timeLeft = ANIMATION_CHANGED;
        dobj->timePassed = arg2;
        process_model_animation(dobj);
        spC0 = dobj->aobjList;
        dobj->aobjList = NULL;
    }

    for (i = ANIM_PARAM_MODEL_MIN; i <= ANIM_PARAM_MODEL_MAX; i++) {
        if (i == ANIM_PARAM_4) {
            continue;
        }
        newRate = 0.0f;
        oldRate = 0.0f;

        if (func_8000EC08(0, dobj, &newValue, &newRate, spC0, arg3, i, setRate, &interpPos, &interpPosReady)) {
            continue;
        }

        func_8000EC08(1, dobj, &oldValue, &oldRate, origAobjList, arg3, i, setRate, &interpPos2, &interpPosReady2);

        if (newValue != oldValue || newRate != oldRate) {
            newAObj = om_dobj_add_aobj(dobj, i);

            if (i == ANIM_PARAM_ROLL || i == ANIM_PARAM_PITCH || i == ANIM_PARAM_YAW) {
                if (ABS(newValue - oldValue) > ABS(newValue - (oldValue + TAU))) {
                    oldValue += TAU;
                }

                if (ABS(newValue - oldValue) > ABS(newValue - (oldValue - TAU))) {
                    oldValue -= TAU;
                }
            }

            switch (setRate) {
                case 0:
                    newAObj->initialValue = oldValue;
                    newAObj->targetValue = newValue;
                    newAObj->kind = ANIM_TYPE_LINEAR;
                    newAObj->invDuration = 1.0f / duration;
                    newAObj->time = -dobj->animSpeed;
                    newAObj->rate = (newAObj->targetValue - newAObj->initialValue) / duration;
                    newAObj->targetRate = 0.0f;
                    break;
                case 1:
                case 2:
                    newAObj->initialValue = oldValue;
                    newAObj->targetValue = newValue;
                    newAObj->kind = ANIM_TYPE_CUBIC;
                    newAObj->invDuration = 1.0f / duration;
                    newAObj->time = -dobj->animSpeed;
                    newAObj->rate = oldRate;
                    newAObj->targetRate = newRate;

                    if (setRate == 2) {
                        func_8000EECC(i, arg6, arg7, arg8, &spA4, newAObj);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    origAObj = dobj->aobjList;
    dobj->aobjList = origAobjList;
    om_dobj_reset_animation(dobj);
    dobj->aobjList = spC0;
    om_dobj_reset_animation(dobj);

    dobj->aobjList = origAObj;
    dobj->animList = NULL;
    dobj->timeLeft = dobj->animSpeed + duration;
    dobj->timePassed = -dobj->animSpeed;

    return spA4;
}

f32 func_8000F708(GObjCommon* arg0, union AnimCmd** arg1, f32 arg2, struct UnkEC64Arg3* arg3, s32 arg4, f32 arg5,
                  f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 ret;
    UNUSED f32 tmp;
    DObj* dobj;

    dobj = arg0->children;
    arg0->animationTime = arg2;
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

    arg0->animationTime = 0.0f;
    return arg5;
}

#ifdef NON_MATCHING
void func_8000F8E4(GObjCommon* obj, union AnimCmd** animLists, f32 skipFrames, struct UnkEC64Arg3* arg3) {
    f32 value;
    DObj* dobj;
    s32 i;
    s32 sp7C;
    Vec3f sp70;
    s32 sp6C;

    dobj = obj->children;
    sp7C = 0;
    sp6C = TRUE;
    obj->animationTime = skipFrames;

    while (dobj != NULL) {
        if (*animLists != NULL) {
            set_model_animation(dobj, *animLists, skipFrames);
            dobj->animCBReceiver = sp6C;
            sp6C = 0;

            for (i = ANIM_PARAM_MODEL_MIN; i <= ANIM_PARAM_MODEL_MAX; i++) {
                if (i == ANIM_PARAM_4) {
                    continue;
                }

                func_8000EC08(0, dobj, &value, NULL, dobj->aobjList, arg3, i, 0, &sp70, &sp7C);

                switch (i) {
                    case ANIM_PARAM_ROLL:
                        dobj->rotation.f[1] = value;
                        break;
                    case ANIM_PARAM_PITCH:
                        dobj->rotation.f[2] = value;
                        break;
                    case ANIM_PARAM_YAW:
                        dobj->rotation.f[3] = value;
                        break;
                    case ANIM_PARAM_POSITION_X:
                        dobj->position.v.x = value;
                        break;
                    case ANIM_PARAM_POSITION_Y:
                        dobj->position.v.y = value;
                        break;
                    case ANIM_PARAM_POSITION_Z:
                        dobj->position.v.z = value;
                        break;
                    case ANIM_PARAM_SCALE_X:
                        dobj->scale.v.x = value;
                        break;
                    case ANIM_PARAM_SCALE_Y:
                        dobj->scale.v.y = value;
                        break;
                    case ANIM_PARAM_SCALE_Z:
                        dobj->scale.v.z = value;
                        break;
                }
            }
        } else {
            dobj->timeLeft = ANIMATION_DISABLED;
            dobj->animCBReceiver = FALSE;
            if (arg3 != NULL) {
                dobj->position.v = arg3->position;
                *((Vec3f*)&dobj->rotation.f[1]) = arg3->rotation;
                dobj->scale.v = arg3->scale;
            }
        }

        animLists++;

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
    om_dobj_append_mtx(dobj, MTX_TYPE_ROTATE_RPY_TRANSLATE, 0);
    om_dobj_append_mtx(dobj, MTX_TYPE_SCALE, 0);
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

void func_8000FBC4(GObjCommon* obj, struct UnkEC64Arg3* arg1, DObj** arg2) {
    int i;
    DObj* dobj;
    s32 trunc;
    DObj* sp44[18];

    for (i = 0; i < ARRAY_COUNT(sp44); i++) {
        sp44[i] = NULL;
    }

    while (arg1->unk00 != 18) {
        trunc = arg1->unk00 & 0xFFF;
        if (trunc != 0) {
            dobj = sp44[trunc] = om_dobj_add_child(sp44[trunc - 1], arg1->unk04);
        } else {
            dobj = sp44[0] = om_gobj_add_dobj(obj, arg1->unk04);
        }
        if (arg1->unk00 & 0xF000) {
            om_dobj_append_mtx(dobj, MTX_TYPE_TRANSLATE, 0);
        }

        if (arg1->unk00 & 0x8000) {
            om_dobj_append_mtx(dobj, MTX_TYPE_44, 0);
        } else if (arg1->unk00 & 0x4000) {
            om_dobj_append_mtx(dobj, MTX_TYPE_46, 0);
        } else if (arg1->unk00 & 0x2000) {
            om_dobj_append_mtx(dobj, MTX_TYPE_48, 0);
        } else if (arg1->unk00 & 0x1000) {
            om_dobj_append_mtx(dobj, MTX_TYPE_50, 0);
        } else {
            func_8000FAFC(dobj);
        }
        dobj->position.v = arg1->position;
        *((Vec3f*)&dobj->rotation.f[1]) = arg1->rotation;
        dobj->scale.v = arg1->scale;

        if (arg2 != NULL) {
            *arg2++ = dobj;
        }

        arg1++;
    }
}

void func_8000FDA0(DObj* dobj, u8 arg1, u8 arg2, u8 arg3) {
    if (arg1 != 0) {
        om_dobj_append_mtx(dobj, arg1, 0);
    }
    if (arg2 != 0) {
        om_dobj_append_mtx(dobj, arg2, 0);
    }
    if (arg3 != 0) {
        om_dobj_append_mtx(dobj, arg3, 0);
    }
}

void func_8000FE08(struct DObj* arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    s32 isTranslate = FALSE;
    s32 isScale = 0;
    s32 phi_v1 = 0;
    s32 phi_t2 = 0;

    switch (arg1) {
        case MTX_TYPE_TRANSLATE:
            isTranslate = TRUE;
            break;
        case MTX_TYPE_ROTATE_RPY:
            phi_v1 = 1;
            break;
        case MTX_TYPE_ROTATE_RPY_TRANSLATE:
            phi_v1 = 1;
            isTranslate = TRUE;
            break;
        case MTX_TYPE_ROTATE_RPY_TRANSLATE_SCALE:
            isScale = 1;
            phi_v1 = 1;
            isTranslate = TRUE;
            break;
        case MTX_TYPE_ROTATE_PYR:
            phi_v1 = 2;
            break;
        case MTX_TYPE_ROTATE_PYR_TRANSLATE:
            phi_v1 = 2;
            isTranslate = TRUE;
            break;
        case MTX_TYPE_ROTATE_PYR_TRANSLATE_SCALE:
            phi_v1 = 2;
            isScale = 1;
            isTranslate = TRUE;
            break;
        case MTX_TYPE_SCALE:
            isScale = 1;
            break;
        case MTX_TYPE_51:
            phi_t2 = 1;
            isScale = 1;
            phi_v1 = 1;
            isTranslate = TRUE;
            break;
        case MTX_TYPE_52:
            phi_v1 = 2;
            phi_t2 = 1;
            isScale = 1;
            isTranslate = TRUE;
            break;
        case MTX_TYPE_54:
            isScale = 1;
            phi_v1 = 1;
            isTranslate = TRUE;
            break;
    }

    switch (arg2) {
        case MTX_TYPE_ROTATE_RPY:
            phi_v1 = 1;
            break;
        case MTX_TYPE_ROTATE_PYR:
            phi_v1 = 2;
            break;
        case MTX_TYPE_SCALE:
            isScale = 1;
            break;
    }

    if (arg3 == MTX_TYPE_SCALE) {
        isScale = 1;
    }

    if (isTranslate) {
        if (phi_t2) {
            om_dobj_append_mtx(arg0, MTX_TYPE_55, 0);
        } else {
            om_dobj_append_mtx(arg0, MTX_TYPE_TRANSLATE, 0);
        }
    }

    if (arg4 & 0x4000) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, MTX_TYPE_46, 0);
        } else {
            om_dobj_append_mtx(arg0, MTX_TYPE_45, 0);
        }
    } else if (arg4 & 0x2000) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, MTX_TYPE_48, 0);
        } else {
            om_dobj_append_mtx(arg0, MTX_TYPE_47, 0);
        }
    } else if (arg4 & 0x1000) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, MTX_TYPE_50, 0);
        } else {
            om_dobj_append_mtx(arg0, MTX_TYPE_49, 0);
        }
    } else if (isScale) {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, MTX_TYPE_44, 0);
        } else {
            om_dobj_append_mtx(arg0, MTX_TYPE_43, 0);
        }
    } else {
        if (phi_v1 == 1) {
            om_dobj_append_mtx(arg0, MTX_TYPE_42, 0);
        } else {
            om_dobj_append_mtx(arg0, MTX_TYPE_41, 0);
        }
    }
}

void func_800100A0(struct GObjCommon* obj, struct UnkEC64Arg3* arg1, struct DObj** arg2, u8 arg3, u8 arg4, u8 arg5) {
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
            dobj = sp54[0] = om_gobj_add_dobj(obj, arg1->unk04);
        }

        if (arg1->unk00 & 0xF000) {
            func_8000FE08(dobj, arg3, arg4, arg5, arg1->unk00 & 0xF000);
        } else {
            func_8000FDA0(dobj, arg3, arg4, arg5);
        }

        dobj->position.v = arg1->position;
        *((Vec3f*)&dobj->rotation.f[1]) = arg1->rotation;
        dobj->scale.v = arg1->scale;

        if (arg2 != NULL) {
            *arg2 = dobj;
            arg2++;
        }
        arg1++;
    }
}

void func_80010230(struct GObjCommon* arg0, struct UnkEC64Arg3* arg1, struct Texture*** arg2, struct DObj** arg3,
                   u8 arg4, u8 arg5, u8 arg6) {
    int i;
    struct DObj* dobj;
    s32 trunc;
    struct DObj* sp5C[18];
    struct Texture** csr;
    struct Texture* msub;

    for (i = 0; i < ARRAY_COUNT(sp5C); i++) {
        sp5C[i] = NULL;
    }

    while (arg1->unk00 != 18) {
        trunc = arg1->unk00 & 0xFFF;
        if (trunc) {
            dobj = sp5C[trunc] = om_dobj_add_child(sp5C[trunc - 1], arg1->unk04);
        } else {
            dobj = sp5C[0] = om_gobj_add_dobj(arg0, arg1->unk04);
        }
        if (arg1->unk00 & 0xF000) {
            func_8000FE08(dobj, arg4, arg5, arg6, arg1->unk00 & 0xF000);
        } else {
            func_8000FDA0(dobj, arg4, arg5, arg6);
        }
        dobj->position.v = arg1->position;
        *((Vec3f*)&dobj->rotation.f[1]) = arg1->rotation;
        dobj->scale.v = arg1->scale;

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
        if (arg3 != NULL) {
            *arg3 = dobj;
            arg3++;
        }
        arg1++;
    }
}

void func_80010404(GObjCommon* arg0, Texture*** arg1) {
    DObj* dobj;
    Texture** csr;
    Texture* msub;

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
        dobj = func_8000C550(dobj);
    }
}

void func_80010498(GObjCommon* obj, struct UnkEC64Arg3* arg1) {
    DObj* dobj;

    dobj = obj->children;
    while (dobj != NULL && arg1->unk00 != 18) {
        dobj->position.v = arg1->position;
        *((Vec3f*)&dobj->rotation.f[1]) = arg1->rotation;
        dobj->scale.v = arg1->scale;

        arg1++;
        dobj = func_8000C550(dobj);
    }
}

void set_camera_animation(OMCamera* cam, union AnimCmd* animList, f32 skipFrames) {
    AObj* aobj;

    aobj = cam->aobjList;
    while (aobj != NULL) {
        aobj->kind = ANIM_TYPE_NONE;
        aobj = aobj->next;
    }
    cam->animList = animList;
    cam->timeLeft = ANIMATION_CHANGED;
    cam->timePassed = skipFrames;
}

void process_camera_animation(OMCamera* cam) {
    struct AObj* aobjArray[ANIM_PARAM_CAMERA_MAX - ANIM_PARAM_CAMERA_MIN + 1];
    struct AObj* aobj;
    s32 i;
    u32 cmd;
    u32 bitMask;
    f32 duration;

    if (cam->timeLeft != ANIMATION_DISABLED) {
        if (cam->timeLeft == ANIMATION_CHANGED) {
            cam->timeLeft = -cam->timePassed;
        } else {
            cam->timeLeft -= cam->animSpeed;
            cam->timePassed += cam->animSpeed;
            cam->obj->animationTime = cam->timePassed;

            if (cam->timeLeft > 0.0f) {
                return;
            }
        }

        for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
            aobjArray[i] = NULL;
        }

        aobj = cam->aobjList;
        while (aobj != NULL) {
            if (aobj->paramID >= ANIM_PARAM_CAMERA_MIN && aobj->paramID <= ANIM_PARAM_CAMERA_MAX) {
                aobjArray[aobj->paramID - ANIM_PARAM_CAMERA_MIN] = aobj;
            }
            aobj = aobj->next;
        }

        do {
            if (cam->animList == NULL) {
                aobj = cam->aobjList;
                while (aobj != NULL) {
                    if (aobj->kind != ANIM_TYPE_NONE) {
                        aobj->time += cam->animSpeed + cam->timeLeft;
                    }
                    aobj = aobj->next;
                }
                cam->timePassed = cam->timeLeft;
                cam->timeLeft = ANIMATION_FINISHED;
                return;
            }

            cmd = cam->animList->w >> 25;
            switch (cmd) {
                case ANIM_CMD_SET_VALUE_ZERO_RATE_LAST:
                case ANIM_CMD_SET_VALUE_ZERO_RATE:
                    duration = (f32)(cam->animList->w & 0x7FFF);
                    bitMask = (cam->animList++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                        if (bitMask == 0) {
                            break;
                        }
                        if (bitMask & 1) {
                            if (aobjArray[i] == NULL) {
                                aobjArray[i] = om_camera_add_aobj(cam, i + ANIM_PARAM_CAMERA_MIN);
                            }
                            aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                            aobjArray[i]->targetValue = cam->animList->f;
                            cam->animList++;
                            aobjArray[i]->rate = aobjArray[i]->targetRate;
                            aobjArray[i]->targetRate = 0.0f;
                            aobjArray[i]->kind = ANIM_TYPE_CUBIC;

                            if (duration != 0.0f) {
                                aobjArray[i]->invDuration = 1.0f / duration;
                            }
                            aobjArray[i]->time = -cam->timeLeft - cam->animSpeed;
                        }
                        bitMask >>= 1;
                    }
                    if (cmd == ANIM_CMD_SET_VALUE_ZERO_RATE_LAST) {
                        cam->timeLeft += duration;
                    }
                    break;
                case ANIM_CMD_SET_VALUE_LAST:
                case ANIM_CMD_SET_VALUE:
                    duration = (f32)(cam->animList->w & 0x7FFF);
                    bitMask = (cam->animList++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                        if (bitMask == 0) {
                            break;
                        }
                        if (bitMask & 1) {
                            if (aobjArray[i] == NULL) {
                                aobjArray[i] = om_camera_add_aobj(cam, i + ANIM_PARAM_CAMERA_MIN);
                            }
                            aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                            aobjArray[i]->targetValue = cam->animList->f;
                            cam->animList++;
                            aobjArray[i]->kind = ANIM_TYPE_LINEAR;

                            if (duration != 0.0f) {
                                aobjArray[i]->rate = (aobjArray[i]->targetValue - aobjArray[i]->initialValue) / duration;
                            }
                            aobjArray[i]->time = -cam->timeLeft - cam->animSpeed;
                            aobjArray[i]->targetRate = 0.0f;
                        }
                        bitMask >>= 1;
                    }
                    if (cmd == ANIM_CMD_SET_VALUE_LAST) {
                        cam->timeLeft += duration;
                    }
                    break;
                case ANIM_CMD_SET_VALUE_WITH_RATE_LAST:
                case ANIM_CMD_SET_VALUE_WITH_RATE:
                    duration = (f32)(cam->animList->w & 0x7FFF);
                    bitMask = (cam->animList++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                        if (bitMask == 0) {
                            break;
                        }
                        if (bitMask & 1) {
                            if (aobjArray[i] == NULL) {
                                aobjArray[i] = om_camera_add_aobj(cam, i + ANIM_PARAM_CAMERA_MIN);
                            }

                            aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                            aobjArray[i]->targetValue = cam->animList->f;
                            cam->animList++;
                            aobjArray[i]->rate = aobjArray[i]->targetRate;
                            aobjArray[i]->targetRate = cam->animList->f;
                            cam->animList++;
                            aobjArray[i]->kind = ANIM_TYPE_CUBIC;
                            if (duration != 0.0f) {
                                aobjArray[i]->invDuration = 1.0f / duration;
                            }
                            aobjArray[i]->time = -cam->timeLeft - cam->animSpeed;
                        }

                        bitMask >>= 1;
                    }

                    if (cmd == ANIM_CMD_SET_VALUE_WITH_RATE_LAST) {
                        cam->timeLeft += duration;
                    }
                    break;
                case ANIM_CMD_SET_TARGET_RATE:
                    bitMask = (cam->animList++->w << 7) >> 22;
                    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                        if (bitMask == 0) {
                            break;
                        }
                        if (bitMask & 1) {
                            if (aobjArray[i] == NULL) {
                                aobjArray[i] = om_camera_add_aobj(cam, i + ANIM_PARAM_CAMERA_MIN);
                            }

                            aobjArray[i]->targetRate = cam->animList->f;
                            cam->animList++;
                        }
                        bitMask >>= 1;
                    }
                    break;
                case ANIM_CMD_WAIT:
                    cam->timeLeft += (f32)(cam->animList++->w & 0x7FFF);
                    break;
                case ANIM_CMD_SET_VALUE_AFTER_LAST:
                case ANIM_CMD_SET_VALUE_AFTER:
                    duration = (f32)(cam->animList->w & 0x7FFF);
                    bitMask = (cam->animList++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                        if (bitMask == 0) {
                            break;
                        }
                        if (bitMask & 1) {
                            if (aobjArray[i] == NULL) {
                                aobjArray[i] = om_camera_add_aobj(cam, i + ANIM_PARAM_CAMERA_MIN);
                            }

                            aobjArray[i]->initialValue = aobjArray[i]->targetValue;
                            aobjArray[i]->targetValue = cam->animList->f;
                            cam->animList++;
                            aobjArray[i]->kind = ANIM_TYPE_STEP;
                            aobjArray[i]->invDuration = duration;
                            aobjArray[i]->time = -cam->timeLeft - cam->animSpeed;
                            aobjArray[i]->targetRate = 0.0f;
                        }
                        bitMask >>= 1;
                    }

                    if (cmd == ANIM_CMD_SET_VALUE_AFTER_LAST) {
                        cam->timeLeft += duration;
                    }

                    break;
                case ANIM_CMD_SET_ANIMATION:
                    cam->animList++;
                    cam->animList = cam->animList->ptr;
                    cam->timePassed = -cam->timeLeft;
                    cam->obj->animationTime = -cam->timeLeft;
                    break;
                case ANIM_CMD_JUMP:
                    cam->animList++;
                    cam->animList = cam->animList->ptr;
                    break;
                case ANIM_CMD_12:
                    duration = (f32)(cam->animList->w & 0x7FFF);
                    bitMask = (cam->animList++->w << 7) >> 22;

                    for (i = 0; i < ARRAY_COUNT(aobjArray); i++) {
                        if (bitMask == 0) {
                            break;
                        }
                        if (bitMask & 1) {
                            if (aobjArray[i] == NULL) {
                                aobjArray[i] = om_camera_add_aobj(cam, i + ANIM_PARAM_CAMERA_MIN);
                            }

                            aobjArray[i]->time += duration;
                        }
                        bitMask >>= 1;
                    }
                    break;
                case ANIM_CMD_13:
                    bitMask = (cam->animList++->w << 7) >> 22;

                    if (bitMask & 0x08) {
                        if (aobjArray[3] == NULL) {
                            aobjArray[3] = om_camera_add_aobj(cam, ANIM_PARAM_CAMERA_28);
                        }

                        aobjArray[3]->unk20 = cam->animList->ptr;
                        cam->animList++;
                    }
                    if (bitMask & 0x80) {
                        if (aobjArray[7] == NULL) {
                            aobjArray[7] = om_camera_add_aobj(cam, ANIM_PARAM_CAMERA_32);
                        }

                        aobjArray[7]->unk20 = cam->animList->ptr;
                        cam->animList++;
                    }
                    break;
                case ANIM_CMD_END:
                    aobj = cam->aobjList;
                    while (aobj != NULL) {
                        if (aobj->kind != ANIM_TYPE_NONE) {
                            aobj->time += cam->animSpeed + cam->timeLeft;
                        }
                        aobj = aobj->next;
                    }
                    cam->timePassed = cam->timeLeft;
                    cam->timeLeft = ANIMATION_FINISHED;
                    return; // not break
                case ANIM_CMD_23:
                    cam->timeLeft += (f32)(cam->animList++->w & 0x7FFF);
                    cam->animList += 2;
                    break;
                default:
                    break;
            }
        } while (cam->timeLeft <= 0.0f);
    }
}

void update_camera_animated_params(OMCamera* camera) {
    AObj* aobj;
    f32 tmp;

    if (camera->timeLeft != ANIMATION_DISABLED) {
        aobj = camera->aobjList;
        while (aobj != NULL) {
            if (aobj->kind != ANIM_TYPE_NONE) {
                if (camera->timeLeft != ANIMATION_FINISHED) {
                    aobj->time += camera->animSpeed;
                }

                if (!(camera->obj->flags & GOBJ_FLAG_2)) {
                    switch (aobj->paramID) {
                        case ANIM_PARAM_CAMERA_XEYE:
                            camera->viewMtx.lookAt.xEye = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_YEYE:
                            camera->viewMtx.lookAt.yEye = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_ZEYE:
                            camera->viewMtx.lookAt.zEye = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_28:
                            tmp = get_aobj_value(aobj);
                            if (tmp < 0.0f) {
                                tmp = 0.0f;
                            } else if (tmp > 1.0f) {
                                tmp = 1.0f;
                            }
                            func_8001FCA4(&camera->viewMtx.lookAt.xEye, aobj->unk20, tmp);
                            break;
                        case ANIM_PARAM_CAMERA_XAT:
                            camera->viewMtx.lookAt.xAt = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_YAT:
                            camera->viewMtx.lookAt.yAt = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_ZAT:
                            camera->viewMtx.lookAt.zAt = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_32:
                            tmp = get_aobj_value(aobj);
                            if (tmp < 0.0f) {
                                tmp = 0.0f;
                            } else if (tmp > 1.0f) {
                                tmp = 1.0f;
                            }
                            func_8001FCA4(&camera->viewMtx.lookAt.xAt, aobj->unk20, tmp);
                            break;
                        case ANIM_PARAM_CAMERA_ROLL:
                            camera->viewMtx.lookAtRoll.roll = get_aobj_value(aobj);
                            break;
                        case ANIM_PARAM_CAMERA_FOVY:
                            camera->perspMtx.persp.fovy = get_aobj_value(aobj);
                            break;
                    }
                }
            }

            aobj = aobj->next;
        }

        if (camera->timeLeft == ANIMATION_FINISHED) {
            camera->timeLeft = ANIMATION_DISABLED;
        }
    }
}

void update_camera_animation(GObjCommon* obj) {
    OMCamera* cam;
    cam = obj->children;
    process_camera_animation(cam);
    update_camera_animated_params(cam);
}

s32 get_animation_duration(union AnimCmd** arg0) {
    union AnimCmd* list;
    u32 bitMask;
    s32 total = 0;
    s32 i;
    u32 cmd;
    u32 id;

    while (*arg0 == NULL) {
        arg0++;
    }

    list = *arg0;

    cmd = list->w;
    id = cmd >> 25;

    while (TRUE) {
        switch (id) {
            case ANIM_CMD_SET_VALUE_LAST:
            case ANIM_CMD_SET_VALUE_ZERO_RATE_LAST:
            case ANIM_CMD_SET_VALUE_AFTER_LAST:
                total += cmd & 0x7FFF;
                /* fallthrough */
            case ANIM_CMD_SET_VALUE:
            case ANIM_CMD_SET_TARGET_RATE:
            case ANIM_CMD_SET_VALUE_ZERO_RATE:
            case ANIM_CMD_SET_VALUE_AFTER:
                bitMask = (cmd << 7) >> 22;
                list++;
                for (i = 0; i < 10; i++) {
                    if (bitMask == 0) {
                        break;
                    }
                    if (bitMask & 1) {
                        list++;
                    }

                    bitMask >>= 1;
                }
                cmd = list->w;
                id = cmd >> 25;
                break;
            case ANIM_CMD_SET_VALUE_WITH_RATE_LAST:
                total += cmd & 0x7FFF;
                /* fallthrough */
            case ANIM_CMD_SET_VALUE_WITH_RATE:
                bitMask = (cmd << 7) >> 22;
                list++;
                for (i = 0; i < 10; i++) {
                    if (bitMask == 0) {
                        break;
                    }
                    if (bitMask & 1) {
                        list += 2;
                    }

                    bitMask >>= 1;
                }
                cmd = list->w;
                id = cmd >> 25;
                break;
            case ANIM_CMD_WAIT:
            case ANIM_CMD_SET_FLAGS:
                total += cmd & 0x7FFF;
                list++;
                cmd = list->w;
                id = cmd >> 25;
                break;
            case ANIM_CMD_12:
                list++;
                cmd = list->w;
                id = cmd >> 25;
                break;
            case ANIM_CMD_13:
                list += 2;
                cmd = list->w;
                id = cmd >> 25;
                break;
            case ANIM_CMD_17:
                total += cmd & 0x7FFF;
                bitMask = (cmd << 7) >> 22;
                list++;
                for (i = 4; i < 14; i++) {
                    if (bitMask == 0) {
                        break;
                    }
                    if (bitMask & 1) {
                        list++;
                    }

                    bitMask >>= 1;
                }
                cmd = list->w;
                id = cmd >> 25;
                break;
            case ANIM_CMD_END:
                return total;
            case ANIM_CMD_JUMP:
            case ANIM_CMD_SET_ANIMATION:
                return -total;
            case ANIM_CMD_16:
                break;
        }
    }
}
