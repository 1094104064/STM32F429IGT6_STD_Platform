/**
  ******************************************************************************
  *
  * @file    edf_device.c
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
#include "edf_device.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

#ifndef EDF_DESC_NUM
#define EDF_DESC_NUM                 (32)
#endif /* EDF_DESC_NUM */
/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
EDF_INLINE int dev_is_root(edf_device_t *dev);
EDF_INLINE edf_device_t *dev_find_child(edf_device_t *parent, const char *name);
EDF_INLINE int dev_register_child(edf_device_t *parent, edf_device_t *child, const char *name);
static int dev_register_by_path(edf_device_t *parent, edf_device_t *dev, const char *path);
static edf_device_t *dev_find_by_path(edf_device_t *parent, const char *path);
EDF_INLINE edf_device_t *dev_find(const char *path);
EDF_INLINE int dev_register(edf_device_t *dev, const char *path);
static int dev_open(edf_device_t *dev, int flags);
static int dev_close(edf_device_t *dev);
EDF_INLINE ssize_t dev_read(edf_device_t *dev, int position, int sync, void *buf, size_t count)
;EDF_INLINE ssize_t dev_write(edf_device_t *dev,
                             int position,
                             int sync,
                             const void *buf,
                             size_t count);
EDF_INLINE int dev_ioctl(edf_device_t *dev, int position, int sync, int cmd, void *args);
static int desc_allocate(const char *path);
EDF_INLINE void desc_free(int desc);
/**********************
 *  STATIC VARIABLES
 **********************/
static edf_device_t root_dev = {
    .name = "dev", 
    .type = EDF_DEV_TYPE_ROOT, 
    .parent = EDF_NULL, 
    .list = EDF_LIST_INIT(root_dev.list), 
    .clist = EDF_LIST_INIT(root_dev.clist)}; 

#ifndef EDF_CFG_DESC_NUM
#define EDF_CFG_DESC_NUM                 (32)
#endif /* EDF_CFG_DESC_NUM */
static edf_dev_desc_t desc_map[EDF_CFG_DESC_NUM] = {0};  

