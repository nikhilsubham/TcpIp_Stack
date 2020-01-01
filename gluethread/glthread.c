#include "glthread1.h"
#include <stdlib.h>

void init_glthread(glthread_t *glthread)
{

    glthread->left = NULL;
    glthread->right = NULL;
}


void glthread_add_last(glthread_base_t *base_glthread_pointer, glthread_t *new_glthread)
{
    if(!base_glthread_pointer->base_glthread)
       {
         base_glthread_pointer->base_glthread = new_glthread;
         base_glthread_pointer->last_glthread = new_glthread;
         return;
       }

    if(!new_glthread)
      return;
    
    glthread_t* last  = base_glthread_pointer -> last_glthread;

    last->right = new_glthread;
    new_glthread->left = last;

    base_glthread_pointer -> last_glthread = new_glthread;
    return;
}


void glthread_add_next(glthread_base_t *base_glthread_pointer, glthread_t *curr_glthread, glthread_t *new_glthread)
{

   if(base_glthread_pointer -> base_glthread == NULL && curr_glthread == NULL)
       {
         base_glthread_pointer->base_glthread = new_glthread;
         base_glthread_pointer->last_glthread = new_glthread;
         return;
       }
   
   if(base_glthread_pointer -> base_glthread != NULL && curr_glthread == NULL)
      return;

   if(base_glthread_pointer -> base_glthread == NULL && curr_glthread != NULL)
      return;

   if(!curr_glthread->right)
      {
        curr_glthread->right = new_glthread;
        new_glthread->left = curr_glthread;
        base_glthread_pointer->last_glthread = new_glthread;
        return;
      }

    glthread_t *temp = curr_glthread->right;
    curr_glthread->right = new_glthread;
    new_glthread->left = curr_glthread;
    new_glthread->right = temp;
    temp->left = new_glthread;
    return;
}


void glthread_add_before(glthread_base_t *base_glthread_pointer, glthread_t *curr_glthread, glthread_t *new_glthread){

    if(base_glthread_pointer -> base_glthread == NULL && curr_glthread == NULL)
       {
         base_glthread_pointer->base_glthread = new_glthread;
         base_glthread_pointer->last_glthread = new_glthread;
         return;
       }

    if(base_glthread_pointer -> base_glthread != NULL && curr_glthread == NULL)
      return;

    if(base_glthread_pointer -> base_glthread == NULL && curr_glthread != NULL)
      return;

    if(!curr_glthread->left){
        new_glthread->left = NULL;
        new_glthread->right = curr_glthread;
        curr_glthread->left = new_glthread;
        base_glthread_pointer->base_glthread = new_glthread;
        return;
    }
    
    glthread_t *temp = curr_glthread->left;
    temp->right = new_glthread;
    new_glthread->left = temp;
    new_glthread->right = curr_glthread;
    curr_glthread->left = new_glthread;
    return;
}



void remove_glthread(glthread_base_t *base_glthread_pointer, glthread_t *curr_glthread){
    
    if(base_glthread_pointer -> base_glthread == NULL)
       return;
     
    if(curr_glthread==NULL)
       return;

    if(!curr_glthread->left)
     {
        if(curr_glthread->right)
          {
            base_glthread_pointer->base_glthread = curr_glthread->right;
            curr_glthread->right->left = NULL;
            curr_glthread->right = 0;
            return;
          }
        base_glthread_pointer->base_glthread = NULL;
        base_glthread_pointer->last_glthread = NULL;
        return;
     }
    if(!curr_glthread->right){
        base_glthread_pointer->last_glthread = curr_glthread->left;
        curr_glthread->left->right = NULL;
        curr_glthread->left = NULL;
        return;
    }

    curr_glthread->left->right = curr_glthread->right;
    curr_glthread->right->left = curr_glthread->left;
    curr_glthread->left = 0;
    curr_glthread->right = 0;
}


void delete_glthread_list(glthread_base_t *base_glthread_pointer){

    glthread_t *glthreadptr = NULL;
    glthread_t *base_glthread =  base_glthread_pointer-> base_glthread;
          
    ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr){
        remove_glthread(base_glthread_pointer, glthreadptr);
    } ITERATE_GLTHREAD_END(base_glthread, glthreadptr);
}



unsigned int get_glthread_list_count(glthread_base_t *base_glthread_pointer){

    unsigned int count = 0;
    glthread_t *glthreadptr = NULL;
    glthread_t *base_glthread =  base_glthread_pointer-> base_glthread;

    ITERATE_GLTHREAD_BEGIN(base_glthread, glthreadptr){
        count++;
    } ITERATE_GLTHREAD_END(base_glthread, glthreadptr);
    return count;
}


void glthread_priority_insert(glthread_base_t *base_glthread_pointer,
                         glthread_t *glthread,
                         int (*comp_fn)(void *, void *),
                         int offset){

    glthread_t *curr = NULL,
               *prev = NULL;
    glthread->left = NULL;
    glthread->right = NULL;

    glthread_t *base_glthread = base_glthread_pointer->base_glthread;
    init_glthread(glthread);
    
   if(!base_glthread)
      {
       glthread_add_next(base_glthread_pointer, NULL, glthread);
       return;
      }
   

    /* Only one node*/
    if(!base_glthread->right && !base_glthread->left)
        {
         if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread,offset),GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread,offset))==-1) 		     glthread_add_before(base_glthread_pointer, base_glthread,glthread);
         else
             glthread_add_next(base_glthread_pointer, base_glthread, glthread);
         return;
       }

    while(base_glthread)
     {
        if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread, offset), GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread, offset)) == 1)
          {
              base_glthread = base_glthread->right;
              continue;
          }
        else
          {
             glthread_add_before(base_glthread_pointer, base_glthread, glthread);
             return;
          }
     }

    /*Add in the end*/
    glthread_add_last(base_glthread_pointer, glthread);
} 


