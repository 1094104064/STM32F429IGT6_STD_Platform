/**
  ******************************************************************************
  *
  * @file    memory_manager.c
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
#include "memory_manager.h"
#include "tlsf.h"
#include "fmc.h"
#include "debug.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static tlsf_t tlsf;
static uint32_t cur_used;
static uint32_t max_used;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void mm_init(void)
{
    STD_FMC_SDRAM_Init();
    int ret = STD_FMC_SDRAM_Benchmark();
    if(0 != ret) {
        pr_fatal("SDRAM initialization failed, the error code is: %d", ret);
        return;
    }

    pr_user("SDRAM initialization successful!");

    tlsf = tlsf_create_with_pool((void *)SDRAM_BASE_ADDR, SDRAM_SIZE);
}

void mm_deinit(void)
{
    tlsf_destroy(tlsf);
    mm_init();
}

void * mm_alloc(size_t size)
{
    void * alloc = tlsf_malloc(tlsf, size);

    return alloc;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







