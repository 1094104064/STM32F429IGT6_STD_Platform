/**
  ******************************************************************************
  *
  * @file    bsp_driver_delay.h
  * @author  Jamin
  * @brief   Header file of bsp_driver_delay module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_DRIVER_DELAY_H
#define _BSP_DRIVER_DELAY_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define DELAY_DEBUG_ENABLE 0    

#if DELAY_DEBUG_ENABLE

    #define delay_dbg(fmt, ...)         printf("%s [%d] : " fmt "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

    #define delay_assert_null(param)                                                            \
            do {                                                                                \
                if(param == NULL) { delay_dbg("NULL pointer: %s", #param); while(1); }          \
            } while (0)    
#else
    #define delay_dbg(fmt, ...)             do {} while (0)
    #define delay_assert_null(param)        do {} while (0)
#endif


/**********************
 *      TYPEDEFS
 **********************/

struct delay_oper {
    void (* pf_delay_us)(uint32_t us);
    void (* pf_delay_ms)(uint32_t ms);
    void (* pf_delay_sec)(uint32_t sec);
};
    
struct delay_driver {
    struct delay_oper * oper;
            
    bool (* pf_init)(struct delay_driver * self);
    void (* pf_delay_us)(struct delay_driver * self, uint32_t us);
    void (* pf_delay_ms)(struct delay_driver * self, uint32_t ms);
    void (* pf_delay_sec)(struct delay_driver * self, uint32_t sec);
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
void bsp_driver_delay_link(struct delay_driver * self, struct delay_oper * oper);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_DRIVER_DELAY_H*/


/******************************* (END OF FILE) *********************************/

