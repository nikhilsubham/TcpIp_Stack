#include "glthread1.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _person{

    int age;
    int weight;
    glthread_t glthread;
} person_t ;

int senior_citizen(void *p1, void *p2){

    person_t *p3 =  (person_t*)p1;
    person_t *p4 =  (person_t*)p2;

    if(p3->age == p4->age) return 0;
    if(p3->age < p4->age) return 1;
    return -1;
}

#define offset(struct_name, fld_name) \
    (unsigned int)&(((struct_name *)0)->fld_name)

//GLTHREAD_TO_STRUCT(thread_to_person, person_t, glthread, glthreadptr);

int main(int argc, char **argv){
    printf("Heello\n");

    person_t person[5];
    memset(person, 0, sizeof(person_t) * 5);
    person[0].age = 1;
    person[0].weight = 2;
    person[1].age = 3;
    person[1].weight = 4;
    person[2].age = 5;
    person[2].weight = 6;
    person[3].age = 7;
    person[3].weight = 8;
    person[4].age = 9;
    person[4].weight = 10;

    glthread_base_t base_glthread_ptr;
    base_glthread_ptr.base_glthread = NULL;
    base_glthread_ptr.last_glthread = NULL;

    glthread_priority_insert(&base_glthread_ptr, &person[0].glthread, senior_citizen, offset(person_t, glthread));
    glthread_priority_insert(&base_glthread_ptr, &person[4].glthread, senior_citizen, offset(person_t, glthread));
    glthread_priority_insert(&base_glthread_ptr, &person[3].glthread, senior_citizen, offset(person_t, glthread));
    glthread_priority_insert(&base_glthread_ptr, &person[2].glthread, senior_citizen, offset(person_t, glthread));
    glthread_priority_insert(&base_glthread_ptr, &person[1].glthread, senior_citizen, offset(person_t, glthread));
    

    remove_glthread(&base_glthread_ptr, base_glthread_ptr.base_glthread);

    glthread_t *curr = NULL;
    glthread_t* base_glthread = base_glthread_ptr.base_glthread;
   
   while(base_glthread)
      {
         person_t *p  =  GLTHREAD_GET_USER_DATA_FROM_OFFSET(base_glthread, offset(person_t, glthread));
         printf("Age = %d\n", p->age);
         base_glthread = base_glthread ->right;
      }

    printf("%d\n", get_glthread_list_count(&base_glthread_ptr));
    return 0;
}
