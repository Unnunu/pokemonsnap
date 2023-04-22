#include "PR/os.h"
#include "PR/controller.h"
#include "PR/os_gbpak.h"
#include "PR/os_motor.h"
#include "macros.h"
#include "sys/main.h"
#include "sys/sched.h"
#include "types.h"

enum ControllerEventTypes {
    CONT_EVENT_READ_AND_UPDATE_GLOBALS = 1,
    CONT_EVENT_UPDATE_GLOBALS = 2,
    CONT_EVENT_HOLD_PARAMS = 3,
    CONT_EVENT_UPDATE_EVERY_TICK = 4,
    CONT_EVENT_RUMBLE_PAK = 5,
    CONT_EVENT_DETECT_INTERVAL = 6,
    CONT_EVENT_GB_PAK = 7,
    CONT_EVENT_UNK_PAK = 8
};

enum ControllerDeviceTypes {
    CONT_DEV_TYPE_NONE = 0,
    CONT_DEV_TYPE_CONTROLLER_PAK = 1,
    CONT_DEV_TYPE_RUMBLE_PAK = 3,
    CONT_DEV_TYPE_GAME_BOY_PAK = 4,
    CONT_DEV_TYPE_UNKNOWN_PAK = 5
};

enum RumbleCommands {
    RUMBLE_CMD_INIT = 0,
    RUMBLE_CMD_START = 1,
    RUMBLE_CMD_STOP = 2
};

typedef struct {
    /* 0x00 */ u16 buttons;
    /* 0x02 */ u16 pressedButtons;
    /* 0x04 */ u16 pressedButtonsAccum;
    /* 0x06 */ u16 heldButtons;
    /* 0x08 */ u16 heldButtonsAccum;
    /* 0x0A */ u16 releasedButtons;
    /* 0x0C */ u16 releasedButtonsAccum;
    /* 0x0E */ s8 stick_x;
    /* 0x0F */ s8 stick_y;
    /* 0x10 */ s32 holdDelay;
    /* 0x14 */ u32 holdInterval;
    /* 0x18 */ s32 counter;
    /* 0x1C */ u8 errno;
    /* 0x1D */ u8 status;
} ControllerInfo; // size = 0x20

typedef struct {
    /* 0x00 */ u16 buttons;
    /* 0x02 */ u16 pressedButtons;
    /* 0x04 */ u16 heldButtons;
    /* 0x06 */ u16 releasedButtons;
    /* 0x08 */ s8 stickX;
    /* 0x09 */ s8 stickY;
} ControllerInput; // size = 0x0A

typedef struct {
    /* 0x00 */ u8 status;
    /* 0x02 */ OSGbpakId id;
} GbpakInfo; // size = 0x52;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04; // event?
    /* 0x08 */ s32 unk08; // controller number
    /* 0x0C */ OSMesgQueue *unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} Unk80045268; // size = 0x18

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04; // event?
    /* 0x08 */ s32 unk08; // controller number
    /* 0x0C */ OSMesgQueue *unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
} UnkCont24; // size = 0x24

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ OSMesgQueue *unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} UnkCont20; // size = 0x20

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ OSMesg mesg;
    /* 0x08 */ OSMesgQueue *cbQueue;
} ControllerEvent; // size = 0x0C

typedef struct {
    /* 0x00 */ ControllerEvent base;
    /* 0x0C */ s32 holdDelay;
    /* 0x10 */ s32 holdInterval;
} ControllerEventTypeHoldParams; // size = 0x14

typedef struct {
    /* 0x00 */ ControllerEvent base;
    /* 0x0C */ s32 enabled;
} ControllerEventUpdateEveryTick; // size = 0x10

typedef struct {
    /* 0x00 */ ControllerEvent base;
    /* 0x0C */ s32 contNo;
    /* 0x10 */ s32 cmd;
} ControllerEventRumble; // size = 0x14

typedef struct {
    /* 0x00 */ ControllerEvent base;
    /* 0x0C */ s32 interval;
} ControllerEventDetectParams; // size = 0x10

