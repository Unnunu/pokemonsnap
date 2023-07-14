#include "PR/synthInternals.h"
#include "PR/os.h"
#include "PR/gu.h"
#include "sys/sched.h"
#include "sys/main.h"
#include "sys/dma.h"
#include "macros.h"
#include "PR/rcp.h"

#define NUM_DMA_BUFFERS 4
#define MAX_BUFFERS 50
#define MAX_BUFFER_LENGTH 1024
#define AUDIO_EXTRA_SAMPLES 80

#define TREMELO_SIN 1
#define TREMELO_SQR 2
#define TREMELO_DSC_SAW 3
#define TREMELO_ASC_SAW 4
#define VIBRATO_SIN 128
#define VIBRATO_SQR 129
#define VIBRATO_DSC_SAW 130
#define VIBRATO_ASC_SAW 131
#define OSC_HIGH 0
#define OSC_LOW 1
#define TWO_PI 6.2831853f
#define OSC_STATE_COUNT 44

typedef struct {
    s32 heapBase;
    s32 heapSize;
    u16 outputRate;
    u8 maxPVoices;
    u8 maxVVoices;
    u8 maxUpdates;
    u8 maxEvents;
    u8 numSounds;
    u8 unk_0F[2];
    s8 unk_11;
    s8 unk_12;
    u8 unk_13;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 romSbkStart;
    u8 unk_30;
} AuAlpha;

typedef struct {
    u8 rate;
    u8 depth;
    u8 oscCount;
} defData;

typedef struct {
    u8 halfdepth;
    u8 baseVol;
} tremSinData;

typedef struct {
    u8 curVal;
    u8 hiVal;
    u8 loVal;
} tremSqrData;

typedef struct {
    u8 baseVol;
    u8 depth;
} tremSawData;

typedef struct {
    f32 depthcents;
} vibSinData;

typedef struct {
    f32 loRatio;
    f32 hiRatio;
} vibSqrData;

typedef struct {
    s32 hicents;
    s32 centsrange;
} vibDSawData;

typedef struct {
    s32 locents;
    s32 centsrange;
} vibASawData;

typedef struct oscData_s {
    struct oscData_s* next;
    u8 type;
    u8 stateFlags;
    u16 maxCount;
    u16 curCount;
    union {
        defData def;
        tremSinData tsin;
        tremSqrData tsqr;
        tremSawData tsaw;
        vibSinData vsin;
        vibSqrData vsqr;
        vibDSawData vdsaw;
        vibASawData vasaw;
    } data;
} oscData;

typedef struct {
    u32 addr;
    u32 len;
    u32 size;
    char* ptr;
} AMDMABuffer;

typedef struct {
    u32 nBuffers;
    u32 currentBuffer;
    AMDMABuffer buffers[NUM_DMA_BUFFERS];
} AMDMAState;

typedef struct N_ALVoice_s {
    ALLink node;
    struct N_PVoice_s* pvoice;
    ALWaveTable* table;
    void* clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} N_ALVoice;

typedef struct {
    N_ALVoice voice;
    ALSound* sound; /* sound referenced here */
    s16 priority;
    f32 pitch;      /* current playback pitch                    */
    s32 state;      /* play state for this sound                 */
    s16 vol;        /* volume - combined with volume from bank   */
    ALPan pan;      /* pan - 0 = left, 127 = right               */
    u8 fxMix;       /* wet/dry mix - 0 = dry, 127 = wet          */
} N_ALSoundState;

extern u8 D_8003D980[]; // rsp ucode
extern u8 D_80044BA0[]; // rsp ucode

s32 D_80041F50[] = {
    8,      0x28a0, 0,    0x100,  9830,  -9830,  0,      0,      0,      0,      0x100, 0x180, 9830,  -9830,
    11140,  0,      0,    0x5000, 0x520, 0x1000, 16384,  -16384, 4587,   0,      0,     0,     0x5a0, 0xce0,
    8192,   -8192,  0,    0,      0,     0,      0x1440, 0x2340, 16384,  -16384, 4587,  0,     0,     0x6000,
    0x14c0, 0x1dc0, 8192, -8192,  0,     0,      0,      0,      0x1dc0, 0x2180, 8192,  -8192, 0,     0,
    0,      0,      0,    0x2560, 18000, 0,      0,      0,      0,      0x7000,
};

s32 D_80042058[] = { 14, 0x4b00, 0,      0x280,  9830, -9830, 0,    0, 0, 0, 0x3c0,  0x500,  3276, -3276, 8191, 0,
                     0,  0,      0x780,  0xa00,  3276, -3276, 5631, 0, 0, 0, 0xa00,  0xdc0,  3276, -3276, 4351, 0,
                     0,  0,      0xc80,  0x12c0, 3276, -3276, 4351, 0, 0, 0, 0xf00,  0x1900, 3276, -3276, 4095, 0,
                     0,  0,      0x1680, 0x1e00, 3276, -3276, 4095, 0, 0, 0, 0x1f40, 0x2580, 3276, -3276, 2559, 0,
                     0,  0,      0x2580, 0x2bc0, 3276, -3276, 2559, 0, 0, 0, 0x2bc0, 0x3200, 3276, -3276, 1535, 0,
                     0,  0,      0x3200, 0x3840, 3276, -3276, 1535, 0, 0, 0, 0x3840, 0x3e80, 3276, -3276, 255,  0,
                     0,  0,      0x3e80, 0x44c0, 3276, -3276, 255,  0, 0, 0, 0,      0x2580, 5000, 0,     0,    0,
                     0,  0x5000 };

s32 D_80042208[2] = { D_80041F50, D_80042058 };

u8 D_80042228[400] = { 50,  50,  50,  50,  50,  50,  50,  50,  30,  30,  50,  50,  50,  50,  90,  90,  50,  50,  50,  50,
                    90,  90,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  100, 100, 100, 100, 100, 100, 100, 100,
                    100, 100, 100, 100, 100, 100, 100, 100, 80,  80,  100, 100, 100, 100, 100, 100, 100, 100, 100, 80,
                    100, 100, 100, 100, 40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,
                    40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,
                    40,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
                    50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50 };

s32 D_800423B8 = 0;
s32 nextDMA = 0;
s32 D_800423C0 = 1;
s32 D_800423C4 = 0;
s32 D_800423C8 = 0;
s32 D_800423CC = 1;
s16 D_800423D0[] = { 0, 0, 0, 0 };
u8 D_800423D8 = 127;
u8 D_800423DC = 0;
u32 D_800423E0 = 0x7F00;

