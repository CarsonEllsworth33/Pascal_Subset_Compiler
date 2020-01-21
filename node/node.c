#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexeme.c"

struct bluenode{
    int type;
    char name[15];
    struct bluenode *next;
    struct bluenode *prev;
};
typedef struct bluenode *blueNode;


struct greennode {
    int returnType;
    char name[15];
    blueNode param_list;
    struct greennode *next;
};
typedef struct greennode *greenNode;

int greenNodeParamCount(greenNode gn){
    int count = 0;
    blueNode temp;
    if(gn->param_list != NULL){
        count++;
        temp = gn->param_list;
        while(temp->next != NULL){
            count++;
            temp = temp->next;
        }
    }
    return count;
}

struct bluenode x1 = {0,"x1",NULL,NULL};
struct bluenode x2 = {0,"x2",NULL,&x1};
struct greennode gn1 = {0,"fun1",&x1,NULL};
greenNode gn = &gn1;

int main(){
    x1.next = &x2;
    int count = greenNodeParamCount(gn);
    printf("greennode %s has %d parameters\n", gn->name,count);
    return 0;
}




#endif