typedef struct {
    /* 0x00 */ ControllerEvent base;
    /* 0x0C */ s32 contNo;
    /* 0x10 */ u16 direction;
    /* 0x14 */ u32 addr;
    /* 0x18 */ u8* data;
    /* 0x1C */ u16 size;
} ControllerEventGbpak; // size >= 0x20

typedef struct {
    /* 0x00 */ ControllerEvent base;
    /* 0x0C */ s32 contNo;
    /* 0x10 */ s32 direction;
    /* 0x14 */ s32 value;
} ControllerEventUnknownDevice; // size >= 0x14

typedef struct {
    /* 0x00 */ s32 busy;
    /* 0x04 */ ControllerEventRumble event;
} ControllerEventRumbleWrapper;

typedef struct {
    /* 0x00 */ s32 busy;
    /* 0x04 */ ControllerEventGbpak event;
} ControllerEventGbpakWrapper;

typedef struct {
    /* 0x00 */ s32 busy;
    /* 0x04 */ ControllerEventUnknownDevice event;
} ControllerEventUnknownDeviceWrapper;

OSMesgQueue contSIEvtQueue;
OSMesg contSIEvtQueueMessages[1];
SCClient contClient;
OSMesg contEventQueueMessages[7];
OSMesgQueue contEventQueue;
OSMesg contGameBoyPakMessages[MAXCONTROLLERS];
OSMesgQueue contGameBoyPakQueue;
OSMesg contRumblePakMessages[MAXCONTROLLERS];
OSMesgQueue contRumblePakQueue;
OSMesg contControllerPakMessages[MAXCONTROLLERS];
OSMesgQueue contControllerPakQueue;
OSMesg contUnknownPakMessages[MAXCONTROLLERS];
OSMesgQueue contUnknownPakQueue;
OSContStatus sContStatus[MAXCONTROLLERS];
OSContPad contPadData[MAXCONTROLLERS];
u32 sNumControllers;
s8 sControllerIndices[MAXCONTROLLERS];
s8 sContDeviceTypes[MAXCONTROLLERS];
ControllerInfo sContInfo[MAXCONTROLLERS];
ControllerInput gContInput[MAXCONTROLLERS];
u32 sNeedUpdateGlobals;
ControllerEvent* sEventUpdateGlobals;
s32 sUpdateEveryTickEnabled;
s32 contDetectInterval;
s32 contDetectCounter;
ControllerEventRumbleWrapper sContEventRumble[MAXCONTROLLERS];
u8 padding4[0x60];
ControllerEventGbpakWrapper sContEventGameBoy[MAXCONTROLLERS];
ControllerEventUnknownDeviceWrapper sContEventUnknownPak;
OSPfs contPfs[MAXCONTROLLERS];
OSPfsState contPfsGameNotes[MAXCONTROLLERS][16];
GbpakInfo contGameBoyPakInfo[MAXCONTROLLERS];
u8 padding5[0x10];
u8 contRamDataBlock[BLOCKSIZE];
u16 gContInputCurrentButtons;
u16 gContInputPressedButtons;
u16 gContInputHeldButtons;
u16 gContInputReleasedButtons;
s8 gContInputStickX;
s8 gContInputStickY;

void contUpdateControllerIndices(void) {
    s32 v0 = 0;
    s32 i;
    
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (sContInfo[i].errno == 0) {
            sControllerIndices[v0++] = i;
        }
    }
    sNumControllers = v0;

    for (i = v0; i < MAXCONTROLLERS; i++) {
        sControllerIndices[i] = -1;
    }
}

