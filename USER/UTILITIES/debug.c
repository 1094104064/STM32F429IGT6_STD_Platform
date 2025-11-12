/**
  ******************************************************************************
  *
  * @file    debug.c
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
#include "debug.h"
#include "gpio.h"
/**********************
 *      MACROS
 **********************/

/*********************
 *      DEFINES
 *********************/

#define DEBUG_TX_PORT   GPIOA
#define DEBUG_TX_PIN    GPIO_Pin_9

#define DEBUG_RX_PORT   GPIOA
#define DEBUG_RX_PIN    GPIO_Pin_10
 

// 配置选项 - 根据需求启用/禁用功能
#define PRINTF_SUPPORT_FLOAT      1
#define PRINTF_SUPPORT_EXPONENTIAL 0  // 指数格式，通常不需要
#define PRINTF_SUPPORT_LONG_LONG  1
#define PRINTF_SUPPORT_PTR_DIFF   1
#define PRINTF_NTOA_BUFFER_SIZE   32  // 数字转换缓冲区

// 格式标志
typedef struct {
    bool left_justify;
    bool force_sign;
    bool space_no_sign;
    bool alternative_form;
    bool zero_pad;
    bool precision_set;
    int width;
    int precision;
} printf_flags_t;

/**********************
 *   GLOBAL VARIABLES
 **********************/ 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static const char* _parse_format(const char *fmt, printf_flags_t *flags);
static void _output_padding(int count, bool zero_pad);

#if PRINTF_SUPPORT_FLOAT
static int _ftoa_format(char *buffer, double value, printf_flags_t *flags);
#endif

static int _ntoa_format(char *buffer, uint64_t value, bool negative, int base, printf_flags_t *flags);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
void debug_init(uint32_t baudrate)
{
    STD_GPIO_Init(DEBUG_TX_PORT, DEBUG_TX_PIN, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_USART1);
    STD_GPIO_Init(DEBUG_RX_PORT, DEBUG_RX_PIN, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_AF_USART1);
    
    USART_InitTypeDef USART_InitStructure = {0};

    /*!< Enable the Usart1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
    
    /*!< USART_InitStructure members configuration*/
    USART_InitStructure.USART_BaudRate 	          = baudrate;                       /**< Setup by the user*/
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            /**< Data length 8 bits*/
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               /**< 1 stop bit*/
    USART_InitStructure.USART_Parity              = USART_Parity_No ;               /**< No parity check*/
    USART_InitStructure.USART_Mode 	              = USART_Mode_Rx | USART_Mode_Tx;  /**< Full duplex communication*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /**< No hardware flow control*/
    USART_Init(USART1, &USART_InitStructure); 

    NVIC_InitTypeDef NVIC_InitStructure = {0};
    
    /*!< Enable Usart1 */
    USART_Cmd(USART1, ENABLE);
    
    /*!< NVIC_InitStructure members configuration */
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;     /**< Enable Usart1 interrupt Channel*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               /**< PreemptionPriority 3*/
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;               /**< SubPriority 1*/
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;          /**< Enable interrupt*/
    NVIC_Init(&NVIC_InitStructure); 
    
    /*!< Enable the receive disrupt of Usart1 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    pr_log("\033[2J\033[H");
    pr_log("\r\n\r\n\r\n");
    pr_user("────────────────────── [ date: %s %s ] ──────────────────────", __DATE__, __TIME__);
    pr_user("Debug serial port initialization successful");
    pr_user("The system clock is: %dMHz\r\n", SystemCoreClock / 1000000);
}


void debug_putc(char c)
{
    /*!< Send single byte */
    USART_SendData(USART1, (uint8_t)c);	
    
    /*!< Waiting for sending to complete */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
}

