/**
  ******************************************************************************
  *
  * @file    dev_service.h
  * @author  
  * @brief   Header file of dev_service module.
  *
  ******************************************************************************
  **/
  
#ifndef _DEV_SERVICE_H
#define _DEV_SERVICE_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/
void dev_service_interrupt_disable(void);
void dev_service_interrupt_enable(void);




/**********************
 *      MACROS
 **********************/

/**
 * @brief This macro function asserts a condition.
 *
 * @param ex The condition to assert.
 */
#define DEV_ASSERT(ex)  

/**
 * @brief This macro function gets the maximum of two values.
 *
 * @param a The first value.
 * @param b The second value.
 *
 * @return The maximum of the two values.
 */
#define DEV_MAX(a, b)                    ({typeof (a) _a = (a); typeof (b) _b = (b); _a > _b ? _a : _b; })

/**
 * @brief This macro function gets the minimum of two values.
 *
 * @param a The first value.
 * @param b The second value.
 *
 * @return The minimum of the two values.
 */
#define DEV_MIN(a, b)                    ({typeof (a) _a = (a); typeof (b) _b = (b); _a < _b ? _a : _b; })

/**
 * @brief This macro function ensures that a value is within a specified range.
 *
 * @param value The value.
 * @param min The minimum value.
 * @param max The maximum value.
 *
 * @return The value within the specified range.
 */
#define DEV_BOUND(value, min, max) \
    (((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value)))

/**
 * @brief This macro function ensures that a value is within a specified range.
 *
 * @param value The value.
 * @param min The minimum value.
 * @param max The maximum value.
 */
#define DEV_LIMIT(value, min, max)       (value) = DEV_BOUND(value, min, max)

/**
 * @brief This macro function swaps two values.
 *
 * @param a The first value.
 * @param b The second value.
 */
#define DEV_SWAP(a, b)                   do { typeof (a) temp = (a); (a) = (b); (b) = temp; } while (0)

/**
 * @addtogroup List
 * @{
 */

/**
 * @brief This macro function initializes a list.
 *
 * @param list The list to initialize.
 */
#define DEV_LIST_INIT(list)              {&(list), &(list)}

/**
 * @brief This macro function gets its structure from its member.
 *
 * @param pointer The pointer to the structure.
 * @param type The type of the structure.
 * @param member The member of the structure.
 *
 * @return A pointer to the structure.
 */
#define DEV_CONTAINER_OF(pointer, type, member) \
    ((type *)((char *)(pointer) - (unsigned long)(&((type *)0)->member)))

/**
 * @brief This macro function checks if a value is set.
 *
 * @param value The value to check.
 * @param mask The mask to check.
 */
#define DEV_BIT_IS_SET(value, mask)      (((value) & (mask)) == (mask))

/**
 * @brief This macro function sets a value.
 *
 * @param value The value to set.
 * @param mask The mask to set.
 */
#define DEV_BIT_SET(value, mask)         ((value) |= (mask))

/**
 * @brief This macro function clears a value.
 *
 * @param value The value to clear.
 * @param mask The mask to clear.
 */
#define DEV_BIT_CLR(value, mask)         ((value) &= ~(mask))







#ifdef __cplusplus
}
#endif

#endif /*_DEV_SERVICE_H*/


/******************************* (END OF FILE) *********************************/


