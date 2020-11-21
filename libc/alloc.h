#ifndef ALLOC_H
#define ALLOC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "string.c"

#define WASM_EXPORT __attribute__((visibility("default")))

/**
 * Possible Modes:
 *     ALLOC_SMALL: size over everything (No possibilty for freeing allocated memory)
 *     ALLOC_DEFAULT: healty inbetween of size and performance
 *     ALLOC_STANDARD: sTaNdArD cOnFoRmAtIe Is ImPoRtAnT (implements standart conform behavior and block merging)
 *     DEFAULT: ALLOC_DEFAULT
 */

#define ALLOC_STANDARD

#ifndef NULL
#  define NULL 0
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

#if defined(ALLOC_SMALL)
#  include "alloc_small.c"
#elif defined(ALLOC_DEFAULT)
#  include "alloc_performance.c"
#elif defined(ALLOC_STANDARD)
#  include "alloc_standard.c"
#else
#  include "alloc.c"
#endif



#ifdef __cplusplus
};
#endif
#endif
