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

void ud_st7735_fill_point(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint32_t color)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    st7735_set_cursor(ops, x, y);

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_data_16bit(self->spi_ops, color);

    panel_spi_stop_transmit(self->spi_ops);
}

void ud_st7735_fill_rectangle(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    st7735_set_window(ops, x, y, x + width - 1, y + height - 1);

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_data_16bit(self->spi_ops, color);

    panel_spi_stop_transmit(self->spi_ops);
}


void ud_st7735_copy_area(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const void * data)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    st7735_set_window(ops, x, y, x + width - 1, y + height - 1);

    panel_spi_start_transmit(self->spi_ops);

    panel_spi_write_buffer(self->spi_ops, data, width * height);

    panel_spi_stop_transmit(self->spi_ops);
}

void ud_st7735_enable_backlight(const struct ud_display_ops ** ops, bool state)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    panel_backlight_enable(self->backlight_ops, state);
}

void ud_st7735_set_backlight_brightness(const struct ud_display_ops ** ops, uint8_t brightness)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    panel_backlight_set_brightness(self->backlight_ops, brightness);
}

void ud_st7735_get_backlight_brightness(const struct ud_display_ops ** ops, uint8_t * brightness)
{
    const struct ud_st7735 * self = (const struct ud_st7735 *)ops;

    panel_backlight_get_brightness(self->backlight_ops, brightness);
}

void ud_st7735_ioctl(const struct ud_display_ops ** ops, unsigned int cmd, void * arg)
{
    // Implementation for IO control on ST7735
}

const struct ud_display_ops st7735_ops = {
    .fill_point                = ud_st7735_fill_point,
    .fill_rectangle            = ud_st7735_fill_rectangle,
    .copy_area                 = ud_st7735_copy_area,
    .enable_backlight          = ud_st7735_enable_backlight,
    .set_backlight_brightness  = ud_st7735_set_backlight_brightness,
    .get_backlight_brightness  = ud_st7735_get_backlight_brightness,
    .ioctl                     = ud_st7735_ioctl,
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

#if 0
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
#endif
