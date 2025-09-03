/**
  ******************************************************************************
  *
  * @file    user_macros.h
  * @author  Jamin
  * @brief   Header file of user_macros module.
  *
  ******************************************************************************
  **/
  
#ifndef _USER_MACROS_H
#define _USER_MACROS_H


#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
 
#define POWERFUL_NUMBER     (0x5F3759DF)                    /* 快速计算平方根倒数 */
 
#define REG8(x)             (*((volatile u8*)(x)))          /* 访问8位内存映射寄存器 (地址x) */
#define REG16(x)            (*((volatile u16*)(x)))         /* 访问16位内存映射寄存器 (地址x) */
#define REG32(x)            (*((volatile u32*)(x)))         /* 访问32位内存映射寄存器 (地址x) */

#define ARRAY_SIZE(x)       (sizeof(x) / sizeof((x)[0]))    /* 计算数组元素个数 */


#define BYTE0(x)            ((u8)((x) & 0xFF))              /* 获取字节0 */
#define BYTE1(x)            ((u8)(((u16)(x) >> 8) & 0xFF))  /* 获取字节1 */
#define BYTE2(x)            ((u8)(((u32)(x) >> 16) & 0xFF)) /* 获取字节2 */
#define BYTE3(x)            ((u8)(((u32)(x) >> 24) & 0xFF)) /* 获取字节3 */

#define SWAP16(x)           ((((u16)(x) >> 8) | ((u16)(x) << 8)))   /* 交换16位数据的高低字节 */

#define BIT_CLR(y, bit)     {(y) &= ~ (1 << (bit));}        /* 清除指定比特位 */
#define BIT_SET(y, bit)     {(y) |= (1 << (bit));}          /* 设置指定比特位 */
#define BIT_TST(y, bit)     ((y) & (1 << (bit)))            /* 测试指定比特位 */
#define MAX_VAL(x, y)       ((x) < (y)? (y) : (x))          /* 获取两个值中的最大值 */
#define MIN_VAL(x, y)       ((x) < (y)? (x) : (y))          /* 获取两个值中的最小值 */
#define ABS_VAL(d)          (((d) > 0)? (d) : 0 - (d))      /* 获取绝对值 */


/**
 * @brief 16位整数大小端转换 (高/低字节互换)
 * @param x 输入的16位无符号整数
 * @return 字节序交换后的16位整数
 * @note 示例：0x1234 -> 0x3412
 */
#define BSWAP16(x)  ((u16)( \
                      (((u16)(x) & 0x00FF) << 8) | \
                      (((u16)(x) & 0xFF00) >> 8) ))

/**
 * @brief 32位整数大小端转换 (字节逆序)
 * @param x 输入的32位无符号整数
 * @return 字节序交换后的32位整数
 * @note 示例：0x12345678 -> 0x78563412
 */
#define BSWAP32(x)  ((u32)( \
                      (((u32)(x) & 0x000000FFUL) << 24) | \
                      (((u32)(x) & 0x0000FF00UL) <<  8) | \
                      (((u32)(x) & 0x00FF0000UL) >>  8) | \
                      (((u32)(x) & 0xFF000000UL) >> 24) ))

/**
 * @brief 64位整数大小端转换 (字节逆序)
 * @param x 输入的64位无符号整数
 * @return 字节序交换后的64位整数
 * @note 示例：0x0123456789ABCDEF -> 0xEFCDAB8967452301
 */
#define BSWAP64(x)  ((u64)( \
                      (((u64)(x) & 0x00000000000000FFULL) << 56) | \
                      (((u64)(x) & 0x000000000000FF00ULL) << 40) | \
                      (((u64)(x) & 0x0000000000FF0000ULL) << 24) | \
                      (((u64)(x) & 0x00000000FF000000ULL) <<  8) | \
                      (((u64)(x) & 0x000000FF00000000ULL) >>  8) | \
                      (((u64)(x) & 0x0000FF0000000000ULL) >> 24) | \
                      (((u64)(x) & 0x00FF000000000000ULL) >> 40) | \
                      (((u64)(x) & 0xFF00000000000000ULL) >> 56) ))

/**********************
 *      TYPEDEFS
 **********************/
 
/**********************
*  GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/



#ifdef __cplusplus
}
#endif

#endif /*_USER_MACROS_H*/


/******************************* (END OF FILE) *********************************/

