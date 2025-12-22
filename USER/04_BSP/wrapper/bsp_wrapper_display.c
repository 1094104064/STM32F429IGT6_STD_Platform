/**
  ******************************************************************************
  *
  * @file    bsp_wrapper_display.c
  * @author  
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

    .draw_pixel         = bsp_wrapper_display_draw_pixel,
    .draw_line          = bsp_wrapper_display_draw_line,
    .draw_circle        = bsp_wrapper_display_draw_circle,
    .draw_triangle      = bsp_wrapper_display_draw_triangle,
    .draw_rect          = bsp_wrapper_display_draw_rect,
    .draw_arc           = bsp_wrapper_display_draw_arc,
    .draw_ellipse       = bsp_wrapper_display_draw_ellipse,
    .switch_framebuffer = bsp_wrapper_display_switch_framebuffer,
    .draw_full_area     = bsp_wrapper_display_draw_full_area,
    .draw_full_screen   = bsp_wrapper_display_draw_full_screen,
    .draw_bitmap        = bsp_wrapper_display_draw_bitmap,

    .get_width          = bsp_wrapper_display_get_width,
    .get_height         = bsp_wrapper_display_get_height,
    .get_framebuffer    = bsp_wrapper_display_get_framebuffer,

    .backlight_init     = bsp_wrapper_display_backlight_init,
    .backlight_on       = bsp_wrapper_display_backlight_on,
    .backlight_off      = bsp_wrapper_display_backlight_off,
    .backlight_set      = bsp_wrapper_display_backlight_set,

    .draw_grad_rgb565   = bsp_wrapper_display_draw_grad_rgb565,
};
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void mempool_init(void);
static display_obj_t * mempool_alloc(void);
static void mempool_free(display_obj_t * obj);
/**********************
 *  STATIC VARIABLES
 **********************/
static display_obj_t gs_mempool[DISPLAY_MAX_NUM];
static display_obj_t * free_list = NULL;     
static display_obj_t * used_list = NULL; 
static bool gs_mempool_initialized = false;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/ 

display_obj_t * bsp_wrapper_display_create(const display_ops_t * ops, const char * const name, void * const user_data)
{
    if(gs_mempool_initialized == false) {
        mempool_init();
    }

    if(ops == NULL) return NULL;

    if(bsp_wrapper_display_find(name) != NULL) return NULL;

    display_obj_t * obj = mempool_alloc();
    if(obj == NULL) return NULL;

    memset(&obj->ctx, 0, sizeof(display_ctx_t));

    obj->ops= ops;
    obj->ctx.is_initialized = true;
    obj->ctx.user_data = user_data;
    strncpy(obj->ctx.name, name, sizeof(obj->ctx.name) - 1);

    return obj;
}

void bsp_wrapper_display_delete(const char * const name)
{
    display_obj_t * obj = bsp_wrapper_display_find(name);

    if(obj != NULL) {
        mempool_free(obj);
    }
}

display_obj_t * bsp_wrapper_display_find(const char * const name)
{
    display_obj_t * obj = used_list;

    while (obj != NULL) {
        if (strncmp(obj->ctx.name, name, DISPLAY_NAME_MAX_LEN) == 0) {
            return obj;
        }
        obj = obj->next;
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

void bsp_wrapper_display_draw_pixel(display_obj_t * obj, uint16_t x, uint16_t y, uint32_t color)
{
    if(obj->ops->pf_put_pixel)
        obj->ops->pf_put_pixel(x, y, color);
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

void bsp_wrapper_display_draw_full_area(display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    if(obj->ops->pf_fill_area)
        obj->ops->pf_fill_area(x, y, width, height, color);
}

void bsp_wrapper_display_draw_full_screen(display_obj_t * obj, uint32_t color)
{
    if( obj->ops->pf_fill_screen)
        obj->ops->pf_fill_screen(color);
}

void bsp_wrapper_display_draw_bitmap(display_obj_t * obj, uint16_t x, uint16_t y, uint16_t width, uint16_t height, void * data)
{
    if(obj->ops->pf_flush)
        obj->ops->pf_flush(x, y, width, height, data);
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

void bsp_wrapper_display_backlight_init(display_obj_t * obj)
{
    if(obj->ops->pf_backlight_init)
        obj->ops->pf_backlight_init();
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
static void mempool_init(void)
{
    if (free_list != NULL) return;

    memset(gs_mempool, 0, sizeof(gs_mempool));

    for(uint32_t i = 0; i < DISPLAY_MAX_NUM; i++) {
        gs_mempool[i].next = &gs_mempool[i + 1];
    }
    gs_mempool[DISPLAY_MAX_NUM - 1].next = NULL;

    free_list = &gs_mempool[0];
    used_list = NULL;
    gs_mempool_initialized = true;
}

static display_obj_t * mempool_alloc(void)
{
    if(!free_list)
        return NULL;

    display_obj_t * obj = free_list;
    free_list = free_list->next;

    obj->next = used_list;
    used_list = obj;

    return obj;
}

static void mempool_free(display_obj_t * obj)
{
    display_obj_t ** pp = &used_list;

    while(*pp) {
        if(*pp == obj) {
            *pp = obj->next;   
            break;
        }
        pp = &((*pp)->next);
    }

    obj->next = free_list;
    free_list = obj;
}



/******************************* (END OF FILE) *********************************/
