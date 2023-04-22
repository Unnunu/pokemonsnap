#include "common.h"
#include "sys/dma.h"
#include "sys/sched.h"

OSPiHandle* gRomPiHandle;
OSMesg sDmaOSMesg;
OSMesgQueue sDmaRetQueue;
s32 sVpkRamAddr;
s32 sVpkBufSize;
s32 sVpkRomAddr;

void create_dma_mq(void) {
    osCreateMesgQueue(&sDmaRetQueue, &sDmaOSMesg, 1);
}

void dma_copy(OSPiHandle *piHandle, u32 devAddr, uintptr_t dramAddr, u32 numBytes, u8 direction) {
    OSIoMesg mb;

    if (direction == OS_WRITE) {
        osWritebackDCache((void*)dramAddr, numBytes);
    } else {
        osInvalDCache((void*)dramAddr, numBytes);
    }
    mb.hdr.pri = OS_MESG_PRI_NORMAL;
    mb.hdr.retQueue = &sDmaRetQueue;
    mb.size = 0x10000;

    while (numBytes > 0x10000) {
        mb.dramAddr = (void*)dramAddr;
        mb.devAddr = devAddr;
        if (!scBeforeReset) {
            osEPiStartDma(piHandle, &mb, direction);
        }
        osRecvMesg(&sDmaRetQueue, NULL, OS_MESG_BLOCK);
        devAddr += 0x10000;
        dramAddr += 0x10000;
        numBytes -= 0x10000;
    }

    if (numBytes != 0) {
        mb.dramAddr = (void*)dramAddr;
        mb.devAddr = devAddr;
        mb.size = numBytes;
        if (!scBeforeReset) {
            osEPiStartDma(piHandle, &mb, direction);
        }
        osRecvMesg(&sDmaRetQueue, NULL, OS_MESG_BLOCK);
    }
}

void load_overlay(Overlay* dmaData) {
    // If there is a text section, invalidate instruction and data caches
    if (dmaData->textVramEnd - dmaData->textVramStart != 0) {
        osInvalICache((void*)dmaData->textVramStart, dmaData->textVramEnd - dmaData->textVramStart);
        osInvalDCache((void*)dmaData->textVramStart, dmaData->textVramEnd - dmaData->textVramStart);
    }
    // If there are data/rodata sections, invalidate data cache
    if (dmaData->dataVramEnd - dmaData->dataVramStart != 0) {
        osInvalDCache((void*)dmaData->dataVramStart, dmaData->dataVramEnd - dmaData->dataVramStart);
    }
    // If there is any segment content, DMA it
    if (dmaData->romEnd - dmaData->romStart != 0) {
        dma_copy(gRomPiHandle, dmaData->romStart, dmaData->vramStart, dmaData->romEnd - dmaData->romStart, 0);
    }
    // Zero bss
    if (dmaData->bssVramEnd - dmaData->bssVramStart != 0) {
        bzero((void*)dmaData->bssVramStart, dmaData->bssVramEnd - dmaData->bssVramStart);
    }
}

void dma_rom_read(u32 devAddr, void* dramAddr, u32 numBytes) {
    dma_copy(gRomPiHandle, devAddr, (uintptr_t)dramAddr, numBytes, OS_READ);
}

void dma_rom_write(void* dramAddr, u32 devAddr, u32 numBytes) {
    dma_copy(gRomPiHandle, devAddr, (uintptr_t)dramAddr, numBytes, OS_WRITE);
}

#pragma GLOBAL_ASM("asm/nonmatchings/sys/dma/func_80002C94.s")
void func_80002C94(u16* data, s32 size, void (*func)(void), u32 arg3);

void initialize_vpk_dma_stream(s32 romAddr, s32 ramAddr, s32 bufSize) {
    sVpkRomAddr = romAddr;
    sVpkRamAddr = ramAddr;
    sVpkBufSize = bufSize;
}

void fill_vpk_dma_buffer(void) {
    dma_rom_read(sVpkRomAddr, sVpkRamAddr, sVpkBufSize);
    sVpkRomAddr += sVpkBufSize;
}

void func_800034C4(s32 romAddr, s32 arg1, s32 ramAddr, s32 bufSize) {
    initialize_vpk_dma_stream(romAddr, ramAddr, bufSize);
    func_80002C94(ramAddr, bufSize, fill_vpk_dma_buffer, arg1);
}

void loadCompressedData(u32 arg0, u32 arg1) {
    u8 buffer[0x400];

    func_800034C4(arg0, arg1, buffer, sizeof(buffer));
}

#pragma GLOBAL_ASM("asm/nonmatchings/sys/dma/func_80003530.s")
