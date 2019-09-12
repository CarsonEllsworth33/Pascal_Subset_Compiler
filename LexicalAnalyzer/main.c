//this is a test file to try new things I learn in c
#include <stdio.h>



int main(void){
    char letter[3] = {'a','b','c'};
    char *ptr0= letter;
    char **ptr1= &ptr0;
    void *ptr2= *ptr1;
    for(int i = 0; i <= 2; i ++){
        printf("\n");
        printf("pointer0: %p, &pointer0: %p, *pointer0: %c\n", ptr0,&ptr0,*ptr0);
        printf("pointer1: %p, &pointer1: %p, *pointer1: %c\n", ptr1,&ptr1,*ptr1);
        printf("pointer2: %p, &pointer2: %p, *pointer2: %p\n", ptr2,&ptr2,*ptr2);
        (*ptr1)++;
    }
    return 0;
}
