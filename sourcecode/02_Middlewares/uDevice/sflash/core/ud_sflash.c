#include "./ud_sflash.h"




void ud_sflash_get_jedec_id(struct ud_sflash * self, uint32_t * jedec_id)
{
    sflash_get_jedec_id_fn_t fn = (*self->ops)->get_jedec_id;

    if(fn) 
        fn(self->ops, jedec_id);
}

void ud_sflash_read(struct ud_sflash * self, uint32_t address, uint8_t * dst, uint32_t length)
{
    sflash_read_fn_t fn = (*self->ops)->read;

    if(fn)
        fn(self->ops, address, dst, length);
}


void ud_sflash_write(struct ud_sflash * self, uint32_t address, const uint8_t * src, uint32_t length)
{
    sflash_write_fn_t fn = (*self->ops)->write;

    if(fn)
        fn(self->ops, address, src, length);
}

void ud_sflash_erase_chip(struct ud_sflash * self)
{
    sflash_erase_chip_fn_t fn = (*self->ops)->erase_chip;

    if(fn)
        fn(self->ops);
}

bool ud_sflash_erase(struct ud_sflash * self, uint32_t address, uint32_t length)
{
    bool result = true;

    /* check the flash address bound */
    if (address + length > self->chip_capacity) {

        return false;
    }

    if (address == 0 && length == self->chip_capacity) {
        
         ud_sflash_erase_chip(self);
        
        return true;
    }

    size_t e_index;

    /* Find the suitable eraser.
     * The largest size eraser is at the end of eraser table.
     * In order to decrease erase command counts, so the find process is from the end of eraser table. */
    for (e_index = SFLASH_ERASE_TYPE_MAX_NUM - 1;; e_index--) {

        if (((*self->ops)->eraser[e_index].erase_size != 0) && 
            (length >= (*self->ops)->eraser[e_index].erase_size) && 
            (address % (*self->ops)->eraser[e_index].erase_size == 0)) 
        {
            break;
        }

        if (e_index == 0) {
            break;
        }
    }

    if(((*self->ops)->eraser[e_index].erase_size == 0) ||
       ((*self->ops)->eraser[e_index].erasing == NULL)) {
        /* No suitable eraser found */
        return false;
    }

    size_t e_size = (*self->ops)->eraser[e_index].erase_size;

    /* loop erase operate. erase unit is erase granularity */
    while (length) {

        result = (*self->ops)->eraser[e_index].erasing(self->ops, address, length);
        if (result != true) {
            goto __exit;
        }


        /* make erase align and calculate next erase address */
        if (address % e_size != 0) {
            if (length > e_size - (address % e_size)) {

                length -= e_size - (address % e_size);
                address += e_size - (address % e_size);
            } 
            else {
                goto __exit;
            }
        } 
        else {
            if (length > e_size) {

                length -= e_size;
                address += e_size;
            } 
            else {
                goto __exit;
            }
        }
    }

__exit:

    return result;
}
