/**
  ******************************************************************************
  *
  * @file    edf_ringbuf.c
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
#include "edf_ringbuf.h"
#include "edf_mem.h"
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
 * @brief This function initialize the ringbuffer.
 *
 * @param ringbuf The ringbuffer to initialize.
 * @param pool The pool of buf.
 * @param size The size of the pool.
 */
void edf_ringbuf_init(edf_ringbuf_t *ringbuf, void *pool, size_t size)
{
    EDF_ASSERT(ringbuf != EDF_NULL);
    EDF_ASSERT((pool != EDF_NULL) || (size == 0));

    ringbuf->read_index = 0;
    ringbuf->write_index = 0;
    ringbuf->read_mirror = 0;
    ringbuf->write_mirror = 0;
    ringbuf->size = size;
    ringbuf->buffer = pool;
}

/**
 * @brief This function allocate memory for the ringbuffer.
 *
 * @param ringbuf The ringbuffer to allocate.
 * @param size The size of the memory.
 *
 * @return MR_ERR_OK on success, otherwise an error code.
 */
int edf_ringbuf_allocate(edf_ringbuf_t *ringbuf, size_t size)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    if (size == edf_ringbuf_get_bufsz(ringbuf)) {
        edf_ringbuf_reset(ringbuf);
        return EDF_EOK;
    }

    /* Allocate new buffer */
    void *pool = edf_malloc(size);
    if ((pool == EDF_NULL) && (size != 0)) {
        return EDF_ENOMEM;
    }

    /* Free old buffer */
    if (ringbuf->size != 0) {
        edf_free(ringbuf->buffer);
    }
    edf_ringbuf_init(ringbuf, pool, size);
    return EDF_EOK;
}

/**
 * @brief This function free the ringbuffer.
 *
 * @param ringbuf The ringbuffer to free.
 */
void edf_ringbuf_free(edf_ringbuf_t *ringbuf)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    edf_free(ringbuf->buffer);
    edf_ringbuf_init(ringbuf, EDF_NULL, 0);
}

/**
 * @brief This function reset the ringbuffer.
 *
 * @param ringbuf The ringbuffer to reset.
 */
void edf_ringbuf_reset(edf_ringbuf_t *ringbuf)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    ringbuf->read_index = 0;
    ringbuf->write_index = 0;
    ringbuf->read_mirror = 0;
    ringbuf->write_mirror = 0;
}

/**
 * @brief This function get the buf size from the ringbuffer.
 *
 * @param ringbuf The ringbuffer to get the buf size.
 *
 * @return The buf size.
 */
size_t edf_ringbuf_get_data_size(edf_ringbuf_t *ringbuf)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    /* Empty or full according to the mirror flag */
    if (ringbuf->read_index == ringbuf->write_index) {
        if (ringbuf->read_mirror == ringbuf->write_mirror) {
            return 0;
        } else {
            return ringbuf->size;
        }
    }

    if (ringbuf->write_index > ringbuf->read_index) {
        return ringbuf->write_index - ringbuf->read_index;
    } else {
        return ringbuf->size - ringbuf->read_index + ringbuf->write_index;
    }
}

/**
 * @brief This function get the space size from the ringbuffer.
 *
 * @param ringbuf The ringbuffer to get the space size.
 *
 * @return The space size.
 */
size_t edf_ringbuf_get_space_size(edf_ringbuf_t *ringbuf)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    return ringbuf->size - edf_ringbuf_get_data_size(ringbuf);
}

/**
 * @brief This function get the buffer size from the ringbuffer.
 *
 * @param ringbuf The ringbuffer to get the buffer size.
 *
 * @return  The buffer size.
 */
size_t edf_ringbuf_get_bufsz(edf_ringbuf_t *ringbuf)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    return ringbuf->size;
}

/**
 * @brief This function pop the buf from the ringbuffer.
 *
 * @param ringbuf The ringbuffer to pop the buf.
 * @param data The buf to pop.
 *
 * @return The size of the actual pop.
 */