int debug_puts(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    int chars_printed = 0;
    char buffer[PRINTF_NTOA_BUFFER_SIZE];
    
    while (*fmt) {
        if (*fmt != '%') {
            debug_putc(*fmt++);
            chars_printed++;
            continue;
        }
        
        fmt++; // 跳过 '%'
        
        // 解析格式
        printf_flags_t flags;
        fmt = _parse_format(fmt, &flags);
        
        // 处理转换说明符
        char specifier = *fmt++;
        int output_len = 0;
        bool numeric_output = false;
        
        switch (specifier) {
            case 'd':
            case 'i': {
                int value = va_arg(args, int);
                output_len = _ntoa_format(buffer, 
                    value < 0 ? (uint64_t)(-value) : (uint64_t)value,
                    value < 0, 10, &flags);
                numeric_output = true;
                break;
            }
            
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                output_len = _ntoa_format(buffer, value, false, 10, &flags);
                numeric_output = true;
                break;
            }
            
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                output_len = _ntoa_format(buffer, value, false, 16, &flags);
                numeric_output = true;
                break;
            }
            
            case 'X': {
                // 大写十六进制 - 需要特殊处理
                unsigned int value = va_arg(args, unsigned int);
                output_len = _ntoa_format(buffer, value, false, 16, &flags);
                // 转换为大写
                for (int i = 0; i < output_len; i++) {
                    if (buffer[i] >= 'a' && buffer[i] <= 'f') {
                        buffer[i] -= 32;
                    }
                }
                numeric_output = true;
                break;
            }
            
            case 'o': {
                unsigned int value = va_arg(args, unsigned int);
                output_len = _ntoa_format(buffer, value, false, 8, &flags);
                numeric_output = true;
                break;
            }
            
            case 'c': {
                char c = (char)va_arg(args, int);
                debug_putc(c);
                chars_printed++;
                break;
            }
            
            case 's': {
                char *str = va_arg(args, char*);
                if (!str) str = "(null)";
                
                // 计算字符串长度
                const char *p = str;
                while (*p) p++;
                output_len = p - str;
                
                if (flags.precision_set && output_len > flags.precision) {
                    output_len = flags.precision;
                }
                
                // 宽度处理
                if (!flags.left_justify && flags.width > output_len) {
                    _output_padding(flags.width - output_len, false);
                    chars_printed += flags.width - output_len;
                }
                
                // 输出字符串
                for (int i = 0; i < output_len; i++) {
                    debug_putc(str[i]);
                }
                chars_printed += output_len;
                
                if (flags.left_justify && flags.width > output_len) {
                    _output_padding(flags.width - output_len, false);
                    chars_printed += flags.width - output_len;
                }
                break;
            }
            
            case 'p': {
                void *ptr = va_arg(args, void*);
                flags.alternative_form = true;
                output_len = _ntoa_format(buffer, (uintptr_t)ptr, false, 16, &flags);
                numeric_output = true;
                break;
            }
            
#if PRINTF_SUPPORT_FLOAT
            case 'f':
            case 'F': {
                double value = va_arg(args, double);
                if (!flags.precision_set) flags.precision = 6;
                output_len = _ftoa_format(buffer, value, &flags);
                numeric_output = true;
                break;
            }
#endif
            
#if PRINTF_SUPPORT_LONG_LONG
            case 'l': {
                // 长整型支持
                if (*fmt == 'l') {
                    fmt++; // 处理ll
                    specifier = *fmt++;
                    switch (specifier) {
                        case 'd':
                        case 'i': {
                            long long value = va_arg(args, long long);
                            output_len = _ntoa_format(buffer,
                                value < 0 ? (uint64_t)(-value) : (uint64_t)value,
                                value < 0, 10, &flags);
                            numeric_output = true;
                            break;
                        }
                        case 'u': {
                            unsigned long long value = va_arg(args, unsigned long long);
                            output_len = _ntoa_format(buffer, value, false, 10, &flags);
                            numeric_output = true;
                            break;
                        }
                    }
                }
                break;
            }
#endif
            
            case '%': {
                debug_putc('%');
                chars_printed++;
                break;
            }
            
            default: {
                // 未知格式说明符
                debug_putc('%');
                debug_putc(specifier);
                chars_printed += 2;
                break;
            }
        }
        
        // 处理数值输出的宽度和对齐
        if (numeric_output && output_len > 0) {
            int total_padding = flags.width - output_len;
            
            // 右对齐（前导填充）
            if (!flags.left_justify && total_padding > 0) {
                _output_padding(total_padding, flags.zero_pad && !flags.precision_set);
                chars_printed += total_padding;
            }
            
            // 输出数字
            for (int i = 0; i < output_len; i++) {
                debug_putc(buffer[i]);
            }
            chars_printed += output_len;
            
            // 左对齐（后置填充）
            if (flags.left_justify && total_padding > 0) {
                _output_padding(total_padding, false);
                chars_printed += total_padding;
            }
        }
    }
    
    va_end(args);
    return chars_printed;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static int _ntoa_format(char *buffer, uint64_t value, bool negative, int base, printf_flags_t *flags) 
{
    static const char digits[] = "0123456789abcdef";
    char *ptr = buffer;
    int len = 0;
    
    // 处理符号
    if (negative) {
        *ptr++ = '-';
        len++;
    } else if (flags->force_sign) {
        *ptr++ = '+';
        len++;
    } else if (flags->space_no_sign) {
        *ptr++ = ' ';
        len++;
    }
    
    // 基数前缀
    if (flags->alternative_form) {
        if (base == 16) {
            *ptr++ = '0';
            *ptr++ = 'x';
            len += 2;
        } else if (base == 8) {
            *ptr++ = '0';
            len++;
        }
    }
    
    // 数字转换
    char temp[PRINTF_NTOA_BUFFER_SIZE];
    char *temp_ptr = temp + sizeof(temp) - 1;
    *temp_ptr = '\0';
    
    do {
        *--temp_ptr = digits[value % base];
        value /= base;
    } while (value > 0);
    
    // 处理精度填充
    int num_len = (temp + sizeof(temp) - 1) - temp_ptr;
    if (flags->precision_set && num_len < flags->precision) {
        int zeros = flags->precision - num_len;
        while (zeros-- > 0) {
            *ptr++ = '0';
            len++;
        }
    }
    
    // 复制数字
    while (*temp_ptr) {
        *ptr++ = *temp_ptr++;
        len++;
    }
    
    *ptr = '\0';
    return len;
}

