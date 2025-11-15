/**
  ******************************************************************************
  *
  * @file    edf_macro.h
  * @author  
  * @brief   Header file of edf_macro module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_MACRO_H
#define _EDF_MACRO_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**
 * @addtogroup Compiler
 * @{
 */
#if defined(__ARMCC_VERSION)
#define EDF_SECTION(x)              	    __attribute__((section(x))) /**< Section */
#define EDF_USED                    	    __attribute__((used))       /**< Used */
#define EDF_WEAK                    	    __attribute__((weak))       /**< Weak */
#define EDF_INLINE                  	    static __inline             /**< Inline */
typedef int ssize_t;                                                /**< ssize_t type */
#elif defined (__IAR_SYSTEMS_ICC__)
#define EDF_SECTION(x)               	@ x                         /**< Section */
#define EDF_USED                     	__root                      /**< Used */
#define EDF_WEAK                     	__weak                      /**< Weak */
#define EDF_INLINE                   	static inline               /**< Inline */
#elif defined (__GNUC__)
#define EDF_SECTION(x)                   __attribute__((section(x))) /**< Section */
#define EDF_USED                         __attribute__((used))       /**< Used */
#define EDF_WEAK                         __attribute__((weak))       /**< Weak */
#define EDF_INLINE                       static __inline             /**< Inline */
#elif defined (__ADSPBLACKFIN__)
#define EDF_SECTION(x)               	__attribute__((section(x))) /**< Section */
#define EDF_USED                     	__attribute__((used))       /**< Used */
#define EDF_WEAK                     	__attribute__((weak))       /**< Weak */
#define EDF_INLINE                   	static inline               /**< Inline */
#elif defined (_MSC_VER)
#define EDF_SECTION(x)                                               /**< Section */
#define EDF_USED                                                     /**< Used */
#define EDF_WEAK                                                     /**< Weak */
#define EDF_INLINE                   	static __inline             /**< Inline */
typedef int ssize_t;  
#elif defined (__TASKING__)
#define EDF_SECTION(x)               	__attribute__((section(x))) /**< Section */
#define EDF_USED                         \
    __attribute__((used, protect))                                  /**< Used */
#define EDF_WEAK                     	__attribute__((weak))       /**< Weak */
#define EDF_INLINE                   	static inline               /**< Inline */
#else
#define EDF_SECTION(x)                   __attribute__((section(x))) /**< Section */
#define EDF_USED                         __attribute__((used))       /**< Used */
#define EDF_WEAK                         __attribute__((weak))       /**< Weak */
#define EDF_INLINE                       static __inline             /**< Inline */
#endif /* __ARMCC_VERSION */
/** @} */

/**
 * @addtogroup Basic
 * @{
 */
#define EDF_NULL                         ((void *)0)                 /**< Null pointer */
#define EDF_DISABLE                      (0)                         /**< Disable */
#define EDF_ENABLE                       (1)                         /**< Enable */
#define EDF_FALSE                        (0)                         /**< False */
#define EDF_TRUE                         (1)                         /**< True */
/** @} */

/**
 * @addtogroup Error
 * @{
 */
#define EDF_EOK                          (0)                         /**< No error */
#define EDF_ENOMEM                       (-1)                        /**< No enough memory */
#define EDF_EIO                          (-2)                        /**< I/O error */
#define EDF_ENOTFOUND                    (-3)                        /**< Not found */
#define EDF_EBUSY                        (-4)                        /**< Resource busy */
#define EDF_EEXIST                       (-5)                        /**< Exists */
#define EDF_ENOTSUP                      (-6)                        /**< Operation not supported */
#define EDF_EINVAL                       (-7)                        /**< Invalid argument */
#define EDF_ETIMEOUT                     (-8)                        /**< Timeout */
/** @} */

#define EDF_MAGIC_NUMBER                 (0xdeadbeef)                /**< Magic number */

#define EDF_SYNC                         (0)                         /**< Synchronous */
#define EDF_ASYNC                        (1)                         /**< Asynchronous */

