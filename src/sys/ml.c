#include "PR/ultratypes.h"
#include "sys/ml.h"
#include "types.h"

void reset_bump_alloc(DynamicBuffer* heap) {
    heap->ptr = heap->start;
}

void* bump_alloc(DynamicBuffer* heap, u32 size, u32 alignment) {
    u8* aligned;
    u32 offset;

    if (alignment != 0) {
        offset = alignment - 1;
        aligned = (u8*)(((uintptr_t)heap->ptr + (offset)) & ~(offset));
    } else {
        aligned = heap->ptr;
    }

    heap->ptr = (void*)(aligned + size);
    if (heap->end < heap->ptr) {
        fatal_printf("ml : alloc overflow #%d\n", heap->id);
        while (TRUE) {}
    }

    return aligned;
}

void init_bump_alloc(DynamicBuffer* heap, u32 id, void* start, u32 size) {
    heap->id = id;
    heap->ptr = start;
    heap->start = start;
    heap->end = (void*)((uintptr_t)start + size);
    bzero(start, size);
}