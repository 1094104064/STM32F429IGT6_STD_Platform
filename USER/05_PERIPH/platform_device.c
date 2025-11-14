/**
  ******************************************************************************
  *
  * @file    platform_device.c
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
#include "platform_device.h"
#include "dev_service.h"
#include "dev_conf.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/
#ifndef DEV_DESC_NUM
#define DEV_DESC_NUM                 (32)
#endif /* DEV_DESC_NUM */
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static inline plaf_dev_t * dev_find(const char * path);
static inline int dev_register(plaf_dev_t * dev, const char * path);
static int dev_open(plaf_dev_t * dev, int flags);
static int dev_close(plaf_dev_t * dev);
static inline int dev_read(plaf_dev_t * dev, int position, int sync, void * buf, size_t count);
static inline int dev_write(plaf_dev_t *dev,
                            int position,
                            int sync,
                            const void *buf,
                            size_t count);
static inline int dev_ioctl(plaf_dev_t * dev, int position, int sync, int cmd, void * args);

static inline void desc_free(int desc);
static inline int desc_allocate(const char * path);
/**********************
 *  STATIC VARIABLES
 **********************/
static plaf_dev_t root_dev = {
    .name = "dev", 
    .type = DEV_TYPE_ROOT, 
    .parent = DEV_NULL, 
    .list = DEV_LIST_INIT(root_dev.list), 
    .clist = DEV_LIST_INIT(root_dev.clist)
}; 


static plaf_dev_desc_t desc_map[DEV_DESC_NUM] = {0};          /**< Device descriptor map */
#define DESC_OF(desc)                   (desc_map[(desc)])          /**< Descriptor of the device */
#define DESC_IS_VALID(desc)             (((desc) >= 0 && (desc) < DEV_DESC_NUM) && ((desc_map[desc].dev) != DEV_NULL))
#define DESC_CHECK(desc)                if (DESC_IS_VALID(desc) == DEV_FALSE) { return DEV_EINVAL; }
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

/**
 * @brief This function register a device.
 *
 * @param dev The device.
 * @param path The path of the device.
 * @param type The type of the device.
 * @param flags The support flags of the device.
 * @param ops The operations of the device.
 * @param drv The driver of the device.
 *
 * @return 0 on success, otherwise an error code.
 *
 * @retval -3 a node in the path is not found.
 * @retval -5 name is already exists.
 * @retval -7 path is invalid.
 */
int platform_device_register(plaf_dev_t * dev,
                             const char * path,
                             int type,
                             int flags,
                             plaf_dev_ops_t * ops,
                             plaf_dev_drv_t * drv)
{
    DEV_ASSERT(dev != DEV_NULL);
    DEV_ASSERT(dev->magic != DEV_MAGIC_NUMBER);
    DEV_ASSERT(path != DEV_NULL);
    DEV_ASSERT(type != DEV_TYPE_ROOT);
    DEV_ASSERT(flags >= DEV_O_CLOSED);
    DEV_ASSERT(ops != DEV_NULL);
    DEV_ASSERT((ops->read != DEV_NULL) || (DEV_BIT_IS_SET(flags, DEV_O_RDONLY) == DEV_DISABLE));
    DEV_ASSERT((ops->write != DEV_NULL) || (DEV_BIT_IS_SET(flags, DEV_O_WRONLY) == DEV_DISABLE));

    /* Initialize the fields */
    dev->magic = 0;
    memset(dev->name, '\0', DEV_NAME_LEN);
    dev->type = type;
    dev->flags = flags;
    dev->parent = DEV_NULL;
    dev_list_init(&dev->list);
    dev_list_init(&dev->clist);
    dev->ref_count = 0;
#ifdef DEV_USING_RDWR_CTL
    dev->lock = 0;
#endif /* DEV_USING_RDWR_CTL */
    dev->sync = DEV_SYNC;
    dev->position = -1;
    dev_list_init(&dev->rd_call_list);
    dev_list_init(&dev->wr_call_list);
    dev->ops = ops;
    dev->drv = drv;

    /* Register the device */
    return dev_register(dev, path);
}

