/**
  ******************************************************************************
  *
  * @file    ud_display.c
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
#include "./ud_display.h"
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

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 
//struct ud_display_ops ** ud_display_create(int type)
//{

//}

void ud_display_register(struct ud_display * self, const struct ud_display_ops ** ops)
{
    self->ops = ops;
}

void ud_display_prepare(struct ud_display * self)
{
    display_prepare_fn_t fn = (*self->ops)->prepare;

    if(fn)
        fn(self->ops);
}

void ud_display_draw_point(struct ud_display * self, uint16_t x, uint16_t y, uint32_t color)
{
    display_put_pixel_fn_t fn = (*self->ops)->put_pixel;

    if(fn)
        fn(self->ops, x, y, color);
}

void ud_display_draw_block(struct ud_display * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    display_fill_area_fn_t fn = (*self->ops)->fill_area;

    if(fn)
        fn(self->ops, x, y, width, height, color);
}

void ud_display_clear(struct ud_display * self, uint32_t color)
{
    uint32_t width = self->width;
    uint32_t height = self->height;

    ud_display_draw_block(self, 0, 0, width, height, color);
}

void ud_display_flush(struct ud_display * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const void * buffer)
{
    display_transfer_pixels_fn_t fn = (*self->ops)->transfer_pixels;

    if(fn)
        fn(self->ops, x, y, width, height, buffer);
}

void ud_display_enable_backlight(struct ud_display * self, bool state)
{
    display_enable_backlight_fn_t fn = (*self->ops)->enable_backlight;

    if(fn)
        fn(self->ops, state);
}

void ud_display_set_backlight_brightness(struct ud_display * self, uint8_t brightness)
{
    display_set_backlight_brightness_fn_t fn = (*self->ops)->set_backlight_brightness;

    if(fn)
        fn(self->ops, brightness);
}

void ud_display_get_backlight_brightness(struct ud_display * self, uint8_t * brightness)
{
    display_get_backlight_brightness_fn_t fn = (*self->ops)->get_backlight_brightness;

    if(fn)
        fn(self->ops, brightness);
}

void ud_display_ioctl(struct ud_display * self, unsigned int cmd, void * arg)
{
    display_ioctl_fn_t fn = (*self->ops)->ioctl;

    if(fn)
        fn(self->ops, cmd, arg);
}

void ud_display_draw_line(struct ud_display * self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
    int32_t dx = 0;
    int32_t dy = 0;
    int32_t sx = 0;
    int32_t sy = 0;
    int32_t err = 0;
    int32_t e2 = 0;
                            
    
    dx = x2 - x1;
    dx = ((dx) > 0 ? (dx) : -(dx));

    dy = y2 - y1;
    dy = ((dy) > 0 ? (dy) : -(dy));


    if(x1 < x2) {
        sx = 1;
    } 
    else {
        sx = -1;
    }
    
    if(y1 < y2) {
        sy = 1;
    } 
    else {
        sy = -1;
    }
    
    err = dx - dy;
    
    while(1) {
        ud_display_draw_point(self, x1, y1, color);
        
        if(x1 == x2 && y1 == y2) {
            break;
        }
        
        e2 = 2 * err;
        
        if(e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        
        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void ud_display_draw_circle(struct ud_display * self, uint16_t x, uint16_t y, uint16_t radius, uint32_t color)
{
	int x_add = -radius, yadd = 0, err = 2 - 2 * radius, e2;
	do {   

		ud_display_draw_point(self, x - x_add, y + yadd, color);
		ud_display_draw_point(self, x + x_add, y + yadd, color);
		ud_display_draw_point(self, x + x_add, y - yadd, color);
		ud_display_draw_point(self, x - x_add, y - yadd, color);

		e2 = err;
		if (e2 <= yadd) {
			err += ++yadd * 2 + 1;
			if (-x_add == yadd && e2 <= x_add) e2 = 0;
		}
		if (e2 > x_add) err += ++x_add * 2 + 1;
    }
    while (x_add <= 0);
}

void ud_display_draw_triangle(struct ud_display * self, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color)
{
    ud_display_draw_line(self, x1, y1, x2, y2, color);
    ud_display_draw_line(self, x2, y2, x3, y3, color);
    ud_display_draw_line(self, x3, y3, x1, y1, color);
}

void ud_display_draw_rectangle(struct ud_display * self, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    ud_display_draw_line(self, x, y, x + width, y, color);
    ud_display_draw_line(self, x + width, y, x + width, y + height, color);
    ud_display_draw_line(self, x + width, y + height, x, y + height, color);
    ud_display_draw_line(self, x, y + height, x, y, color);
}

void ud_display_draw_ellipse(struct ud_display * self, int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color)
{
    int32_t x_add = -r1, y_add = 0, err = 2 - 2 * r1, e2;
    float K = 0, rad1 = 0, rad2 = 0;

    rad1 = r1;
    rad2 = r2;

    if (r1 > r2)
    { 
        do {
            K = (float)(rad1/rad2);

            ud_display_draw_point(self, x - x_add, y + (uint16_t)(y_add / K), color);
            ud_display_draw_point(self, x + x_add, y + (uint16_t)(y_add / K), color);
            ud_display_draw_point(self, x + x_add, y - (uint16_t)(y_add / K), color);
            ud_display_draw_point(self, x - x_add, y - (uint16_t)(y_add / K), color);

            e2 = err;
            if (e2 <= y_add) {
                err += ++y_add * 2 + 1;
                if (-x_add == y_add && e2 <= x_add) e2 = 0;
            }
            if (e2 > x_add) err += ++x_add * 2 + 1;
        }
        while (x_add <= 0);
    }
    else
    {
        y_add = -r2; 
        x_add = 0;
        do { 
            K = (float)(rad2/rad1);

            ud_display_draw_point(self, x - (uint16_t)(x_add / K), y + y_add, color);
            ud_display_draw_point(self, x + (uint16_t)(x_add / K), y + y_add, color);
            ud_display_draw_point(self, x + (uint16_t)(x_add / K), y - y_add, color);
            ud_display_draw_point(self, x - (uint16_t)(x_add / K), y - y_add, color);

            e2 = err;
            if (e2 <= x_add) {
                err += ++x_add * 3 + 1;
                if (-y_add == x_add && e2 <= y_add) e2 = 0;
            }
            if (e2 > y_add) err += ++y_add * 3 + 1;
        }
        while (y_add <= 0);
    }
}

void ud_display_draw_arc(struct ud_display * self, uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color)
{
    int32_t x = 0;
    int32_t y = r;
    int32_t d = 3 - 2 * r;
    
    while(x <= y) {
        if(start_angle <= 45 && end_angle >= 45) {
            ud_display_draw_point(self, x0 + x, y0 + y, color);
            ud_display_draw_point(self, x0 - x, y0 + y, color);
        }
        
        if(start_angle <= 135 && end_angle >= 135) {
            ud_display_draw_point(self, x0 + y, y0 + x, color);
            ud_display_draw_point(self, x0 - y, y0 + x, color);
        }
        
        if(start_angle <= 225 && end_angle >= 225) {
            ud_display_draw_point(self, x0 - x, y0 - y, color);
            ud_display_draw_point(self, x0 + x, y0 - y, color);
        }
        
        if(start_angle <= 315 && end_angle >= 315) {
            ud_display_draw_point(self, x0 - y, y0 - x, color);
            ud_display_draw_point(self, x0 + y, y0 - x, color);
        }
        
        if(d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        
        x++;
    }
}

void ud_display_draw_rgb_grad(struct ud_display * self, uint16_t grid_size)
{

}

/**********************
 *   STATIC FUNCTIONS
 **********************/




/******************************* (END OF FILE) *********************************/



 







