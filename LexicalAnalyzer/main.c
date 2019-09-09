//this is a test file to try new things I learn in c
#include <stdio.h>

char buffer[3]= {' ','  ','\n'};
char *ptr=buffer;

int main(void){

    for(int i = 0; i < 3; i++){
        printf("pointer: %p\nvalue: %d\n",ptr,*ptr);
        ptr++;
    }
    return 0;
}