#define DESC_OF(desc)                   (desc_map[(desc)])          /**< Descriptor of the device */
/* Check if the descriptor is valid */
#define DESC_IS_VALID(desc)             (((desc) >= 0 && (desc) < EDF_CFG_DESC_NUM) && ((DESC_OF(desc).dev) != EDF_NULL))
#ifdef EDF_USING_DESC_CHECK
#define EDF_DESC_CHECK(desc)             if (DESC_IS_VALID(desc) == EDF_FALSE) { return EDF_EINVAL; }
#else
#define EDF_DESC_CHECK(desc)
#endif /* EDF_USING_DESC_CHECK */


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
int edf_dev_register(edf_device_t *dev,
                    const char *path,
                    int type,
                    int flags,
                    edf_dev_ops_t *ops,
                    edf_drv_t *drv)
{
    EDF_ASSERT(dev != EDF_NULL);
    EDF_ASSERT(dev->magic != EDF_MAGIC_NUMBER);
    EDF_ASSERT(path != EDF_NULL);
    EDF_ASSERT(type != EDF_DEV_TYPE_ROOT);
    EDF_ASSERT(flags >= EDF_O_CLOSED);
    EDF_ASSERT(ops != EDF_NULL);
    EDF_ASSERT((ops->read != EDF_NULL) || (EDF_BIT_IS_SET(flags, EDF_O_RDONLY) == EDF_DISABLE));
    EDF_ASSERT((ops->write != EDF_NULL) || (EDF_BIT_IS_SET(flags, EDF_O_WRONLY) == EDF_DISABLE));

    /* Initialize the fields */
    dev->magic = 0;
    memset(dev->name, '\0', EDF_CFG_DEV_NAME_LEN);
    dev->type = type;
    dev->flags = flags;
    dev->parent = EDF_NULL;
    edf_list_init(&dev->list);
    edf_list_init(&dev->clist);
    dev->ref_count = 0;
#ifdef EDF_USING_RDWR_CTL
    dev->lock = 0;
#endif /* EDF_USING_RDWR_CTL */
    dev->sync = EDF_SYNC;
    dev->position = -1;
    edf_list_init(&dev->rd_call_list);
    edf_list_init(&dev->wr_call_list);
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
int edf_dev_isr(edf_device_t *dev, int event, void *args)
{
    EDF_ASSERT(dev != EDF_NULL);
    EDF_ASSERT(event >= 0);

    if (dev->ref_count == 0) {
        return EDF_ENOTSUP;
    }

    if (dev->ops->isr != EDF_NULL) {
        ssize_t ret = dev->ops->isr(dev, event, args);
        if (ret < 0) {
            return (int)ret;
        }
    }

    /* Call the all set callbacks */
    switch (event & EDF_ISR_MASK) {
        case EDF_ISR_RD: {
            for (edf_list_t *list = dev->rd_call_list.next;
                 list != &dev->rd_call_list;
                 list = list->next) {
                 edf_dev_desc_t *desc = (edf_dev_desc_t *)EDF_CONTAINER_OF(list,
                                                                           edf_dev_desc_t,
                                                                           rd_call.list);
                if (desc->rd_call.fn != EDF_NULL) {
                    desc->rd_call.fn((int)(desc - &desc_map[0]), args);
                }
            }
            return EDF_EOK;
        }
        case EDF_ISR_WR: {
#ifdef EDF_USING_RDWR_CTL
            dev_lock_release(dev, EDF_LOCK_NONBLOCK);
#endif /* EDF_USING_RDWR_CTL */
            for (struct edf_list *list = dev->wr_call_list.next;
                 list != &dev->wr_call_list;
                 list = list->next) {
                 edf_dev_desc_t *desc = (edf_dev_desc_t *)EDF_CONTAINER_OF(list,
                                                                                edf_dev_desc_t,
                                                                                wr_call.list);
                if (desc->wr_call.fn != EDF_NULL) {
                    desc->wr_call.fn((int)(desc - &desc_map[0]), args);
                }
            }
            return EDF_EOK;
        }
        default: {
            return EDF_ENOTSUP;
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
int edf_dev_open(const char *path, int flags)
{
    EDF_ASSERT(path != EDF_NULL);
    EDF_ASSERT(flags >= EDF_O_CLOSED);

    /* Query device flags */
    if (flags == EDF_O_QUERY) {
        edf_device_t *dev = dev_find(path);
        if (dev == EDF_NULL) {
            return EDF_ENOTFOUND;
        }
        return dev->flags;
    }

    /* Allocate descriptor and open device */
    int desc = desc_allocate(path);
    if (desc < 0) {
        return desc;
    }
    int ret = dev_open(DESC_OF(desc).dev, flags);
    if (ret < 0) {
        desc_free(desc);
        return ret;
    }

    /* Set descriptor flags */
    DESC_OF(desc).flags = flags;
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
int edf_dev_close(int desc)
{
    EDF_DESC_CHECK(desc);

    /* Close the device and free the descriptor */
    int ret = dev_close(DESC_OF(desc).dev);
    if (ret < 0) {
        return ret;
    }
    desc_free(desc);
    return EDF_EOK;
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
ssize_t edf_dev_read(int desc, void *buf, size_t count)
{
    EDF_ASSERT((buf != EDF_NULL) || (count == 0));
    EDF_DESC_CHECK(desc);

#ifdef EDF_USING_RDWR_CTL
    if (EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_RDONLY) == EDF_DISABLE) {
        return EDF_ENOTSUP;
    }
#endif /* EDF_USING_RDWR_CTL */

    /* Read buffer from the device */
    return dev_read(DESC_OF(desc).dev,
                    DESC_OF(desc).position,
                    EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_NONBLOCK),
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
ssize_t edf_dev_write(int desc, const void *buf, size_t count)
{
    EDF_ASSERT((buf != EDF_NULL) || (count == 0));
    EDF_DESC_CHECK(desc);

#ifdef EDF_USING_RDWR_CTL
    if (EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_WRONLY) == EDF_DISABLE) {
        return EDF_ENOTSUP;
    }
#endif /* EDF_USING_RDWR_CTL */

    /* Write buffer to the device */
    return dev_write(DESC_OF(desc).dev,
                     DESC_OF(desc).position,
                     EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_NONBLOCK),
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
int edf_dev_ioctl(int desc, int cmd, void *args)
{
    EDF_DESC_CHECK(desc);

    switch (cmd) {
        case EDF_IOC_SPOS: {
            if (args != EDF_NULL) {
                int position = *(int *)args;

                DESC_OF(desc).position = position;
                return sizeof(position);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_SRCB: {
#ifdef EDF_USING_RDWR_CTL
            if (EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_RDONLY) == EDF_DISABLE) {
                return EDF_ENOTSUP;
            }
#endif /* EDF_USING_RDWR_CTL */
            void (*fn)(int desc, void *args) =  (void (*)(int desc, void *args))args;

            /* Link or unlink the callback function */
            DESC_OF(desc).rd_call.fn = fn;
            edf_interrupt_disable();
            if (fn != EDF_NULL) {
                edf_list_insert_before(&DESC_OF(desc).dev->rd_call_list,
                                      &DESC_OF(desc).rd_call.list);
            } else {
                edf_list_remove(&DESC_OF(desc).rd_call.list);
            }
            edf_interrupt_enable();
            return sizeof(fn);
        }
        case EDF_IOC_SWCB: {
#ifdef EDF_USING_RDWR_CTL
            if (EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_WRONLY) == EDF_DISABLE) {
                return EDF_ENOTSUP;
            }
#endif /* EDF_USING_RDWR_CTL */
            void (*fn)(int desc, void *args) =  (void (*)(int desc, void *args))args;

            /* Link or unlink the callback function */
            DESC_OF(desc).wr_call.fn = fn;
            edf_interrupt_disable();
            if (fn != EDF_NULL) {
                edf_list_insert_before(&DESC_OF(desc).dev->wr_call_list,
                                      &DESC_OF(desc).wr_call.list);
            } else {
                edf_list_remove(&DESC_OF(desc).wr_call.list);
            }
            edf_interrupt_enable();
            return sizeof(fn);
        }
        case EDF_IOC_GPOS: {
            if (args != EDF_NULL) {
                int *position = (int *)args;

                *position = DESC_OF(desc).position;
                return sizeof(position);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_GRCB: {
#ifdef EDF_USING_RDWR_CTL
            if (EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_RDONLY) == EDF_DISABLE) {
                return EDF_ENOTSUP;
            }
#endif /* EDF_USING_RDWR_CTL */
            if (args != EDF_NULL) {
                *(void (**)(int desc, void *args))args = DESC_OF(desc).rd_call.fn;
                return sizeof(DESC_OF(desc).rd_call.fn);
            }
            return EDF_EINVAL;
        }
        case EDF_IOC_GWCB: {
#ifdef EDF_USING_RDWR_CTL
            if (EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_WRONLY) == EDF_DISABLE) {
                return EDF_ENOTSUP;
            }
#endif /* EDF_USING_RDWR_CTL */
            if (args != EDF_NULL) {
                *(void (**)(int desc, void *args))args = DESC_OF(desc).wr_call.fn;
                return sizeof(DESC_OF(desc).wr_call.fn);
            }
            return EDF_EINVAL;
        }
        default: {
            /* I/O control to the device */
            return dev_ioctl(DESC_OF(desc).dev,
                             DESC_OF(desc).position,
                             EDF_BIT_IS_SET(DESC_OF(desc).flags, EDF_O_NONBLOCK),
                             cmd,
                             args);
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
EDF_INLINE int dev_is_root(edf_device_t *dev)
{
    return dev->type == EDF_DEV_TYPE_ROOT;
}

EDF_INLINE edf_device_t *dev_find_child(edf_device_t *parent, const char *name)
{
    for (edf_list_t *list = parent->clist.next; list != &parent->clist; list = list->next) {
        edf_device_t *dev = (edf_device_t *)EDF_CONTAINER_OF(list, edf_device_t, list);
        if (strncmp(name, dev->name, EDF_CFG_DEV_NAME_LEN) == 0) {
            return dev;
        }
    }
    return EDF_NULL;
}

EDF_INLINE int dev_register_child(edf_device_t *parent, edf_device_t *child, const char *name)
{
    if (dev_find_child(parent, name) != EDF_NULL) {
        return EDF_EEXIST;
    }

    child->magic = EDF_MAGIC_NUMBER;
    strncpy(child->name, name, EDF_CFG_DEV_NAME_LEN);
    child->parent = parent;
    edf_list_insert_before(&parent->clist, &child->list);
    return EDF_EOK;
}

static int dev_register_by_path(edf_device_t *parent, edf_device_t *dev, const char *path)
{
    if (path[0] == '/') {
        path++;
    }

    /* Check for child path separator */
    const char *child_path = strchr(path, '/');
    if (child_path != EDF_NULL) {
        char child_name[EDF_CFG_DEV_NAME_LEN + 1] = {0};
        size_t len = EDF_BOUND(child_path - path, 0, EDF_CFG_DEV_NAME_LEN);

        /* Find the child device */
        strncpy(child_name, path, len);
        child_name[len] = '\0';
        edf_device_t *child = dev_find_child(parent, child_name);
        if (child == EDF_NULL) {
            return EDF_ENOTFOUND;
        }

        /* Register recursively */
        return dev_register_by_path(child, dev, child_path);
    } else {
        /* Register with parent */
        return dev_register_child(parent, dev, path);
    }
}

static edf_device_t *dev_find_by_path(edf_device_t *parent, const char *path)
{
    if (path[0] == '/') {
        path++;
    }

    /* Check for child path separator */
    const char *child_path = strchr(path, '/');
    if (child_path != EDF_NULL) {
        char child_name[EDF_CFG_DEV_NAME_LEN + 1] = {0};
        size_t len = EDF_BOUND(child_path - path, 0, EDF_CFG_DEV_NAME_LEN);

        /* Find the child device */
        strncpy(child_name, path, len);
        child_name[len] = '\0';
        edf_device_t *child = dev_find_child(parent, child_name);
        if (child == EDF_NULL) {
            return EDF_NULL;
        }

        /* Find recursively */
        return dev_find_by_path(child, child_path);
    } else {
        /* Find with parent */
        return dev_find_child(parent, path);
    }
}

#ifdef EDF_USING_RDWR_CTL
static int dev_lock_take(edf_device_t *dev, uint32_t take, uint32_t set)
{
    /* Continue iterating until reach the root device */
    if (dev_is_root(dev->parent) != EDF_TRUE) {
        int ret = dev_lock_take(dev->parent, take, set);
        if (ret < 0) {
            return ret;
        }
    }

    if (dev->lock & take) {
        return EDF_EBUSY;
    }
    EDF_BIT_SET(dev->lock, set);
    return EDF_EOK;
}

static void dev_lock_release(edf_device_t *dev, uint32_t release)
{
    /* Continue iterating until reach the root device */
    if (dev_is_root(dev->parent) != EDF_TRUE) {
        dev_lock_release(dev->parent, release);
    }

    EDF_BIT_CLR(dev->lock, release);
}
#endif /* EDF_USING_RDWR_CTL */

EDF_INLINE edf_device_t *dev_find(const char *path)
{
    /* Check whether the path is absolute */
    if (*path == '/') {
        path++;
        const char *next_slash = strchr(path, '/');
        if ((next_slash == EDF_NULL) ||
            (strncmp(path, root_dev.name, EDF_BOUND(next_slash - path, 0, EDF_CFG_DEV_NAME_LEN)) !=
             0)) {
            return EDF_NULL;
        }
        path += EDF_BOUND(next_slash - path, 0, EDF_CFG_DEV_NAME_LEN);
    }

    /* Find the device from the root device */
    return dev_find_by_path(&root_dev, path);
}

EDF_INLINE int dev_register(edf_device_t *dev, const char *path)
{
    /* Check whether the path is absolute */
    if (*path == '/') {
        path++;
        const char *next_slash = strchr(path, '/');
        if ((next_slash == EDF_NULL) ||
            (strncmp(path, root_dev.name, EDF_BOUND(next_slash - path, 0, EDF_CFG_DEV_NAME_LEN)) !=
             0)) {
            return EDF_EINVAL;
        }
        path += EDF_BOUND(next_slash - path, 0, EDF_CFG_DEV_NAME_LEN);
    }

    /* Register the device with the root device */
    edf_interrupt_disable();
    int ret = dev_register_by_path(&root_dev, dev, path);
    edf_interrupt_enable();
    return ret;
}

static int dev_open(edf_device_t *dev, int flags)
{
#ifdef EDF_USING_RDWR_CTL
    if (EDF_BIT_IS_SET(dev->flags, flags) != EDF_ENABLE) {
        return EDF_ENOTSUP;
    }
#endif /* EDF_USING_RDWR_CTL */

    /* Check whether the device is opened */
    if (dev->ref_count == 0) {
        /* Continue iterating until reach the root device */
        if (dev_is_root(dev->parent) != EDF_TRUE) {
            int ret = dev_open(dev->parent, flags);
            if (ret < 0) {
                return ret;
            }
        }

        /* Open the device */
        if (dev->ops->open != EDF_NULL) {
            int ret = dev->ops->open(dev);
            if (ret < 0) {
                return ret;
            }
        }
    }

    /* Increase the reference count */
    dev->ref_count++;
    return EDF_EOK;
}

static int dev_close(edf_device_t *dev)
{
    /* Decrease the reference count */
    dev->ref_count--;

    /* Check whether the device needs to be closed */
    if (dev->ref_count == 0) {
        /* Continue iterating until reach the root device */
        if (dev_is_root(dev->parent) != EDF_TRUE) {
            int ret = dev_close(dev->parent);
            if (ret < 0) {
                return ret;
            }
        }

        /* Close the device */
        if (dev->ops->close != EDF_NULL) {
            return dev->ops->close(dev);
        }
    }
    return EDF_EOK;
}

EDF_INLINE ssize_t dev_read(edf_device_t *dev, int position, int sync, void *buf, size_t count)
{
#ifdef EDF_USING_RDWR_CTL
    do {
        edf_interrupt_disable();
        int ret = dev_lock_take(dev, (EDF_LOCK_RD | EDF_LOCK_SLEEP), EDF_LOCK_RD);
        if (ret < 0) {
            edf_interrupt_enable();
            return ret;
        }
        edf_interrupt_enable();
    } while (0);
#endif /* EDF_USING_RDWR_CTL */

    /* Update information */
    dev->sync = sync;
    dev->position = position;

    /* Read buffer from the device */
    ssize_t ret = dev->ops->read(dev, buf, count);

#ifdef MR_USING_RDWR_CTL
    dev_lock_release(dev, MR_LOCK_RD);
#endif /* MR_USING_RDWR_CTL */
    return ret;
}

EDF_INLINE ssize_t dev_write(edf_device_t *dev,
                             int position,
                             int sync,
                             const void *buf,
                             size_t count)
{
#ifdef EDF_USING_RDWR_CTL
    do {
        edf_interrupt_disable();
        int ret = dev_lock_take(dev,
                                (EDF_LOCK_WR |
                                 EDF_LOCK_SLEEP |
                                 (sync == EDF_SYNC ? EDF_LOCK_NONBLOCK : 0)),
                                EDF_LOCK_WR);
        if (ret < 0) {
            edf_interrupt_enable();
            return ret;
        }
        edf_interrupt_enable();
    } while (0);
#endif /* EDF_USING_RDWR_CTL */

    /* Update information */
    dev->sync = sync;
    dev->position = position;

    /* Write buffer to the device */
    ssize_t ret = dev->ops->write(dev, buf, count);

#ifdef EDF_USING_RDWR_CTL
    dev_lock_release(dev, EDF_LOCK_WR);
    if ((sync == EDF_ASYNC) && (ret > 0)) {
        edf_interrupt_disable();
        dev_lock_take(dev, 0, EDF_LOCK_NONBLOCK);
        edf_interrupt_enable();
    }
#endif /* EDF_USING_RDWR_CTL */
    return ret;
}

EDF_INLINE int dev_ioctl(edf_device_t *dev, int position, int sync, int cmd, void *args)
{
    if (dev->ops->ioctl == EDF_NULL) {
        return EDF_ENOTSUP;
    }

#ifdef EDF_USING_RDWR_CTL
    do {
        /* Lock only when user -> device command */
        if (cmd) {
            edf_interrupt_disable();
            int ret = dev_lock_take(dev,
                                    (EDF_LOCK_RDWR | EDF_LOCK_SLEEP | EDF_LOCK_NONBLOCK),
                                    EDF_LOCK_RDWR);
            if (ret < 0) {
                edf_interrupt_enable();
                return ret;
            }
            edf_interrupt_enable();
        }
    } while (0);
#endif /* EDF_USING_RDWR_CTL */

    /* Update information */
    dev->sync = sync;
    dev->position = position;

    /* I/O control to the device */
    int ret = dev->ops->ioctl(dev, cmd, args);

#ifdef MR_USING_RDWR_CTL
    dev_lock_release(dev, MR_LOCK_RDWR);
#endif /* MR_USING_RDWR_CTL */
    return ret;
}

EDF_INLINE int desc_allocate(const char *path)
{
    int desc = -1;

    edf_device_t *dev = dev_find(path);
    if (dev == EDF_NULL) {
        return EDF_ENOTFOUND;
    }

    /* Find a free descriptor */
    for (size_t i = 0; i < EDF_CFG_DESC_NUM; i++) {
        if (DESC_OF(i).dev == EDF_NULL) {
            desc = (int)i;
            break;
        }
    }
    if (desc < 0) {
        return EDF_ENOMEM;
    }

    DESC_OF(desc).dev = dev;
    DESC_OF(desc).flags = EDF_O_CLOSED;
    DESC_OF(desc).position = -1;
    DESC_OF(desc).rd_call.fn = EDF_NULL;
    DESC_OF(desc).wr_call.fn = EDF_NULL;
    edf_list_init(&DESC_OF(desc).rd_call.list);
    edf_list_init(&DESC_OF(desc).wr_call.list);
    return desc;
}

EDF_INLINE void desc_free(int desc)
{
    if (DESC_IS_VALID(desc) == EDF_TRUE) {
        DESC_OF(desc).dev = EDF_NULL;
        DESC_OF(desc).flags = EDF_O_CLOSED;
        DESC_OF(desc).position = -1;
        DESC_OF(desc).rd_call.fn = EDF_NULL;
        DESC_OF(desc).wr_call.fn = EDF_NULL;
        edf_list_remove(&DESC_OF(desc).rd_call.list);
        edf_list_remove(&DESC_OF(desc).wr_call.list);
    }
}



/******************************* (END OF FILE) *********************************/



 







