#include "string.c"

#define WASM_EXPORT __attribute__((visibility("default")))

/**
 * Possible Modes:
 *     ALLOC_0Z: size over everything (No possibilty for freeing allocated memory)
 *     ALLOC_0S: healty inbetween of size and performance
 *     ALLOC_01: a little performance increase over ALLOC_0S (block merging)
 *     DEFAULT: ALLOC_0S
 */
#if !(defined(ALLOC_0Z) && defined(ALLOC_0S) && defined(ALLOC_01))
#  define ALLOC_0S
#endif

#ifndef NULL
#  define NULL 0
#endif

#ifndef ALLOC_H
#define ALLOC_H
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long size_t;

union header {
	struct {
		unsigned int size;
		unsigned char is_free;
		union header *next;
	} s;
	char align_stub[16]; // aligining to 8bytes so you can properly use HEAPF64
};
union header *head, *tail;


extern unsigned char __heap_base;
unsigned long sbrk_ptr = (unsigned long)&__heap_base;

void* malloc(size_t size);
void free(void *block);
void *realloc(void *block, size_t size);
void *calloc(size_t nitems, size_t item_size);

#ifdef __cplusplus
};
#endif
#endif
