#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "glthread.h"
#include <stddef.h>

int senior_employee(emp_t *e1, emp_t *e2) {
    if(e1->emp_id == e2->emp_id) return 0;
    if(e1->emp_id < e2->emp_id) return 1;
    return -1;
}

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthreadptr, offset)  \
    (void *)((char *)(glthreadptr) - offset)

#define offset(struct_name, fld_name) \
        (unsigned int)&(((struct_name *)0)->fld_name)

int main(){
   emp_t employee[3];
   memset(employee, 0, sizeof(emp_t) * 3);
   strcpy(employee[0].name, "John");
   employee[0].salary = 30000;
   strcpy(employee[0].destination, "LA");
   employee[0].emp_id = 1;

   strcpy(employee[1].name, "Kim");
   employee[1].salary = 40000;
   strcpy(employee[1].destination, "TX");
   employee[1].emp_id = 2;
   printf("%s\n", employee[0].name);
   glthread_t base_glthread;
   init_glthread(&base_glthread);

   glthread_priority_insert(&base_glthread, &employee[1].glthread, senior_employee, offsetof(emp_t, glthread));
   glthread_priority_insert(&base_glthread, &employee[0].glthread, senior_employee, offsetof(emp_t, glthread));

   glthread_t* crr= (&base_glthread)->right;

   size_t b_offset = offsetof(emp_t, salary);
   char* sc = (char*) crr;
   printf("Salary %d\n", *(unsigned int*)(sc+b_offset - (sizeof(emp_t) - sizeof(glthread_t))));
   
}