/**
 * @brief This function handle device interrupt.
 *
 * @param dev The device to be handle.
 * @param event The event to be handle.
 * @param args The arguments of the event.
 *
 * @return 0 on success, otherwise an error code.
 *
 * @retval -6 device is closed or not supported.
 * @retval other error code.
 */
int platform_device_isr(plaf_dev_t * dev, int event, void * args)
{
    DEV_ASSERT(dev != DEV_NULL);
    DEV_ASSERT(event >= 0);

    if (dev->ref_count == 0) {
        return DEV_ENOTSUP;
    }

    if (dev->ops->isr != DEV_NULL) {
        int ret = dev->ops->isr(dev, event, args);
        if (ret < 0) {
            return (int)ret;
        }
    }

    /* Call the all set callbacks */
    switch (event & DEV_ISR_MASK) {
        case DEV_ISR_RD: {
            for (plaf_dev_list_t * list = dev->rd_call_list.next;
                 list != &dev->rd_call_list;
                 list = list->next) {
                plaf_dev_desc_t * desc = (plaf_dev_desc_t *)DEV_CONTAINER_OF(list,
                                                                             plaf_dev_desc_t,
                                                                             rd_call.list);
                if (desc->rd_call.fn != DEV_NULL) {
                    desc->rd_call.fn((int)(desc - &desc_map[0]), args);
                }
            }
            return DEV_EOK;
        }
        case DEV_ISR_WR: {
#ifdef DEV_USING_RDWR_CTL
            dev_lock_release(dev, DEV_LOCK_NONBLOCK);
#endif /* DEV_USING_RDWR_CTL */
            for (plaf_dev_list_t * list = dev->wr_call_list.next;
                 list != &dev->wr_call_list;
                 list = list->next) {
                 plaf_dev_desc_t * desc = (plaf_dev_desc_t *)DEV_CONTAINER_OF(list,
                                                                              plaf_dev_desc_t,
                                                                              wr_call.list);
                if (desc->wr_call.fn != DEV_NULL) {
                    desc->wr_call.fn((int)(desc - &desc_map[0]), args);
                }
            }
            return DEV_EOK;
        }
        default: {
            return DEV_ENOTSUP;
        }
    }
}

/**
 * @brief This function open a device.
 *
 * @param path The path of the device.
 * @param flags The open flags.
 *
 * @return The descriptor of the device on success, the support flags of the device if flags is a query,
 *         otherwise an error code.
 *
 * @retval -1 not enough descriptor or memory.
 * @retval -3 the path is not found.
 * @retval -6 not supported flags.
 * @retval other error code.
 */
int platform_device_open(const char * path, int flags)
{
    DEV_ASSERT(path != DEV_NULL);
    DEV_ASSERT(flags >= DEV_O_CLOSED);

    /* Query device flags */
    if (flags == DEV_O_QUERY) {
        plaf_dev_t * dev = dev_find(path);
        if (dev == DEV_NULL) {
            return DEV_ENOTFOUND;
        }
        return dev->flags;
    }

    /* Allocate descriptor and open device */
    int desc = desc_allocate(path);
    if (desc < 0) {
        return desc;
    }
    int ret = dev_open(desc_map[desc].dev, flags);
    if (ret < 0) {
        desc_free(desc);
        return ret;
    }

    /* Set descriptor flags */
    desc_map[desc].flags = flags;
    return desc;
}

/**
 * @brief This function close a device.
 *
 * @param desc The descriptor of the device.
 *
 * @return 0 on success, otherwise an error code.
 *
 * @retval -7 descriptor is invalid.
 * @retval other error code.
 */
int platform_device_close(int desc)
{
    DESC_CHECK(desc);

    /* Close the device and free the descriptor */
    int ret = dev_close(desc_map[desc].dev);
    if (ret < 0) {
        return ret;
    }
    desc_free(desc);
    return DEV_EOK;
}