/* [31:24] are for lock, [23:0] reserved */
#define EDF_LOCK_RD                      (0x01 << 24)                /**< Read lock */
#define EDF_LOCK_WR                      (0x02 << 24)                /**< Write lock */
#define EDF_LOCK_RDWR                    (0x03 << 24)                /**< Read/write lock */
#define EDF_LOCK_NONBLOCK                (0x04 << 24)                /**< Non-blocking lock */
#define EDF_LOCK_SLEEP                   (0x08 << 24)                /**< Sleep lock */

/* [31:24] are for basic flags, [23:0] can define user flags */
#define EDF_O_CLOSED                     (0)                         /**< Closed flag */
#define EDF_O_QUERY                      (0)                         /**< Query flag */
#define EDF_O_RDONLY                     (0x01 << 24)                /**< Read only flag */
#define EDF_O_WRONLY                     (0x02 << 24)                /**< Write only flag */
#define EDF_O_RDWR                       (0x03 << 24)                /**< Read/write flag */
#define EDF_O_NONBLOCK                   (0x04 << 24)                /**< Non-blocking flag */

/* [31:24] are for basic commands, [23:0] can define user commands. (>0): user -> device, (<0): user <- device */
#define EDF_IOC_SPOS                     (0x01 << 24)                /**< Set position command */
#define EDF_IOC_SRCB                     (0x02 << 24)                /**< Set read callback command */
#define EDF_IOC_SWCB                     (0x03 << 24)                /**< Set write callback command */
#define EDF_IOC_SCFG                     (0x04 << 24)                /**< Set configuration command */
#define EDF_IOC_SRBSZ                    (0x05 << 24)                /**< Set read buffer size command */
#define EDF_IOC_SWBSZ                    (0x06 << 24)                /**< Set write buffer size command */
#define EDF_IOC_CRBD                     (0x07 << 24)                /**< Clear read buffer data command */
#define EDF_IOC_CWBD                     (0x08 << 24)                /**< Clear write buffer data command */

#define EDF_IOC_GPOS                     (-(0x01 << 24))             /**< Get position command */
#define EDF_IOC_GRCB                     (-(0x02 << 24))             /**< Get read callback command */
#define EDF_IOC_GWCB                     (-(0x03 << 24))             /**< Get write callback command */
#define EDF_IOC_GCFG                     (-(0x04 << 24))             /**< Get configuration command */
#define EDF_IOC_GRBSZ                    (-(0x05 << 24))             /**< Get read buffer size command */
#define EDF_IOC_GWBSZ                    (-(0x06 << 24))             /**< Get write buffer size command */
#define EDF_IOC_GRBDSZ                   (-(0x07 << 24))             /**< Get read buffer data size command */
#define EDF_IOC_GWBDSZ                   (-(0x08 << 24))             /**< Get write buffer data size command */
/* [31:24] are for interrupt flags, [23:0] can define user flags */
#define EDF_ISR_RD                       (0x01 << 24)                /**< Read interrupt event */
#define EDF_ISR_WR                       (0x02 << 24)                /**< Write interrupt event */
#define EDF_ISR_MASK                     (0x7f << 24)                /**< Interrupt event mask */
 

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/




/**********************
 *      MACROS
 **********************/
/**
 * @addtogroup Basic
 * @{
 */

/**
 * @brief This macro function concatenates two strings.
 *
 * @param a The first string.
 * @param b The second string.
 *
 * @return The concatenated string.
 */
#define EDF_CONCAT(a, b)                 a##b

/**
 * @brief This macro function converts an integer to a string.
 *
 * @param a The integer to convert.
 *
 * @return The string representation of the integer.
 */
#define EDF_STR(a)                       #a

/**
 * @brief This macro function gets its structure from its member.
 *
 * @param pointer The pointer to the structure.
 * @param type The type of the structure.
 * @param member The member of the structure.
 *
 * @return A pointer to the structure.
 */
#define EDF_CONTAINER_OF(pointer, type, member) \
    ((type *)((char *)(pointer) - (unsigned long)(&((type *)0)->member)))

/**
 * @brief This macro function aligns a value upwards.
 *
 * @param value The value to align.
 * @param align The alignment.
 *
 * @return The aligned value.
 */
