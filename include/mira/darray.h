#ifndef MIRA_DARRAY_H_
#define MIRA_DARRAY_H_

#include <stddef.h>
#include <stdlib.h>

/*
 * MSVC doesn't always define max_align_t, even if you include <stddef.h>.
 * Go figure.
 */
#if defined(_MSC_VER) && !defined(__clang__)
#  ifndef _MAX_ALIGN_T_DEFINED
typedef struct {
    long long __max_align_ll;
    long double __max_align_ld;
} max_align_t;
#    define _MAX_ALIGN_T_DEFINED
#  endif
#endif

/* This macro's only purpose it readability */
#define DARRAY(T) T*

/* 
 * Header layout ensuring proper memory alignment.
 * max_align_t guarantees that the memory immediately following this 
 * header (the actual array) is aligned for any valid C data type.
 */
typedef union {
    struct {
        size_t capacity;
        size_t size;
    } d;
    max_align_t align;
} mira_darray_header;

/* Internal macro to retrieve the header pointer from the array pointer */
#define mira_darray_meta(arr) \
    (((mira_darray_header *)(arr)) - 1)

/* Public Macros */
#define DARRAY_CAPACITY(arr) \
    ((arr) ? mira_darray_meta(arr)->d.capacity : (size_t)0)

#define DARRAY_SIZE(arr) \
    ((arr) ? mira_darray_meta(arr)->d.size : (size_t)0)

/* 
 * Pushes a value to the back of the array.
 * If the array is NULL, it automatically allocates and initializes it.
 */
#define DARRAY_PUSH(arr, val) \
    ( \
        ((!(arr) || DARRAY_SIZE(arr) >= DARRAY_CAPACITY(arr)) \
            ? ((arr) = mira_darray_grow_impl((arr), sizeof(*(arr)))) \
            : (arr)), \
        ((arr)[mira_darray_meta(arr)->d.size++] = (val)) \
    )

/* Returns the last element. Undefined behavior if the array is empty. */
#define DARRAY_BACK(arr) \
    ((arr)[mira_darray_meta(arr)->d.size - 1])

/* Decrements the size by 1. Undefined behavior if the array is empty. */
#define DARRAY_POP(arr) \
    ((void)(mira_darray_meta(arr)->d.size--))

/* 
 * Standard C17 foreach. 
 * Needs the explicit type since there's no typeof() in C17 without GNU extensions.
 */
#define DARRAY_FOREACH(type, elem, arr) \
    for (type *elem = (arr); elem != (arr) + DARRAY_SIZE(arr); ++elem)

/* 
 * Reserves memory for at least 'new_cap' elements. 
 * If the current capacity is already greater than or equal to 'new_cap', 
 * this does nothing.
 */
#define DARRAY_RESERVE(arr, new_cap) \
    ((arr) = mira_darray_reserve_impl((arr), sizeof(*(arr)), (new_cap)))

#define DARRAY_FREE(arr) \
    do { \
        if (arr) { \
            free(mira_darray_meta(arr)); \
            (arr) = NULL; \
        } \
    } while(0)

/* Internal allocation function declarations */
void* mira_darray_reserve_impl(void* arr, size_t elem_size, size_t new_cap);
void* mira_darray_grow_impl(void* arr, size_t elem_size);

#ifdef MIRA_DARRAY_IMPL

void* mira_darray_reserve_impl(void* arr, size_t elem_size, size_t new_cap) {
    size_t current_cap = arr ? mira_darray_meta(arr)->d.capacity : 0;
    
    if (new_cap <= current_cap) {
        return arr;
    }
    
    size_t new_size = sizeof(mira_darray_header) + (new_cap * elem_size);
    mira_darray_header* hdr = arr ? mira_darray_meta(arr) : NULL;
    mira_darray_header* new_hdr = (mira_darray_header*)realloc(hdr, new_size);
    
    if (!new_hdr) {
        /* Allocation failed. */
        return NULL;
    }
    
    if (!arr) {
        new_hdr->d.size = 0;
    }
    new_hdr->d.capacity = new_cap;
    
    return (void*)(new_hdr + 1);
}

void* mira_darray_grow_impl(void* arr, size_t elem_size) {
    size_t cap = arr ? mira_darray_meta(arr)->d.capacity : 0;
    /* Start at 16, double if capacity is exceeded */
    size_t new_cap = cap ? cap * 2 : 16;
    
    return mira_darray_reserve_impl(arr, elem_size, new_cap);
}

#endif /* MIRA_DARRAY_IMPL */

#endif /* MIRA_DARRAY_H_ */