size_t edf_ringbuf_pop(edf_ringbuf_t *ringbuf, uint8_t *data)
{
    EDF_ASSERT(ringbuf != EDF_NULL);
    EDF_ASSERT(data != EDF_NULL);

    /* Get the buf size */
    if (edf_ringbuf_get_data_size(ringbuf) == 0) {
        return 0;
    }

    *data = ringbuf->buffer[ringbuf->read_index];

    if (ringbuf->read_index == ringbuf->size - 1) {
        ringbuf->read_mirror = ~ringbuf->read_mirror;
        ringbuf->read_index = 0;
    } else {
        ringbuf->read_index++;
    }
    return 1;
}

/**
 * @brief This function reads from the ringbuffer.
 *
 * @param ringbuf The ringbuffer to be read.
 * @param buffer The buf buffer to be read from the ringbuffer.
 * @param size The size of the read.
 *
 * @return The size of the actual read.
 */
size_t edf_ringbuf_read(edf_ringbuf_t *ringbuf, void *buffer, size_t size)
{
    uint8_t *read_buffer = (uint8_t *)buffer;

    EDF_ASSERT(ringbuf != EDF_NULL);
    EDF_ASSERT((buffer != EDF_NULL) || (size == 0));
    
    /* Get the buf size */
    size_t data_size = edf_ringbuf_get_data_size(ringbuf);
    if (data_size == 0) {
        return 0;
    }

    /* Adjust the number of bytes to read if it exceeds the available buf */
    if (size > data_size) {
        size = data_size;
    }

    /* Copy the buf from the ringbuf to the buffer */
    if ((ringbuf->size - ringbuf->read_index) > size) {
        memcpy(read_buffer, &ringbuf->buffer[ringbuf->read_index], size);
        ringbuf->read_index += size;
        return size;
    }

    memcpy(read_buffer, &ringbuf->buffer[ringbuf->read_index], ringbuf->size - ringbuf->read_index);
    memcpy(&read_buffer[ringbuf->size - ringbuf->read_index],
           &ringbuf->buffer[0],
           size - (ringbuf->size - ringbuf->read_index));

    ringbuf->read_mirror = ~ringbuf->read_mirror;
    ringbuf->read_index = size - (ringbuf->size - ringbuf->read_index);
    return size;
}

/**
 * @brief This function push the buf to the ringbuffer.
 *
 * @param ringbuf The ringbuffer to be pushed.
 * @param data The buf to be pushed.
 *
 * @return The size of the actual write.
 */
size_t edf_ringbuf_push(edf_ringbuf_t *ringbuf, uint8_t data)
{
    EDF_ASSERT(ringbuf != EDF_NULL);

    /* Get the space size */
    if (edf_ringbuf_get_space_size(ringbuf) == 0) {
        return 0;
    }

    ringbuf->buffer[ringbuf->write_index] = data;

    if (ringbuf->write_index == ringbuf->size - 1) {
        ringbuf->write_mirror = ~ringbuf->write_mirror;
        ringbuf->write_index = 0;
    } else {
        ringbuf->write_index++;
    }
    return 1;
}

/**
 * @brief This function force to push the buf to the ringbuffer.
 *
 * @param ringbuf The ringbuffer to be pushed.
 * @param data The buf to be pushed.
 *
 * @return The size of the actual write.
 */
size_t edf_ringbuf_push_force(edf_ringbuf_t *ringbuf, uint8_t data)
{
    int state = 0;

    EDF_ASSERT(ringbuf != EDF_NULL);

    /* Get the buffer size */
    if (edf_ringbuf_get_bufsz(ringbuf) == 0) {
        return 0;
    }

    /* Get the space size */
    if (edf_ringbuf_get_space_size(ringbuf) == 0) {
        state = 1;
    }

    ringbuf->buffer[ringbuf->write_index] = data;

    if (ringbuf->write_index == ringbuf->size - 1) {
        ringbuf->write_mirror = ~ringbuf->write_mirror;
        ringbuf->write_index = 0;
        if (state == 1) {
            ringbuf->read_mirror = ~ringbuf->read_mirror;
            ringbuf->read_index = ringbuf->write_index;
        }
    } else {
        ringbuf->write_index++;
        if (state == 1) {
            ringbuf->read_index = ringbuf->write_index;
        }
    }
    return 1;
}