static u8 auHeapBuffer[0x4B000];
static u32 D_80096250;
static ALHeap auHeap;
static void* D_80096268;
static s32 D_8009626C;
static SCClient auClient;
static OSMesgQueue auGameTickQueue;
static OSMesgQueue auSPTaskMessageQueue;
static OSMesgQueue auDMAMessageQueue;
static OSMesg auGameTickMessages[1];
static OSMesg auSPTaskMessages[1];
static OSMesg auDMAMessages[50];
static OSIoMesg audDMAIOMesgBuf[MAX_BUFFERS]; // 96390
static ALGlobals D_80096840;
static s32 D_8009688C;
static s16* D_80096890[3];
static Acmd* D_8009689C;
static Acmd* D_800968A0[2];
static SCTaskAudio* auTask;
static SCTaskAudio* D_800968B0[2];
static ALBank* D_800968B8;
static s32* D_800968BC;
static s32* auCurrentSound;
static s8* auSoundId; // returned by alSndpAllocate
static u8* auSoundPriority;
static u8* D_800968CC;
static f32* auSoundPitch;
static u16* auSoundVolume;
static u8* auSoundPan;
static u8* auSoundReverbAmt;
static ALInstrument* D_800968E0;
static ALSndPlayer* auSoundPlayer;
static ALBank* auSeqBank;
static ALSeqFile* auSeqFile;
static ALCSPlayer* auSeqPlayers[2];
static ALCSeq* auSequences[2];
static u8 auGlobalSongPriority;
static u8* auSeqData[2];
static u8* auSeqPlayerStatus;
static s32* auSeqPlayerSongId;
static s32 auSongVolTimer[2];
static f32 auSongVolume[2];
static f32 auSongVolRate[2];
static AuAlpha D_80096930;
static OSTime D_80096968;
static oscData* freeOscStateList; // 96970

AuAlpha D_800423E4 = {
    auHeapBuffer,
    sizeof(auHeapBuffer),
    32000,
    0x16,
    0x18,
    0x40,
    0x40,
    10,       
    { 16, 16 },        
    0,          
    50,         
    100,        
    0x00BA6C20, 
    0x00BB6940,
    0x00BB6940,
    0x00AFEEE0,        
    0x00B04430, 
    0x00B04430, 
    0x00AEFC10, 
    AL_FX_CUSTOM
};

static void _bnkfPatchBank(ALBank* bank, s32 offset, s32 table);
static void _bnkfPatchInst(ALInstrument* i, s32 offset, s32 table);
static void _bnkfPatchSound(ALSound* s, s32 offset, s32 table);
static void _bnkfPatchWaveTable(ALWaveTable* w, s32 offset, s32 table);

void func_80022B14(void);
void auStopSongPlayers(void);
void auStopPlayer(s32 arg0);

void alHeapInit(ALHeap* hp, u8* base, s32 len) {
    s32 extraAlign = (AL_CACHE_ALIGN + 1) - ((s32)base & AL_CACHE_ALIGN);

    if (extraAlign != AL_CACHE_ALIGN + 1)
        hp->base = base + extraAlign;
    else
        hp->base = base;

    hp->len = len;
    hp->cur = hp->base;
    hp->count = 0;
}