/**
 * @brief This function read a device.
 *
 * @param desc The descriptor of the device.
 * @param buf The buf buffer to be read.
 * @param count The count of read.
 *
 * @return The size of the actual read, otherwise an error code.
 *
 * @retval -4 is currently accessed by another.
 * @retval -6 not supported read.
 * @retval -7 descriptor is invalid.
 * @retval other error code.
 */
int platform_device_read(int desc, void * buf, size_t count)
{
    DEV_ASSERT((buf != DEV_NULL) || (count == 0));
    DESC_CHECK(desc);

#ifdef DEV_USING_RDWR_CTL
    if (DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_RDONLY) == DEV_DISABLE) {
        return DEV_ENOTSUP;
    }
#endif /* DEV_USING_RDWR_CTL */

    /* Read buffer from the device */
    return dev_read(DESC_OF(desc).dev,
                    DESC_OF(desc).position,
                    DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_NONBLOCK),
                    buf,
                    count);
}

/**
 * @brief This function write a device.
 *
 * @param desc The descriptor of the device.
 * @param buf The buf buffer to be written.
 * @param count The count of write.
 *
 * @return The size of the actual write, otherwise an error code.
 *
 * @retval -4 is currently accessed by another.
 * @retval -6 not supported write.
 * @retval -7 descriptor is invalid.
 * @retval other error code.
 */
int platform_device_write(int desc, const void * buf, size_t count)
{
    DEV_ASSERT((buf != DEV_NULL) || (count == 0));
    DESC_CHECK(desc);

#ifdef DEV_USING_RDWR_CTL
    if (DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_WRONLY) == DEV_DISABLE) {
        return DEV_ENOTSUP;
    }
#endif /* DEV_USING_RDWR_CTL */

    /* Write buffer to the device */
    return dev_write(DESC_OF(desc).dev,
                     DESC_OF(desc).position,
                     DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_NONBLOCK),
                     buf,
                     count);
}

/**
 * @brief This function ioctl a device.
 *
 * @param desc The descriptor of the device.
 * @param cmd The command of the device.
 * @param args The arguments of the device.
 *
 * @return The size of the actual control, otherwise an error code.
 *
 * @retval -4 is currently accessed by another.
 * @retval -6 not supported ioctl or command.
 * @retval -7 descriptor or argument is invalid.
 * @retval other error code.
 */
int platform_device_ioctl(int desc, int cmd, void * args)
{
    DESC_CHECK(desc);

    switch (cmd) {
        case DEV_IOC_SPOS: {
            if (args != DEV_NULL) {
                int position = *(int *)args;

                DESC_OF(desc).position = position;
                return sizeof(position);
            }
            return DEV_EINVAL;
        }
        case DEV_IOC_SRCB: {
#ifdef DEV_USING_RDWR_CTL
            if (DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_RDONLY) == DEV_DISABLE) {
                return DEV_ENOTSUP;
            }
#endif /* DEV_USING_RDWR_CTL */
            void (*fn)(int desc, void *args) =  (void (*)(int desc, void *args))args;

            /* Link or unlink the callback function */
            DESC_OF(desc).rd_call.fn = fn;
            dev_service_interrupt_disable();
            if (fn != DEV_NULL) {
                dev_list_insert_before(&DESC_OF(desc).dev->rd_call_list,
                                      &DESC_OF(desc).rd_call.list);
            } else {
                dev_list_remove(&DESC_OF(desc).rd_call.list);
            }
            dev_service_interrupt_enable();
            return sizeof(fn);
        }
        case DEV_IOC_SWCB: {
#ifdef DEV_USING_RDWR_CTL
            if (DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_WRONLY) == DEV_DISABLE) {
                return DEV_ENOTSUP;
            }
#endif /* DEV_USING_RDWR_CTL */
            void (*fn)(int desc, void *args) =  (void (*)(int desc, void *args))args;

            /* Link or unlink the callback function */
            DESC_OF(desc).wr_call.fn = fn;
            dev_service_interrupt_disable();
            if (fn != DEV_NULL) {
                dev_list_insert_before(&DESC_OF(desc).dev->wr_call_list,
                                      &DESC_OF(desc).wr_call.list);
            } else {
                dev_list_remove(&DESC_OF(desc).wr_call.list);
            }
            dev_service_interrupt_enable();
            return sizeof(fn);
        }
        case DEV_IOC_GPOS: {
            if (args != DEV_NULL) {
                int *position = (int *)args;

                *position = DESC_OF(desc).position;
                return sizeof(position);
            }
            return DEV_EINVAL;
        }
        case DEV_IOC_GRCB: {
#ifdef DEV_USING_RDWR_CTL
            if (DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_RDONLY) == DEV_DISABLE) {
                return DEV_ENOTSUP;
            }
