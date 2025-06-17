#include <MCXA153.h>
#include "fifo16.h"

void f16_init(fifo_t *f, uint16_t *buffer, const uint32_t size)
{
#ifdef DEBUG
    if ((size & (size - 1)) != 0)
    {
        while (1) {} // Error: not power of 2
    }
#endif

    f->data = buffer;
    f->head = 0;
    f->tail = 0;
    f->cnt = 0;
    f->size = size;
    f->mask = size - 1;
}

bool f16_push(fifo_t *f, uint16_t data)
{
    if (f->cnt < f->size)
    {
        uint32_t m = __get_PRIMASK();
        __disable_irq();

        f->head = (f->head + 1) & f->mask;
        f->data[f->head] = data;
        f->cnt++;

        __set_PRIMASK(m);
        return true;
    }
    return false;
}

bool f16_pop(fifo_t *f, uint16_t *data)
{
    if (f->cnt > 0)
    {
        uint32_t m = __get_PRIMASK();
        __disable_irq();

        f->tail = (f->tail + 1) & f->mask;
        *data = f->data[f->tail];
        f->cnt--;

        __set_PRIMASK(m);
        return true;
    }
    return false;
}

inline uint32_t f16_cnt(fifo_t *f)
{
    return f->cnt;
}
