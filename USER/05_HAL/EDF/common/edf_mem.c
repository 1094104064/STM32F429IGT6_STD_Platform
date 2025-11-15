/**
  ******************************************************************************
  *
  * @file    edf_mem.c
  * @author  
  * @brief   
  *
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  **/
  
  
/*********************
 *      INCLUDES
 *********************/
#include "edf_mem.h"

#include "edf_port.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#ifndef EDF_CFG_HEAP_SIZE
#define EDF_CFG_HEAP_SIZE                (4 * 1024)                  /**< If not defined, use 4KB */
#elif (EDF_CFG_HEAP_SIZE < 16)
#define EDF_CFG_HEAP_SIZE                (16)                        /**< If less than 16, use 16B */
#endif /* EDF_CFG_HEAP_SIZE */
#define EDF_HEAP_BLOCK_FREE              (0)
#define EDF_HEAP_BLOCK_ALLOCATED         (1)
#define EDF_HEAP_BLOCK_MIN_SIZE          (sizeof(struct edf_heap_block) << 1)
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void heap_insert_block(edf_heap_block_t *block);
/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t heap_mem[EDF_CFG_HEAP_SIZE] = {0};                    /**< Heap memory */


static edf_heap_block_t heap_start = {                           /**< Heap start block */
    .next = EDF_NULL, 
    .size = 0, 
    .allocated = EDF_HEAP_BLOCK_FREE
};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
 * @brief This function initialize the heap.
 */
void edf_heap_init(void)
{
     edf_heap_block_t *first_block = (edf_heap_block_t *)&heap_mem;

    /* Initialize the first block */
    first_block->next = EDF_NULL;
    first_block->size = sizeof(heap_mem) - sizeof(edf_heap_block_t);
    first_block->allocated = EDF_HEAP_BLOCK_FREE;
    heap_start.next = first_block;
}

/**
 * @brief This function allocate memory.
 *
 * @param size The size of the memory.
 *
 * @return A pointer to the allocated memory.
 */
EDF_WEAK void *edf_malloc(size_t size)
{
    edf_heap_block_t *block_prev = &heap_start;
    edf_heap_block_t *block = block_prev->next;

    edf_interrupt_disable();

    /* Check size and residual memory */
    if ((size == 0) || (size > (UINT32_MAX >> 1) || (block == EDF_NULL))) {
        edf_interrupt_enable();
        return EDF_NULL;
    }

    /* Align the size to the next multiple of 4 bytes */
    size = EDF_ALIGN_UP(size, 4);

    /* Search for and take blocks that match the criteria */
    while (block->size < size) {
        if (block->next == EDF_NULL) {
            edf_interrupt_enable();
            return EDF_NULL;
        }
        block_prev = block;
        block = block->next;
    }
    block_prev->next = block->next;

    /* Allocate memory */
    void *memory = (void *)((uint8_t *)block + sizeof(edf_heap_block_t));
    size_t residual = block->size - size;

    /* Set the block information */
    block->size = size;
    block->next = EDF_NULL;
    block->allocated = EDF_HEAP_BLOCK_ALLOCATED;
    /* Check if we need to allocate a new block */
    if (residual > EDF_HEAP_BLOCK_MIN_SIZE) {
        edf_heap_block_t *new_block = (edf_heap_block_t *)(((uint8_t *)memory) + size);

        /* Set the new block information */
        new_block->size = residual - sizeof(edf_heap_block_t);
        new_block->next = EDF_NULL;
        new_block->allocated = EDF_HEAP_BLOCK_FREE;

        /* Insert the new block */
        heap_insert_block(new_block);
    }

    edf_interrupt_enable();
    return memory;
}

/**
 * @brief This function free memory.
 *
 * @param memory The memory to free.
 */
EDF_WEAK void edf_free(void *memory)
{
    if (memory != EDF_NULL) {
        edf_heap_block_t *block = (edf_heap_block_t *)((uint8_t *)memory -
                                                               sizeof(edf_heap_block_t));
        edf_interrupt_disable();

        /* Check the block */
        if (block->allocated == EDF_HEAP_BLOCK_ALLOCATED && block->size != 0) {
            block->allocated = EDF_HEAP_BLOCK_FREE;

            /* Insert the free block */
            heap_insert_block(block);
        }

        edf_interrupt_enable();
    }
}

/**
 * @brief This function get the usable size of the memory.
 *
 * @param memory The memory.
 *
 * @return The usable size of the memory.
 */
EDF_WEAK size_t edf_malloc_usable_size(void *memory)
{
    if (memory != EDF_NULL) {
        /* Get the block information */
        edf_heap_block_t *block = (edf_heap_block_t *)((uint8_t *)memory -
                                                               sizeof(edf_heap_block_t));
        return block->size;
    }
    return 0;
}

/**
 * @brief This function initialize the memory.
 *
 * @param num The number of the memory.
 * @param size The size of the memory.
 *
 * @return A pointer to the allocated memory.
 */
EDF_WEAK void *edf_calloc(size_t num, size_t size)
{
    size_t total = num * size;

    void *memory = edf_malloc(total);
    if (memory != EDF_NULL) {
        memset(memory, 0, total);
    }
    return memory;
}

/**
 * @brief This function realloc memory.
 *
 * @param memory The memory.
 * @param size The size of the memory.
 *
 * @return A pointer to the allocated memory.
 */
EDF_WEAK void *edf_realloc(void *memory, size_t size)
{
    size_t old_size = edf_malloc_usable_size(memory);

    void *new_memory = edf_malloc(size);
    if (new_memory != EDF_NULL) {
        memcpy(new_memory, memory, old_size);
        edf_free(memory);
    }
    return new_memory;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void heap_insert_block(edf_heap_block_t *block)
{
    edf_heap_block_t *block_prev = &heap_start;

    /* Search for the previous block */
    while (((block_prev->next != EDF_NULL) && (block_prev->next < block))) {
        block_prev = block_prev->next;
    }

    /* Insert the block */
    if (block_prev->next != EDF_NULL) {
        /* Merge with the previous block */
        if ((void *)(((uint8_t *)block_prev) + sizeof(edf_heap_block_t) + block_prev->size) ==
            (void *)block) {
            block_prev->size += block->size + sizeof(edf_heap_block_t);
            block = block_prev;
        }

        /* Merge with the next block */
        if ((void *)(((uint8_t *)block) + sizeof(edf_heap_block_t) + block->size) ==
            (void *)block_prev->next) {
            block->size += block_prev->next->size + sizeof(edf_heap_block_t);
            block->next = block_prev->next->next;
            if (block != block_prev) {
                block_prev->next = block;
                block = block_prev;
            }
        }
    }

    /* Insert the block */
    if (block != block_prev) {
        block->next = block_prev->next;
        block_prev->next = block;
    }
}




/******************************* (END OF FILE) *********************************/



 







