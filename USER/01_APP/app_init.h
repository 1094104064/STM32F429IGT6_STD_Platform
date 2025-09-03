/**
  ******************************************************************************
  *
  * @file    app_init.h
  * @author  Jamin
  * @brief   Header file of app_init module.
  *
  ******************************************************************************
  **/
  
#ifndef _APP_INIT_H
#define _APP_INIT_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void app_init_core(void);
void app_init_bsp(void);
void app_init_middleware(void);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_APP_INIT_H*/


/******************************* (END OF FILE) *********************************/

