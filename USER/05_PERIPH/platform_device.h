/**
  ******************************************************************************
  *
  * @file    platform_device.h
  * @author  
  * @brief   Header file of platform_device module.
  *
  ******************************************************************************
  **/
  
#ifndef _PLATFORM_DEVICE_H
#define _PLATFORM_DEVICE_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <string.h>
#include "dev_conf.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct platform_device_list plaf_dev_list_t;
typedef struct platform_device_drv plaf_dev_drv_t;
typedef struct platform_device_ops plaf_dev_ops_t;
typedef struct platform_device_call plaf_dev_call_t;
typedef struct platform_device plaf_dev_t;
typedef struct platform_device_desc plaf_dev_desc_t;

/**
 * @brief List structure.
 */
struct platform_device_list
{
    struct platform_device_list * next;                                           /**< Point to next node */
    struct platform_device_list * prev;                                           /**< Point to prev node */
};

/**
 * @brief Driver structure.
 */
struct platform_device_drv
{
    void * ops;                                                      /**< Operations */
    void * data;                                                     /**< Data */
};

/**
 * @brief Device operations structure.
 */
struct platform_device_ops
{
    int (* open)    (plaf_dev_t * dev);
    int (* close)   (plaf_dev_t * dev);
    int (* read)    (plaf_dev_t * dev, void * buf, size_t count);
    int (* write)   (plaf_dev_t * dev, const void * buf, size_t count);
    int (* ioctl)   (plaf_dev_t * dev, int cmd, void * args);
    int (* isr)     (plaf_dev_t * dev, int event, void * args);
};

/**
 * @brief Device callback structure.
 */
struct platform_device_call
{
    plaf_dev_list_t list;                                            /**< List of callbacks */

    void (* fn)(int desc, void * args);                               /**< Callback function */
};


/**
 * @brief Device structure.
 */
struct platform_device {
    uint32_t magic;                                                 /**< Magic number */
#ifndef DEV_NAME_LEN
#define DEV_NAME_LEN             (8)
#endif 
    char name[DEV_NAME_LEN];                                        /**< Name */
    uint32_t type;                                                  /**< Type */
    int flags;                                                      /**< Flags */
    void * parent;                                                   /**< Parent device */
    plaf_dev_list_t list;                                            /**< Same level device list */
    plaf_dev_list_t clist;                                           /**< Child device list */

    size_t ref_count;                                               /**< Reference count */
#ifdef DEV_USING_RDWR_CTL
    volatile uint32_t lock;                                         /**< Lock flags */
#endif /* MR_USING_RDWR_CTL */
    int sync;                                                       /**< Sync flag */
    int position;                                                   /**< Position */

    plaf_dev_list_t rd_call_list;                                    /**< Read callback list */
    plaf_dev_list_t wr_call_list;                                    /**< Write callback list */

    const plaf_dev_ops_t * ops;                                   /**< Operations */
    const plaf_dev_drv_t * drv;                                       /**< Driver */
}; 

/**
 * @brief Device descriptor structure.
 */
struct platform_device_desc
{
    plaf_dev_t * dev;                                             /**< Device */
    int flags;                                                      /**< Open flags */
    int position;                                                   /**< Current position */
    plaf_dev_call_t rd_call;                                     /**< Read callback */
    plaf_dev_call_t wr_call;                                     /**< Write callback */
};

/**********************
*  GLOBAL PROTOTYPES
 **********************/
int platform_device_register(plaf_dev_t * dev,
                             const char * path,
                             int type,
                             int flags,
                             plaf_dev_ops_t * ops,
                             plaf_dev_drv_t * drv);
int platform_device_isr     (plaf_dev_t * dev, int event, void * args);
int platform_device_open    (const char * path, int flags);
int platform_device_close   (int desc);
int platform_device_read    (int desc, void * buf, size_t count);
int platform_device_write   (int desc, const void * buf, size_t count);
int platform_device_ioctl   (int desc, int cmd, void * args);
/**
* @brief This function initialize a double list.
*
* @param list The list to initialize.
*/
static inline void dev_list_init(plaf_dev_list_t * list)
{
    list->next = list;
    list->prev = list;
}

/**
 * @brief This function insert a node before a node in a double list.
 *
 * @param list The list to insert before.
 * @param node The node to insert.
 */
static inline void dev_list_insert_before(plaf_dev_list_t * list, plaf_dev_list_t * node)
{
    list->prev->next = node;
    node->prev = list->prev;
    list->prev = node;
    node->next = list;
}

/**
 * @brief This function remove a node from a double list.
 *
 * @param node The node to remove.
 */
static inline void dev_list_remove(plaf_dev_list_t * node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = node->prev = node;
}

/**********************
 *      MACROS
 **********************/





#ifdef __cplusplus
}
#endif

#endif /*_PLATFORM_DEVICE_H*/


/******************************* (END OF FILE) *********************************/