void contDetectDevices(void) {
    s32 i, j, ret;    

    osContStartQuery(&contSIEvtQueue);
    osRecvMesg(&contSIEvtQueue, NULL, OS_MESG_BLOCK);
    osContGetQuery(sContStatus);
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (sContStatus[i].errno != 0 || sContStatus[i].status & CONT_CARD_PULL) {
            if (sContInfo[i].errno == 0 && (sContInfo[i].status & CONT_CARD_ON)) {
                sContStatus[i].status = CONT_CARD_PULL;
            } else {
                sContStatus[i].status = 0;
            }
            sContDeviceTypes[i] = CONT_DEV_TYPE_NONE;
        }

        if (sContStatus[i].errno == 0 &&
            (sContStatus[i].status & CONT_CARD_ON) &&
            (!(sContInfo[i].status & CONT_CARD_ON) || sContInfo[i].errno != 0))
        {
            // new device inserted
            for (j = 0; j < BLOCKSIZE; j++) {
                contRamDataBlock[j] = OS_PFS_CHECK_ID;
            }
            ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
            if (ret == PFS_ERR_NEW_PACK) {
                ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
            }
            if (ret != 0) {
                continue;
            }

            ret =  __osContRamRead(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock);
            if (ret == 0 && contRamDataBlock[0x1F] == OS_PFS_CHECK_ID) {
                sContDeviceTypes[i] = CONT_DEV_TYPE_NONE;
            } else {
                for (j = 0; j < BLOCKSIZE; j++) {
                    contRamDataBlock[j] = 0x85; // what does 0x85 mean?
                }
                ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
                if (ret == PFS_ERR_NEW_PACK) {
                    ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
                }
                if (ret != 0) {
                    continue;
                }

                ret =  __osContRamRead(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock);
                if (ret == 0 && contRamDataBlock[0x1F] == 0x85) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_UNKNOWN_PAK;
                    if (contUnknownPakQueue.msg == NULL) {
                        osCreateMesgQueue(&contUnknownPakQueue, contUnknownPakMessages, ARRAY_COUNT(contUnknownPakMessages));
                    }
                } else if (osGbpakInit(&contSIEvtQueue, &contPfs[i], i) == 0) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_GAME_BOY_PAK;
                    if (contGameBoyPakQueue.msg == NULL) {
                        osCreateMesgQueue(&contGameBoyPakQueue, contGameBoyPakMessages, ARRAY_COUNT(contGameBoyPakMessages));
                    }
                    osGbpakReadId(&contPfs[i], &contGameBoyPakInfo[i].id, &contGameBoyPakInfo[i].status);
                    osGbpakPower(&contPfs[i], OS_GBPAK_POWER_OFF);
                } else if (osMotorInit(&contSIEvtQueue, &contPfs[i], i) == 0) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_RUMBLE_PAK;
                    if (contRumblePakQueue.msg == NULL) {
                        osCreateMesgQueue(&contRumblePakQueue, contRumblePakMessages, ARRAY_COUNT(contRumblePakMessages));
                    }
                } else if (osPfsInitPak(&contSIEvtQueue, &contPfs[i], i) == 0) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_CONTROLLER_PAK;
                    if (contControllerPakQueue.msg == NULL) {
                        osCreateMesgQueue(&contControllerPakQueue, contControllerPakMessages, ARRAY_COUNT(contControllerPakMessages));
                    }
                    for (j = 0; j < 0x10; j++) {
                        osPfsFileState(&contPfs[i], j, &contPfsGameNotes[i][j]);
                    }
                }
            }
        }
        sContInfo[i].errno = sContStatus[i].errno;
        sContInfo[i].status = sContStatus[i].status;
    }
}

