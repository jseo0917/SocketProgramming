
#ifndef __GLUETHREAD__
#define __GLUETHREAD__

typedef struct _glthread{
    struct _glthread *left;
    struct _glthread *right;
} glthread_t;

typedef struct emp_ {
  char name[30];
  unsigned int salary;
  char destination[30];
  int emp_id;
  glthread_t glthread;
} emp_t;

#define ITERATE_GLTHREAD_BEGIN(glthreadptrstart, glthreadptr)                                      \
{                                                                                                  \
    glthread_t *_glthread_ptr = NULL;                                                              \
    glthreadptr = glthreadptrstart->right;                                                         \
    for(; glthreadptr!= NULL; glthreadptr = _glthread_ptr){                                        \
        _glthread_ptr = (glthreadptr)->right;

#define ITERATE_GLTHREAD_END(glthreadptrstart, glthreadptr)                                        \
        }}

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthreadptr, offset)  \
    (void *)((char *)(glthreadptr) - offset)

// Check if the list is empty
#define IS_LIST_EMPTY(node) \
    ((node->right == 0) && (node->left == 0))

void glthread_add_right(glthread_t *base_glthread, glthread_t *new_glthread);

void glthread_add_left(glthread_t *base_glthread, glthread_t *new_glthread);

void remove_glthread(glthread_t *glthread);

void init_glthread(glthread_t *glthread);

void glthread_add_last(glthread_t *base_glthread, glthread_t *new_glthread);

void glthread_priority_insert(glthread_t* base_glthread,
                              glthread_t* glthread,
                              int (*comp_fn)(emp_t*, emp_t*),
                              int offset);
#endif /* __GLUETHREAD__ */
