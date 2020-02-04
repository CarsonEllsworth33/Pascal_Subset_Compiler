#ifndef NODE_H
#define NODE_H
/*
What needs to be done:
    proper working nodes in terms of creation addressing printing
    node scope structure

What is done:
    rough creation of node structure as well as some functionality
    nodes can be created and added to a blue node stack
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexeme.c"

struct bluenode{
    int type;
    char name[15];
    struct bluenode *next;
    struct bluenode *prev;
    struct bluenode *self; //points to its own address
    int tkn;
};
typedef struct bluenode *blueNode;

struct bluestack{
    blueNode topNode;
};
typedef struct bluestack *blstack;

void make_add_blue_node(blstack bs,char name[15],int type,int tkn){
    //create space for a bluenode in memory and return an address to its location
    blueNode new_node = (blueNode) malloc(sizeof(bluenode));
    //assign a name to the node
    strcpy(new_node->name,name);
    //assign its address to self
    new_node->self = new_node;
    new_node->type = type;
    if(bs->topNode == NULL){
        bs->topNode = new_node;
        return;
    }
    else{
        blueNode temp = bs->topNode;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_node;
        new_node->prev = temp;
    }
}

void print_blue_stack(blstack bs){
    blueNode temp;
    //printf("topnode addr: %p \n",bs->topNode);
    temp = bs->topNode;
    //printf("temp addr: %p \n",temp);
    while(temp->next != NULL){
        printf("blue node name: %s type: %d \n",temp->name,temp->type);
        temp = temp->next;
    }
    printf("blue node name: %s type: %d \n",temp->name,temp->type);
}

void blueaddtype(blueNode bn, int type){
    printf("%s now has %d type\n",bn->name,type );
    bn->type = type;
}






struct greennode {
    int returnType;
    char name[15];
    blueNode param_list;
    struct greennode *parent;
    struct greennode *child; //points to a node within its code body
    struct greennode *next; //points to the next node out of its body
    struct greennode *prev;
    struct greennode *self;
    int tkn;
};
typedef struct greennode *greenNode;

void greenaddtype(greenNode gn, int type){
    printf("%s now has %d return type\n",gn->name,type );
    gn->returnType = type;
}

void make_add_greed_node(int return_type,char name[15],int is_child){}

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
struct greenstack{
    greenNode topNode;
    greenNode curNode;
};
typedef struct greenstack *greenstack;



struct bluenode x1 = {0,"x1",NULL,NULL};
struct bluenode x2 = {0,"x2",NULL,&x1};
struct greennode gn1 = {0,"fun1",&x1,NULL};
struct greennode gn2 = {0,"fun2",NULL, NULL};
struct bluestack bs1 = {NULL};
greenNode gn = &gn1;
greenNode gnn = &gn2;
blueNode bn = &x1;
blstack bs = &bs1;

int main(){
    x1.next = &x2;
    int count = greenNodeParamCount(gn);
    int count2 = greenNodeParamCount(gnn);
    make_add_blue_node(bs,"1",TYPEINT,ID);
    print_blue_stack(bs);
    return 0;
}




#endif