void contReadData(void) {
    s32 i;

    osContStartReadData(&contSIEvtQueue);
    osRecvMesg(&contSIEvtQueue, NULL, OS_MESG_BLOCK);
    osContGetReadData(contPadData);

    for (i = 0; i != MAXCONTROLLERS; i++) {
        if (!contPadData[i].errno && (sContStatus[i].status & CONT_CARD_ON) && sContInfo[i].errno) {
            osMotorInit(&contSIEvtQueue, &contPfs[i], i);
        }
        sContInfo[i].errno = contPadData[i].errno;

        if ((contPadData[i].errno & 0xFF) == 0) {
            sContInfo[i].pressedButtons = (contPadData[i].button ^ sContInfo[i].buttons) & contPadData[i].button;
            sContInfo[i].releasedButtons = (contPadData[i].button ^ sContInfo[i].buttons) & sContInfo[i].buttons;

            if (contPadData[i].button ^ sContInfo[i].buttons) {
                // new buttons pressed or released
                sContInfo[i].heldButtons = sContInfo[i].pressedButtons;
                sContInfo[i].counter = sContInfo[i].holdDelay;
            } else {
                sContInfo[i].counter--;
                if (sContInfo[i].counter > 0) {
                    sContInfo[i].heldButtons = 0;
                } else {
                    sContInfo[i].heldButtons = contPadData[i].button;
                    sContInfo[i].counter = sContInfo[i].holdInterval;
                }
            }

            sContInfo[i].buttons = contPadData[i].button;
            sContInfo[i].stick_x = contPadData[i].stick_x;
            sContInfo[i].stick_y = contPadData[i].stick_y;

            sContInfo[i].pressedButtonsAccum |= sContInfo[i].pressedButtons;
            sContInfo[i].releasedButtonsAccum |= sContInfo[i].releasedButtons;
            sContInfo[i].heldButtonsAccum |= sContInfo[i].heldButtons;
        }
    }
    sNeedUpdateGlobals = TRUE;
}

void contUpdateGlobals(void) {
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (!sContInfo[i].errno) {
            gContInput[i].buttons = sContInfo[i].buttons;
            gContInput[i].pressedButtons = sContInfo[i].pressedButtonsAccum;
            gContInput[i].releasedButtons = sContInfo[i].releasedButtonsAccum;
            gContInput[i].heldButtons = sContInfo[i].heldButtonsAccum;
            gContInput[i].stickX = sContInfo[i].stick_x;
            gContInput[i].stickY = sContInfo[i].stick_y;

            sContInfo[i].pressedButtonsAccum =
            sContInfo[i].releasedButtonsAccum =
            sContInfo[i].heldButtonsAccum = 0;
        }
    }
    contUpdateControllerIndices();
    gContInputPressedButtons = gContInput[0].pressedButtons;
    gContInputCurrentButtons = gContInput[0].buttons;    
    gContInputHeldButtons = gContInput[0].heldButtons;
    gContInputReleasedButtons = gContInput[0].releasedButtons;
    gContInputStickX = gContInput[0].stickX;
    gContInputStickY = gContInput[0].stickY;

    sNeedUpdateGlobals = FALSE;
}

