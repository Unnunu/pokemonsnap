#include "common.h"

extern Sprite sprite_menu_new_game_cursor;

// BSS
extern s32 D_80118110;
extern s32 D_80118114;
static s32 D_80168120;
static s32 D_80168124;
static s32 D_80168128;
static s8 D_8016812C;
static u8 D_80168130[16];
static u8 D_80168140[3];
static u8 D_80168143;
static u8 D_80168144;
static GObjCommon* D_80168148;
static u8 D_8016814C;

void func_800E2800_A5DBB0(void);
void func_800E2758_A5DB08(void);
void func_800E28C0_A5DC70(void);
u8** func_800E18B4_A5CC64(s32 arg0);

void func_800E2200_A5D5B0(void) {
    if (D_80168124 < 19) {
        func_80370038(4, 5);
        func_8036FFE0(D_80168120 * 13 + 25, D_80168124 * 10 + 22);
        return;
    }
    if (D_80168120 < 2) {
        func_80370038(4, 5);
        func_8036FFE0(D_80168120 * 13 + 25, D_80168124 * 10 + 22);
        return;
    }
    func_80370038(15, 5);
    func_8036FFE0(63, D_80168124 * 10 + 22);
}

void func_800E2314_A5D6C4(void) {
    D_80168120 = 2;
    D_80168124 = 19;
    func_800E2200_A5D5B0();
    func_800228E4(65);
}

void func_800E2350_A5D700(void) {
    s8 i;

    for (i = 0; i < 8; i++) {
        D_80168130[i] = 0;
    }
    D_80168143 = 0;
}

void func_800E2384_A5D734(void) {
    D_80168120 = D_80168124 = D_80168128 = 0;
    D_8016812C = 0;
}

void func_800E23B0_A5D760(void) {
    D_80168144 = 1;
    func_800E2384_A5D734();
    func_800E2350_A5D700();
}

void func_800E23E0_A5D790(s32 buttons) {
    if (D_80168124 < 19) {
        if (buttons & 0x10000) {
            D_80168124--;
            if (D_80168124 < 0) {
                D_80168124 = 19;
            }
        }
        if (buttons & 0x20000) {
            D_80168124++;
        }
        if (buttons & 0x80000) {
            D_80168120--;
            if (D_80168120 < 0) {
                D_80168120 = 4;
            }
        }
        if (buttons & 0x40000) {
            D_80168120++;
            D_80168120 %= 5;
        }
        if (D_80168124 >= 19) {
            D_80168128 = D_80168120;
            D_80168120 = D_80168120 / 3;
            func_800228E4(65);
        } else {
            func_800228E4(69);
        }
    } else {
        if (buttons & 0x10000) {
            D_80168124--;
        }
        if (buttons & 0x20000) {
            D_80168124++;
            D_80168124 = D_80168124 % 20;
        }
        if (buttons & 0x80000) {
            D_80168120--;
            if (D_80168120 < 0) {
                D_80168120 = 0;
            }
        }
        if (buttons & 0x40000) {
            D_80168120++;
            if (D_80168120 >= 3) {
                D_80168120 = 2;
            }
        }
        if (D_80168124 < 19) {
            D_80168120 = D_80168128;
            func_800228E4(69);
        } else {
            func_800228E4(65);
        }
    }
}

//TODO: make struct for argument
void func_800E2590_A5D940(s32* arg0) {
    func_800E23E0_A5D790(arg0[6]);
    func_800E2200_A5D5B0();
}

void func_800E25B8_A5D968(void) {
}

void func_800E25C0_A5D970(void) {
    u8* src;
    u8* dst;

    if (D_80168143 >= 14) {
        func_800E2314_A5D6C4();
        return;
    }

    dst = &D_80168130[D_80168143];
    src = D_80168140;    
    while (*src != 0) {
        *dst++ = *src++;
    }

    func_8036A8E4(D_80118110);
    func_8036B9EC(D_80118110, 0, 0);
    func_8036C898(D_80118110, D_80168130);
    if (D_80168143 < 14) {
        D_80168143 += 2;
    }
    if (D_80168143 == 14) {
        func_800E2314_A5D6C4();
    }
}

void func_800E26A0_A5DA50(void) {
    switch (D_80168120) {
        case 0:
            func_800E2800_A5DBB0();
            break;
        case 1:
            func_800E2758_A5DB08();
            break;
        default:
            func_800E28C0_A5DC70();
            D_80168144 = 2;
            break;
    }
}

void func_800E270C_A5DABC(void) {
    if (D_80168124 < 19) {
        func_800E25C0_A5D970();
        func_800228E4(66);
    } else {
        func_800E26A0_A5DA50();
    }
}

void func_800E2758_A5DB08(void) {
    u8 sp24[] = "　";

    if (D_80168143 < 14) {
        u8* src = sp24;
        u8* dst = &D_80168130[D_80168143];        
        
        func_800228E4(66);
       
        while (*src != 0) {
            *dst++ = *src++;
        }
        D_80168143 += 2;
    } else {
        func_800E2314_A5D6C4();
    }
}

void func_800E2800_A5DBB0(void) {
    if (D_80168143 == 0) {
        D_80168130[D_80168143] = 0;
        D_80168130[D_80168143 + 1] = 0;
        func_800E2314_A5D6C4();
    } else {
        D_80168130[--D_80168143] = 0;
        D_80168130[--D_80168143] = 0;
        func_800228E4(74);
    }
    func_8036A8E4(D_80118110);
    func_8036B9EC(D_80118110, 0, 0);
    func_8036C898(D_80118110, D_80168130);
}

