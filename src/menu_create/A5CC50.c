#include "common.h"
#include "unknown_structs.h"

// TODO: use header
extern u8 D_80369F80;
extern u8 D_8016A010;

// TODO split bin at 0xA5F070
extern s32 D_80109880;
extern s32 D_80117F98;

char* D_800E2FF0[] = {
    "あ", "い", "う", "え", "お", "か", "き", "く",
    "け", "こ", "さ", "し", "す", "せ", "そ", "た",
    "ち", "つ", "て", "と", "な", "に", "ぬ", "ね",
    "の", "は", "ひ", "ふ", "へ", "ほ", "ま", "み",
    "む", "め", "も", "や", "ゆ", "よ", "　", "　",
    "ら", "り", "る", "れ", "ろ", "わ", "を", "ん",
    "　", "　", "が", "ぎ", "ぐ", "げ", "ご", "ざ",
    "じ", "ず", "ぜ", "ぞ", "だ", "ぢ", "づ", "で",
    "ど", "ば", "び", "ぶ", "べ", "ぼ", "ぱ", "ぴ",
    "ぷ", "ぺ", "ぽ", "ぁ", "ぃ", "ぅ", "ぇ", "ぉ",
    "っ", "ゃ", "ゅ", "ょ", "\xA1\xDD", "ア", "イ", "ウ",
    "エ", "オ", "カ", "キ", "ク", "ケ", "コ", "サ",
    "シ", "ス", "セ", "ソ", "タ", "チ", "ツ", "テ",
    "ト", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ", "ヒ",
    "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ",
    "ヤ", "ユ", "ヨ", "　", "　", "ラ", "リ", "ル",
    "レ", "ロ", "ワ", "ヲ", "ン", "　", "　", "ガ",
    "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ", "ゼ",
    "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "バ", "ビ",
    "ブ", "ベ", "ボ", "パ", "ピ", "プ", "ペ", "ポ",
    "ァ", "ィ", "ゥ", "ェ", "ォ", "ッ", "ャ", "ュ",
    "ョ", "\xA1\xDD", "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ",
    "Ｇ", "Ｈ", "Ｉ", "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ",
    "Ｏ", "Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ", "Ｕ", "Ｖ",
    "Ｗ", "Ｘ", "Ｙ", "Ｚ", "　", "　", "　", "　",
    "ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ", "ｈ",
    "ｉ", "ｊ", "ｋ", "ｌ", "ｍ", "ｎ", "ｏ", "ｐ",
    "ｑ", "ｒ", "ｓ", "ｔ", "ｕ", "ｖ", "ｗ", "ｘ",
    "ｙ", "ｚ", "　", "　", "　", "　", "０", "１",
    "２", "３", "４", "５", "６", "７", "８", "９",
    "！", "？", "、", "。", "・", "\xA1\xDD", "\xA1\xC1", "　",
    "　", "　", "　", "　", "　", "　", "　", "Ａ",
    "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ",
    "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "Ｐ", "Ｑ",
    "Ｒ", "Ｓ", "Ｔ", "Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ",
    "Ｚ", "ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ",
    "ｈ", "ｉ", "ｊ", "ｋ", "ｌ", "ｍ", "ｎ", "ｏ",
    "ｐ", "ｑ", "ｒ", "ｓ", "ｔ", "ｕ", "ｖ", "ｗ",
    "ｘ", "ｙ", "ｚ", "０", "１", "２", "３", "４",
    "５", "６", "７", "８", "９", "†", "‡", "¶",
    "\xA1\xC1", "！", "？", "＠", "＃", "％", "＆", "＊",
    "（", "）", "＿", "＋", "＝", "ー", "’", "”",
    "；", "：", "／", "＄", "．", "，", "＜", "＞",
    "♂", "♀", "［", "］", "ε", "　", "\0",
};

char* D_800E356C[] = { "←", "→", "　", "End" };

// TODO: STRUCT
u32 D_800E357C_A5E92C[] = {
    0x803B5000,
    0x803DA800,
    0x00000000,
    0x00000000,
    0x00000140,
    0x000000F0,
    0x00016A99,
};

Lights1 D_800E3598 = gdSPDefLights1(100, 100, 100, 180, 180, 180, 30, 30, 30);

Gfx D_800E35B0[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPFogFactor(256, -512),
    gsDPSetFogColor(0, 0, 0, 255),
    gsSPSetGeometryMode(G_FOG),
    gsSPSetGeometryMode(G_LIGHTING),
    gsSPSetLights1(D_800E3598),
    gsSPEndDisplayList(),
};

