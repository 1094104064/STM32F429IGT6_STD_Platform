/**
  ******************************************************************************
  *
  * @file    lw_ll.h
  * @author  
  * @brief   Header file of low level driver module.
  *
  ******************************************************************************
  **/
  
#ifndef _LW_LL_H
#define _LW_LL_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
/*********************
 *      DEFINES
 *********************/
#define LW_LL_MEMSET(node, size)    memset((node), 0, (size))
#define LW_LL_MALLOC(size)          malloc((size))
#define LW_LL_FREE(ptr)             free((ptr))
/**********************
 *      TYPEDEFS
 **********************/
typedef struct lw_ll_node lw_ll_node_t;
typedef struct lw_ll lw_ll_t;

/* 节点结构 */
struct lw_ll_node 
{
    struct lw_ll_node * prev;
    struct lw_ll_node * next;
};

/* 链表结构 */
struct lw_ll
{
    lw_ll_node_t * head;
    lw_ll_node_t * tail;
    size_t node_size;    // 每个节点结构的大小，创建时传入
};
/**********************
*  GLOBAL PROTOTYPES
 **********************/
/* 初始化链表 */
void lw_ll_init(lw_ll_t * ll, size_t node_size);

/* 插入节点 */
void * lw_ll_ins_head(lw_ll_t * ll);
void * lw_ll_ins_tail(lw_ll_t * ll);
void * lw_ll_ins_before(lw_ll_t * ll, lw_ll_node_t * n);

/* 删除节点 */
void lw_ll_remove(lw_ll_t * ll, lw_ll_node_t * node);

/* 清空链表（不释放外部数据）*/
void lw_ll_clear(lw_ll_t * ll);
/**********************
 *      MACROS
 **********************/
#define lw_ll_get_head(ll)          ((ll)->head)
#define lw_ll_get_tail(ll)          ((ll)->tail)
#define lw_ll_get_next(node)        ((node)->next)
#define lw_ll_get_prev(node)        ((node)->prev)

#define LW_LL_READ_ITER(ll, it) \
    for (it = (void *)(ll)->head; it != NULL; it = (void *)(((lw_ll_node_t *)it)->next))

#ifdef __cplusplus
}
#endif

#endif /*_LW_LL_H*/


/******************************* (END OF FILE) *********************************/


