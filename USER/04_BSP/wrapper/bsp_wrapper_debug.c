/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_debug.c
  * @author  Jamin
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
#include "bsp_wrapper_debug.h"
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
static struct debug_wrapper debug_wrappers[DEBUG_MAX_NUM];
static int8_t debug_wrapper_count = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void bsp_wrapper_debug_link(struct debug_wrapper * self)
{
    debug_wrapper_count++;
    if (debug_wrapper_count >= DEBUG_MAX_NUM) {
        debug_wrapper_count = DEBUG_MAX_NUM - 1;
    }

    int8_t idx = debug_wrapper_count;

    memset(&debug_wrappers[idx], 0, sizeof(debug_wrappers[idx]));

    debug_wrappers[idx].idx = self->idx;
    debug_wrappers[idx].user_data = self->user_data;
    debug_wrappers[idx].init = self->init;
    debug_wrappers[idx].putc = self->putc;
}

void bsp_wrapper_debug_init()
{
    struct debug_wrapper * self = &debug_wrappers[debug_wrapper_count];

    if(self->init) {
        self->init(self);
    }
}

void bsp_wrapper_debug_deinit()
{
    struct debug_wrapper * self = &debug_wrappers[debug_wrapper_count];

    memset(self, 0, sizeof(struct debug_wrapper));
}

void bsp_wrapper_debug_putc(char c)
{
    struct debug_wrapper * self = &debug_wrappers[debug_wrapper_count];

    if(self->putc) {
        self->putc(self, c);
    }
}

void bsp_wrapper_debug_puts(const char * const fmt, ...)
{
    char str[256];
    uint16_t len;
    char * ptr = &str[0];
    
    va_list args;
    
    /* print to the buffer */
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    /* send the data */
    len = strlen((char *)str);
    
    for(uint32_t i = 0; i < len; i++) {
       
        /*!< Send single byte */
        bsp_wrapper_debug_putc((uint8_t)*ptr & 0xFFU);	
        ptr++;
       
    }
}


/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
