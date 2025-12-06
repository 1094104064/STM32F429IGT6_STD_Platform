/**
  ******************************************************************************
  *
  * @file    lw_ll.c
  * @author  
  * @brief   source file of link list module.
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
#include "lw_ll.h"
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
static void * alloc_node(size_t size);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
/**
  * @brief  Initialize a link list.
  * @param  ll: Pointer to the link list structure.
  * @param  node_size: Size of each node in the link list.
  * @retval None
  */
void lw_ll_init(lw_ll_t * ll, size_t node_size)
{
    ll->head = NULL;
    ll->tail = NULL;
    ll->node_size = node_size;
}

/**
  * @brief  Insert a new node at the head of the link list.
  * @param  ll: Pointer to the link list structure.
  * @retval Pointer to the allocated node, or NULL if allocation failed.
  */
void * lw_ll_ins_head(lw_ll_t * ll)
{
    lw_ll_node_t * n = alloc_node(ll->node_size);
    if (!n) return NULL;

    n->next = ll->head;
    if (ll->head)
        ll->head->prev = n;
    else
        ll->tail = n;

    ll->head = n;
    return n;
}

/**
  * @brief  Insert a new node at the tail of the link list.
  * @param  ll: Pointer to the link list structure.
  * @retval Pointer to the allocated node, or NULL if allocation failed.
  */
void * lw_ll_ins_tail(lw_ll_t * ll)
{
    lw_ll_node_t * n = alloc_node(ll->node_size);
    if (!n) return NULL;

    n->prev = ll->tail;
    if (ll->tail)
        ll->tail->next = n;
    else
        ll->head = n;

    ll->tail = n;
    return n;
}

/**
  * @brief  Insert a new node before the given node in the link list.
  * @param  ll: Pointer to the link list structure.
  * @param  node: Pointer to the node before which the new node will be inserted.
  * @retval Pointer to the allocated node, or NULL if allocation failed.
  */
void * lw_ll_ins_before(lw_ll_t * ll, lw_ll_node_t * node)
{
    if(node == NULL) return lw_ll_ins_tail(ll);

    lw_ll_node_t * n = alloc_node(ll->node_size);
    if (!n) return NULL;

    n->next = node;
    n->prev = node->prev;

    if(node->prev)
        node->prev->next = n;
    else
        ll->head = n;

    node->prev = n;

    return n;
}

/**
  * @brief  Remove the given node from the link list.
  * @param  ll: Pointer to the link list structure.
  * @param  node: Pointer to the node to be removed.
  * @retval None
  */
void lw_ll_remove(lw_ll_t * ll, lw_ll_node_t * node)
{
    if(node->prev)
        node->prev->next = node->next;
    else
        ll->head = node->next;

    if(node->next)
        node->next->prev = node->prev;
    else
        ll->tail = node->prev;

    LW_LL_FREE(node);
}

/**
  * @brief  Clear all nodes from the link list.
  * @param  ll: Pointer to the link list structure.
  * @retval None
  */
void lw_ll_clear(lw_ll_t * ll)
{
    lw_ll_node_t * n = ll->head;
    while(n)
    {
        lw_ll_node_t * next = n->next;
        LW_LL_FREE(n);
        n = next;
    }
    ll->head = ll->tail = NULL;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
  * @brief  Allocate a new node with the given size.
  * @param  size: Size of the node to allocate.
  * @retval Pointer to the allocated node, or NULL if allocation failed.
  */
static void * alloc_node(size_t size)
{
    lw_ll_node_t * n = LW_LL_MALLOC(size);
    if(n == NULL) return NULL;
    LW_LL_MEMSET(n, size);
    return n;
}



/******************************* (END OF FILE) *********************************/



 