void contInitialize(void) {
    s32 i, j;

    u8 pattern;
    s32 ret;
    s32 tmp = 5; // required to match

    osCreateMesgQueue(&contSIEvtQueue, contSIEvtQueueMessages, ARRAY_COUNT(contSIEvtQueueMessages));
    osSetEventMesg(OS_EVENT_SI, &contSIEvtQueue, (OSMesg)1);
    contGameBoyPakQueue.msg = contUnknownPakQueue.msg = contControllerPakQueue.msg = contRumblePakQueue.msg = NULL;
    osContInit(&contSIEvtQueue, &pattern, sContStatus);

    for (i = 0; i < MAXCONTROLLERS; i++) {
        sContDeviceTypes[i] = CONT_DEV_TYPE_NONE;
        if (((pattern >> i) & 1) && (sContStatus[i].status & CONT_CARD_ON)) {
            for (j = 0; j < BLOCKSIZE; j++) {
                contRamDataBlock[j] = OS_PFS_CHECK_ID;
            }
            ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
            if (ret == PFS_ERR_NEW_PACK) {
                ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
            }
            if (ret != 0) {
                continue;
            }

            ret =  __osContRamRead(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock);
            if (ret == 0 && contRamDataBlock[0x1F] == OS_PFS_CHECK_ID) {
                sContDeviceTypes[i] = CONT_DEV_TYPE_NONE;
            } else {
                for (j = 0; j < BLOCKSIZE; j++) {
                    contRamDataBlock[j] = 0x85; // what does 0x85 mean?
                }
                ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
                if (ret == PFS_ERR_NEW_PACK) {
                    ret =  __osContRamWrite(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock, 0);
                }
                if (ret != 0) {
                    continue;
                }

                ret =  __osContRamRead(&contSIEvtQueue, i, PFS_BANK_ADDR, contRamDataBlock);
                if (ret == 0 && contRamDataBlock[0x1F] == 0x85) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_UNKNOWN_PAK;
                    osCreateMesgQueue(&contUnknownPakQueue, contUnknownPakMessages, ARRAY_COUNT(contUnknownPakMessages));
                } else if (osGbpakInit(&contSIEvtQueue, &contPfs[i], i) == 0) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_GAME_BOY_PAK;
                    osCreateMesgQueue(&contGameBoyPakQueue, contGameBoyPakMessages, ARRAY_COUNT(contGameBoyPakMessages));
                    osGbpakReadId(&contPfs[i], &contGameBoyPakInfo[i].id, &contGameBoyPakInfo[i].status);
                    osGbpakPower(&contPfs[i], OS_GBPAK_POWER_OFF);
                } else if (osMotorInit(&contSIEvtQueue, &contPfs[i], i) == 0) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_RUMBLE_PAK;
                    osCreateMesgQueue(&contRumblePakQueue, contRumblePakMessages, ARRAY_COUNT(contRumblePakMessages));
                } else if (osPfsInitPak(&contSIEvtQueue, &contPfs[i], i) == 0) {
                    sContDeviceTypes[i] = CONT_DEV_TYPE_CONTROLLER_PAK;
                    osCreateMesgQueue(&contControllerPakQueue, contControllerPakMessages, ARRAY_COUNT(contControllerPakMessages));
                    for (j = 0; j < 0x10; j++) {
                        osPfsFileState(&contPfs[i], j, &contPfsGameNotes[i][j]);
                    }
                }
            }
        }
    }
    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (sContDeviceTypes[i] == CONT_DEV_TYPE_RUMBLE_PAK) {
            sContEventRumble[i].busy = FALSE;
            sContEventRumble[i].event.base.type = tmp; // CONT_EVENT_RUMBLE_PAK
            sContEventRumble[i].event.base.mesg = (OSMesg)i;
            sContEventRumble[i].event.base.cbQueue = &contRumblePakQueue;
        }
    }

    for (i = 0; i < MAXCONTROLLERS; i++) {
        sContEventGameBoy[i].busy = FALSE;
        sContEventGameBoy[i].event.base.type = CONT_EVENT_GB_PAK;
        sContEventGameBoy[i].event.base.mesg = (OSMesg)i;
        sContEventGameBoy[i].event.base.cbQueue = &contGameBoyPakQueue;
    }

    sContEventUnknownPak.busy = FALSE;
    sContEventUnknownPak.event.base.type = CONT_EVENT_UNK_PAK;
    sContEventUnknownPak.event.base.mesg = (OSMesg)0;
    sContEventUnknownPak.event.base.cbQueue = &contUnknownPakQueue;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        sContInfo[i].buttons = sContInfo[i].pressedButtons = sContInfo[i].heldButtons = contPadData[i].button = 0;
        sContInfo[i].stick_y = 0;
        sContInfo[i].holdDelay = sContInfo[i].counter = 30;
        sContInfo[i].holdInterval = tmp; // 5

        sContInfo[i].stick_x = sContInfo[i].stick_y;
        sContInfo[i].errno = sContStatus[i].errno;
        sContInfo[i].status = sContStatus[i].status;

        gContInput[i].heldButtons = 0;
        gContInput[i].pressedButtons = 0;
        gContInput[i].buttons = 0;

        gContInput[i].stickX = gContInput[i].stickY = 0;
    }

    contUpdateControllerIndices();
    sNeedUpdateGlobals = FALSE;
    sEventUpdateGlobals = NULL;
    sUpdateEveryTickEnabled = TRUE;
    contDetectCounter = contDetectInterval = 1;
}

