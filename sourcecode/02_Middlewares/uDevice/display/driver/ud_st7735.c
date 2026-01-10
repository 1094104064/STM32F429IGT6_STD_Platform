#include "./ud_st7735.h"
#include "../../service/ud_macros.h"
#include "../../service/ud_logger.h"

static void st7735_set_cursor(const struct ud_display_ops ** ops, uint16_t x, uint16_t y);
static void st7735_set_window(const struct ud_display_ops ** ops, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);




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
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    st7735_set_cursor(ops, x, y);

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_data_16bit(self->spi_ops, color);

    panel_spi_stop_transmit(self->spi_ops);
}

void ud_st7735_fill_area(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    uint32_t i = 0;
    uint32_t size = width * height;
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    st7735_set_window(ops, x, y, x + width - 1, y + height - 1);

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_data_16bit(self->spi_ops, color);

    panel_spi_stop_transmit(self->spi_ops);
}

void ud_st7735_fill_screen(const struct ud_display_ops ** ops, uint32_t color)
{
    // st7735_fill_area(self, 0, 0, self->width - 1, self->height - 1, color);
}

void ud_st7735_transfer_pixels(const struct ud_display_ops ** ops, const void * data, size_t size)
{
    // Implementation for transferring pixels to ST7735
}

void ud_st7735_enable_backlight(const struct ud_display_ops ** ops, bool state)
{
    panel_backlight_enable((struct panel_backlight_ops **)ops, state);
}

void ud_st7735_set_backlight_brightness(const struct ud_display_ops ** ops, uint8_t brightness)
{
    panel_backlight_set_brightness((struct panel_backlight_ops **)ops, brightness);
}

uint8_t ud_st7735_get_backlight_brightness(const struct ud_display_ops ** ops)
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

void ud_st7735_init(struct ud_st7735 * self, 
                    const struct panel_spi_ops ** spi_ops,
                    const struct panel_backlight_ops ** backlight_ops)
{
    self->display_ops = &st7735_ops;
}

static void st7735_set_cursor(const struct ud_display_ops ** ops, uint16_t x, uint16_t y)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_command(self->spi_ops, ST7735_CASET);
    panel_spi_write_data_16bit(self->spi_ops, x);

    panel_spi_write_command(self->spi_ops, ST7735_RASET);
    panel_spi_write_data_16bit(self->spi_ops, y);

    panel_spi_write_command(self->spi_ops, ST7735_WRITE_RAM);

    panel_spi_stop_transmit(self->spi_ops);
}

static void st7735_set_window(const struct ud_display_ops ** ops, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_command(self->spi_ops, ST7735_CASET);
    panel_spi_write_data_16bit(self->spi_ops, x0);
    panel_spi_write_data_16bit(self->spi_ops, x1);

    panel_spi_write_command(self->spi_ops, ST7735_RASET);
    panel_spi_write_data_16bit(self->spi_ops, y0);
    panel_spi_write_data_16bit(self->spi_ops, y1);

    panel_spi_write_command(self->spi_ops, ST7735_WRITE_RAM);

    panel_spi_stop_transmit(self->spi_ops);
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