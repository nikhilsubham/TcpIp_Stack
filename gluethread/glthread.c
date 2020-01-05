#include "glthread.h"
#include <stdlib.h>

void init_glthread(glthread_node_t *glthread_node)
{
    glthread_node->left = NULL;
    glthread_node->right = NULL;
}


void glthread_add_last(glthread_base_t *base_glthread_pointer, glthread_node_t *new_glthread_node)
{
    if(!new_glthread_node)
      return;

    if(!base_glthread_pointer->base_glthread_node)
       {
         base_glthread_pointer->base_glthread_node = new_glthread_node;
         base_glthread_pointer->last_glthread_node = new_glthread_node;
         return;
       }

    glthread_node_t* last_node  = base_glthread_pointer -> last_glthread_node;
    last_node->right = new_glthread_node;
    new_glthread_node->left = last_node;
    base_glthread_pointer -> last_glthread_node = new_glthread_node;
    return;
}


void glthread_add_next(glthread_base_t *base_glthread_pointer, glthread_node_t *curr_glthread_node, glthread_node_t *new_glthread_node)
{
   if(!new_glthread_node)
      return;

   if(base_glthread_pointer -> base_glthread_node != NULL && curr_glthread_node == NULL)
      return;
   
   if(base_glthread_pointer -> base_glthread_node == NULL && curr_glthread_node != NULL)
      return;

   if(base_glthread_pointer -> base_glthread_node == NULL && curr_glthread_node == NULL)
       {
         base_glthread_pointer -> base_glthread_node = new_glthread_node;
         base_glthread_pointer -> last_glthread_node = new_glthread_node;
         return;
       }
   
  if(!curr_glthread_node->right)
      {
        curr_glthread_node->right = new_glthread_node;
        new_glthread_node->left = curr_glthread_node;
        base_glthread_pointer->last_glthread_node = new_glthread_node;
        return;
      }

    glthread_node_t *temp_node = curr_glthread_node->right;
    curr_glthread_node->right = new_glthread_node;
    new_glthread_node->left = curr_glthread_node;
    new_glthread_node->right = temp_node;
    temp_node->left = new_glthread_node;
    return;
}


void glthread_add_before(glthread_base_t *base_glthread_pointer, glthread_node_t *curr_glthread_node, glthread_node_t *new_glthread_node)
{
    if(!new_glthread_node)
       return;

    if(base_glthread_pointer -> base_glthread_node != NULL && curr_glthread_node == NULL)
      return;

    if(base_glthread_pointer -> base_glthread_node == NULL && curr_glthread_node != NULL)
      return;

    if(base_glthread_pointer -> base_glthread_node == NULL && curr_glthread_node == NULL)
       {
         base_glthread_pointer->base_glthread_node = new_glthread_node;
         base_glthread_pointer->last_glthread_node = new_glthread_node;
         return;
       }

    if(!curr_glthread_node->left)
      {
        new_glthread_node->left = NULL;
        new_glthread_node->right = curr_glthread_node;
        curr_glthread_node->left = new_glthread_node;
        base_glthread_pointer->base_glthread_node = new_glthread_node;
        return;
      }
    
    glthread_node_t *temp_node = curr_glthread_node->left;
    temp_node->right = new_glthread_node;
    new_glthread_node->left = temp_node;
    new_glthread_node->right = curr_glthread_node;
    curr_glthread_node->left = new_glthread_node;
    return;
}



void remove_glthread(glthread_base_t *base_glthread_pointer, glthread_node_t *curr_glthread_node)
{
    if(curr_glthread_node==NULL)
       return;
    
    if(base_glthread_pointer -> base_glthread_node == NULL)
       return;
     
     if(!curr_glthread_node->left)
     {
        if(curr_glthread_node->right)
          {
            base_glthread_pointer->base_glthread_node = curr_glthread_node->right;
            curr_glthread_node->right->left = NULL;
            curr_glthread_node->right = 0;
            return;
          }
        base_glthread_pointer->base_glthread_node = NULL;
        base_glthread_pointer->last_glthread_node = NULL;
        return;
     }
    if(!curr_glthread_node->right){
        base_glthread_pointer->last_glthread_node = curr_glthread_node->left;
        curr_glthread_node->left->right = NULL;
        curr_glthread_node->left = NULL;
        return;
    }

    curr_glthread_node->left->right = curr_glthread_node->right;
    curr_glthread_node->right->left = curr_glthread_node->left;
    curr_glthread_node->left = 0;
    curr_glthread_node->right = 0;
}


void delete_glthread_list(glthread_base_t *base_glthread_pointer){

    glthread_node_t *glthreadptr_node = NULL;
    glthread_node_t *base_glthread_node =  base_glthread_pointer-> base_glthread_node;
          
    ITERATE_GLTHREAD_BEGIN(base_glthread_node, glthreadptr_node){
        remove_glthread(base_glthread_pointer, glthreadptr_node);
    } ITERATE_GLTHREAD_END(base_glthread, glthreadptr_node);
}



unsigned int get_glthread_list_count(glthread_base_t *base_glthread_pointer){

    unsigned int count = 0;
    glthread_node_t *glthreadptr_node = NULL;
    glthread_node_t *base_glthread_node =  base_glthread_pointer-> base_glthread_node;

    ITERATE_GLTHREAD_BEGIN(base_glthread_node, glthreadptr_node){
        count++;
    } ITERATE_GLTHREAD_END(base_glthread_node, glthreadptr_node);
    return count;
}


void glthread_priority_insert(glthread_base_t *base_glthread_pointer,
                         glthread_node_t *glthread_node,
                         int (*comp_fn)(void *, void *),
                         int offset){

    glthread_node_t *curr = NULL,
                    *prev = NULL;
    glthread_node -> left = NULL;
    glthread_node -> right = NULL;

    glthread_node_t *base_glthread_node = base_glthread_pointer->base_glthread_node;
    init_glthread(glthread_node);
    
   if(!base_glthread_node)
      {
       glthread_add_next(base_glthread_pointer, NULL, glthread_node);
       return;
      }
   
    /* Only one node*/
    if(!base_glthread_node->right && !base_glthread_node->left)
        {
      if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread_node,offset),GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread_node,offset))==-1) 		      glthread_add_before(base_glthread_pointer, base_glthread_node,glthread_node);
         else
              glthread_add_next(base_glthread_pointer, base_glthread_node, glthread_node);
         return;
       }

    while(base_glthread_node)
     {
    if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread_node, offset), GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread_node, offset))== 1)
          {
              base_glthread_node = base_glthread_node->right;
              continue;
          }
        else
          {
             glthread_add_before(base_glthread_pointer, base_glthread_node, glthread_node);
             return;
          }
     }
    /*Add in the end*/
    glthread_add_last(base_glthread_pointer, glthread_node);
} 


