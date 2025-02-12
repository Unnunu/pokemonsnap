#include "common.h"
#include "ld_addrs.h"
#include "camera_check.h"

void func_camera_check_801DCA60(void);

Gfx D_camera_check_801E3FF0[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsSPClipRatio(FRUSTRATIO_6),
    gsSPEndDisplayList(),
};

ScreenSettings D_camera_check_801E4060 = {
    D_803B5000,
    D_803DA800,
    NULL,
    NULL,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    0x16A99,
};

SceneSetup D_camera_check_801E407C = {
    {
        0,
        omUpdateAll,
        omDrawAll,
        camera_check_VRAM_END,
        0,
        0x1,
        0x1,
        0xD000,
        0x2000,
        0,
        0,
        0x3000,
        0x2,
        0x3000,
        func_800A1A50,
        contUpdate,
    },
    0x20,
    0x2000,
    0x20,
    0,
    0x40,
    0x80,
    sizeof(GObj),
    0x400,
    0,
    0,
    0,
    0,
    0,
    sizeof(DObj),
    0,
    0x60,
    0x4,
    sizeof(OMCamera),
    func_camera_check_801DCA60,
};

u8 D_camera_check_802098F0[0x40000];

static void nullsub() {
}

static void nullsub2() {
}

void func_camera_check_801DC8D0(GObj* arg0) {
    while (true) {
        ohWait(1);
    }
}

void func_camera_check_801DC910(void) {
    UNUSED s32 temp_t3;
    OMCamera* camera;
    GObj* camObj;

    camObj = ohCreateCamera(0x200, ohUpdateDefault, 5, 5, ren_func_800191D8, 0x14, 0x100000, 0x100000, 1, 0,
                            func_camera_check_801DC8D0, 0, 1);
    camera = camObj->data.cam;
    camObj->unk_38 = 0x100000;
    camera->flags |= CAMERA_FLAG_2 | CAMERA_FLAG_4;
    camera->flags &= ~(CAMERA_FLAG_1 | CAMERA_FLAG_2);
    func_800A844C(camera, 0, 0, 640, 480);
    camera->viewMtx.lookAt.up.x = 0.0f;
    camera->viewMtx.lookAt.up.y = 0.0f;
    camera->viewMtx.lookAt.up.z = 1.0f;
    camera->viewMtx.lookAt.at = D_800AF0C4;
    camera->viewMtx.lookAt.eye = D_800AF094;
    camera->bgColor = 0x7101;
    camera->perspMtx.persp.scale = 1.0f;
    camera->perspMtx.persp.far = 8000.0f;
    camera->perspMtx.persp.fovy = 33.0f;
    camera->perspMtx.persp.near = camera->perspMtx.persp.far / 128.0;
}

void func_camera_check_801DCA60(void) {
    UIMem_CreateHeap(D_camera_check_802098F0, sizeof(D_camera_check_802098F0));
    func_800AAE28();
    func_camera_check_801DC910();
    func_80374D40_8484F0();
    UIText_Initialize();
    func_80375284_848A34(3);
    func_80375284_848A34(2);
    auSetBGMVolume(0, 0x7F00);
    func_camera_check_801DF938();
}

s32 func_camera_check_801DCACC(s32 arg0) {
    gtlDisableNearClipping(1);
    gtlSetIntervals(1, 2);
    viApplyScreenSettings(&D_camera_check_801E4060);
    D_camera_check_801E407C.gtlSetup.heapSize = (uintptr_t) window_VRAM - (uintptr_t) camera_check_VRAM_END;
    omSetupScene(&D_camera_check_801E407C);
    return D_800AF3C0;
}
