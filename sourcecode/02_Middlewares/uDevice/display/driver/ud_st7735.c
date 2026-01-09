#include "./ud_st7735.h"
#include "ud_macros.h"

static void st7735_set_cursor(struct ud_display_ops ** ops, uint16_t x, uint16_t y);
static void st7735_set_window(struct ud_display_ops ** ops, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);


void ud_st7735_prepare(struct ud_display_ops ** ops)
{
    // Implementation for preparing ST7735 display
}


void ud_st7735_set_orientation(struct ud_display_ops ** ops, uint8_t orientation)
{
    // Implementation for setting orientation on ST7735
}

void ud_st7735_put_pixel(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint32_t color)
{
    struct ud_st7735 * self = UD_CONTAINER_OF(ops, struct ud_st7735, display_ops);

}

void ud_st7735_fill_area(struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
    // Implementation for filling an area on ST7735
}

void ud_st7735_fill_screen(struct ud_display_ops ** ops, uint32_t color)
{
    // Implementation for filling the screen on ST7735
}

void ud_st7735_transfer_pixels(struct ud_display_ops ** ops, const void * data, size_t size)
{
    // Implementation for transferring pixels to ST7735
}

void ud_st7735_enable_backlight(struct ud_display_ops ** ops, bool state)
{
    panel_backlight_enable((struct panel_backlight_ops **)ops, state);
}

void ud_st7735_set_backlight_brightness(struct ud_display_ops ** ops, uint8_t brightness)
{
    panel_backlight_set_brightness((struct panel_backlight_ops **)ops, brightness);
}

uint8_t ud_st7735_get_backlight_brightness(struct ud_display_ops ** ops)
{
    return panel_backlight_get_brightness((struct panel_backlight_ops **)ops);
}

void ud_st7735_ioctl(struct ud_display_ops ** ops, unsigned int cmd, void * arg)
{
    // Implementation for IO control on ST7735
}

const struct ud_display_ops st7735_ops = {
    .put_pixel                = ud_st7735_put_pixel,
    .fill_area                = ud_st7735_fill_area,
    .fill_screen              = ud_st7735_fill_screen,
    .transfer_pixels          = ud_st7735_transfer_pixels,
    .enable_backlight         = ud_st7735_enable_backlight,
    .set_backlight_brightness = ud_st7735_set_backlight_brightness,
    .get_backlight_brightness = ud_st7735_get_backlight_brightness,
    .ioctl                    = ud_st7735_ioctl,
};

static void st7735_set_cursor(struct ud_display_ops ** ops, uint16_t x, uint16_t y)
{
    struct ud_st7735 * self = UD_CONTAINER_OF(ops, struct ud_st7735, display_ops);

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_command(self->spi_ops, ST7735_CASET);
    panel_spi_write_data_16bit(self->spi_ops, x);

    panel_spi_write_command(self->spi_ops, ST7735_RASET);
    panel_spi_write_data_16bit(self->spi_ops, y);

    panel_spi_write_command(self->spi_ops, ST7735_WRITE_RAM);

    panel_spi_stop_transmit(self->spi_ops);
}

static void st7735_set_window(struct ud_display_ops ** ops, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Implementation for setting window area on ST7735
}

static void st7735_enable(struct ud_display_ops ** ops)
{
    // Implementation for enabling/disabling ST7735 display
}

static void st7735_disable(struct ud_display_ops ** ops)
{
    // Implementation for disabling ST7735 display
}

static void st7735_sleep_in(struct ud_display_ops ** ops)
{
    // Implementation for putting ST7735 to sleep
}

static void st7735_sleep_out(struct ud_display_ops ** ops)
{
    // Implementation for waking ST7735 from sleep
}