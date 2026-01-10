#include "stm32f4xx.h"
#include "panel_backlight_impl_st.h"
#include "../../adapter/panel_backlight.h"

void backlight_enable(const struct panel_backlight_ops ** ops, bool state)
{
    // Implement backlight enable for STM32
}


void backlight_set_brightness(const struct panel_backlight_ops ** ops, uint8_t brightness)
{
    // Implement backlight brightness setting for STM32
}

void backlight_get_brightness(const struct panel_backlight_ops ** ops, uint8_t * brightness)
{
    // Implement backlight brightness getting for STM32
}

static const struct panel_backlight_ops backlight_ops = {
    .enable          = backlight_enable,
    .set_brightness  = backlight_set_brightness,
    .get_brightness  = backlight_get_brightness,
};


const struct panel_backlight_ops * ud_stm32_get_backlight(void)
{
    return &backlight_ops;
}

