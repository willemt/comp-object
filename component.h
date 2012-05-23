
#define COMPONENT_MSG_INIT 0
#define COMPONENT_MSG_RELEASE 1

typedef struct
{
    void *in;
    int type;
} component_t;

typedef int (*component_func_release_data) (int event, void *data);
typedef int (*component_func_event) (void *universe, int event, void *comp,
                                     void *subject, void *data);
typedef int (*component_func_init) (void *comp, void *subject);
typedef int (*component_func_release) (void *comp, void *subject);


unsigned long component_hash(const void *a);

long component_cmp(const void *a, const void *b);

long component_get_type(void *comp);

void *component_new(const int type);

void component_free(void *c);


void *component_ctx_new();

void component_ctx_free(void *ctx);

void *component_ctx_get_component_type_from_typeid(void *ctx, const long type);

int component_ctx_get_component_typeid_from_name(void *ctx, const char *name);

void component_ctx_set_universe(void *ctx, void *universe);

void component_ctx_set_func_event(void *ctx, component_func_event func);

void component_ctx_set_func_init(void *ctx, component_func_init func);

void component_ctx_set_func_release(void *ctx, component_func_release func);

void component_ctx_run_event(void *ctx, const int event, void *comp,
                             void *subject, void *data);

void component_ctx_run_init(void *ctx, void *comp, void *subject);

void component_ctx_run_release(void *ctx, void *comp, void *subject);

void *component_ctx_type_new(void *c, const char *name, const long typeid);

/*----------------------------------------------------------------------------*/


void *component_manager_new(void *ctx, void *subject);

void component_manager_free(void *cm);

void *component_manager_add_component(void *cm, const long type);

void component_manager_remove_component(void *cm, const long type);

void *component_manager_get_component(void *cm, const long type);

int component_manager_has_component(void *cm, const long type);

int component_manager_get_num_components(void *cm);

int component_manager_offer_event(void *cm, const long type, void *data);

int component_manager_process_events(void *cm);
