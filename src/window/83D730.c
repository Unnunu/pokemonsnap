#include "common.h"

typedef struct HeapChunk {
    /* 0x00 */ struct HeapChunk* prev;
    /* 0x04 */ s32 size;
    /* 0x08 */ s32 allocated;
    /* 0x0C */ u8 unk_0C[0x4];
    /* 0x10 */ union {
        struct {
            /* 0x10 */ struct HeapChunk* next;
            /* 0x14 */ struct HeapChunk* prev;
        } link;
        u8 data[1];
    };
} HeapChunk; // size >= 0x18 + VLA

extern void* D_803A6900;
extern s32 D_803A6904;
extern u32 D_803A6908;
extern HeapChunk D_803A6910;

void mem_insert_free_chunk(HeapChunk* prev, HeapChunk* curr, HeapChunk* next) {
    prev->link.next = curr;
    curr->link.next = next;
    next->link.prev = curr;
    curr->link.prev = prev;
}

void mem_add_free_chunk(HeapChunk* curr) {
    mem_insert_free_chunk(D_803A6910.link.prev, curr, &D_803A6910);
}

void mem_remove_chunk_from_free_list(HeapChunk* chunk) {
    HeapChunk* prev = chunk->link.prev;
    HeapChunk* next = chunk->link.next;

    prev->link.next = next;
    next->link.prev = prev;
}

#ifdef NON_MATCHING
void func_80369FD4_83D784(void* base, s32 size) {
    HeapChunk* header = base;

    D_803A6900 = header;
    D_803A6904 = size;
    D_803A6908 = (u32)base + size;

    header->prev = NULL;
    header->size = size;
    header->allocated = FALSE;
    mem_insert_free_chunk(&D_803A6910, header, &D_803A6910);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/window/83D730/func_80369FD4_83D784.s")
void func_80369FD4_83D784(void* base, s32 size);
#endif

HeapChunk* mem_find_chunk(s32 size) {
    HeapChunk* curr = D_803A6910.link.next;

    while (curr != &D_803A6910) {
        if (curr->size >= size) {
            return curr;
        }
        curr = curr->link.next;
    }

    return NULL;
}

#ifdef NON_MATCHING
HeapChunk* mem_split_chunk(HeapChunk* chunk, s32 offset) {
    u32 newSize;
    HeapChunk* child;
    HeapChunk* next;

    child = (HeapChunk*)((u32)chunk + offset);

    child->prev = chunk;
    newSize = chunk->size - offset;
    child->size = newSize;
    child->allocated = FALSE;

    next = (HeapChunk*)((u32)child + newSize);
    if ((u32)next < D_803A6908) {
        next->prev = child;
    }

    chunk->size = offset;
    return child;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/window/83D730/func_8036A07C_83D82C.s")
HeapChunk* mem_split_chunk(HeapChunk* chunk, s32 offset);
#endif

#ifdef NON_MATCHING
void mem_merge_chunks(HeapChunk* chunk) {
    s32 size;
    HeapChunk* prev;
    HeapChunk* nextNext;
    HeapChunk* next;

    prev = chunk->prev;
    size = chunk->size;

    next = (HeapChunk*)((u32)(chunk) + size);
    if (prev != NULL && !prev->allocated) {
        // merge with previous
        prev->size += size;
        size = (u32)next;
        if (size < (u32)D_803A6908) {
            next->prev = prev;
        }
        chunk = prev;
    } else {
        mem_add_free_chunk(chunk);
        chunk->allocated = FALSE;
    }
    if ((u32)next < D_803A6908 && !next->allocated) {
        nextNext = (HeapChunk*)((u32)next + next->size);
        if ((u32)nextNext < (u32)D_803A6908) {
            nextNext->prev = chunk;
        }
        chunk->size += next->size;
        mem_remove_chunk_from_free_list(next);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/window/83D730/func_8036A0BC_83D86C.s")
void mem_merge_chunks(HeapChunk* chunk);
#endif

void* mem_alloc(u32 size) {
    s32 size_with_header;
    HeapChunk* chunk;

    if (size < sizeof(chunk->link)) {
        size = sizeof(chunk->link);
    }

    size_with_header = ALIGN(size + offsetof(HeapChunk, data), 8);
    chunk = mem_find_chunk(size_with_header);
    if (chunk == NULL) {
        return NULL;
    }
    if (chunk->size - size_with_header > sizeof(HeapChunk)) {
        mem_add_free_chunk(mem_split_chunk(chunk, size_with_header));
    }
    mem_remove_chunk_from_free_list(chunk);
    chunk->allocated = TRUE;
    return (u32)(chunk) + offsetof(HeapChunk, data);
}

// public
void mem_free(void* data) {
    mem_merge_chunks((u32)data - offsetof(HeapChunk, data));
}

#pragma GLOBAL_ASM("asm/nonmatchings/window/83D730/func_8036A248_83D9F8.s")
void mem_realloc(void* data, u32 size);

void mem_create_heap(u8* buffer, u32 size) {
    func_80369FD4_83D784(buffer, size);
}

#pragma GLOBAL_ASM("asm/nonmatchings/window/83D730/func_8036A418_83DBC8.s")
s32 mem_get_allocated_memory(void);
