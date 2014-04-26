
#include "component.h"
#include "linked_list_hashmap.h"
//#include "cbuffer.h"
//#include "linked_list_queue.h"
#include <stdlib.h>
#include <assert.h>

typedef struct
{
    /**
     * list of components */
    void *ctx;
    hashmap_t *components;
    void *subject;
#if 0
    linked_list_queue_t *event_queue;
#else
    cbuf_t *event_queue;
#endif

} component_manager_t;

typedef struct
{
    int type;
    void *data;
} event_t;

#if 0
static int __has_next(void *data)
{
    tea_iter_t *iter = data;

    component_manager_t *cm;

    cm = iter->data;

    if (!cm)
        return 0;

    if (iter->current >= MAX_COMPONENTS)
//        assert(FALSE);
        return 0;

    /* find used slot */
    for (; iter->current < MAX_COMPONENTS; iter->current++)
    {
        if (cm->components[iter->current].in)
        {
            return 1;
        }
    }

    return 0;
}

static void *__next(void *data)
{
    tea_iter_t *iter = data;

    component_manager_t *cm;

    assert(data);

    cm = iter->data;

    if (!__has_next(iter))
        return NULL;

    return &cm->components[iter->current++];
}

static void __done(void *data)
{
    tea_pool_free(data);
}
#endif

/*----------------------------------------------------------------------------*/

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

/**
 * create a new component manager */
void *component_manager_new(void *ctx, void *subject)
{
    component_manager_t *cm;

    cm = calloc(1, sizeof(component_manager_t));
    cm->components = hashmap_new(__ulong_hash, __ulong_compare,11);
    cm->ctx = ctx;
    cm->subject = subject;
//    cm->event_queue = llqueue_new();
    cm->event_queue = cbuf_new(16);
    return cm;
}

void component_manager_free(void *cm)
{
    component_manager_t *self = cm;

    //llqueue_free(self->event_queue);
    cbuf_free(self->event_queue);
    free(self);
}

/*----------------------------------------------------------------------------*/

/**
 * Add this component to this component manager. */
void *component_manager_add_component(void *cm, const long typeid)
{
    component_manager_t *self = cm;

    /*  don't add same component type again */
    if (hashmap_get(self->components, (void *) typeid))
    {
        return NULL;
    }
    else
    {
        void *comp;

        comp = component_new(typeid);
        hashmap_put(self->components, (void *) typeid, comp);
        component_ctx_run_event(self->ctx, COMPONENT_MSG_INIT, comp,
                                self->subject, NULL);
        return comp;
    }
}

/**
 * Remove this component from this component manager. */
void component_manager_remove_component(void *cm, const long typeid)
{
    component_manager_t *self = cm;
    void *comp;

    comp = hashmap_remove(self->components, (void *) typeid);

    if (comp)
    {
        component_ctx_run_event(self->ctx, COMPONENT_MSG_RELEASE, comp,
                                self->subject, NULL);
        component_free(comp);
    }
}

/**
 * get component instance according to this component type ID*/
void *component_manager_get_component(void *cm, const long typeid)
{
    component_manager_t *self = cm;

    return hashmap_get(self->components, (void *) typeid);
}

/**
 * @return 1 if component manager has this component, 0 otherwise */
int component_manager_has_component(void *cm, const long typeid)
{
    return (NULL != component_manager_get_component(cm, typeid));
}

/*----------------------------------------------------------------------------*/
#if 0
/**
 * iterate through all of the component manager's components */
iterator_t *component_manager_get_component_iter(void *icm)
{
    tea_iter_t *iter;
    component_manager_t *cm = icm;

//    tea_precond(component_managerisEnt(ent));
    iter = calloc(1, sizeof(tea_iter_t));
    iter->data = cm;
    iter->done = __done;
    iter->next = __next;
    iter->has_next = __has_next;
    iter->current = 0;
    return iter;
}
#endif

int component_manager_get_num_components(void *cm)
{
    component_manager_t *self = cm;

    return hashmap_count(self->components);
}

/*----------------------------------------------------------------------------*/

int component_manager_offer_event(void *cm, const long type, void *data)
{
    component_manager_t *self = cm;

#if 0
    event_t *ev;

    ev = calloc(1, sizeof(event_t));
    ev->type = type;
  ev -:data = data;
    llqueue_offer(cm->event_queue, ev);
#else
    event_t ev;

    ev.type = type;
    ev.data = data;
    cbuf_offer(self->event_queue, (void *) &ev, sizeof(event_t));
#endif

    return 1;
}

int component_manager_process_events(void *cm)
{
    component_manager_t *self = cm;
    event_t *ev;

    /* poll from queue */
#if 0
    while ((ev = llqueue_poll(cm->event_queue)))
#else

    while ((ev = (void *) cbuf_poll(self->event_queue, sizeof(event_t))))
#endif
    {
        int *id;
        hashmap_iterator_t iter;

        hashmap_iterator(self->components, &iter);

        /* process on each component */
        while ((id = hashmap_iterator_next(self->components, &iter)))
        {
            void *comp;

            comp = hashmap_get(self->components, (void *) id);
            //printf("running event, ev->type: %d\n", ev->type);
            component_ctx_run_event(self->ctx, ev->type, comp, self->subject,
                                    ev->data);
        }

        /* release data if possible */
        //component_ctx_run_data_release(self->ctx, ev->type, ev->data);
    }

    return 1;
}

/*----------------------------------------------------------------------------*/
