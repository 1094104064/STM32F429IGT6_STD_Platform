

#include "ud_logger.h"

static ud_log_print_cb_t log_print_cb = NULL;

void ud_log_register(ud_log_print_cb_t print_cb)
{
    log_print_cb = print_cb;
}

void ud_log_print(const char *fmt, ...)
{
    log_print_cb = log_print_cb;
}

