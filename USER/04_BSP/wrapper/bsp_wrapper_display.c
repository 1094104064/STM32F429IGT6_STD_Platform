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
const struct display_wrapper wrp_display = 
{
    .obj_create         = bsp_wrapper_display_create,
    .obj_delete         = bsp_wrapper_display_delete,
    .find               = bsp_wrapper_display_find,

    .init               = bsp_wrapper_display_init,
    .backlight_on       = bsp_wrapper_display_backlight_on,
    .backlight_off      = bsp_wrapper_display_backlight_off,
    .backlight_set      = bsp_wrapper_display_backlight_set,
    .draw_pixel         = bsp_wrapper_display_draw_pixel,
    .fill_rect          = bsp_wrapper_display_fill_rect,
    .fill_screen        = bsp_wrapper_display_fill_screen,
    .draw_image         = bsp_wrapper_display_draw_image,
    .draw_line          = bsp_wrapper_display_draw_line,
    .draw_circle        = bsp_wrapper_display_draw_circle,
    .draw_triangle      = bsp_wrapper_display_draw_triangle,
    .draw_rect          = bsp_wrapper_display_draw_rect,
    .draw_arc           = bsp_wrapper_display_draw_arc,
    .draw_ellipse       = bsp_wrapper_display_draw_ellipse,
    .switch_framebuffer = bsp_wrapper_display_switch_framebuffer,
    .get_width          = bsp_wrapper_display_get_width,
    .get_height         = bsp_wrapper_display_get_height,
    .get_framebuffer    = bsp_wrapper_display_get_framebuffer,
    .draw_grad_rgb565   = bsp_wrapper_display_draw_grad_rgb565,
};
/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static display_obj_t gs_mempool[DISPLAY_MAX_NUM];
static uint8_t gsuc_index = 0;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

display_obj_t * bsp_wrapper_display_create(const display_ops_t * ops, const char * const name, void * const user_data)
{
    if(gsuc_index >= DISPLAY_MAX_NUM) {
        gsuc_index = 0;
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_display_find(name) != NULL) return NULL;

    memset(&gs_mempool[gsuc_index], 0, sizeof(display_obj_t));

    gs_mempool[gsuc_index].ops = ops;

    gs_mempool[gsuc_index].ctx.idx       = gsuc_index;
    gs_mempool[gsuc_index].ctx.user_data = user_data;
    strncpy(gs_mempool[gsuc_index].ctx.name, name, sizeof(gs_mempool[gsuc_index].ctx.name) - 1);

    gsuc_index++;

    return &gs_mempool[gsuc_index - 1];
}

void bsp_wrapper_display_delete(const char * const name)
{
    display_obj_t * obj = bsp_wrapper_display_find(name);
    if(obj != NULL) {
        memset(obj, 0, sizeof(display_obj_t));
    }
}

display_obj_t * bsp_wrapper_display_find(const char * const name)
{
    uint8_t i = 0;

    for(i = 0; i < DISPLAY_MAX_NUM; i++) {
        if(strncmp(gs_mempool[i].ctx.name, name, DISPLAY_NAME_MAX_LEN) == 0) {
            return &gs_mempool[i];
        }
    }

    return NULL;
}

bool bsp_wrapper_display_init(display_obj_t * obj)
{
    if(obj->ctx.is_initialized == true) return true;

    int ret = 1;
    if(obj->ops->pf_init)
        obj->ops->pf_init();

    if(ret != 0) {
        return false;
    }

    obj->ctx.is_initialized = true;

    return true;
}

void bsp_wrapper_display_backlight_on(display_obj_t * obj)
{
    if(obj->ops->pf_backlight_on)
        obj->ops->pf_backlight_on();
}

void bsp_wrapper_display_backlight_off(display_obj_t * obj)
{
    if(obj->ops->pf_backlight_off)
        obj->ops->pf_backlight_off();
}

void bsp_wrapper_display_backlight_set(display_obj_t * obj, uint8_t brightness)
{
    if(obj->ops->pf_backlight_set)
        obj->ops->pf_backlight_set(brightness);
}

void bsp_wrapper_display_draw_pixel(display_obj_t * obj, uint16_t x, uint16_t y, uint32_t color)
{
    if(obj->ops->pf_put_pixel)
        obj->ops->pf_put_pixel(x, y, color);
}

void bsp_wrapper_display_fill_rect(display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    if(obj->ops->pf_fill_rect)
        obj->ops->pf_fill_rect(x, y, width, height, color);
}