#define EDF_ALIGN_UP(value, align)       (((value) + (align) - 1) & ~((align) - 1))

/**
 * @brief This macro function aligns a value downwards.
 *
 * @param value The value to align.
 * @param align The alignment.
 *
 * @return The aligned value.
 */
#define EDF_ALIGN_DOWN(value, align)     ((value) & ~((align) - 1))

/**
 * @brief This macro function checks if a value is set.
 *
 * @param value The value to check.
 * @param mask The mask to check.
 */
#define EDF_BIT_IS_SET(value, mask)      (((value) & (mask)) == (mask))

/**
 * @brief This macro function sets a value.
 *
 * @param value The value to set.
 * @param mask The mask to set.
 */
#define EDF_BIT_SET(value, mask)         ((value) |= (mask))

/**
 * @brief This macro function clears a value.
 *
 * @param value The value to clear.
 * @param mask The mask to clear.
 */
#define EDF_BIT_CLR(value, mask)         ((value) &= ~(mask))

/**
 * @brief This macro function creates a local variable.
 *
 * @param type The type of the variable.
 * @param ... The arguments.
 *
 * @return A pointer to the variable.
 *
 * @note The variable is local, please use it carefully.
 */
#define EDF_MAKE_LOCAL(type, ...)        (&((type){__VA_ARGS__}))

/**
 * @brief This macro function gets the number of elements in an array.
 *
 * @param array The array.
 *
 * @return The number of elements in the array.
 */
#define EDF_ARRAY_NUM(array)             (sizeof(array)/sizeof((array)[0]))

/**
 * @brief This macro function gets the maximum of two values.
 *
 * @param a The first value.
 * @param b The second value.
 *
 * @return The maximum of the two values.
 */
#define EDF_MAX(a, b)                    ({typeof (a) _a = (a); typeof (b) _b = (b); _a > _b ? _a : _b; })

/**
 * @brief This macro function gets the minimum of two values.
 *
 * @param a The first value.
 * @param b The second value.
 *
 * @return The minimum of the two values.
 */
#define EDF_MIN(a, b)                    ({typeof (a) _a = (a); typeof (b) _b = (b); _a < _b ? _a : _b; })

/**
 * @brief This macro function ensures that a value is within a specified range.
 *
 * @param value The value.
 * @param min The minimum value.
 * @param max The maximum value.
 *
 * @return The value within the specified range.
 */
#define EDF_BOUND(value, min, max)       (((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value)))

/**
 * @brief This macro function ensures that a value is within a specified range.
 *
 * @param value The value.
 * @param min The minimum value.
 * @param max The maximum value.
 */
#define EDF_LIMIT(value, min, max)       (value) = EDF_BOUND(value, min, max)

/**
 * @brief This macro function swaps two values.
 *
 * @param a The first value.
 * @param b The second value.
 */
#define EDF_SWAP(a, b)                   do { typeof (a) temp = (a); (a) = (b); (b) = temp; } while (0)

/**
 * @brief This macro function converts a value to a boolean.
 *
 * @param value The value to convert.
 *
 * @return The boolean value.
 */
#define EDF_TO_BOOL(value)               (!!(value))
/** @} */

/**
 * @addtogroup Assert
 * @{
 */

/**
 * @brief This macro function asserts a condition.
 *
 * @param ex The condition to assert.
 */
#ifdef EDF_USING_ASSERT
#define EDF_ASSERT(ex)                  \
    do                                  \
    {                                   \
        if (!(ex))                      \
        {                               \
            mr_printf("%-8s "           \
                      "failed: %s, "    \
                      "function: %s, "  \
                      "file: %s, "      \
                      "line: %d.\r\n",  \
                      "ASSERT:",        \
                      #ex,              \
                      (__FUNCTION__),   \
                      (__FILE__),       \
                      (__LINE__));      \
            while(1);                   \
        }                               \
    } while(0)
#else
#define EDF_ASSERT(ex)
#endif /* EDF_USING_ASSERT */
/** @} */





#ifdef __cplusplus
}
#endif

#endif /*_EDF_MACRO_H*/


/******************************* (END OF FILE) *********************************/