void contSendEvent(ControllerEvent *evt) {
    OSMesg mesg[1];
    OSMesgQueue queue;

    osCreateMesgQueue(&queue, mesg, ARRAY_COUNT(mesg));
    evt->mesg    = (OSMesg)1;
    evt->cbQueue = &queue;

    osSendMesg(&contEventQueue, (OSMesg)evt, OS_MESG_NOBLOCK);
    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
}

void contReadAndUpdate(void) {
    ControllerEvent evt;
    evt.type = CONT_EVENT_READ_AND_UPDATE_GLOBALS;
    contSendEvent(&evt);
}

void contUpdate(void) {
    ControllerEvent evt;
    evt.type = CONT_EVENT_UPDATE_GLOBALS;
    contSendEvent(&evt);
}

void contSetHoldParams(s32 delay, s32 interval) {
    ControllerEventTypeHoldParams evt;
    evt.base.type = CONT_EVENT_HOLD_PARAMS;
    evt.holdDelay = delay;
    evt.holdInterval = interval;
    contSendEvent(&evt.base);
}

void contSetUpdateEveryTick(s32 enabled) {
    ControllerEventUpdateEveryTick evt;
    evt.base.type = CONT_EVENT_UPDATE_EVERY_TICK;
    evt.enabled = enabled;
    contSendEvent(&evt.base);
}

void contSetDetectInterval(s32 interval) {
    ControllerEventDetectParams evt;
    evt.base.type = CONT_EVENT_DETECT_INTERVAL;
    evt.interval = interval;
    contSendEvent(&evt.base);
}

void contExecuteRumbleCommand(s32 contNo, s32 cmd) {
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (!sContEventRumble[i].busy) {
            break;
        }
    }

    if (i == MAXCONTROLLERS) {
        osRecvMesg(&contRumblePakQueue, (OSMesg *)&i, OS_MESG_BLOCK);
    } else {
        sContEventRumble[i].busy = TRUE;
    }
    sContEventRumble[i].event.contNo = contNo;
    sContEventRumble[i].event.cmd = cmd;
    osSendMesg(&contEventQueue, (OSMesg)&sContEventRumble[i].event, OS_MESG_NOBLOCK);
}

void contRumbleStart(s32 contNo) {
    contExecuteRumbleCommand(contNo, RUMBLE_CMD_START);
}

void contRumbleStop(s32 contNo) {
    contExecuteRumbleCommand(contNo, RUMBLE_CMD_STOP);
}

void contRumbleInit(s32 contNo) {
    contExecuteRumbleCommand(contNo, RUMBLE_CMD_INIT);
}

void contExecuteGbpakCommand(s32 contNo, u16 direction, u16 addr, u8* buffer, u16 size) {
    s32 i;

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if (!sContEventGameBoy[i].busy) {
            break;
        }
    }

    if (i == MAXCONTROLLERS) {
        osRecvMesg(&contGameBoyPakQueue, (OSMesg *)&i, OS_MESG_BLOCK);
    } else {
        sContEventGameBoy[i].busy = TRUE;
    }

    sContEventGameBoy[i].event.contNo = contNo;
    sContEventGameBoy[i].event.direction = direction;
    sContEventGameBoy[i].event.addr = addr;
    sContEventGameBoy[i].event.data = buffer;
    sContEventGameBoy[i].event.size = size;
    osSendMesg(&contEventQueue, (OSMesg)&sContEventGameBoy[i].event, OS_MESG_NOBLOCK);
}

void contGbpakRead(s32 contNo, u16 addr, u8* buffer, u16 size) {
    contExecuteGbpakCommand(contNo, OS_READ, addr, buffer, size);
}

void contGbpakWrite(s32 contNo, u16 addr, u8* buffer, u16 size) {
    contExecuteGbpakCommand(contNo, OS_WRITE, addr, buffer, size);
}

