
#include "component.h"
#include "linked_list_hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*----------------------------------------------------------------------------*/

typedef struct
{
    char *name;
} component_type_t;

typedef struct
{
    component_func_release_data release_data_func;
    component_func_event event_func;
    component_func_init init_func;
    component_func_release release_func;

    hashmap_t *ctypes;
    hashmap_t *ctypes_by_name;

    void *universe;
} _ctx_t;

/*----------------------------------------------------------------------------*/

static unsigned long __string_hash(const void *e1)
{
    const unsigned char *str = e1;
    unsigned long hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;

}

static long __string_compare(const void *e1, const void *e2)
{

    return strcmp(e1, e2);
}

static unsigned long __ulong_hash(const void *e1)
{
    const long i1 = (unsigned long) e1;

    assert(i1 >= 0);

    return i1;
}

static long __ulong_compare(const void *e1, const void *e2)
{
    const long i1 = (unsigned long) e1, i2 = (unsigned long) e2;

    return i1 - i2;
}

/*----------------------------------------------------------------------------*/
void *component_ctx_new()
{
    _ctx_t *ctx;

    ctx = calloc(1, sizeof(_ctx_t));
    ctx->ctypes = hashmap_new(__ulong_hash, __ulong_compare);

    return ctx;
}

void component_ctx_free(void *ctx)
{
    free(ctx);
}

void *component_ctx_get_component_type_from_typeid(void *ctx, const long type)
{
    return NULL;
}

int component_ctx_get_component_typeid_from_name(void *ctx, const char *name)
{
    return 0;
}

/*----------------------------------------------- Functions run on components */
void component_ctx_set_universe(void *ctx, void *universe)
{
    _ctx_t *c = ctx;

    c->universe = universe;
}

void component_ctx_set_func_release_data(void *ctx,
                                         component_func_release_data func)
{
    _ctx_t *c = ctx;

    c->release_data_func = func;
}

void component_ctx_set_func_event(void *ctx, component_func_event func)
{
    _ctx_t *c = ctx;

    c->event_func = func;
}

void component_ctx_run_data_release(void *ctx, const int type, void *data)
{
    _ctx_t *c = ctx;

    if (c->release_data_func)
        c->release_data_func(type, data);
}

void component_ctx_run_event(void *ctx, const int event, void *comp,
                             void *subject, void *data)
{
    _ctx_t *c = ctx;

    if (c->event_func)
        c->event_func(c->universe, event, comp, subject, data);
}

/*----------------------------------------------------------------------------*/

/**
 * create a new Component Type
 * Don't allow the creation of duplicates
 * @return new component type, otherwise NULL */
void *component_ctx_type_new(void *c, const char *name, const long typeid)
{
    _ctx_t *ctx = c;


    assert(name);
    assert(0 < typeid);


    /*  don't add same component type again */
    if (hashmap_get(ctx->ctypes, (void *) typeid))
    {
        return NULL;
    }
    else
    {
        component_type_t *ct;

        ct = calloc(1, sizeof(component_type_t));
        ct->name = strdup(name);
        hashmap_put(ctx->ctypes, (void *) typeid, ct);
        return ct;
    }
}

/*----------------------------------------------------------------------------*/

/**
 * get the id of this type fro the name
 * @return -1 on error, type id otherwise
 */
int component_ctx_get_component_type_id_from_name(const char *name)
{
    return 0;
}
