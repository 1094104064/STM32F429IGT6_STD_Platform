/**
  ******************************************************************************
  *
  * @file    edf_list.h
  * @author  
  * @brief   Header file of edf_list module.
  *
  ******************************************************************************
  **/
  
#ifndef _EDF_LIST_H
#define _EDF_LIST_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include "edf_macro.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/**
 * @addtogroup List
 * @{
 */
typedef struct edf_list edf_list_t;
/**
 * @brief List structure.
 */
struct edf_list
{
    edf_list_t *next;                                           /**< Point to next node */
    edf_list_t *prev;                                           /**< Point to prev node */
};
/** @} */

/**********************
*  GLOBAL PROTOTYPES
 **********************/




/**********************
 *      MACROS
 **********************/

/**
 * @addtogroup List
 * @{
 */

/**
 * @brief This macro function initializes a list.
 *
 * @param list The list to initialize.
 */
#define EDF_LIST_INIT(list)              {&(list), &(list)}

/**
 * @brief This function checks if a list is empty.
 *
 * @param list The list to check.
 *
 * @return True if the list is empty, edf_false otherwise.
 */
EDF_INLINE int edf_list_is_empty(edf_list_t *list)
{
    return list->next == list;
}

/**
* @brief This function initialize a double list.
*
* @param list The list to initialize.
*/
EDF_INLINE void edf_list_init(edf_list_t *list)
{
    list->next = list;
    list->prev = list;
}

/**
 * @brief This function insert a node after a node in a double list.
 *
 * @param list The list to insert after.
 * @param node The node to insert.
 */
EDF_INLINE void edf_list_insert_after(edf_list_t *list, edf_list_t *node)
{
    list->next->prev = node;
    node->next = list->next;
    list->next = node;
    node->prev = list;
}

/**
 * @brief This function insert a node before a node in a double list.
 *
 * @param list The list to insert before.
 * @param node The node to insert.
 */
EDF_INLINE void edf_list_insert_before(edf_list_t *list, edf_list_t *node)
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
EDF_INLINE void edf_list_remove(edf_list_t *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = node->prev = node;
}

/**
 * @brief This function get the length of a double list.
 *
 * @param list The list to get the length of.
 *
 * @return The length of the list.
 */
EDF_INLINE size_t edf_list_get_len(edf_list_t *list)
{
    edf_list_t *node = list;
    size_t len = 0;

    while (node->next != list) {
        node = node->next;
        len++;
    }
    return len;
}
/** @} */




#ifdef __cplusplus
}
#endif

#endif /*_EDF_LIST_H*/


/******************************* (END OF FILE) *********************************/


