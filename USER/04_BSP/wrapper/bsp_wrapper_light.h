/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_light.h
  * @author  Jamin
  * @brief   Header file of bsp_wrapper_light module.
  *
  ******************************************************************************
  **/
  
#ifndef _BSP_WRAPPER_LIGHT_H
#define _BSP_WRAPPER_LIGHT_H


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
#define LIGHT_MAX_NUM       2
#define LIGHT_NAME_MAX_LEN  16
/**********************
 *      TYPEDEFS
 **********************/
typedef struct light_object light_obj_t;
typedef struct light_ops light_ops_t;
typedef struct light_ctx light_ctx_t;

struct light_ops
{
    int  (* pf_init)(void);
    void (* pf_on)  (void);
    void (* pf_off) (void);
};

struct light_ctx
{
    uint8_t         idx;
    void *          user_data;
    char            name[LIGHT_NAME_MAX_LEN];
    bool            is_initialized;
};

struct light_object {
    const light_ops_t * ops;
    light_ctx_t         ctx;
};



struct light_wrapper
{
    light_obj_t *   (* obj_create)  (const light_ops_t * ops, const char * const name, void * const user_data);
    void            (* obj_delete)  (const char * const name);
    light_obj_t *   (* find)        (const char * const name);
    
    bool            (* init)        (light_obj_t * obj);
    void            (* on)          (light_obj_t * obj);
    void            (* off)         (light_obj_t * obj);
};

extern const struct light_wrapper wrp_light;

/**********************
*  GLOBAL PROTOTYPES
 **********************/
light_obj_t *   bsp_wrapper_light_obj_create    (const light_ops_t * ops, const char * const name, void * const user_data);
void            bsp_wrapper_light_obj_delete    (const char * const name);
light_obj_t *   bsp_wrapper_light_find          (const char * const name);
bool            bsp_wrapper_light_init          (light_obj_t * obj);
void            bsp_wrapper_light_on            (light_obj_t * obj);
void            bsp_wrapper_light_off           (light_obj_t * obj);
/**********************
 *      MACROS
 **********************/




#ifdef __cplusplus
}
#endif

#endif /*_BSP_WRAPPER_LIGHT_H*/


/******************************* (END OF FILE) *********************************/

