
#include "glthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
void init_glthread(glthread_t *glthread){
    glthread->left = NULL;
    glthread->right = NULL;
}

void glthread_add_right(glthread_t *curr_glthread, glthread_t *new_glthread){

  if(!curr_glthread->right){
    curr_glthread->right = new_glthread;
    new_glthread->left = curr_glthread;
    return;
  }

  glthread_t* temp = curr_glthread->right;
  curr_glthread->right = new_glthread;
  new_glthread->left = curr_glthread;
  new_glthread->right = temp;
  temp->left = new_glthread;
}

void glthread_add_left(glthread_t *curr_glthread, glthread_t *new_glthread){

  if(!curr_glthread->left){
    new_glthread->left = NULL;
    new_glthread->right = curr_glthread;
    curr_glthread->left = new_glthread;
    return;
  }

  glthread_t* temp =curr_glthread->left;
  new_glthread->left = temp;
  new_glthread->right = curr_glthread;
  curr_glthread->left = new_glthread;
  temp->right = new_glthread;
}


void remove_glthread(glthread_t *curr_glthread){

  if(!curr_glthread->left){
    glthread_t* temp = curr_glthread->right;
    if(temp) {
      temp->left = NULL;
      curr_glthread->right = NULL;
    }

    return;
  }

  if(!curr_glthread->right) {
    glthread_t* temp = curr_glthread->left;
    temp->right = NULL;
    curr_glthread->left = NULL;

    return;
  }

  glthread_t* left_thread = curr_glthread->left;
  glthread_t* right_thread = curr_glthread->right;
  left_thread->right = right_thread;
  right_thread->left = left_thread;
  curr_glthread->left = NULL;
  curr_glthread->right = NULL;
}

void glthread_add_last(glthread_t *base_glthread, glthread_t *new_glthread){
  glthread_t* crr = NULL;
  glthread_t* prev = NULL;

  ITERATE_GLTHREAD_BEGIN(base_glthread, crr){
    prev = crr;
  } ITERATE_GLTHREAD_END(base_glthread, crr)

  if(prev)
    glthread_add_right(prev, new_glthread);
  else
    glthread_add_right(base_glthread, new_glthread);
}

void glthread_priority_insert(glthread_t *base_glthread,
                              glthread_t *glthread,
                              int (*comp_fn)(emp_t*, emp_t*),
                              int offset) {
  glthread_t* crr = NULL,
            * prev = NULL;

  init_glthread(glthread);

  if(IS_LIST_EMPTY(base_glthread)){
    // printf("Case-1\n");
    glthread_add_right(base_glthread, glthread);
    return;
  }

  if(base_glthread->right && !base_glthread->right->right) {
    // printf("Case-2\n");
    if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread->right, offset),
        GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset)) == -1){
          // printf("Case-2-2\n");
          glthread_add_right(base_glthread->right, glthread);
        }
        else {
          glthread_add_right(base_glthread, glthread);
        }
        return;
  }

  if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset),
        GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread->right, offset)) == -1) {
          glthread_add_right(base_glthread, glthread);
          // printf("Case-3");
          return;
        }

  ITERATE_GLTHREAD_BEGIN(base_glthread, crr){
    // printf("Case-4");
    if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset),
      GLTHREAD_GET_USER_DATA_FROM_OFFSET(crr, offset)) != -1) {
        prev =crr;
        continue;
      }

    if(!prev)
      glthread_add_right(base_glthread, glthread);

    else
      glthread_add_right(prev,glthread);

    return;
  }ITERATE_GLTHREAD_END(base_glthread, crr);
  // printf("Case-5");
  glthread_add_right(prev, glthread);

}