void* alHeapDBAlloc(u8* file, s32 line, ALHeap* hp, s32 num, s32 size) {
    s32 bytes;
    u8* ptr = 0;

    bytes = (num * size + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;

    if ((hp->cur + bytes) <= (hp->base + hp->len)) {
        ptr = hp->cur;
        hp->cur += bytes;
    }

    return ptr;
}

void alSeqFileNew(ALSeqFile* file, u8* base) {
    s32 offset = (s32)base;
    s32 i;

    /*
     * patch the file so that offsets are pointers
     */
    for (i = 0; i < file->seqCount; i++) {
        file->seqArray[i].offset = (u8*)((u8*)file->seqArray[i].offset + offset);
    }
}

void alBnkfNew(ALBankFile* file, u8* table) {
    s32 offset = (s32)file;
    s32 woffset = (s32)table;

    s32 i;

    /*
     * patch the file so that offsets are pointers
     */
    for (i = 0; i < file->bankCount; i++) {
        file->bankArray[i] = (ALBank*)((u8*)file->bankArray[i] + offset);
        if (file->bankArray[i])
            _bnkfPatchBank(file->bankArray[i], offset, woffset);
    }
}

void _bnkfPatchBank(ALBank* bank, s32 offset, s32 table) {
    s32 i;

    if (bank->flags)
        return;

    bank->flags = 1;

    if (bank->percussion) {
        bank->percussion = (ALInstrument*)((u8*)bank->percussion + offset);
        _bnkfPatchInst(bank->percussion, offset, table);
    }

    for (i = 0; i < bank->instCount; i++) {
        bank->instArray[i] = (ALInstrument*)((u8*)bank->instArray[i] + offset);
        if (bank->instArray[i])
            _bnkfPatchInst(bank->instArray[i], offset, table);
    }
}

void _bnkfPatchInst(ALInstrument* inst, s32 offset, s32 table) {
    s32 i;

    if (inst->flags)
        return;

    inst->flags = 1;

    for (i = 0; i < inst->soundCount; i++) {
        inst->soundArray[i] = (ALSound*)((u8*)inst->soundArray[i] + offset);
        _bnkfPatchSound(inst->soundArray[i], offset, table);
    }
}

void _bnkfPatchSound(ALSound* s, s32 offset, s32 table) {
    if (s->flags)
        return;

    s->flags = 1;

    s->envelope = (ALEnvelope*)((u8*)s->envelope + offset);
    s->keyMap = (ALKeyMap*)((u8*)s->keyMap + offset);

    s->wavetable = (ALWaveTable*)((u8*)s->wavetable + offset);
    _bnkfPatchWaveTable(s->wavetable, offset, table);
}

void _bnkfPatchWaveTable(ALWaveTable* w, s32 offset, s32 table) {
    if (w->flags)
        return;

    w->flags = 1;

    w->base += table;

    if (w->type == AL_ADPCM_WAVE) {
        w->waveInfo.adpcmWave.book = (ALADPCMBook*)((u8*)w->waveInfo.adpcmWave.book + offset);
        if (w->waveInfo.adpcmWave.loop)
            w->waveInfo.adpcmWave.loop = (ALADPCMloop*)((u8*)w->waveInfo.adpcmWave.loop + offset);
    } else if (w->type == AL_RAW16_WAVE) {
        if (w->waveInfo.rawWave.loop)
            w->waveInfo.rawWave.loop = (ALRawLoop*)((u8*)w->waveInfo.rawWave.loop + offset);
    }
}

void auRomRead(u32 src_addr, void* dest_addr, u32 size) {
    OSIoMesg DmaIoMessage;

    osInvalDCache((void*)dest_addr, (s32)size);

    DmaIoMessage.hdr.pri = OS_MESG_PRI_NORMAL;
    DmaIoMessage.hdr.retQueue = &auDMAMessageQueue;
    DmaIoMessage.dramAddr = dest_addr;
    DmaIoMessage.devAddr = src_addr;
    DmaIoMessage.size = size;

    osEPiStartDma(gRomPiHandle, &DmaIoMessage, OS_READ);
    osRecvMesg(&auDMAMessageQueue, NULL, OS_MESG_BLOCK);
}

s32 auDMA(s32 addr, s32 len, void* state) {
    void* freeBuffer;
    AMDMAState* dState = state;
    s32 delta = 0;
    u32 bStartAddr;
    u32 bEndAddr;
    AMDMABuffer* dBuff = &dState->buffers[dState->currentBuffer];
    OSMesg dummyMesg;

    /*
     * Is it in the last buffer
     */

    bStartAddr = (u32)dBuff->addr;
    bEndAddr = (u32)bStartAddr + dBuff->len;

    if ((addr >= bStartAddr) && (addr + len <= bEndAddr)) {
        freeBuffer = dBuff->ptr + addr - dBuff->addr;
    } else {
        if (++dState->currentBuffer >= dState->nBuffers) {
            dState->currentBuffer = 0;
        }
        dBuff = &dState->buffers[dState->currentBuffer];
        freeBuffer = dBuff->ptr;
        delta = addr & 0x1;
        addr -= delta;

        dBuff->addr = addr;
        dBuff->len = dBuff->size;

        audDMAIOMesgBuf[nextDMA].hdr.pri = OS_MESG_PRI_HIGH;
        audDMAIOMesgBuf[nextDMA].hdr.retQueue = &auDMAMessageQueue;
        audDMAIOMesgBuf[nextDMA].dramAddr = freeBuffer;
        audDMAIOMesgBuf[nextDMA].devAddr = (u32)addr;
        audDMAIOMesgBuf[nextDMA].size = dBuff->size;

        osEPiStartDma(gRomPiHandle, &audDMAIOMesgBuf[nextDMA++], OS_READ);
    }
    return (int)osVirtualToPhysical(freeBuffer) + delta;
}

ALDMAproc auDMANew(AMDMAState** state) {
    int i;
    AMDMAState* dState;

    dState = (AMDMAState*)alHeapAlloc(&auHeap, 1, sizeof(AMDMAState));
    dState->currentBuffer = 0;
    dState->nBuffers = NUM_DMA_BUFFERS;
    for (i = 0; i < NUM_DMA_BUFFERS; i++) {
        dState->buffers[i].ptr = alHeapAlloc(&auHeap, 1, MAX_BUFFER_LENGTH);
        dState->buffers[i].addr = 0;
        dState->buffers[i].len = 0;
        dState->buffers[i].size = MAX_BUFFER_LENGTH;
    }
    *state = (AMDMAState*)dState;
    return auDMA;
}

f32 _depth2Cents(u8 depth) {
    f32 x = 1.03099303;
    f32 cents = 1.0;

    while (depth) {
        if (depth & 1)
            cents *= x;
        x *= x;
        depth >>= 1;
    }

    return (cents);
}

ALMicroTime initOsc(void** oscState, f32* initVal, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay) {
    oscData* statePtr;
    ALMicroTime deltaTime = 0;

    if (freeOscStateList) /* yes there are oscStates available */
    {
        statePtr = freeOscStateList;
        freeOscStateList = freeOscStateList->next;
        statePtr->type = oscType;
        *oscState = statePtr;

        /*
         * Convert delay into usec's, In this example, multiply by
         * 0x4000, but could easily use another conversion method.
         */
        deltaTime = oscDelay * 0x4000;

        switch (oscType) /* set the initVal */
        {
            case TREMELO_SIN:
                statePtr->curCount = 0;
                statePtr->maxCount = 259 - oscRate; /* gives values 4-259 */
                statePtr->data.tsin.halfdepth = oscDepth >> 1;
                statePtr->data.tsin.baseVol = AL_VOL_FULL - statePtr->data.tsin.halfdepth;
                *initVal = (f32)statePtr->data.tsin.baseVol;
                break;

            case TREMELO_SQR:
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                statePtr->stateFlags = OSC_HIGH;
                statePtr->data.tsqr.loVal = AL_VOL_FULL - oscDepth;
                statePtr->data.tsqr.hiVal = AL_VOL_FULL;
                statePtr->data.tsqr.curVal = AL_VOL_FULL;
                *initVal = (f32)AL_VOL_FULL;
                break;

            case TREMELO_DSC_SAW:
                statePtr->maxCount = 256 - oscRate;
                statePtr->curCount = 0;
                statePtr->data.tsaw.depth = oscDepth;
                statePtr->data.tsaw.baseVol = AL_VOL_FULL;
                *initVal = (f32)statePtr->data.tsaw.baseVol;
                break;

            case TREMELO_ASC_SAW:
                statePtr->maxCount = 256 - oscRate;
                statePtr->curCount = 0;
                statePtr->data.tsaw.depth = oscDepth;
                statePtr->data.tsaw.baseVol = AL_VOL_FULL - oscDepth;
                *initVal = (f32)statePtr->data.tsaw.baseVol;
                break;

            case VIBRATO_SIN:
                statePtr->data.vsin.depthcents = _depth2Cents(oscDepth);
                statePtr->curCount = 0;
                statePtr->maxCount = 259 - oscRate; /* gives values 4-259 */
                *initVal = 1.0f;                    /* start at unity pitch */
                break;

            case VIBRATO_SQR: {
                s32 cents;
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                statePtr->stateFlags = OSC_HIGH;
                cents = _depth2Cents(oscDepth);
                statePtr->data.vsqr.loRatio = alCents2Ratio(-cents);
                statePtr->data.vsqr.hiRatio = alCents2Ratio(cents);
                *initVal = statePtr->data.vsqr.hiRatio;
            } break;

            case VIBRATO_DSC_SAW: {
                s32 cents;
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                cents = _depth2Cents(oscDepth);
                statePtr->data.vdsaw.hicents = cents;
                statePtr->data.vdsaw.centsrange = 2 * cents;
                *initVal = alCents2Ratio(statePtr->data.vdsaw.hicents);
            } break;

            case VIBRATO_ASC_SAW: {
                s32 cents;
                statePtr->maxCount = 256 - oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                cents = _depth2Cents(oscDepth);
                statePtr->data.vasaw.locents = -cents;
                statePtr->data.vasaw.centsrange = 2 * cents;
                *initVal = alCents2Ratio(statePtr->data.vasaw.locents);
            } break;
        }
    }
    return (deltaTime); /* if there are no oscStates, return zero, but if
                           oscState was available, return delay in usecs */
}

ALMicroTime updateOsc(void* oscState, f32* updateVal) {
    f32 tmpFlt;
    oscData* statePtr = (oscData*)oscState;
    ALMicroTime deltaTime = AL_USEC_PER_FRAME; /* in this example callback every */
                                               /* frame, but could be at any interval */

    switch (statePtr->type)                    /* perform update calculations */
    {
        case TREMELO_SIN:
            statePtr->curCount++;
            if (statePtr->curCount >= statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt = sinf(tmpFlt * TWO_PI);
            tmpFlt = tmpFlt * (f32)statePtr->data.tsin.halfdepth;
            *updateVal = (f32)statePtr->data.tsin.baseVol + tmpFlt;
            break;

        case TREMELO_SQR:
            if (statePtr->stateFlags == OSC_HIGH) {
                *updateVal = (f32)statePtr->data.tsqr.loVal;
                statePtr->stateFlags = OSC_LOW;
            } else {
                *updateVal = (f32)statePtr->data.tsqr.hiVal;
                statePtr->stateFlags = OSC_HIGH;
            }
            deltaTime *= statePtr->maxCount;
            break;

        case TREMELO_DSC_SAW:
            statePtr->curCount++;
            if (statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;

            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.tsaw.depth;
            *updateVal = (f32)statePtr->data.tsaw.baseVol - tmpFlt;
            break;

        case TREMELO_ASC_SAW:
            statePtr->curCount++;
            if (statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.tsaw.depth;
            *updateVal = (f32)statePtr->data.tsaw.baseVol + tmpFlt;
            break;

        case VIBRATO_SIN:
            /* calculate a sin value (from -1 to 1) and multiply it by depthcents.
               Then convert cents to ratio. */

            statePtr->curCount++;
            if (statePtr->curCount >= statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt = sinf(tmpFlt * TWO_PI) * statePtr->data.vsin.depthcents;
            *updateVal = alCents2Ratio((s32)tmpFlt);
            break;

        case VIBRATO_SQR:
            if (statePtr->stateFlags == OSC_HIGH) {
                statePtr->stateFlags = OSC_LOW;
                *updateVal = statePtr->data.vsqr.loRatio;
            } else {
                statePtr->stateFlags = OSC_HIGH;
                *updateVal = statePtr->data.vsqr.hiRatio;
            }
            deltaTime *= statePtr->maxCount;
            break;

        case VIBRATO_DSC_SAW:
            statePtr->curCount++;
            if (statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.vdsaw.centsrange;
            tmpFlt = (f32)statePtr->data.vdsaw.hicents - tmpFlt;
            *updateVal = alCents2Ratio((s32)tmpFlt);
            break;

        case VIBRATO_ASC_SAW:
            statePtr->curCount++;
            if (statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.vasaw.centsrange;
            tmpFlt += (f32)statePtr->data.vasaw.locents;
            *updateVal = alCents2Ratio((s32)tmpFlt);
            break;
    }

    return (deltaTime);
}

void stopOsc(void* oscState) {
    ((oscData*)oscState)->next = freeOscStateList;
    freeOscStateList = (oscData*)oscState;
}

void auInit(void) {
    scAddClient(&auClient, &auGameTickQueue, auGameTickMessages, 1);
    osCreateMesgQueue(&auDMAMessageQueue, auDMAMessages, 50);
    osCreateMesgQueue(&auSPTaskMessageQueue, auSPTaskMessages, 1);
    osSendMesg(&auSPTaskMessageQueue, (OSMesg)NULL, OS_MESG_BLOCK);
}

void func_80020C40(void) {
    s32 tmp;
    ALBankFile* bankFile;
    s32 i;
    s32 len;

    bzero(D_80096930.heapBase, D_80096930.heapSize);
    alHeapInit(&auHeap, D_80096930.heapBase, D_80096930.heapSize);

    if ((u32)D_80096930.unk_20 >= 0x80000000U) {
        auSeqBank = D_80096930.unk_20;
    } else {
        len = D_80096930.unk_24 - D_80096930.unk_20;
        bankFile = alHeapAlloc(&auHeap, 1, len);
        auRomRead(D_80096930.unk_20, bankFile, len);
        alBnkfNew(bankFile, D_80096930.unk_28);
        auSeqBank = bankFile->bankArray[0];
    }

    if ((u32)D_80096930.unk_14 >= 0x80000000U) {
        D_800968B8 = D_80096930.unk_14;
    } else {
        len = D_80096930.unk_18 - D_80096930.unk_14;
        bankFile = alHeapAlloc(&auHeap, 1, len);
        auRomRead(D_80096930.unk_14, bankFile, len);
        alBnkfNew(bankFile, D_80096930.unk_1C);
        D_800968B8 = bankFile->bankArray[0];
    }

    if ((u32)D_80096930.romSbkStart >= 0x80000000U) {
        auSeqFile = D_80096930.romSbkStart;
    } else {
        auSeqFile = alHeapAlloc(&auHeap, 1, 4);
        auRomRead(D_80096930.romSbkStart, auSeqFile, 4);
        tmp = auSeqFile->seqCount * sizeof(ALSeqData) + 4;
        len = tmp;
        auSeqFile = alHeapAlloc(&auHeap, 1, len);
        auRomRead(tmp = D_80096930.romSbkStart, auSeqFile, len);
        alSeqFileNew(auSeqFile, D_80096930.romSbkStart);
    }

    // get maximal seq length
    for (i = 0, len = 0; i < auSeqFile->seqCount; i++) {
        auSeqFile->seqArray[i].len += auSeqFile->seqArray[i].len & 1;
        if (len < auSeqFile->seqArray[i].len) {
            len = auSeqFile->seqArray[i].len;
        }
    }

    for (i = 0; i < 2; i++) {
        auSeqData[i] = alHeapAlloc(&auHeap, 1, len);
    }

    D_800968A0[0] = alHeapAlloc(&auHeap, 1, 0x8000);
    D_800968A0[1] = alHeapAlloc(&auHeap, 1, 0x8000);

    D_800968B0[0] = alHeapAlloc(&auHeap, 1, sizeof(SCTaskAudio));
    D_800968B0[1] = alHeapAlloc(&auHeap, 1, sizeof(SCTaskAudio));

    D_80096890[0] = alHeapAlloc(&auHeap, 1, 0xE60);
    D_80096890[1] = alHeapAlloc(&auHeap, 1, 0xE60);
    D_80096890[2] = alHeapAlloc(&auHeap, 1, 0xE60);
}

void auCreatePlayers(void) {
    ALSynConfig synConfig;
    ALSeqpConfig seqConfig;
    ALSndpConfig sndpConfig;
    s32 i;
    s32 j;

    D_80096268 = auHeap.cur;
    D_8009626C = auHeap.count;

    synConfig.maxVVoices = D_80096930.maxVVoices;
    synConfig.maxPVoices = D_80096930.maxPVoices;
    synConfig.maxUpdates = D_80096930.maxUpdates;
    synConfig.dmaproc = auDMANew;
    synConfig.outputRate = osAiSetFrequency(D_80096930.outputRate);
    synConfig.heap = &auHeap;

    D_800423C4 = D_80096930.unk_30;

    if (D_800423C4 < AL_FX_CUSTOM) {
        synConfig.fxType = D_800423C4;
    } else {
        synConfig.fxType = AL_FX_CUSTOM;
        synConfig.params = D_80042208[D_800423C4 - AL_FX_CUSTOM];
    }

    alInit(&D_80096840, &synConfig);

    D_8009688C = (s32)(synConfig.outputRate / (f32)60) + 1;
    if (D_8009688C & 0xf) {
        D_8009688C = (D_8009688C & ~0xf) + 0x10;
    }

    D_800968E0 = D_800968B8->instArray[0];

    auSoundPlayer = alHeapAlloc(&auHeap, 1, sizeof(ALSndPlayer));
    auSoundId = alHeapAlloc(&auHeap, 1, D_80096930.numSounds);
    D_800968BC = alHeapAlloc(&auHeap, 1, D_80096930.numSounds * 4);
    auCurrentSound = alHeapAlloc(&auHeap, 1, D_80096930.numSounds * 4);
    auSoundPriority = alHeapAlloc(&auHeap, 1, D_80096930.numSounds);
    D_800968CC = alHeapAlloc(&auHeap, 1, D_80096930.numSounds);
    auSoundPitch = alHeapAlloc(&auHeap, 1, D_80096930.numSounds * 4);
    auSoundVolume = alHeapAlloc(&auHeap, 1, D_80096930.numSounds * 2);
    auSoundPan = alHeapAlloc(&auHeap, 1, D_80096930.numSounds);
    auSoundReverbAmt = alHeapAlloc(&auHeap, 1, D_80096930.numSounds);

    for (i = 0; i < D_80096930.numSounds; i++) {
        D_800968BC[i] = -1;
        auCurrentSound[i] = -1;
        auSoundId[i] = -1;
        auSoundPriority[i] = 0;
        D_800968CC[i] = 255;
        auSoundPitch[i] = 1.0f;
        auSoundVolume[i] = 0x7FFF;
        auSoundPan[i] = 64;
        auSoundReverbAmt[i] = 0;
    }

    sndpConfig.maxEvents = D_80096930.maxEvents;
    sndpConfig.maxSounds = D_80096930.numSounds;
    sndpConfig.heap = &auHeap;
    alSndpNew(auSoundPlayer, &sndpConfig);

    auSeqPlayerStatus = alHeapAlloc(&auHeap, 1, 2);
    auSeqPlayerSongId = alHeapAlloc(&auHeap, 1, 8);

    auGlobalSongPriority = D_80096930.unk_13;

    freeOscStateList = alHeapAlloc(&auHeap, sizeof(oscData), OSC_STATE_COUNT);
    for (j = 0; j < OSC_STATE_COUNT - 1; j++) {
        freeOscStateList[j].next = &freeOscStateList[j + 1];
    }
    freeOscStateList[j].next = NULL;

    for (i = 0; i < 2; i++) {
        seqConfig.maxVoices = D_80096930.unk_0F[i];
        seqConfig.maxEvents = D_80096930.maxEvents;
        seqConfig.maxChannels = 16;
        seqConfig.heap = &auHeap;
        seqConfig.initOsc = initOsc;
        seqConfig.updateOsc = updateOsc;
        seqConfig.stopOsc = stopOsc;

        auSeqPlayers[i] = alHeapAlloc(&auHeap, 1, sizeof(ALCSPlayer) + 4); // TODO: N_ALCSPlayer ??
        alCSPNew(auSeqPlayers[i], &seqConfig);
        alCSPSetBank(auSeqPlayers[i], auSeqBank);
        auSequences[i] = alHeapAlloc(&auHeap, 1, sizeof(ALCSeq));
        auSeqPlayerStatus[i] = 0;
        auSeqPlayerSongId[i] = -1;
        auSongVolTimer[i] = 0;
        auSongVolume[i] = 32512.0f;
        auSongVolRate[i] = 0.0f;
    }
}

void thread4_audio(UNUSED s32 arg) {
    s32 i;
    s32 sp78;
    s32 sp74;
    s32 sp70 = 0;
    s32 sp6C;

    auInit();
    D_80096930 = D_800423E4;
    func_80020C40();
    auCreatePlayers();
    D_800423E4 = D_80096930;
    osSendMesg(&gThreadingQueue, (OSMesg)1, OS_MESG_NOBLOCK);

    while (TRUE) {
        sp6C = osGetCount();
        i = D_800423CC & 1;
        auTask = D_800968B0[i];
        D_8009689C = D_800968A0[i];

        sp74 = D_800423CC % 3;
        D_800423D0[sp74] = 16 + (D_8009688C - sp70 + AUDIO_EXTRA_SAMPLES) & ~0x0f;
        if (D_800423D0[sp74] < D_8009688C - 16) {
            D_800423D0[sp74] = D_8009688C - 16;
        }

        D_80096968 = osGetTime();
        D_8009689C = alAudioFrame(D_8009689C, &sp78, osVirtualToPhysical(D_80096890[sp74]), D_800423D0[sp74]);

        auTask->info.type = SC_TASK_TYPE_AUDIO;
        auTask->info.priority = 80;
        auTask->info.fnCheck = NULL;
        auTask->info.unk18 = 1;
        auTask->info.retVal = 0;
        auTask->info.mq = &auSPTaskMessageQueue;

        auTask->task.t.type = M_AUDTASK;
        auTask->task.t.flags = 0;
        auTask->task.t.data_ptr = D_800968A0[i];
        auTask->task.t.data_size = (D_8009689C - D_800968A0[i]) * sizeof(Acmd);
        auTask->task.t.ucode_boot = gRspBootCode;
        auTask->task.t.ucode_boot_size = sizeof(gRspBootCode);
        auTask->task.t.ucode = D_8003D980;
        auTask->task.t.ucode_size = 0x1000;
        auTask->task.t.ucode_data = D_80044BA0;
        auTask->task.t.ucode_data_size = 0x800;
        auTask->task.t.dram_stack = NULL;
        auTask->task.t.dram_stack_size = 0;
        auTask->task.t.yield_data_ptr = NULL;
        auTask->task.t.yield_data_size = 0;
        auTask->task.t.output_buff = NULL;
        auTask->task.t.output_buff_size = NULL;

        D_80096250 = (osGetCount() - sp6C) / 2971;
        osRecvMesg(&auGameTickQueue, NULL, OS_MESG_BLOCK);

        sp70 = IO_READ(AI_LEN_REG) >> 2;
        osRecvMesg(&auSPTaskMessageQueue, NULL, OS_MESG_BLOCK);

        sp74 = (D_800423CC - 1) % 3;
        if (D_800423C0 == 0) {
            for (i = 0; i < 2 * D_800423D0[sp74]; i += 2) {
                sp78 = (D_80096890[sp74][i] + D_80096890[sp74][i + 1]) / 2;
                D_80096890[sp74][i] = sp78;
                D_80096890[sp74][i + 1] = sp78;
            }
        }

        osWritebackDCacheAll();
        osAiSetNextBuffer(D_80096890[sp74], D_800423D0[sp74] << 2);

        for (i = 0; i < nextDMA; i++) {
            osRecvMesg(&auDMAMessageQueue, NULL, OS_MESG_NOBLOCK);
        }

        osWritebackDCacheAll();
        osSendMesg(&scTaskQueue, (OSMesg)auTask, OS_MESG_NOBLOCK);
        D_800423CC++;
        nextDMA = 0;

        for (i = 0; i < D_80096930.numSounds; i++) {
            if (auSoundId[i] == -1) {
                if (auCurrentSound[i] >= 0) {
                    N_ALSoundState* sState;
                    auSoundId[i] = alSndpAllocate(auSoundPlayer, D_800968E0->soundArray[auCurrentSound[i]]);
                    alSndpSetSound(auSoundPlayer, auSoundId[i]);
                    sState = auSoundPlayer->sndState;
                    sState[auSoundId[i]].pitch = auSoundPitch[i];
                    sState[auSoundId[i]].pan = auSoundPan[i];
                    sState[auSoundId[i]].fxMix = auSoundReverbAmt[i] + D_800423DC;
                    sState[auSoundId[i]].vol =
                        (auSoundVolume[i] * sState[auSoundId[i]].sound->sampleVolume * D_800423D8) >> 14;
                    alSndpPlay(auSoundPlayer);
                    alSndpSetPriority(auSoundPlayer, auSoundId[i], auSoundPriority[i]);
                    auCurrentSound[i] = -1;
                }
            } else {
                alSndpSetSound(auSoundPlayer, auSoundId[i]);
                if (alSndpGetState(auSoundPlayer) == AL_STOPPED) {
                    alSndpDeallocate(auSoundPlayer, auSoundId[i]);
                    auSoundId[i] = -1;
                    D_800968BC[i] = -1;
                }
            }
        }

        for (i = 0; i < 2; i++) {
            switch (auSeqPlayerStatus[i]) {
                case 1:
                    if (auSeqPlayers[i]->state != AL_STOPPED) {
                        alCSPStop(auSeqPlayers[i]);
                    } else {
                        if (auSeqPlayerSongId[i] < 0) {
                            auSeqPlayerStatus[i]--;
                        } else {
                            auRomRead(auSeqFile->seqArray[auSeqPlayerSongId[i]].offset, auSeqData[i],
                                      auSeqFile->seqArray[auSeqPlayerSongId[i]].len);
                            auSeqPlayerStatus[i]++;
                        }
                    }
                    break;
                case 2:
                    alCSeqNew(auSequences[i], auSeqData[i]);
                    alCSPSetSeq(auSeqPlayers[i], auSequences[i]);
                    alCSPPlay(auSeqPlayers[i]);
                    for (sp78 = 0; sp78 < 16; sp78++) {
                        alCSPSetChlPriority(auSeqPlayers[i], sp78, auGlobalSongPriority);
                    }
                    auSeqPlayerStatus[i]++;
                    break;
                case 3:
                    if (auSeqPlayers[i]->state == AL_STOPPED) {
                        auSeqPlayerStatus[i] = 0;
                        auSeqPlayerSongId[i] = -1;
                    }
                    break;
            }
        }

        for (i = 0; i < 2; i++) {
            if (auSongVolTimer[i] != 0) {
                auSongVolTimer[i]--;
                auSongVolume[i] += auSongVolRate[i];
                if (auSongVolume[i] < 0.0f) {
                    auSongVolume[i] = 0.0f;
                } else if (auSongVolume[i] > 32512.0f) {
                    auSongVolume[i] = 32512.0f;
                }
                alCSPSetVol(auSeqPlayers[i], auSongVolume[i]);
            }
        }

        if (D_800423B8) {
            sp78 = D_80096930.numSounds + 2;
            for (i = 0; i < D_80096930.numSounds; i++) {
                if (auSoundId[i] == -1) {
                    sp78--;
                }
            }

            for (i = 0; i < 2; i++) {
                if (auSeqPlayerStatus[i] == 0) {
                    sp78--;
                }
            }

            if (sp78 == 0) {
                osRecvMesg(&auSPTaskMessageQueue, NULL, OS_MESG_BLOCK);
                osSendMesg(&auSPTaskMessageQueue, (OSMesg)0, OS_MESG_BLOCK);
                alClose(&D_80096840);

                D_80096930 = D_800423E4;
                func_80020C40();
                auCreatePlayers();
                D_800423E4 = D_80096930;
                D_800423B8 = 0;
                sp70 = 0;
                osSendMesg(&gThreadingQueue, (OSMesg)1, OS_MESG_NOBLOCK);
            } else {
                auStopSongPlayers();
                func_80022B14();
            }
        }

        if (D_800423C8) {
            sp78 = D_80096930.numSounds + 2;
            for (i = 0; i < D_80096930.numSounds; i++) {
                if (auSoundId[i] == -1) {
                    sp78--;
                }
            }

            for (i = 0; i < 2; i++) {
                if (auSeqPlayerStatus[i] == 0) {
                    sp78--;
                }
            }

            if (sp78 == 0) {
                osRecvMesg(&auSPTaskMessageQueue, NULL, OS_MESG_BLOCK);
                osSendMesg(&auSPTaskMessageQueue, (OSMesg)0, OS_MESG_BLOCK);
                alClose(&D_80096840);
                auHeap.cur = D_80096268;
                auHeap.count = D_8009626C;

                auCreatePlayers();
                D_800423E4 = D_80096930;
                D_800423C8 = 0;
                sp70 = IO_READ(AI_LEN_REG) >> 2;
            } else {
                auStopSongPlayers();
                func_80022B14();
            }
        }
    }
}

void func_800222D8(void) {
    D_800423C0 = 1;
}

void func_800222E8(void) {
    D_800423C0 = 0;
}

void func_800222F4(s32 arg0) {
    D_800423C0 = arg0;
}

void func_80022300(s32 fxType) {
    if (fxType != D_800423C4) {
        D_800423C8++;
        D_80096930.unk_30 = fxType;
    }
}

void auStopSongPlayers(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        auStopPlayer(i);
    }
}

u32 auPlaySong(s32 playerID, u32 songID) {
    if (songID < auSeqFile->seqCount) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        auSeqPlayerStatus[playerID] = 1;
        auSeqPlayerSongId[playerID] = songID;
        osSetIntMask(mask);
        return songID;
    } else {
        return -1;
    }
}

void auStopPlayer(s32 playerId) {
    OSIntMask mask = osSetIntMask(OS_IM_NONE);
    auSeqPlayerStatus[playerId] = 1;
    auSeqPlayerSongId[playerId] = -1;
    osSetIntMask(mask);
}

void func_80022454(s32 playerID, u32 vol) {
    if (vol > 0x7F00) {
        vol = 0x7F00;
    }

    alCSPSetVol(auSeqPlayers[playerID], vol);
    auSongVolume[playerID] = vol;
    auSongVolTimer[playerID] = 0;
}

void func_800224DC(s32 playerID, u32 vol, u32 arg2) {
    if (vol > 0x7F00) {
        vol = 0x7F00;
    }

    if (arg2 != 0) {
        auSongVolTimer[playerID] = arg2;
        auSongVolRate[playerID] = (vol - auSongVolume[playerID]) / arg2;
    } else {
        func_80022454(playerID, vol);
    }
}

void func_80022578(s32 arg0, u32 reverb) {
    s32 i;

    if (reverb > 127) {
        reverb = 127;
    }

    for (i = 0; i < 16; i++) {
        alCSPSetChlFXMix(auSeqPlayers[arg0], i, reverb);
    }
}

void func_800225F0(s32 arg0, u8 priority) {
    s32 i;

    if (priority > 127) {
        priority = 127;
    }
    auGlobalSongPriority = priority;

    for (i = 0; i < 16; i++) {
        alCSPSetChlPriority(auSeqPlayers[arg0], i, priority);
    }
}

s32 func_80022674(s32 arg0) {
    if (auSeqPlayerStatus[arg0] != 0 && auSeqPlayerSongId[arg0] >= 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// TODO find out what is func_8002891C
void func_8002891C(ALCSPlayer*, s8);

void func_800226BC(s32 arg0, s8 arg1) {
    func_8002891C(auSeqPlayers[arg0], arg1);
}

// TODO find out what is func_8002E0CC
void func_8002E0CC(ALCSPlayer*, u8, s8);

void func_800226F4(s32 arg0, u8 arg1, s8 arg2) {
    func_8002E0CC(auSeqPlayers[arg0], arg1, arg2);
}

void func_80022734(s32 arg0, s32 tempo) {
    alCSPSetTempo(auSeqPlayers[arg0], tempo);
}

s32 func_80022760(s32 arg0) {
    return alCSPGetTempo(auSeqPlayers[arg0]);
}

s32 func_8002278C(u8 priority) {
    s32 i;
    s32 t0 = 0;
    s32 ret = -1;

    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundId[i] < 0 && auCurrentSound[i] < 0) {
            return i;
        }
    }

    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundPriority[i] < priority) {
            priority = auSoundPriority[i];
            ret = i;
            t0 = D_800968CC[i];
        } else if (auSoundPriority[i] == priority) {
            if (t0 < D_800968CC[i]) {
                t0 = D_800968CC[i];
                ret = i;
            }
        }
    }

    if (ret >= 0) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        alSndpSetSound(auSoundPlayer, auSoundId[ret]);
        alSndpStop(auSoundPlayer);
        osSetIntMask(mask);
    }

    return ret;
}

s32 auPlaySound(u32 soundID) {
    s32 i;

    for (i = 0; i < D_80096930.numSounds; i++) {
        if (D_800968CC[i] < 255) {
            D_800968CC[i]++;
        }
    }

    if (soundID < D_800968E0->soundCount) {
        i = func_8002278C(D_80042228[soundID]);
        if (i >= 0) {
            OSIntMask mask = osSetIntMask(OS_IM_NONE);
            D_800968BC[i] = soundID;
            auCurrentSound[i] = soundID;
            D_800968CC[i] = 0;
            auSoundPriority[i] = D_80042228[soundID];
            auSoundPitch[i] = 1.0f;
            auSoundVolume[i] = 0x7FFF;
            auSoundPan[i] = 64;
            auSoundReverbAmt[i] = 0;
            osSetIntMask(mask);
        }
        return i;
    } else {
        return -1;
    }
}

s32 auPlaySoundWithParams(u32 soundID, s32 volume, s32 pan, f32 pitch, s32 reverbAmt) {
    s32 i;
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    i = auPlaySound(soundID);

    if (i >= 0) {
        auSoundPitch[i] = pitch;
        auSoundVolume[i] = volume;
        auSoundPan[i] = pan;
        auSoundReverbAmt[i] = reverbAmt;
        osSetIntMask(mask);
        return i;
    } else {
        osSetIntMask(mask);
        return -1;
    }
}

void func_80022B14(void) {
    s32 i;
    OSIntMask mask = osSetIntMask(OS_IM_NONE);

    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundId[i] != -1) {
            alSndpSetSound(auSoundPlayer, auSoundId[i]);
            alSndpStop(auSoundPlayer);
        }
        D_800968BC[i] = -1;
        auCurrentSound[i] = -1;
    }
    osSetIntMask(mask);
}

