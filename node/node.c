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
    int node_color;
};
typedef struct bluenode *blueNode;

void blueaddtype(blueNode bn, int type){
    printf("%s now has %d type\n",bn->name,type );
    bn->type = type;
}

struct greennode {
    int returnType;
    char name[15];
    blueNode param_list;
    struct greennode *next_in; //points to a node within its code body
    struct greennode *next_out; //points to the next node out of its body
    struct greennode *prev_in;
    struct greennode *prev_out;
    int node_color;
};
typedef struct greennode *greenNode;

void greenaddtype(greenNode gn, int type){
    printf("%s now has %d return type\n",gn->name,type );
    gn->returnType = type;
}

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


/*
struct programStack{
    void *topNode;
};
typedef struct programStack *stack;

void print_program_stack(stack s){
    void *trav_node = s->topNode;
    void *out_node;
    //BLUE NODE
    if(trav_node->node_color == 0){

        trav_node=trav_node->next;
    }
    //GREEN NODE
    else{

        trav_node=trav_node->next_in;
    }
}
*/



struct bluenode x1 = {0,"x1",NULL,NULL};
struct bluenode x2 = {0,"x2",NULL,&x1};
struct greennode gn1 = {0,"fun1",&x1,NULL};
struct greennode gn2 = {0,"fun2",NULL, NULL};
greenNode gn = &gn1;
greenNode gnn = &gn2;
blueNode bn = &x1;

int main(){
    x1.next = &x2;
    int count = greenNodeParamCount(gn);
    int count2 = greenNodeParamCount(gnn);
    printf("greennode %s has %d parameters\n", gn->name,count);
    printf("greennode2 %s has %d parameters\n", gn->name,count2);
    greenaddtype(gn,TYPEINT);
    blueaddtype(bn,TYPEINT);
    return 0;
}




#endif
