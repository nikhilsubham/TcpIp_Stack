#ifndef __GLUETHREAD__
#define __GLUETHREAD__

typedef struct _glthread{

    struct _glthread *left;
    struct _glthread *right;
} glthread_t;


typedef struct glthread_base{

    glthread_t * base_glthread;
    glthread_t *last_glthread;
} glthread_base_t;


void
glthread_add_next(glthread_base_t *base_glthread_pointer, glthread_t *base_glthread, glthread_t *new_glthread);

void
glthread_add_before(glthread_base_t *base_glthread_pointer, glthread_t *base_glthread, glthread_t *new_glthread);

void
remove_glthread(glthread_base_t *base_glthread_pointer, glthread_t *glthread);

void
init_glthread(glthread_t *glthread);

void
glthread_add_last(glthread_base_t *base_glthread_pointer, glthread_t *new_glthread);

#define IS_GLTHREAD_LIST_EMPTY(glthreadptr)         \
    ((glthreadptr)->right == 0 && (glthreadptr)->left == 0)

#define GLTHREAD_TO_STRUCT(fn_name, structure_name, field_name)                        \
    static inline structure_name * fn_name(glthread_t *glthreadptr){                   \
        return (structure_name *)((char *)(glthreadptr) - (char *)&(((structure_name *)0)->field_name)); \
    }

/* delete safe loop*/
/*Normal continue and break can be used with this loop macro*/

#define BASE(glthreadptr)   ((glthreadptr)->right)

#define ITERATE_GLTHREAD_BEGIN(glthreadptrstart, glthreadptr)                                      \
{                                                                                                  \
    glthread_t *_glthread_ptr = NULL;                                                              \
    glthreadptr = BASE(glthreadptrstart);                                                          \
    for(; glthreadptr!= NULL; glthreadptr = _glthread_ptr){                                        \
        _glthread_ptr = (glthreadptr)->right;

#define ITERATE_GLTHREAD_END(glthreadptrstart, glthreadptr)                                        \
        }}

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthreadptr, offset)  \
    (void *)((char *)(glthreadptr) - offset)

void
delete_glthread_list(glthread_base_t *base_glthread_pointer);

unsigned int 
get_glthread_list_count(glthread_base_t *base_glthread_pointer);

void
glthread_priority_insert(glthread_base_t *base_glthread_pointer,     
                         glthread_t *glthread,
                         int (*comp_fn)(void *, void *),
                         int offset);



#endif /* __GLUETHREAD__ */