void func_80022C0C(u32 arg0) {
    if (arg0 > 0x7F00) {
        arg0 = 0x7F00;
    }
    D_800423E0 = arg0;
}

void func_80022C28(s32 arg0, u32 vol) {
    if (auSoundId[arg0] != -1) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        alSndpSetSound(auSoundPlayer, auSoundId[arg0]);
        if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[arg0] >= 0) {
            if (vol > 0x7FFF) {
                vol = 0x7FFF;
            }
            auSoundVolume[arg0] = vol;
            vol = (vol * ((N_ALSoundState*)auSoundPlayer->sndState)[auSoundId[arg0]].sound->sampleVolume * D_800423D8) >>
                  14;
            alSndpSetVol(auSoundPlayer, vol);
        }
        osSetIntMask(mask);
    }
}

void func_80022D54(s32 i, u32 arg1) {
    if (auSoundId[i] != -1) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        alSndpSetSound(auSoundPlayer, auSoundId[i]);
        if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[i] >= 0) {
            if (arg1 > 127) {
                arg1 = 127;
            }
            alSndpSetPan(auSoundPlayer, arg1);
        }
        osSetIntMask(mask);
    }
}

void func_80022E20(s32 arg0, u32 arg1) {
    if (auSoundId[arg0] != -1) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        alSndpSetSound(auSoundPlayer, auSoundId[arg0]);
        if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[arg0] >= 0) {
            if (arg1 > 127) {
                arg1 = 127;
            }
            alSndpSetFXMix(auSoundPlayer, arg1);
        }
        osSetIntMask(mask);
    }
}

