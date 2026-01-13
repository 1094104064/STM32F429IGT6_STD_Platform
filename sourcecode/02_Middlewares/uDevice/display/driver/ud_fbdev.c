#include "./ud_fbdev.h"


void ud_fbdev_prepare(const struct ud_display_ops ** ops)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;

}

void ud_fbdev_fill_point(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint32_t color)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;

    void * dst = self->fix->start_address;
    uint32_t pixel_bytes = self->var->bits_per_pixel / 8;
    uint32_t width = self->var->width; 

    *(volatile uint32_t*)((uint32_t)dst + pixel_bytes * (y * width + x) ) = color ;
}

void ud_fbdev_fill_rectangle(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;

    const void * dst = self->fix->start_address;

    panel_rgb_fill_data(self->rgb_ops, x, y, width, height, dst, color);
}

void ud_fbdev_copy_area(const struct ud_display_ops ** ops, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const void * src)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;

    const void * dst = self->fix->start_address;

    panel_rgb_copy_buffer(self->rgb_ops, x, y, width, height, src, dst);
}

void ud_fbdev_enable_backlight(const struct ud_display_ops ** ops, bool state)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;
}

void ud_fbdev_set_backlight_brightness(const struct ud_display_ops ** ops, uint8_t brightness)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;
}

void ud_fbdev_get_backlight_brightness(const struct ud_display_ops ** ops, uint8_t * brightness)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;
}

void ud_fbdev_ioctl(const struct ud_display_ops ** ops, unsigned int cmd, void * arg)
{
    const struct ud_fbdev * self = (const struct ud_fbdev *)ops;
}



static void ud_fbdev_register()
{
    
}

const struct ud_fbdev_var fbdev_var =
{
    .width = 800,
    .height = 480,

    .bits_per_pixel = 16,

    .red = { .length = 5, .offset = 11, .msb_right = 0 },
    .green = { .length = 6, .offset = 5, .msb_right = 0 },
    .blue = { .length = 5, .offset = 0, .msb_right = 0 },
    .alpha = { .length = 0, .offset = 0, .msb_right = 0 }
};
