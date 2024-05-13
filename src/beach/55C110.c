#include "common.h"
#include "beach.h"
#include "world/world.h"
#include "ld_addrs.h"

// TODO type
u32 D_beach_802CBE10[] = {
    0x00FD0107,
    0x01560108,
    0x01570108,
    0x01580108,
    0x00750008,
    0x008E0008,
    0x008F0006,
    0x00FE0108,
    0x00FF0108,
    0x006E0008,
    0x0104010A,
    0x0105010A,
    0x0106010A,
    0x0069000A,
    0x0068000A,
    0x0125010A,
    0x0126010A,
    0x0127010A,
    0x0163010A,
    0x0128010A,
    0x0129010A,
    0x007D0007,
    0x00D20108,
    0x00D30108,
    0x01780008,
    0x00720008,
    0x017F0228,
    0x00FB010A,
    0x00FC010A,
    0x016E000A,
    0x0079000A,
    0x007A000A,
    0x012A0109,
    0x012B0109,
    0x012C0109,
    0x012D0109,
    0x012E0109,
    0x01070108,
    0x0108010A,
    0x01090108,
    0x010A0108,
    0x010B0208,
    0x010C0208,
    0x01730008,
    0x007F000A,
    0x0168000A,
    0x01000108,
    0x01010107,
    0x007C0007,
    0x0034010F,
    0x0130010A,
    0x016B000A,
};

AnimalDef beachAnimalData[] = {
    {
        0xC,
        func_beach_802C7E38,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x54,
        func_beach_802C7E38,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x85,
        func_beach_802C7AD0,
        func_80362D2C_50313C,
        NULL
    },
    {
        0x73,
        func_beach_802CA05C,
        func_80363EB4_5042C4,
        func_80364280_504690
    },
    {
        0x8F,
        func_beach_802C5C28,
        func_80363EB4_5042C4,
        func_80364280_504690
    },
    {
        0x83,
        func_beach_802C8274,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x71,
        func_beach_802C9A7C,
        func_80363EB4_5042C4,
        func_80364280_504690
    },
    {
        0x34,
        func_beach_802C7270,
        func_beach_802C71E8,
        func_80364280_504690
    },
    {
        0x10,
        func_beach_802C8D3C,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x7B,
        func_beach_802C91CC,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x81,
        func_beach_802CAC48,
        func_80363EB4_5042C4,
        func_80364280_504690
    },
    {
        0x19,
        func_beach_802CBCA8,
        func_80363EB4_5042C4,
        func_80364280_504690
    },
    {
        0x3EB,
        func_8035E0D4_4FE4E4,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x3EC,
        func_beach_802CA418,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x3ED,
        func_beach_802CA2BC,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    {
        0x3E9,
        func_beach_802CAB14,
        func_80363DBC_5041CC,
        func_80364280_504690
    },
    { },
};

AnimalDef D_beach_802CBFF4 = {
    0x81,
    func_beach_802CAC48,
    func_80363EB4_5042C4,
    func_80364280_504690
};

void beachSpawnMagikarpAtGObj(GObj* obj) {
    DObj* a0;
    struct Mtx3Float* position;
    GObj* pokemonObj;
    ObjectSpawn spawn;
    WorldBlock* roomA;
    PokemonDef def = D_beach_802CBFF4;

    roomA = getCurrentWorldBlock(); // TODO: type
    spawn.id = PokemonID_MAGIKARP;
    spawn.translation.x = 0.0;
    spawn.translation.y = 0.0;
    spawn.translation.z = 0.0;
    spawn.euler.x = 0.0;
    spawn.euler.y = 0.0;
    spawn.euler.z = 0.0;
    spawn.scale.x = 1.0;
    spawn.scale.y = 1.0;
    spawn.scale.z = 1.0;

    pokemonObj = pokemonAddOne(roomA, roomA, &spawn, &def);

    position = &obj->data.dobj->position;
    a0 = pokemonObj->data.dobj;
    GET_TRANSFORM(a0)->pos.v.x = position->v.x;
    GET_TRANSFORM(a0)->pos.v.y = position->v.y;
    GET_TRANSFORM(a0)->pos.v.z = position->v.z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/beach/55C110/func_beach_802C416C.s")

void beachPokemonAdd(WorldBlock* param_1, WorldBlock* param_2) {
    pokemonAdd(param_1, param_2, beachPokemonData);
}

void func_beach_802C42F8(WorldBlock* param_1, WorldBlock* param_2) {
    func_8036406C_50447C(param_1, param_2, beachPokemonData);
}

void func_beach_802C431C(WorldBlock* arg0) {
    func_803641B8_5045C8(arg0, beachPokemonData);
}

void func_beach_802C4340(void) {
    setHeightMap(&D_beach_80318F00);
    createWorld(&D_8011B914, 99, 100, 126, 9, 3, beachPokemonAdd, func_beach_802C431C, func_beach_802C42F8);
    setSkyBoxFollowPlayer();
    func_80363928_503D38(0x80, 0xE4, 3, 5);
}

void func_beach_802C43CC(s32 arg0) {
    D_beach_802CC018 = arg0;
    func_800A19D8(arg0);
    func_80359074_4F9484();
    func_8036650C_50691C();
    auStopBGM();
    auStopAllSounds();
    destroyWorld();
    func_80356FBC_4F73CC();
    func_803586C0_4F8AD0();
    func_800067DC();
}

void func_beach_802C4430(s32 arg0) {
}

void func_beach_802C4438(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/beach/55C110/func_beach_802C4440.s")

void func_beach_802C44D4(void) {
    void* sp1C;
    void* temp_v0;
    void* temp_v0_2;

    sp1C = func_800A73C0((u32) AB5860_ROM_START, (u32) AB5860_ROM_END);
    temp_v0 = func_800A73C0((u32) AB5980_ROM_START, (u32) AB5980_ROM_END);
    if (sp1C != NULL && temp_v0 != NULL) {
        func_800A1ED0(3, sp1C, temp_v0);
    }
    sp1C = func_800A73C0((u32) AB85E0_ROM_START, (u32) AB85E0_ROM_END);
    temp_v0_2 = func_800A73C0((u32) AB8780_ROM_START, (u32) AB8780_ROM_END);
    if (sp1C != NULL && temp_v0_2 != NULL) {
        func_800A1ED0(0, sp1C, temp_v0_2);
    }
    D_80382D10_523120 = func_800A2094(4, 100, func_803586F8_4F8B08());
    D_80382D14_523124 = func_800A5E08(0xA);
    func_800A5DF4(0xC0, 0x30);
}

#pragma GLOBAL_ASM("asm/nonmatchings/beach/55C110/func_beach_802C45C0.s")

void func_beach_802C4738(s32 arg0) {
}

s32 func_beach_802C4740(s32 sceneID) {
    D_beach_802CC038.gtlSetup.heapSize = (s32) beach_code_VRAM - (s32) &D_801B0310;
    gtlSetIntervals(1, 2);
    gtlDisableNearClipping(1);
    omSetupScene(&D_beach_802CC038);

    switch (D_beach_802CC018) {
        case 6:
            return SCENE_BEACH;
        case 7:
        case 8:
            return SCENE_8;
        default:
            if (func_8009BC68() > 0) {
                return SCENE_CAMERA_CHECK;
            }
            func_800AAED0(0x10);
            return SCENE_13;
    }
}