void func_800E28C0_A5DC70(void) {
    u8 sp44[] = "Ｔｏｄｄ";
    u8 sp34[] = "　　　　　　　";
    u8* sp30;
    u8* sp2C;
    u8* sp28;

    sp30 = sp44;
    sp28 = sp34;
    sp2C = D_80168130;
    D_8016814C = 0;
    func_8000BCA8(1);

    while (*sp2C != 0) {
        if (*sp2C != *sp28++) {
            break;
        }
        sp2C++;
    }

    if (*sp2C == 0) {
        sp2C = D_80168130;
        while (TRUE) {
            *sp2C++ = *sp30;
            if (*sp30 == 0) {
                break;
            }
            sp30++;
        }
        func_8036A8E4(D_80118110);
        func_8036B9EC(D_80118110, 0, 0);
        func_8036C898(D_80118110, D_80168130);
        func_80022A58(96, 0x7FFF, 84, 1.0f, 10);
    }

    func_800BF44C_5C2EC(D_80168130);
    func_800228E4(66);
}

s32 func_800E2A24_A5DDD4(char* arg0) {
    u8 i = 0;
    char sp4[] = "　";

    while (i < 3) {
        if (sp4[i++] != *arg0++) {
            return 0;
        }
    }
    return 1;
}

void func_800E2A84_A5DE34(char* arg0) {
    u8** temp_v0;
    s16 sp1A;    
    SObj* unk_48;

    func_8036D344(12);
    sp1A = func_8036D4F0(D_80168130) + 203;
    unk_48 = D_80168148->children;
    unk_48->sp.y = 64;
    unk_48->sp.x = sp1A;
    if (D_80168143 >= 14 || D_80168124 >= 19) {
        if (D_80168143 < 14) {
            D_8016814C = 1;
        } else {
            D_8016814C = 0;
        }
        func_8036D1A4(D_80118114, 1);
        return;
    }
    temp_v0 = func_800E18B4_A5CC64(D_8016812C);
    arg0[0] = temp_v0[D_80168124 * 5 + D_80168120][0];
    arg0[1] = temp_v0[D_80168124 * 5 + D_80168120][1];
    arg0[2] = 0;
    if (func_800E2A24_A5DDD4(arg0) != 0) {
        D_8016814C = 1;
        func_8036D1A4(D_80118114, 1);
    } else {
        D_8016814C = 0;
        func_8036D1A4(D_80118114, 0);
    }
    func_8036A8E4(D_80118114);
    func_8036B9EC(D_80118114, 0, 0);
    func_8036CBA0(D_80118114, sp1A, 0x40);
    func_8036C898(D_80118114, arg0);
}

void func_800E2C38_A5DFE8(GObj* arg0) {
    s16 var_s0 = 10;
    s32 var_s1 = 5;

    while (TRUE) {
        var_s0 += var_s1;
        if (var_s0 > 255) {
            var_s0 = 255;
            var_s1 = -5;
        } else if (var_s0 < 120) {
            var_s0 = 120;
            var_s1 = 5;
        }
        if (var_s0 > 160) {
            func_8036D248(D_80118114, 255, 255, 255, 255);
        } else {
            func_8036D248(D_80118114, 255, 255, 255, 0);
        }
        func_8000BCA8(1);
    }
}

void func_800E2D18_A5E0C8(GObjCommon* arg0) {
    s16 var_s0 = 10;
    s16 var_s2 = 5;
    SObj* unk_48 = arg0->children;

    while (TRUE) {
        if (D_8016814C == 0) {
            var_s0 = 10;
            var_s2 = 5;
            unk_48->sp.attr |= 4;
        } else {
            unk_48->sp.attr &= ~4;
        }

        var_s0 += var_s2;
        if (var_s0 > 255) {
            var_s0 = 255;
            var_s2 = -5;
        } else if (var_s0 < 120) {
            var_s0 = 120;
            var_s2 = 5;
        }
        unk_48->sp.alpha = var_s0;
        func_8000BCA8(1);
    }
}

void func_800E2E00_A5E1B0(void) {
    SObj* unk_48;

    om_create_process(om_add_gobj(14, NULL, 0, 0x80000000), func_800E2C38_A5DFE8, 0, 1);
    D_8016814C = 0;
    D_80168148 = create_sprite(14, func_8000BC84, 0, 0x80000000, draw_sprite, 1, 0x80000000, -1, &sprite_menu_new_game_cursor, 0, func_800E2D18_A5E0C8, 1);
    unk_48 = D_80168148->children;
    unk_48->sp.attr = 0x201;
    unk_48->sp.x = 203; unk_48->sp.y = 64;
}

void func_800E2ED0_A5E280(void) {

    func_800E23B0_A5D760();
    func_800E2E00_A5E1B0();
    func_800E2A84_A5DE34(D_80168140);
    while (TRUE) {
        s32* temp_v0 = func_800AA38C(0);
        s32 temp_v1 = temp_v0[6];
        if (temp_v1 & 0xF0000) {
            func_800E2590_A5D940(temp_v0);
            func_800E2A84_A5DE34(D_80168140);
        } else if (temp_v1 & 0x4000) {
            func_800E2800_A5DBB0();
            func_800E2A84_A5DE34(D_80168140);
        } else if (temp_v1 & 0x8000) {
            func_800E270C_A5DABC();
            if (D_80168144 != 2) {
                func_800E2A84_A5DE34(D_80168140);
            }
        } else if (temp_v1 & 0x1000) {
            func_800E2314_A5D6C4();
            func_800E2A84_A5DE34(D_80168140);
        }
        if (D_80168144 == 2) {
            break;
        }
        func_8000BCA8(1);
    }
}