void func_80022EEC(s32 i, f32 arg1) {
    if (auSoundId[i] != -1) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        alSndpSetSound(auSoundPlayer, auSoundId[i]);
        if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[i] >= 0) {
            if (arg1 > 2.0) {
                arg1 = 2.0;
            }
            alSndpSetPitch(auSoundPlayer, arg1);
        }
        osSetIntMask(mask);
    }
}

void func_80022FD0(s32 i) {
    if (auSoundId[i] != -1) {
        OSIntMask mask = osSetIntMask(OS_IM_NONE);
        alSndpSetSound(auSoundPlayer, auSoundId[i]);
        alSndpStop(auSoundPlayer);
        osSetIntMask(mask);
    }
    D_800968BC[i] = -1;
    auCurrentSound[i] = -1;
}

void func_80023074(s32 i, u8 priority) {
    if (auSoundId[i] != -1) {
        auSoundPriority[i] = priority;
        alSndpSetPriority(auSoundPlayer, auSoundId[i], priority);
    }
}

void func_800230DC(void) {
    D_800423B8 = TRUE;
}

s32 func_800230EC(void) {
    return D_800423B8;
}

s32 func_800230F8(void) {
    return D_800423C8;
}

s32 func_80023104(void) {
    return D_800423C8 | D_800423B8;
}