void bsp_wrapper_display_draw_line(display_obj_t * obj, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
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
        bsp_wrapper_display_draw_pixel(obj, x1, y1, color);
        
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

void bsp_wrapper_display_draw_circle(display_obj_t * obj, uint16_t x, uint16_t y, uint16_t radius, uint32_t color)
{
	int x_add = -radius, yadd = 0, err = 2 - 2 * radius, e2;
	do {   

		bsp_wrapper_display_draw_pixel(obj, x - x_add, y + yadd, color);
		bsp_wrapper_display_draw_pixel(obj, x + x_add, y + yadd, color);
		bsp_wrapper_display_draw_pixel(obj, x + x_add, y - yadd, color);
		bsp_wrapper_display_draw_pixel(obj, x - x_add, y - yadd, color);

		e2 = err;
		if (e2 <= yadd) {
			err += ++yadd * 2 + 1;
			if (-x_add == yadd && e2 <= x_add) e2 = 0;
		}
		if (e2 > x_add) err += ++x_add * 2 + 1;
    }
    while (x_add <= 0);
}

void bsp_wrapper_display_draw_triangle(display_obj_t * obj, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color)
{
    bsp_wrapper_display_draw_line(obj, x1, y1, x2, y2, color);
    bsp_wrapper_display_draw_line(obj, x2, y2, x3, y3, color);
    bsp_wrapper_display_draw_line(obj, x3, y3, x1, y1, color);
}

void bsp_wrapper_display_draw_rect(display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    bsp_wrapper_display_draw_line(obj, x, y, x + width, y, color);
    bsp_wrapper_display_draw_line(obj, x + width, y, x + width, y + height, color);
    bsp_wrapper_display_draw_line(obj, x + width, y + height, x, y + height, color);
    bsp_wrapper_display_draw_line(obj, x, y + height, x, y, color);
}

void bsp_wrapper_display_draw_arc(display_obj_t * obj, uint16_t x0, uint16_t y0, uint16_t r, int32_t start_angle, int32_t end_angle, uint32_t color)
{
    int32_t x = 0;
    int32_t y = r;
    int32_t d = 3 - 2 * r;
    
    while(x <= y) {
        if(start_angle <= 45 && end_angle >= 45) {
            bsp_wrapper_display_draw_pixel(obj, x0 + x, y0 + y, color);
            bsp_wrapper_display_draw_pixel(obj, x0 - x, y0 + y, color);
        }
        
        if(start_angle <= 135 && end_angle >= 135) {
            bsp_wrapper_display_draw_pixel(obj, x0 + y, y0 + x, color);
            bsp_wrapper_display_draw_pixel(obj, x0 - y, y0 + x, color);
        }
        
        if(start_angle <= 225 && end_angle >= 225) {
            bsp_wrapper_display_draw_pixel(obj, x0 - x, y0 - y, color);
            bsp_wrapper_display_draw_pixel(obj, x0 + x, y0 - y, color);
        }
        
        if(start_angle <= 315 && end_angle >= 315) {
            bsp_wrapper_display_draw_pixel(obj, x0 - y, y0 - x, color);
            bsp_wrapper_display_draw_pixel(obj, x0 + y, y0 - x, color);
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

void bsp_wrapper_display_draw_ellipse(display_obj_t * obj, int32_t x, int32_t y, int32_t r1, int32_t r2, uint32_t color)
{
  int32_t x_add = -r1, y_add = 0, err = 2 - 2 * r1, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = r1;
  rad2 = r2;
  
  if (r1 > r2)
  { 
    do {
      K = (float)(rad1/rad2);

		bsp_wrapper_display_draw_pixel(obj, x - x_add, y + (uint16_t)(y_add / K), color);
		bsp_wrapper_display_draw_pixel(obj, x + x_add, y + (uint16_t)(y_add / K), color);
		bsp_wrapper_display_draw_pixel(obj, x + x_add, y - (uint16_t)(y_add / K), color);
		bsp_wrapper_display_draw_pixel(obj, x - x_add, y - (uint16_t)(y_add / K), color);

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

		bsp_wrapper_display_draw_pixel(obj, x - (uint16_t)(x_add / K), y + y_add, color);
		bsp_wrapper_display_draw_pixel(obj, x + (uint16_t)(x_add / K), y + y_add, color);
		bsp_wrapper_display_draw_pixel(obj, x + (uint16_t)(x_add / K), y - y_add, color);
		bsp_wrapper_display_draw_pixel(obj, x - (uint16_t)(x_add / K), y - y_add, color);

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

void bsp_wrapper_display_fill_screen(display_obj_t * obj, uint32_t color)
{
    if( obj->ops->pf_fill_screen)
        obj->ops->pf_fill_screen(color);
}

void bsp_wrapper_display_draw_image(display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t * image_data)
{
    if(obj->ops->pf_copy_buffer)
        obj->ops->pf_copy_buffer(x, y, width, height, image_data);
}

void bsp_wrapper_display_switch_framebuffer(display_obj_t * obj, uint8_t layerx)
{
    if(obj->ops->pf_switch_framebuffer)
        obj->ops->pf_switch_framebuffer(layerx);
}

uint16_t bsp_wrapper_display_get_width(display_obj_t * obj)
{
    if(obj->ops->pf_get_width)
        return obj->ops->pf_get_width();
    return 0;
}

uint16_t bsp_wrapper_display_get_height(display_obj_t * obj)
{
    if(obj->ops->pf_get_height)
        return obj->ops->pf_get_height();
    return 0;
}

uint32_t bsp_wrapper_display_get_framebuffer(display_obj_t * obj)
{
    if(obj->ops->pf_get_framebuffer)
        return obj->ops->pf_get_framebuffer();
    return 0;
}


void bsp_wrapper_display_draw_grad_rgb565(display_obj_t * obj, uint16_t grid_size)
{
    uint16_t x, y;
    uint32_t color;
    uint8_t grid_color;
    uint16_t width = bsp_wrapper_display_get_width(obj);
    uint16_t height = bsp_wrapper_display_get_height(obj);

    for(y = 0; y < height; y++) {
        for(x = 0; x < width; x++) {
            grid_color = ((x / grid_size) + (y / grid_size)) % 3;
            if (grid_color == 0)
                color = DISP_RGB565_RED; 
            else if (grid_color == 1)
                color = DISP_RGB565_GREEN; 
            else
                color = DISP_RGB565_BLUE; 

            bsp_wrapper_display_draw_pixel(obj, x, y, color);
        }
    }
}
/**********************
 *   STATIC FUNCTIONS
 **********************/


/******************************* (END OF FILE) *********************************/
