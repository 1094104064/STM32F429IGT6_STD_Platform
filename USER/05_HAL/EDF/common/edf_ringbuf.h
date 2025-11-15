/**
  ******************************************************************************
  *
  * @file    edf_ringbuf.h
  * @author  
  * @brief   Header file of edf_ringbuf module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_RINGBUF_H
#define _EDF_RINGBUF_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct edf_ringbuf edf_ringbuf_t;
/**
 * @brief Ring buffer structure.
 */
struct edf_ringbuf
{
    uint8_t *buffer;                                                /**< Buffer pool */
    size_t size;                                                    /**< Buffer pool size */
    uint32_t read_mirror: 1;                                        /**< Read mirror flag */
    uint32_t write_mirror: 1;                                       /**< Write mirror flag */
    uint32_t _reserved: 30;                                         /**< Reserved */
    size_t read_index;                                              /**< Read index */
    size_t write_index;                                             /**< Write index */
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void edf_ringbuf_init(edf_ringbuf_t *ringbuf, void *pool, size_t size);
int edf_ringbuf_allocate(edf_ringbuf_t *ringbuf, size_t size);
void edf_ringbuf_free(edf_ringbuf_t *ringbuf);
void edf_ringbuf_reset(edf_ringbuf_t *ringbuf);
size_t edf_ringbuf_get_data_size(edf_ringbuf_t *ringbuf);
size_t edf_ringbuf_get_space_size(edf_ringbuf_t *ringbuf);
size_t edf_ringbuf_get_bufsz(edf_ringbuf_t *ringbuf);
size_t edf_ringbuf_pop(edf_ringbuf_t *ringbuf, uint8_t *data);
size_t edf_ringbuf_read(edf_ringbuf_t *ringbuf, void *buffer, size_t size);
size_t edf_ringbuf_push(edf_ringbuf_t *ringbuf, uint8_t data);
size_t edf_ringbuf_push_force(edf_ringbuf_t *ringbuf, uint8_t data);
size_t edf_ringbuf_write(edf_ringbuf_t *ringbuf, const void *buffer, size_t size);
size_t edf_ringbuf_write_force(edf_ringbuf_t *ringbuf, const void *buffer, size_t size);

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_EDF_RINGBUF_H*/


/******************************* (END OF FILE) *********************************/