void func_8002311C(void) {
    osRecvMesg(&gThreadingQueue, NULL, OS_MESG_NOBLOCK);
    D_800423B8 = 1;
    osRecvMesg(&gThreadingQueue, NULL, OS_MESG_BLOCK);
}

s32 func_80023168(u32 soundID, u32 reverbAmt) {
    s32 i;

    for (i = 0; i < D_80096930.numSounds; i++) {
        if (D_800968CC[i] < 255) {
            D_800968CC[i]++;
        }
    }

    if (soundID < D_800968E0->soundCount) {
        i = func_8002278C(D_80042228[soundID]);
        if (i >= 0) {
            OSIntMask mask = osSetIntMask(OS_IM_NONE);
            D_800968BC[i] = soundID;
            auCurrentSound[i] = soundID;
            D_800968CC[i] = 0;
            auSoundPriority[i] = D_80042228[soundID];
            auSoundPitch[i] = 1.0f;
            auSoundVolume[i] = 0x7FFF;
            auSoundPan[i] = 64;
            if (reverbAmt > 127) {
                reverbAmt = 127;
            }
            auSoundReverbAmt[i] = reverbAmt;
            osSetIntMask(mask);
        }
        return i;
    } else {
        return -1;
    }
}

s32 func_800232FC(u32 soundID, u32 vol) {
    s32 i;

    for (i = 0; i < D_80096930.numSounds; i++) {
        if (D_800968CC[i] < 255) {
            D_800968CC[i]++;
        }
    }

    if (soundID < D_800968E0->soundCount) {
        i = func_8002278C(D_80042228[soundID]);
        if (i >= 0) {
            OSIntMask mask = osSetIntMask(OS_IM_NONE);
            D_800968BC[i] = soundID;
            auCurrentSound[i] = soundID;
            D_800968CC[i] = 0;
            auSoundPriority[i] = D_80042228[soundID];
            auSoundPitch[i] = 1.0f;
            auSoundVolume[i] = vol;
            auSoundPan[i] = 64;
            auSoundReverbAmt[i] = 0;
            osSetIntMask(mask);
        }
        return i;
    } else {
        return -1;
    }
}

