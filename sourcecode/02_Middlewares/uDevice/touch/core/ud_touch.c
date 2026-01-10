
#include "./ud_touch.h"


void ud_touch_register()
{

}

void ud_touch_prepare()
{

}

bool ud_touch_read_point(struct ud_touch * self, struct ud_touch_point * points, uint8_t point_num)
{
    touch_read_data_fn_t fn = (*self->ops)->read_data;

    if(fn)
        return fn(self->ops, points, point_num);

    return false;
}

void ud_touch_get_xy(struct ud_touch_point * points, uint16_t * x, uint16_t * y)
{
    *x = points->x;
    *y = points->y;
}

void ud_touch_ioctrl()
{

}
