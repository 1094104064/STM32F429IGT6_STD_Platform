/**
  ******************************************************************************
  *
  * @file    lw_ringbuf.c
  * @author  
  * @brief   source file of ring buffer module.
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
#include "lw_ringbuf.h"
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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
  * @brief  Initialize the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @param  buf: Pointer to the buffer memory.
  * @param  buf_size: Size of the buffer memory in bytes.
  * @retval True if initialization is successful, false otherwise.
  */
bool lw_ringbuf_init(lw_ringbuf_t * self, char * buf, size_t buf_size)
{
    if(buf_size & (buf_size - 1))
        return false;

    if(self == NULL || buf == NULL)
        return false;

    self->buf       = buf;
    self->mask      = buf_size - 1;
    self->read_pos  = 0;
    self->write_pos = 0;

    return true;
}

/**
  * @brief  Reset the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @retval None
  */
void lw_ringbuf_reset(lw_ringbuf_t * self)
{
    if (!self) return;
    self->read_pos = 0;
    self->write_pos = 0;
}

/**
  * @brief  Write a byte to the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @param  byte: Byte to be written.
  * @param  mode: Write mode (overwrite or try).
  * @retval True if byte is successfully written, false otherwise.
  */
bool lw_ringbuf_write_byte(lw_ringbuf_t * self, 
                           char byte, 
                           lw_ringbuf_write_mode_t mode)
{
    /* Is buffer full? */
    bool is_full = lw_ringbuf_is_full(self);

    if(is_full) {
        /* Is going to overwrite the oldest byte */
        /* Increase tail index */
        if(mode == LW_RINGBUF_WRITE_OVERWRITE) {
            self->read_pos = ((self->read_pos + 1) & self->mask);
        }
        /* Do Nothing */
        else {
            return false;
        }
    }
    
    self->buf[self->write_pos] = byte;
    self->write_pos = ((self->write_pos + 1) & self->mask);

    return true;
}

/**
  * @brief  Write data to the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @param  data: Pointer to the data to be written.
  * @param  len: Number of bytes to write.
  * @param  mode: Write mode (overwrite, try, or fill part).
  * @retval Number of bytes successfully written.
  */
size_t lw_ringbuf_write(lw_ringbuf_t * self, 
                        const char * data, 
                        size_t len,
                        lw_ringbuf_write_mode_t mode)
{
    if (!self || !data || len == 0) return 0;
    size_t buf_size = self->mask + 1;
    size_t free_space = lw_ringbuf_get_free(self);
    size_t write_len = 0;

    switch (mode) {
    case LW_RINGBUF_WRITE_OVERWRITE:
    default:
        /* 总是写入 len 个字节，必要时覆盖旧数据 */ 
        write_len = len;
        if (write_len > buf_size) {
            /* 只保留最后 buf_size 个字节 */
            data += (write_len - buf_size);
            write_len = buf_size;
        }
        /* 如果写入长度大于空闲空间，需要移动 read_pos */
        if (write_len > free_space) {
            size_t overwrite_count = write_len - free_space;
            self->read_pos = (self->read_pos + overwrite_count) & self->mask;
        }
        break;
    case LW_RINGBUF_WRITE_TRY:
        /* 只有空闲空间足够才写入 */
        if (len > free_space) return 0;
        write_len = len;
        break;
    case LW_RINGBUF_WRITE_FILL_PART:
        /* 能写多少写多少 */
        write_len = (len > free_space) ? free_space : len;
        break;
    }

    // 计算写入位置和剩余空间
    size_t first_part = LW_RINGBUF_MIN(write_len, buf_size - self->write_pos);
    LW_RINGBUF_MEMCPY(&self->buf[self->write_pos], data, first_part);
    if (write_len > first_part) {
        LW_RINGBUF_MEMCPY(&self->buf[0], data + first_part, write_len - first_part);
    }
    self->write_pos = (self->write_pos + write_len) & self->mask;
    return write_len;
}

/**
  * @brief  Read a byte from the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @param  byte: Pointer to store the read byte.
  * @param  mode: Read mode (read all or part).
  * @retval True if byte is successfully read, false otherwise.
  */
bool lw_ringbuf_read_byte(lw_ringbuf_t * self, char * byte)
{
    /* Is buffer empty? */
    bool is_empty = lw_ringbuf_is_empty(self);

    if(is_empty) {
        /* No items */
        return false;
    }

    *byte = self->buf[self->read_pos];
    self->read_pos = ((self->read_pos + 1) & self->mask);
    return true;
}

/**
  * @brief  Read data from the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @param  data: Pointer to store the read data.
  * @param  len: Number of bytes to read.
  * @param  mode: Read mode (read all or part).
  * @retval Number of bytes successfully read.
  */