// 浮点数支持（可选）
#if PRINTF_SUPPORT_FLOAT
static int _ftoa_format(char *buffer, double value, printf_flags_t *flags) {
    // 简化版浮点转换 - 实际使用时建议使用更完整的实现
    if (value < 0) {
        buffer[0] = '-';
        value = -value;
        return 1 + _ftoa_format(buffer + 1, value, flags);
    }
    
    int integer_part = (int)value;
    double fractional = value - integer_part;
    
    int len = _ntoa_format(buffer, integer_part, false, 10, flags);
    
    if (flags->precision_set && flags->precision > 0) {
        buffer[len++] = '.';
        
        // 转换小数部分
        for (int i = 0; i < flags->precision; i++) {
            fractional *= 10;
            int digit = (int)fractional;
            buffer[len++] = '0' + digit;
            fractional -= digit;
        }
    }
    
    buffer[len] = '\0';
    return len;
}
#endif

// 解析格式字符串
static const char* _parse_format(const char *fmt, printf_flags_t *flags) 
{
    // 重置标志
    flags->left_justify = false;
    flags->force_sign = false;
    flags->space_no_sign = false;
    flags->alternative_form = false;
    flags->zero_pad = false;
    flags->precision_set = false;
    flags->width = 0;
    flags->precision = 0;
    
    // 解析标志
    while (*fmt) {
        switch (*fmt) {
            case '-': flags->left_justify = true; break;
            case '+': flags->force_sign = true; break;
            case ' ': flags->space_no_sign = true; break;
            case '#': flags->alternative_form = true; break;
            case '0': flags->zero_pad = true; break;
            default: goto parse_width;
        }
        fmt++;
    }
    
parse_width:
    // 解析宽度
    if (*fmt >= '0' && *fmt <= '9') {
        flags->width = 0;
        while (*fmt >= '0' && *fmt <= '9') {
            flags->width = flags->width * 10 + (*fmt - '0');
            fmt++;
        }
    }
    
    // 解析精度
    if (*fmt == '.') {
        fmt++;
        flags->precision_set = true;
        flags->precision = 0;
        
        if (*fmt >= '0' && *fmt <= '9') {
            while (*fmt >= '0' && *fmt <= '9') {
                flags->precision = flags->precision * 10 + (*fmt - '0');
                fmt++;
            }
        }
    }
    
    return fmt;
}

// 输出填充
static void _output_padding(int count, bool zero_pad) {
    char pad_char = zero_pad ? '0' : ' ';
    while (count-- > 0) {
        debug_putc(pad_char);
    }
}




/******************************* (END OF FILE) *********************************/

 #if defined(__CC_ARM) && (__ARMCC_VERSION < 6000000)

    #pragma import(__use_no_semihosting)
    typedef struct __FILE FILE;

    // 定义FILE结构，以满足某些库函数的基本要求
    struct __FILE
    {
        int handle;
    };
    FILE __stdout;

    // 实现_sys_exit，避免半主机模式
    void _sys_exit(int x)
    {
        x = x; // 空实现
    }

    // 实现_ttywrch
    void _ttywrch(int ch)
    {
        ch = ch; // 空实现
    }
 
#endif



#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6000000)

    __asm (".global __use_no_semihosting\n\t");
    void _sys_exit(int x) 
    {
        x = x;
    }

    void _ttywrch(int ch)
    {
        ch = ch;
    }

    FILE __stdout;
 
#endif

/**
  * @brief  redirect fputc function to use printf.
  * @param  c: character
  * @param  fp: file pointer
  * @retval Returns a character if normal, or EOF if there is an error.
  */
int fputc(int c, FILE *fp)
{
   /*!< Send single byte */
   USART_SendData(USART1, (uint8_t)c);	
   
   /*!< Waiting for sending to complete */
   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	

    return (c); 
}

 







