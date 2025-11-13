/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_touchpad.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_touchpad module.
  *
  ******************************************************************************
  **/

#ifndef _BSP_WRAPPER_TOUCHPAD_H
#define _BSP_WRAPPER_TOUCHPAD_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
/*********************
 *      DEFINES
 *********************/
#define TOUCHPAD_MAX_NUM       2
#define TOUCHPAD_NAME_MAX_LEN  16
/**********************
 *      TYPEDEFS
 **********************/
typedef struct touchpad_object touchpad_obj_t;
typedef struct touchpad_ops touchpad_ops_t;
typedef struct touchpad_ctx touchpad_ctx_t;

struct touchpad_ops
{
    int  (* pf_init)            (void);
    void (* pf_reset)           (void);
    void (* pf_scan)            (void);
    bool (* pf_is_pressed)      (void);
    void (* pf_get_coordinates) (uint16_t * x, uint16_t * y, uint8_t num);
};

struct touchpad_ctx
{
    uint8_t         idx;
    void *          user_data;
    char            name[TOUCHPAD_NAME_MAX_LEN];
    bool            is_initialized;
};

struct touchpad_object {
    const touchpad_ops_t * ops;
    touchpad_ctx_t         ctx;
};

struct touchpad_wrapper 
{
    touchpad_obj_t *    (* obj_create)          (const touchpad_ops_t * ops, const char * const name, void * const user_data);
    void                (* obj_delete)          (const char * const name);
    touchpad_obj_t *    (* find)                (const char * const name);

    bool                (* init)                (touchpad_obj_t * obj);
    void                (* reset)               (touchpad_obj_t * obj);
    void                (* scan)                (touchpad_obj_t * obj);
    bool                (* is_pressed)          (touchpad_obj_t * obj);
    void                (* get_coordinates)     (touchpad_obj_t * obj, uint16_t * x, uint16_t * y, uint8_t num);
};




/**********************
*  GLOBAL PROTOTYPES
 **********************/
touchpad_obj_t *    bsp_wrapper_touchpad_obj_create            (const touchpad_ops_t * ops, const char * const name, void * const user_data);
void                bsp_wrapper_touchpad_obj_delete            (const char * const name);
touchpad_obj_t *    bsp_wrapper_touchpad_find                  (const char * const name);
bool                bsp_wrapper_touchpad_init                  (touchpad_obj_t * obj);
void                bsp_wrapper_touchpad_reset                 (touchpad_obj_t * obj);
void                bsp_wrapper_touchpad_scan                  (touchpad_obj_t * obj);
bool                bsp_wrapper_touchpad_is_pressed            (touchpad_obj_t * obj);
void                bsp_wrapper_touchpad_get_coordinates       (touchpad_obj_t * obj, uint16_t * x, uint16_t * y, uint8_t num);
/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_TOUCHPAD_H*/


/******************************* (END OF FILE) *********************************/