// BSS
extern Struct_8000C37C* D_801180B0;
extern Struct_8000C37C* D_801180B4;
extern Struct_8000C37C* D_801180B8;
extern s32 D_801180BC; // padding?
extern s32 D_801180C0[20];
extern s32 D_80118110;
extern s32 D_80118114;
extern s32 D_80118118;

void func_800E2ED0_A5E280(void);

void func_800E18A0_A5CC50(Struct_8000C37C_Sub24* arg0, u8 arg1) {
    arg0->unk_28 = arg1;
    arg0->unk_29 = arg1;
    arg0->unk_2A = arg1;
}

u8** func_800E18B4_A5CC64(s32 arg0) {
    return &D_800E2FF0[255];
}

void func_800E18C4_A5CC74(void) {

}

void func_800E18CC_A5CC7C(void) {
    Struct_8000C37C_Sub24* unk_48;

    D_801180B4 = func_8000C37C(14, func_8000BC84, 0, 0x80000000, func_80017768, 1, 0x80000000, -1, &D_80109880, 0, 0, 1);
    unk_48 = D_801180B4->unk_48;
    unk_48->unk_24 = 0x201;
}

void func_800E1950_A5CD00(void) {
    Struct_8000C37C_Sub24* unk_48;

    D_801180B8 = func_8000C37C(14, func_8000BC84, 0, 0x80000000, func_80017768, 1, 0x80000000, -1, &D_80117F98, 0, 0, 1);
    unk_48 = D_801180B8->unk_48; 
    unk_48->unk_10 = 96; unk_48->unk_12 = 21;
    unk_48->unk_24 = 0x201;
}

void func_800E19E4_A5CD94(void) {
    func_800E18CC_A5CC7C();
    func_800E1950_A5CD00();
}

void func_800E1A0C_A5CDBC(void) {
    s32 id;
    s32 id2;
    s32 unused_var; // required to match
    s8 i;
    ColorRGB sp28;

    id2 = func_8036F78C();
    func_8036F684(id2, 1);
    func_8036F738(id2, &sp28);
    func_8036F1F4(id2, 184, 320);
    func_8036F0DC(id2, 1);
    func_8036F0A0(id2, 0);
    func_8036FE54(id2, 1);

    // description
    id = func_8036AC6C(104, 173, 192, 47, 0);
    func_8036B870(id, 1, 255, 255, 255, 255);
    func_8036B870(id, 0, sp28.r, sp28.g, sp28.b, 255);
    func_8036B734(id);
    func_8036B9EC(id, 0, 0);
    func_8036D448(1);
    func_8036D3E8(-1, 3);
    func_8036CB58(id, 12);
    func_8036C898(id, "Enter your name on the card.\n"
                      "Choose with the Control Stick\n"
                      "and select with the \\a Button!");

    // character grid
    for (i = 0; i < 20; i++) {
        func_8036D4A0(0);
        func_8036D448(1);
        func_8036D3E8(-1, 3);
        id = func_8036AC6C(23, i * 10 + 20, 60, 8, 0);
        func_8036CB58(id, 8);
        func_8036B870(id, 0, 0, 0, 0, 0);
        func_8036B734(id);
        D_801180C0[i] = id;
    }

    func_8036D4A0(0);
    func_8036D448(0);
    func_8036D3E8(0, 4);

    // name input
    id = func_8036AC6C(203, 64, 62, 12, 0x400);
    func_8036CB58(id, 12);
    func_8036B870(id, 0, 0, 0, 0, 0);
    func_8036B870(id, 1, 0, 0, 0, 255);
    func_8036B734(id);
    D_80118110 = id;

    func_8036D4A0(0);
    func_8036D448(0);
    func_8036D3E8(0, 4);

    id = func_8036AC6C(206, 64, 12, 12, 0x400);
    func_8036CB58(id, 12);
    func_8036B870(id, 0, 0, 0, 0, 0);
    func_8036B870(id, 1, 255, 0, 0, 255);
    func_8036B734(id);
    D_80118114 = id;
}