/**
 * @brief This function write the ringbuffer.
 *
 * @param ringbuf The ringbuffer to be written.
 * @param buffer The buf buffer to be written to ringbuffer.
 * @param size The size of write.
 *
 * @return The size of the actual write.
 */
size_t edf_ringbuf_write(edf_ringbuf_t *ringbuf, const void *buffer, size_t size)
{
    uint8_t *write_buffer = (uint8_t *)buffer;

    EDF_ASSERT(ringbuf != EDF_NULL);
    EDF_ASSERT((buffer != EDF_NULL) || (size == 0));

    /* Get the space size */
    size_t space_size = edf_ringbuf_get_space_size(ringbuf);
    if (space_size == 0) {
        return 0;
    }

    /* Adjust the number of bytes to write if it exceeds the available buf */
    if (size > space_size) {
        size = space_size;
    }

    /* Copy the buf from the buffer to the ringbuf */
    if ((ringbuf->size - ringbuf->write_index) > size) {
        memcpy(&ringbuf->buffer[ringbuf->write_index], write_buffer, size);
        ringbuf->write_index += size;

        return size;
    }

    memcpy(&ringbuf->buffer[ringbuf->write_index],
           write_buffer,
           ringbuf->size - ringbuf->write_index);
    memcpy(&ringbuf->buffer[0],
           &write_buffer[ringbuf->size - ringbuf->write_index],
           size - (ringbuf->size - ringbuf->write_index));

    ringbuf->write_mirror = ~ringbuf->write_mirror;
    ringbuf->write_index = size - (ringbuf->size - ringbuf->write_index);
    return size;
}

/**
 * @brief This function force write the ringbuffer.
 *
 * @param ringbuf The ringbuffer to be written.
 * @param buffer The buf buffer to be written to ringbuffer.
 * @param size The size of write.
 *
 * @return The size of the actual write.
 */
size_t edf_ringbuf_write_force(edf_ringbuf_t *ringbuf, const void *buffer, size_t size)
{
    uint8_t *write_buffer = (uint8_t *)buffer;
    
    EDF_ASSERT(ringbuf != EDF_NULL);
    EDF_ASSERT((buffer != EDF_NULL) || (size == 0));

    if ((edf_ringbuf_get_bufsz(ringbuf) == 0) || (size == 0)) {
        return 0;
    }

    /* Get the space size */
    size_t space_size = edf_ringbuf_get_space_size(ringbuf);

    /* If the buf exceeds the buffer space_size, the front buf is discarded */
    if (size > ringbuf->size) {
        write_buffer = &write_buffer[size - ringbuf->size];
        size = ringbuf->size;
    }

    /* Copy the buf from the buffer to the ringbuf */
    if ((ringbuf->size - ringbuf->write_index) > size) {
        memcpy(&ringbuf->buffer[ringbuf->write_index], write_buffer, size);
        ringbuf->write_index += size;
        if (size > space_size) {
            ringbuf->read_index = ringbuf->write_index;
        }
        return size;
    }

    memcpy(&ringbuf->buffer[ringbuf->write_index],
           write_buffer,
           ringbuf->size - ringbuf->write_index);
    memcpy(&ringbuf->buffer[0],
           &write_buffer[ringbuf->size - ringbuf->write_index],
           size - (ringbuf->size - ringbuf->write_index));

    ringbuf->write_mirror = ~ringbuf->write_mirror;
    ringbuf->write_index = size - (ringbuf->size - ringbuf->write_index);

    if (size > space_size) {
        if (ringbuf->write_index <= ringbuf->read_index) {
            ringbuf->read_mirror = ~ringbuf->read_mirror;
        }

        ringbuf->read_index = ringbuf->write_index;
    }
    return size;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/



/******************************* (END OF FILE) *********************************/



 