#endif /* DEV_USING_RDWR_CTL */
            if (args != DEV_NULL) {
                *(void (**)(int desc, void *args))args = DESC_OF(desc).rd_call.fn;
                return sizeof(DESC_OF(desc).rd_call.fn);
            }
            return DEV_EINVAL;
        }
        case DEV_IOC_GWCB: {
#ifdef DEV_USING_RDWR_CTL
            if (DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_WRONLY) == DEV_DISABLE) {
                return DEV_ENOTSUP;
            }
#endif /* DEV_USING_RDWR_CTL */
            if (args != DEV_NULL) {
                *(void (**)(int desc, void *args))args = DESC_OF(desc).wr_call.fn;
                return sizeof(DESC_OF(desc).wr_call.fn);
            }
            return DEV_EINVAL;
        }
        default: {
            /* I/O control to the device */
            return dev_ioctl(DESC_OF(desc).dev,
                             DESC_OF(desc).position,
                             DEV_BIT_IS_SET(DESC_OF(desc).flags, DEV_O_NONBLOCK),
                             cmd,
                             args);
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static inline int dev_is_root(plaf_dev_t * dev)
{
    return dev->type == DEV_TYPE_ROOT;
}

static inline plaf_dev_t * dev_find_child(plaf_dev_t * parent, const char * name)
{
    for (plaf_dev_list_t * list = parent->clist.next; list != &parent->clist; list = list->next) {
        plaf_dev_t * dev = (plaf_dev_t *)DEV_CONTAINER_OF(list, plaf_dev_t, list);
        if (strncmp(name, dev->name, DEV_NAME_LEN) == 0) {
            return dev;
        }
    }
    return DEV_NULL;
}

static inline int dev_register_child(plaf_dev_t * parent, plaf_dev_t * child, const char *name)
{
    if (dev_find_child(parent, name) != DEV_NULL) {
        return DEV_EEXIST;
    }

    child->magic = DEV_MAGIC_NUMBER;
    strncpy(child->name, name, DEV_NAME_LEN);
    child->parent = parent;
    dev_list_insert_before(&parent->clist, &child->list);
    return DEV_EOK;
}

static int dev_register_by_path(plaf_dev_t * parent, plaf_dev_t * dev, const char * path)
{
    if (path[0] == '/') {
        path++;
    }

    /* Check for child path separator */
    const char *child_path = strchr(path, '/');
    if (child_path != DEV_NULL) {
        char child_name[DEV_NAME_LEN + 1] = {0};
        size_t len = DEV_BOUND(child_path - path, 0, DEV_NAME_LEN);

        /* Find the child device */
        strncpy(child_name, path, len);
        child_name[len] = '\0';
        plaf_dev_t *child = dev_find_child(parent, child_name);
        if (child == DEV_NULL) {
            return DEV_ENOTFOUND;
        }

        /* Register recursively */
        return dev_register_by_path(child, dev, child_path);
    } else {
        /* Register with parent */
        return dev_register_child(parent, dev, path);
    }
}

static plaf_dev_t * dev_find_by_path(plaf_dev_t * parent, const char * path)
{
    if (path[0] == '/') {
        path++;
    }

    /* Check for child path separator */
    const char *child_path = strchr(path, '/');
    if (child_path != DEV_NULL) {
        char child_name[DEV_NAME_LEN + 1] = {0};
        size_t len = DEV_BOUND(child_path - path, 0, DEV_NAME_LEN);

        /* Find the child device */
        strncpy(child_name, path, len);
        child_name[len] = '\0';
        plaf_dev_t * child = dev_find_child(parent, child_name);
        if (child == DEV_NULL) {
            return DEV_NULL;
        }

        /* Find recursively */
        return dev_find_by_path(child, child_path);
    } else {
        /* Find with parent */
        return dev_find_child(parent, path);
    }
}

static inline plaf_dev_t * dev_find(const char * path)
{
    /* Check whether the path is absolute */
    if (*path == '/') {
        path++;
        const char * next_slash = strchr(path, '/');
        if ((next_slash == DEV_NULL) ||
            (strncmp(path, root_dev.name, DEV_BOUND(next_slash - path, 0, DEV_NAME_LEN)) !=
             0)) {
            return DEV_NULL;
        }
        path += DEV_BOUND(next_slash - path, 0, DEV_NAME_LEN);
    }

    /* Find the device from the root device */
    return dev_find_by_path(&root_dev, path);
}

static inline int dev_register(plaf_dev_t * dev, const char * path)
{
    /* Check whether the path is absolute */
    if (*path == '/') {
        path++;
        const char * next_slash = strchr(path, '/');
        if ((next_slash == DEV_NULL) ||
            (strncmp(path, root_dev.name, DEV_BOUND(next_slash - path, 0, DEV_NAME_LEN)) !=
             0)) {
            return DEV_EINVAL;
        }
        path += DEV_BOUND(next_slash - path, 0, DEV_NAME_LEN);
    }

    /* Register the device with the root device */
    dev_service_interrupt_disable();
    int ret = dev_register_by_path(&root_dev, dev, path);
    dev_service_interrupt_enable();
    return ret;
}

static int dev_open(plaf_dev_t * dev, int flags)
{
#ifdef DEV_USING_RDWR_CTL
    if (DEV_BIT_IS_SET(dev->flags, flags) != DEV_ENABLE) {
        return DEV_ENOTSUP;
    }
#endif /* DEV_USING_RDWR_CTL */

    /* Check whether the device is opened */
    if (dev->ref_count == 0) {
        /* Continue iterating until reach the root device */
        if (dev_is_root(dev->parent) != DEV_TRUE) {
            int ret = dev_open(dev->parent, flags);
            if (ret < 0) {
                return ret;
            }
        }

        /* Open the device */
        if (dev->ops->open != DEV_NULL) {
            int ret = dev->ops->open(dev);
            if (ret < 0) {
                return ret;
            }
        }
    }

    /* Increase the reference count */
    dev->ref_count++;
    return DEV_EOK;
}

static int dev_close(plaf_dev_t * dev)
{
    /* Decrease the reference count */
    dev->ref_count--;

    /* Check whether the device needs to be closed */
    if (dev->ref_count == 0) {
        /* Continue iterating until reach the root device */
        if (dev_is_root(dev->parent) != DEV_TRUE) {
            int ret = dev_close(dev->parent);
            if (ret < 0) {
                return ret;
            }
        }

        /* Close the device */
        if (dev->ops->close != DEV_NULL) {
            return dev->ops->close(dev);
        }
    }
    return DEV_EOK;
}

static inline int dev_read(plaf_dev_t * dev, int position, int sync, void * buf, size_t count)
{
#ifdef DEV_USING_RDWR_CTL
    do {
        dev_service_interrupt_disable();
        int ret = dev_lock_take(dev, (DEV_LOCK_RD | DEV_LOCK_SLEEP), DEV_LOCK_RD);
        if (ret < 0) {
            dev_service_interrupt_enable();
            return ret;
        }
        dev_service_interrupt_enable();
    } while (0);
#endif /* DEV_USING_RDWR_CTL */

    /* Update information */
    dev->sync = sync;
    dev->position = position;

    /* Read buffer from the device */
    int ret = dev->ops->read(dev, buf, count);

#ifdef DEV_USING_RDWR_CTL
    dev_lock_release(dev, DEV_LOCK_RD);
#endif /* DEV_USING_RDWR_CTL */
    return ret;
}

static inline int dev_write(plaf_dev_t *dev,
                            int position,
                            int sync,
                            const void *buf,
                            size_t count)
{
#ifdef DEV_USING_RDWR_CTL
    do {
        dev_service_interrupt_disable();
        int ret = dev_lock_take(dev,
                                (DEV_LOCK_WR |
                                 DEV_LOCK_SLEEP |
                                 (sync == DEV_SYNC ? DEV_LOCK_NONBLOCK : 0)),
                                DEV_LOCK_WR);
        if (ret < 0) {
            dev_service_interrupt_enable();
            return ret;
        }
        dev_service_interrupt_enable();
    } while (0);
#endif /* DEV_USING_RDWR_CTL */

    /* Update information */
    dev->sync = sync;
    dev->position = position;

    /* Write buffer to the device */
    int ret = dev->ops->write(dev, buf, count);

#ifdef DEV_USING_RDWR_CTL
    dev_lock_release(dev, DEV_LOCK_WR);
    if ((sync == DEV_ASYNC) && (ret > 0)) {
        dev_service_interrupt_disable();
        dev_lock_take(dev, 0, DEV_LOCK_NONBLOCK);
        dev_service_interrupt_enable();
    }
#endif /* DEV_USING_RDWR_CTL */
    return ret;
}

static inline int dev_ioctl(plaf_dev_t * dev, int position, int sync, int cmd, void * args)
{
    if (dev->ops->ioctl == DEV_NULL) {
        return DEV_ENOTSUP;
    }

#ifdef DEV_USING_RDWR_CTL
    do {
        /* Lock only when user -> device command */
        if (cmd) {
            dev_service_interrupt_disable();
            int ret = dev_lock_take(dev,
                                    (DEV_LOCK_RDWR | DEV_LOCK_SLEEP | DEV_LOCK_NONBLOCK),
                                    DEV_LOCK_RDWR);
            if (ret < 0) {
                dev_service_interrupt_enable();
                return ret;
            }
            dev_service_interrupt_enable();
        }
    } while (0);
#endif /* DEV_USING_RDWR_CTL */

    /* Update information */
    dev->sync = sync;
    dev->position = position;

    /* I/O control to the device */
    int ret = dev->ops->ioctl(dev, cmd, args);

#ifdef DEV_USING_RDWR_CTL
    dev_lock_release(dev, DEV_LOCK_RDWR);
#endif /* DEV_USING_RDWR_CTL */
    return ret;
}

static inline int desc_allocate(const char * path)
{
    int desc = -1;

    plaf_dev_t * dev = dev_find(path);
    if (dev == DEV_NULL) {
        return DEV_ENOTFOUND;
    }

    /* Find a free descriptor */
    for (size_t i = 0; i < DEV_DESC_NUM; i++) {
        if (DESC_OF(i).dev == DEV_NULL) {
            desc = (int)i;
            break;
        }
    }
    if (desc < 0) {
        return DEV_ENOMEM;
    }

    DESC_OF(desc).dev = dev;
    DESC_OF(desc).flags = DEV_O_CLOSED;
    DESC_OF(desc).position = -1;
    DESC_OF(desc).rd_call.fn = DEV_NULL;
    DESC_OF(desc).wr_call.fn = DEV_NULL;
    dev_list_init(&DESC_OF(desc).rd_call.list);
    dev_list_init(&DESC_OF(desc).wr_call.list);
    return desc;
}

static inline void desc_free(int desc)
{
    if (DESC_IS_VALID(desc) == DEV_TRUE) {
        DESC_OF(desc).dev = DEV_NULL;
        DESC_OF(desc).flags = DEV_O_CLOSED;
        DESC_OF(desc).position = -1;
        DESC_OF(desc).rd_call.fn = DEV_NULL;
        DESC_OF(desc).wr_call.fn = DEV_NULL;
        dev_list_remove(&DESC_OF(desc).rd_call.list);
        dev_list_remove(&DESC_OF(desc).wr_call.list);
    }
}
/******************************* (END OF FILE) *********************************/



 







