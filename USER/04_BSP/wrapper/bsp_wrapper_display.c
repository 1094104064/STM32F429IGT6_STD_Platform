/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_display.c
  * @author  Jamin
  * @brief   Display wrapper for BSP
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
#include "bsp_wrapper_display.h"
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
static struct display_wrapper display_wrappers[DISPLAY_MAX_NUM];
static uint8_t current_display_idx = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

void bsp_wrapper_display_link(struct display_wrapper * self)
{
    uint8_t idx = 0;

    current_display_idx++;
 
    if(current_display_idx < DISPLAY_MAX_NUM) {
        idx = current_display_idx;
    }
    else {
        current_display_idx = 0;
    }

    memset(&display_wrappers[idx], 0, sizeof(struct display_wrapper));

    display_wrappers[idx] = *self;
    display_wrappers[idx].idx = idx;

    if(display_wrappers[idx].name == NULL) {
        pr_warn("This wrapper has no name and will be filled with a default name");
        display_wrappers[idx].name = "display_default";
    }
}

bool bsp_wrapper_display_init(void)
{
    int ret = 0;
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    assert_null(self->pf_init);
    assert_null(self->pf_backlight_on);
    assert_null(self->pf_backlight_off);
    assert_null(self->pf_put_pixel);
    assert_null(self->pf_fill_rect);
    assert_null(self->pf_fill_screen);
    assert_null(self->pf_copy_buffer);

    if( self->pf_init           == NULL || self->pf_backlight_on    == NULL ||
        self->pf_backlight_off  == NULL || self->pf_put_pixel       == NULL ||
        self->pf_fill_rect      == NULL || self->pf_fill_screen     == NULL ||
        self->pf_copy_buffer    == NULL ) {
        return false;
    }

    ret = self->pf_init(self);

    if(ret != 0) {
        pr_error("%s : failed to initialize, error code: %d", self->name, ret);
        return false;
    }

    pr_info("%s : initialized successfully", self->name);

    return true;
}

void bsp_wrapper_display_backlight_on(void)
{
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    self->pf_backlight_on(self);
}

void bsp_wrapper_display_backlight_off(void)
{
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    self->pf_backlight_off(self);
}

void bsp_wrapper_display_draw_pixel(uint16_t x, uint16_t y, uint32_t color)
{
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    self->pf_put_pixel(self, x, y, color);
}

void bsp_wrapper_display_fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    self->pf_fill_rect(self, x, y, width, height, color);
}

void bsp_wrapper_display_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
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
    } else {
        sx = -1;
    }
    
    if(y1 < y2) {
        sy = 1;
    } else {
        sy = -1;
    }
    
    err = dx - dy;
    
    while(1) {
        bsp_wrapper_display_draw_pixel(x1, y1, color);
        
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

void bsp_wrapper_display_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color)
{
	int x_add = -radius, yadd = 0, err = 2 - 2 * radius, e2;
	do {   

		bsp_wrapper_display_draw_pixel(x - x_add, y + yadd, color);
		bsp_wrapper_display_draw_pixel(x + x_add, y + yadd, color);
		bsp_wrapper_display_draw_pixel(x + x_add, y - yadd, color);
		bsp_wrapper_display_draw_pixel(x - x_add, y - yadd, color);

		e2 = err;
		if (e2 <= yadd) {
			err += ++yadd * 2 + 1;
			if (-x_add == yadd && e2 <= x_add) e2 = 0;
		}
		if (e2 > x_add) err += ++x_add * 2 + 1;
    }
    while (x_add <= 0);
}

void bsp_wrapper_display_draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color)
{
    bsp_wrapper_display_draw_line(x1, y1, x2, y2, color);
    bsp_wrapper_display_draw_line(x2, y2, x3, y3, color);
    bsp_wrapper_display_draw_line(x3, y3, x1, y1, color);
}

void bsp_wrapper_display_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    bsp_wrapper_display_draw_line(x, y, x + width, y, color);
    bsp_wrapper_display_draw_line(x + width, y, x + width, y + height, color);
    bsp_wrapper_display_draw_line(x + width, y + height, x, y + height, color);
    bsp_wrapper_display_draw_line(x, y + height, x, y, color);
}

void bsp_wrapper_display_draw_arc(uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color)
{
    int32_t x = 0;
    int32_t y = r;
    int32_t d = 3 - 2 * r;
    
    while(x <= y) {
        if(start_angle <= 45 && end_angle >= 45) {
            bsp_wrapper_display_draw_pixel(x0 + x, y0 + y, color);
            bsp_wrapper_display_draw_pixel(x0 - x, y0 + y, color);
        }
        
        if(start_angle <= 135 && end_angle >= 135) {
            bsp_wrapper_display_draw_pixel(x0 + y, y0 + x, color);
            bsp_wrapper_display_draw_pixel(x0 - y, y0 + x, color);
        }
        
        if(start_angle <= 225 && end_angle >= 225) {
            bsp_wrapper_display_draw_pixel(x0 - x, y0 - y, color);
            bsp_wrapper_display_draw_pixel(x0 + x, y0 - y, color);
        }
        
        if(start_angle <= 315 && end_angle >= 315) {
            bsp_wrapper_display_draw_pixel(x0 - y, y0 - x, color);
            bsp_wrapper_display_draw_pixel(x0 + y, y0 - x, color);
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

void bsp_wrapper_display_draw_ellipse(int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color)
{
  int32_t x_add = -r1, y_add = 0, err = 2 - 2 * r1, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = r1;
  rad2 = r2;
  
  if (r1 > r2)
  { 
    do {
      K = (float)(rad1/rad2);

		bsp_wrapper_display_draw_pixel(x - x_add, y + (uint16_t)(y_add / K), color);
		bsp_wrapper_display_draw_pixel(x + x_add, y + (uint16_t)(y_add / K), color);
		bsp_wrapper_display_draw_pixel(x + x_add, y - (uint16_t)(y_add / K), color);
		bsp_wrapper_display_draw_pixel(x - x_add, y - (uint16_t)(y_add / K), color);

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

		bsp_wrapper_display_draw_pixel(x - (uint16_t)(x_add / K), y + y_add, color);
		bsp_wrapper_display_draw_pixel(x + (uint16_t)(x_add / K), y + y_add, color);
		bsp_wrapper_display_draw_pixel(x + (uint16_t)(x_add / K), y - y_add, color);
		bsp_wrapper_display_draw_pixel(x - (uint16_t)(x_add / K), y - y_add, color);

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

void bsp_wrapper_display_fill_screen(uint32_t color)
{
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    self->pf_fill_screen(self, color);
}

void bsp_wrapper_display_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * image_data)
{
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    self->pf_copy_buffer(self, x, y, width, height, image_data);
}

void bsp_wrapper_display_draw_grad_rgb565(uint16_t grid_size)
{
    uint16_t x, y;
    uint32_t color;
    uint8_t grid_color;
    struct display_wrapper * self = &display_wrappers[current_display_idx];

    for(y = 0; y < self->height; y++) {
        for(x = 0; x < self->width; x++) {
            grid_color = ((x / grid_size) + (y / grid_size)) % 3;
            if (grid_color == 0)
                color = DISP_RGB565_RED; 
            else if (grid_color == 1)
                color = DISP_RGB565_GREEN; 
            else
                color = DISP_RGB565_BLUE; 

            bsp_wrapper_display_draw_pixel(x, y, color);
        }
    }
}
/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