void func_80023474(u8 arg0) {
    UNUSED s32 temp;
    s32 i;
    OSIntMask mask;

    if (arg0 > 127) {
        arg0 = 127;
    }

    D_800423D8 = arg0;
    mask = osSetIntMask(OS_IM_NONE);
    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundId[i] != -1) {
            alSndpSetSound(auSoundPlayer, auSoundId[i]);
            if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[i] >= 0) {
                s32 vol = (auSoundVolume[i] * ((N_ALSoundState*)auSoundPlayer->sndState)[auSoundId[i]].sound->sampleVolume *
                           D_800423D8) >>
                          14;
                alSndpSetVol(auSoundPlayer, vol);
            }
        }
    }
    osSetIntMask(mask);
}

void func_80023600(u8 arg0) {
    UNUSED s32 temp;
    s32 i;
    OSIntMask mask;

    mask = osSetIntMask(OS_IM_NONE);
    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundId[i] != -1) {
            alSndpSetSound(auSoundPlayer, auSoundId[i]);
            if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[i] >= 0) {
                alSndpSetVol(auSoundPlayer,
                             (auSoundVolume[i] *
                              ((N_ALSoundState*)auSoundPlayer->sndState)[auSoundId[i]].sound->sampleVolume * arg0) >>
                                 14);
            }
        }
    }
    osSetIntMask(mask);
}