void func_800E1CF8_A5D0A8(s8 arg0) {
    s16 i;
    s16 j;
    s32 temp_s2;
    s16 temp_s1;
    u8** temp_s4 = func_800E18B4_A5CC64(arg0);

    for (i = 0; i < 20; i++) {
        func_8036A8E4(D_801180C0[i]);
    }

    temp_s1 = 0;
    func_8036D4B4(1, 0);   

    for (i = 0; i < 19; i++) {
        temp_s2 = D_801180C0[i];
        for (j = 0; j < 5; j++) {
            func_8036B9EC(temp_s2, j * 13, 0);
            func_8036C898(temp_s2, temp_s4[temp_s1++]);
        }
    }

    func_8036D4B4(1, 1);
    temp_s1 = 0;
    temp_s2 = D_801180C0[i];
    for (j = 0; j < 4; j++) {
        func_8036B9EC(temp_s2, j * 13, 0);
        func_8036C898(temp_s2, D_800E356C[temp_s1++]);
    }
}

void func_800E1E94_A5D244(void) {
    func_80370428();
    func_803700A4(0);
    func_80370038(4, 5);
    func_8036FFE0(25, 22);
    func_80370134();
    func_803700A4(1);
}

void func_800E1EE4_A5D294(void) {
    u8 var_a1;

    var_a1 = 255; do {
        func_800E18A0_A5CC50(D_801180B4->unk_48, var_a1);
        func_8000BCA8(1);
        var_a1 -= 5;
    } while (var_a1 > 128);
    func_800E18A0_A5CC50(D_801180B4->unk_48, 128);
    func_8000BCA8(1);
}

void func_800E1F58_A5D308(GObj* arg0) {
    s32 sp24;
    s32 sp20;    
    u8 i;

    func_800E19E4_A5CD94();
    func_800E1A0C_A5CDBC();
    func_800E1CF8_A5D0A8(0);
    func_800E1E94_A5D244();
    func_800E1EE4_A5D294();
    func_80022454(0, 0x7F00);
    func_80022374(0, 12);
    func_800E2ED0_A5E280();
    func_800224DC(0, 0, 60);
    func_800A7470(0, 0, 0);
    func_800A7860(0, 1.0f);
    for (i = 0; i < 60; i++) {
        func_8000BCA8(1);
    }
    func_800BFC18_5CAB8(sp24, sp20);
    func_80007A34(sp24, sp24, sp20, sp20);
    func_800067DC();
    while (1) {
        func_8000BCA8(1);
    }
}

void func_800E2054_A5D404(Gfx** gfxPtr) {
    gSPDisplayList((*gfxPtr)++, D_800E35B0);
}

void func_800E2078_A5D428(void) {
    func_8036A3F8(&D_80118118, 0x50000);
    func_800AAE28();
    func_800AA85C(24, 6);
    func_800AA870(0xF0000);
    func_8036EB98();
    func_8000C4B0(0, 0x80000000, 0x64, 6, 0);
    D_801180B0 = func_8000C3FC(3, func_8000BC84, 0, 0x80000000, func_8001977C, 3, 2, -1, 1, 1, 0, 1, 1);
    D_801180B0->unk_48->unk_80 = 8;
    func_800A7F68(0, 0x101);
    runGObjProcess(func_8000A410(0xE, 0, 0, 0x80000000), func_800E1F58_A5D308, 0, 1);
}

// TODO: STRUCT
u32 D_800E3608[] = {
    0x00000000,
    func_8000AFFC,
    func_8000ADA0,
    &D_8016A010,
    0x00000000,
    0x00000002,
    0x00000001,
    0x00005000,
    0x00002000,
    0x00000000,
    0x00000000,
    0x0000C800,
    0x00000000,
    0x00002000,
    func_800A1A50,
    func_80004860,
    0x00000020,
    0x00000400,
    0x00000080,
    0x00000000,
    0x00000040,
    0x00000040,
    0x0000005C,
    0x00000400,
    0x00000000,
    0x00000000,
    0x00000010,
    0x00000008,
    0x00000010,
    0x00000088,
    0x00000080,
    0x00000058,
    0x00000008,
    0x00000090,
    func_800E2078_A5D428,
    0x00000000,
    0x00000000,
    0x00000000,
};

s32 func_800E218C_A5D53C(s32 arg0) {
    func_80007BC4(D_800E357C_A5E92C);
    D_800E3608[4] = &D_80369F80 - &D_8016A010;
    func_80005448(1);
    func_800073AC(D_800E3608);
    func_800BFEBC_5CD5C(0x10, 1);
    return 11;
}