void contGbpakReadWrite(ControllerEventGbpak* arg0) {    
    OSPfs* pfs = &contPfs[arg0->contNo];
    s32 contId = arg0->contNo;
    u8* data = arg0->data;
    u16 addr;
    u16 size = arg0->size;
    
    s32 i;
    u8 sp4C[BLOCKSIZE];

    if (osGbpakCheckConnector(pfs, &contGameBoyPakInfo[contId].status) != 0) {
        return;
    }

    if (!(contGameBoyPakInfo[contId].status & (OS_GBPAK_GBCART_ON | OS_GBPAK_POWER))) {
        return;
    }

    // allow access to ram
    for (i = 0; i < BLOCKSIZE; i++) {
        sp4C[i] = 0xA;
    }
    osGbpakReadWrite(pfs, OS_WRITE, 0, sp4C, BLOCKSIZE);

    // select bank
    for (i = 0; i < BLOCKSIZE; i++) {
        sp4C[i] = arg0->addr >> 13; // bank id
    }
    osGbpakReadWrite(pfs, OS_WRITE, 0x4000, sp4C, BLOCKSIZE);

    // read/write data
    addr = (arg0->addr & 0x1FFF) | 0xA000;
    while (addr < 0xC000 && size > 0) {
        osGbpakReadWrite(pfs, arg0->direction, addr, data, MIN(size, BLOCKSIZE));
        addr += BLOCKSIZE;
        data += BLOCKSIZE;
        if (size < BLOCKSIZE) {
            size = 0;
        } else {
            size -= BLOCKSIZE;
        }
    }
    osGbpakPower(&contPfs[arg0->contNo], OS_GBPAK_POWER_OFF);
}

void contUnknownDeviceExecute(s32 contNo, u16 direction, u16 value) {
    sContEventUnknownPak.event.contNo = contNo;
    sContEventUnknownPak.event.direction = direction;
    sContEventUnknownPak.event.value = value;
    osSendMesg(&contEventQueue, &sContEventUnknownPak.event, 0);
}

void contUnknownDeviceReadWrite(ControllerEventUnknownDevice* arg0) {
    switch (arg0->direction) {
        case OS_READ:
            __osContRamRead(&contSIEvtQueue, arg0->contNo, (0xC000/BLOCKSIZE), contRamDataBlock);
            arg0->value = contRamDataBlock[0x1F];
            break;
        case OS_WRITE:
            contRamDataBlock[0x1F] = arg0->value;
            __osContRamWrite(&contSIEvtQueue, arg0->contNo, (0xC000/BLOCKSIZE), contRamDataBlock, PFS_FORCE);
            break;
    }
}

s32 contIsUnknownDeviceInserted(void) {
    // check if it's inserted into the last slot
    if (sContDeviceTypes[3] == CONT_DEV_TYPE_UNKNOWN_PAK) {
        return TRUE;
    } else {
        return FALSE;
    }
}

u8 contUnknownDeviceRead(void) {
    OSMesg mesg;

    contUnknownDeviceExecute(3, OS_READ, 0);
    osRecvMesg(&contUnknownPakQueue, &mesg, OS_MESG_BLOCK);
    return sContEventUnknownPak.event.value;
}

void contUnknownDeviceWrite(u8 value) {
    OSMesg mesg;

    contUnknownDeviceExecute(3, OS_WRITE, value);
    osRecvMesg(&contUnknownPakQueue, &mesg, OS_MESG_BLOCK);
}