void func_80023770(u8 arg0) {
    s32 i;
    OSIntMask mask;

    if (arg0 > 127) {
        arg0 = 127;
    }

    mask = osSetIntMask(OS_IM_NONE);
    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundId[i] != -1) {
            alSndpSetSound(auSoundPlayer, auSoundId[i]);
            if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[i] >= 0) {
                alSndpSetFXMix(auSoundPlayer, arg0);
            }
        }
    }
    osSetIntMask(mask);
}

void func_80023880(u8 arg0) {
    UNUSED s32 temp;
    s32 i;
    OSIntMask mask;

    if (arg0 > 127) {
        arg0 = 127;
    }

    mask = osSetIntMask(OS_IM_NONE);
    for (i = 0; i < D_80096930.numSounds; i++) {
        if (auSoundId[i] != -1) {
            alSndpSetSound(auSoundPlayer, auSoundId[i]);
            if (alSndpGetState(auSoundPlayer) == AL_PLAYING || auCurrentSound[i] >= 0) {
                u8 a1 = auSoundReverbAmt[i] + arg0;
                if (a1 > 127) {
                    a1 = 127;
                }
                alSndpSetFXMix(auSoundPlayer, a1);
            }
        }
    }
    D_800423DC = arg0;
    osSetIntMask(mask);
}

ALBank* func_800239CC(void) {
    return D_800968B8;
}
