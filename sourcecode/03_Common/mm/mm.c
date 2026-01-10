#include "mm.h"
#include "tlsf.h"


static tlsf_t tlsf;
static unsigned long ulCurUsed;
static unsigned long ulMaxUsed;


void mm_init(void * mem_pool, size_t size)
{
    tlsf_create_with_pool(mem_pool, size);
}

void mm_deinit(void)
{
    tlsf_destroy(tlsf);
}

void * mm_alloc(size_t size)
{
    void * alloc = tlsf_malloc(tlsf, size);

    if(alloc == NULL) {
        // Handle allocation failure (e.g., log error)
        return NULL;
    }

    ulCurUsed += size;
    if(ulCurUsed > ulMaxUsed) {
        ulMaxUsed = ulCurUsed;
    }

    return alloc;
}

void mm_free(void * data)
{
    if(data == NULL) return;

    size_t size = tlsf_block_size(data);

    tlsf_free(tlsf, data);

    if(ulCurUsed > size) {
        ulCurUsed -= size;
    } else {
        ulCurUsed = 0;
    }
}

void * mm_realloc(void * data, size_t new_size)
{
    if(new_size == 0) {
        mm_free(data);
        return NULL;
    }

    if(data == NULL) {
        return mm_alloc(new_size);
    }

    void * new_p = tlsf_realloc(tlsf, data, new_size);

    if(new_p == NULL) {
        // Handle allocation failure (e.g., log error)
        return NULL;
    }

    return new_p;
}

