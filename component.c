
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "component.h"

/*----------------------------------------------------------------------------*/

#if 0
unsigned long component_hash(const void *a)
{
    const component_t *c;

    c = a;

    return c->type;
}

long component_cmp(const void *a, const void *b)
{

    const component_t *c1, *c2;

    c1 = a;
    c2 = b;

    return c1->type - c2->type;
}
#endif

typedef struct
{
    component_t comp;

} _component_t;

/*----------------------------------------------------------------------------*/

void *component_new(const int type)
{
    _component_t *c;

    c = calloc(1, sizeof(_component_t));
    c->comp.type = type;

    return c;
}

long component_get_type(void *comp)
{
    _component_t *c = comp;

    return c->comp.type;
}

void component_free(void *comp)
{
    _component_t *c = comp;

    free(c);
}