size_t lw_ringbuf_read(lw_ringbuf_t * self, 
                       char * data, 
                       size_t len,
                       lw_ringbuf_read_mode_t mode)
{
    if (!self || !data || len == 0) return 0;
    size_t buf_size = self->mask + 1;
    size_t used_size = lw_ringbuf_get_used(self);
    size_t read_len = 0;

    switch (mode) {
    case LW_RINGBUF_READ_ALL:
        /* 只在数据足够时读取全部，否则不读 */
        if (len > used_size) return 0;
        read_len = len;
        break;
    case LW_RINGBUF_READ_PART:
    default:
        /* 能读多少读多少 */
        read_len = (len > used_size) ? used_size : len;
        break;
    }

    /* 分段 memcpy 提高效率 */ 
    size_t first_part = LW_RINGBUF_MIN(read_len, buf_size - self->read_pos);
    LW_RINGBUF_MEMCPY(data, &self->buf[self->read_pos], first_part);
    if (read_len > first_part) {
        LW_RINGBUF_MEMCPY(data + first_part, &self->buf[0], read_len - first_part);
    }
    self->read_pos = (self->read_pos + read_len) & self->mask;
    return read_len;
}

/**
  * @brief  Peek data from the ring buffer without advancing read_pos.
  * @param  self: Pointer to the ring buffer structure.
  * @param  skip_cnt: Number of bytes to skip from the read_pos.
  * @param  data: Pointer to store the peeked data.
  * @param  len: Number of bytes to peek.
  * @retval Number of bytes successfully peeked.
  */
size_t lw_ringbuf_peek(lw_ringbuf_t * self,
                       size_t skip_cnt,
                       char * data,
                       size_t len)
{
    if (!self || !data || len == 0) return 0;
    size_t buf_size = self->mask + 1;
    size_t used_size = lw_ringbuf_get_used(self);
    if (skip_cnt >= used_size) return 0;

    size_t r_ptr = (self->read_pos + skip_cnt) & self->mask;

    size_t available = used_size - skip_cnt;
    size_t peek_len = (len > available) ? available : len;
    if (peek_len == 0) return 0;

    size_t first_part = LW_RINGBUF_MIN(peek_len, buf_size - r_ptr);
    LW_RINGBUF_MEMCPY(data, &self->buf[r_ptr], first_part);
    if (peek_len > first_part) {
        LW_RINGBUF_MEMCPY(data + first_part, &self->buf[0], peek_len - first_part);
    }
    return peek_len;
}

/**
 * @brief 标记部分数据为已读（移动 read_pos，不做实际读取）
 * @param self 环形缓冲区指针
 * @param len 要标记为已读的数据长度
 * @return 实际标记为已读的字节数
 */
size_t lw_ringbuf_mark_read(lw_ringbuf_t * self, size_t len)
{
    if (!self || len == 0) return 0;
    size_t used_size = lw_ringbuf_get_used(self);
    size_t mark_len = (len > used_size) ? used_size : len;
    self->read_pos = (self->read_pos + mark_len) & self->mask;
    return mark_len;
}

/**
 * @brief 手动移动写指针（标记部分数据为已写）
 * @param self 环形缓冲区指针
 * @param len 要标记为已写的数据长度
 * @return 实际标记为已写的字节数
 */
size_t lw_ringbuf_mark_write(lw_ringbuf_t * self, size_t len)
{
    if (!self || len == 0) return 0;
    size_t free_size = lw_ringbuf_get_free(self);
    size_t mark_len = (len > free_size) ? free_size : len;
    self->write_pos = (self->write_pos + mark_len) & self->mask;
    return mark_len;
}

/**
 * @brief 在环形缓冲区查找指定字节序列
 * @param self 环形缓冲区指针
 * @param needle 需要查找的字节序列指针
 * @param len 需要查找的字节序列长度
 * @param start_offset 起始查找偏移
 * @param found_idx 找到时返回偏移
 * @return 是否找到（1找到，0未找到）
 */
bool lw_ringbuf_find(const lw_ringbuf_t * self, const char * needle, size_t len, size_t start_offset, size_t * found_idx)
{
    if (!self || !needle || len == 0 || !found_idx) return 0;
    *found_idx = 0;
    size_t used_size = lw_ringbuf_get_used((lw_ringbuf_t *)self);
    if (used_size < (len + start_offset)) return 0;

    size_t max_x = used_size - len;
    size_t r_ptr_base = self->read_pos;
    bool found = false;

    for (size_t skip_x = start_offset; !found && skip_x <= max_x; ++skip_x) {
        found = true;
        size_t r_ptr = (r_ptr_base + skip_x) & self->mask;
        for (size_t idx = 0; idx < len; ++idx) {
            if (self->buf[r_ptr] != needle[idx]) {
                found = false;
                break;
            }
            r_ptr = (r_ptr + 1) & self->mask;
        }
        if (found) {
            *found_idx = skip_x;
        }
    }
    return found;
}

/**
  * @brief  Get the number of free bytes in the ring buffer.
  * @param  self: Pointer to the ring buffer structure.
  * @retval Number of free bytes in the ring buffer.
  */
size_t lw_ringbuf_get_free(lw_ringbuf_t * self)
{
    size_t buf_size = self->mask + 1;
    size_t used_size = lw_ringbuf_get_used(self);
    return buf_size - used_size;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/



/******************************* (END OF FILE) *********************************/



 