void contHandleEvent(ControllerEvent *evt) {
    switch (evt->type) {
        case CONT_EVENT_READ_AND_UPDATE_GLOBALS:
            {
                contReadData();
                contUpdateGlobals();
                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
        case CONT_EVENT_UPDATE_GLOBALS:
            {
                if (sNeedUpdateGlobals) {
                    contUpdateGlobals();
                    if (evt->cbQueue != NULL) {
                        osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                    }
                } else {
                    // call contUpdateGlobals() on next game tick
                    sEventUpdateGlobals = evt;
                }
                break;
            }
        case CONT_EVENT_HOLD_PARAMS:
            {
                ControllerEventTypeHoldParams* evtHoldParams = (ControllerEventTypeHoldParams*)evt;
                s32 i;
                for (i = 0; i < MAXCONTROLLERS; i++) {
                    sContInfo[i].holdDelay = evtHoldParams->holdDelay;
                    sContInfo[i].holdInterval = evtHoldParams->holdInterval;
                }

                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
        case CONT_EVENT_UPDATE_EVERY_TICK:
            {
                ControllerEventUpdateEveryTick* evtUpdateEveryTick = (ControllerEventUpdateEveryTick*)evt;
                sUpdateEveryTickEnabled = evtUpdateEveryTick->enabled;
                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
        case CONT_EVENT_DETECT_INTERVAL:
            {
                ControllerEventDetectParams* evtDetectParams = (ControllerEventDetectParams*)evt;
                contDetectInterval = evtDetectParams->interval;
                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
        case CONT_EVENT_RUMBLE_PAK:
            {
                ControllerEventRumble* evtRumble = (ControllerEventRumble*)evt;

                if (!sContInfo[evtRumble->contNo].errno && (sContInfo[evtRumble->contNo].status & CONT_CARD_ON)) {
                    switch (evtRumble->cmd) {
                        case RUMBLE_CMD_INIT:
                            osMotorInit(&contSIEvtQueue, &contPfs[evtRumble->contNo], evtRumble->contNo);
                            break;
                        case RUMBLE_CMD_START:
                            if (!scBeforeReset) {
                                osMotorStart(&contPfs[evtRumble->contNo]);
                            }
                            break;
                        case RUMBLE_CMD_STOP:
                            osMotorStop(&contPfs[evtRumble->contNo]);
                            break;
                    }
                }

                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
        case CONT_EVENT_GB_PAK:
            {
                ControllerEventGbpak* evtGbpak = (ControllerEventGbpak*)evt;
                if (!sContInfo[evtGbpak->contNo].errno && (sContInfo[evtGbpak->contNo].status & CONT_CARD_ON)) {
                    contGbpakReadWrite(evtGbpak);
                }
                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
        case CONT_EVENT_UNK_PAK:
            {
                ControllerEventUnknownDevice* evtUnkDev = (ControllerEventUnknownDevice*)evt;
                if (!sContInfo[evtUnkDev->contNo].errno && (sContInfo[evtUnkDev->contNo].status & CONT_CARD_ON)) {
                    contUnknownDeviceReadWrite(evtUnkDev);
                }
                if (evt->cbQueue != NULL) {
                    osSendMesg(evt->cbQueue, evt->mesg, OS_MESG_NOBLOCK);
                }
                break;
            }
    }
}

void contMain(UNUSED void *arg) {
    OSMesg mesg;

    contInitialize();
    scAddClient(&contClient, &contEventQueue, contEventQueueMessages, ARRAY_COUNT(contEventQueueMessages));
    osSendMesg(&gThreadingQueue, (OSMesg)1, OS_MESG_NOBLOCK);

    while (TRUE) {
        osRecvMesg(&contEventQueue, &mesg, OS_MESG_BLOCK);
        if ((intptr_t)mesg == 1) {
            // game tick
            if (contDetectCounter != 0) {
                contDetectCounter--;
            }
            if (contDetectCounter == 0) {
                contDetectDevices();
                contDetectCounter = contDetectInterval;
            }

            if (!sUpdateEveryTickEnabled) {
                continue;
            }
            contReadData();

            if (sEventUpdateGlobals == NULL) {
                continue;
            }
            // received CONT_EVENT_UPDATE_GLOBALS
            contUpdateGlobals();
            if (sEventUpdateGlobals->cbQueue != NULL) {
                osSendMesg(sEventUpdateGlobals->cbQueue, sEventUpdateGlobals->mesg, OS_MESG_NOBLOCK);
            }
            sEventUpdateGlobals = NULL;
        } else {
            contHandleEvent((ControllerEvent *)mesg);
        }
    }
}