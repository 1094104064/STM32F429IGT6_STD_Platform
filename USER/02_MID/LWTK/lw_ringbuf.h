/**
  ******************************************************************************
  *
  * @file    lw_ringbuf.h
  * @author  
  * @brief   Header file of lw_ringbuf module.
  *
  ******************************************************************************
  **/
  
#ifndef _LW_RINGBUF_H
#define _LW_RINGBUF_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/
#define LW_RINGBUF_MIN(x, y) ((x) < (y) ? (x) : (y))
#define LW_RINGBUF_MAX(x, y) ((x) > (y) ? (x) : (y))

#define LW_RINGBUF_MEMCPY memcpy
/**********************
 *      TYPEDEFS
 **********************/
typedef struct lw_ringbuf lw_ringbuf_t;

struct lw_ringbuf
{
    char * buf;
    size_t mask;
    size_t read_pos;
    size_t write_pos;
};

typedef enum
{
    LW_RINGBUF_READ_ALL,         // 读取所有数据
    LW_RINGBUF_READ_PART,        // 只能读取部分数据，剩余的就放弃
} lw_ringbuf_read_mode_t;

typedef enum
{
    LW_RINGBUF_WRITE_OVERWRITE,   // 覆盖写入（写不下就覆盖旧数据）
    LW_RINGBUF_WRITE_TRY,         // 有剩余空间才写入，否则不写入
    LW_RINGBUF_WRITE_FILL_PART    // 能写多少写多少，写满后剩余的就放弃
} lw_ringbuf_write_mode_t;

/**********************
*  GLOBAL PROTOTYPES
 **********************/

static inline bool lw_ringbuf_is_empty(lw_ringbuf_t * self)
{
    return (self->read_pos == self->write_pos);
}

static inline bool lw_ringbuf_is_full(lw_ringbuf_t * self)
{
    return ((self->write_pos - self->read_pos) & self->mask) == self->mask;
}

static inline size_t lw_ringbuf_get_used(lw_ringbuf_t * self)
{
    return (self->write_pos - self->read_pos) & self->mask;
}

bool lw_ringbuf_init(lw_ringbuf_t * self, char * buf, size_t buf_size);
void lw_ringbuf_reset(lw_ringbuf_t * self);
bool lw_ringbuf_write_byte(lw_ringbuf_t * self, 
                           char byte, 
                           lw_ringbuf_write_mode_t mode);
size_t lw_ringbuf_write(lw_ringbuf_t * self, 
                        const char * data, 
                        size_t len,
                        lw_ringbuf_write_mode_t mode);
bool lw_ringbuf_read_byte(lw_ringbuf_t * self, char * byte);
size_t lw_ringbuf_read(lw_ringbuf_t * self, 
                         char * data, 
                         size_t len,
                         lw_ringbuf_read_mode_t mode);
size_t lw_ringbuf_peek(lw_ringbuf_t * self,
                         size_t skip_cnt,
                         char * data,
                         size_t len);
size_t lw_ringbuf_mark_read(lw_ringbuf_t * self, size_t len);
size_t lw_ringbuf_mark_write(lw_ringbuf_t * self, size_t len);
bool lw_ringbuf_find(const lw_ringbuf_t * self, const char * needle, size_t len, size_t start_offset, size_t * found_idx);
size_t lw_ringbuf_get_free(lw_ringbuf_t * self);

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_LW_RINGBUF_H*/


/******************************* (END OF FILE) *********************************/


