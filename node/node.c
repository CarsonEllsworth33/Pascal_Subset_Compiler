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

#define GREEN 101
#define BLUE 100
/*
struct stack {
    void *stackbase;
    void *stacktop;
};
typedef struct stack *stack_ptr;

struct bluenode{
    int type;
    char name[15];
    void *next;
    void *prev;
    //struct bluenode *self; //points to its own address
    int color;
    int tkn;
};
typedef struct bluenode *blueNode;

struct bluestack{
    blueNode topNode;
};
typedef struct bluestack *blstack;

void make_add_blue_node(blstack bs,char name[15],int type,int tkn){
    //create space for a bluenode in memory and return an address to its location
    blueNode new_node = (blueNode) malloc(sizeof(struct bluenode));
    //assign a name to the node
    strcpy(new_node->name,name);
    //assign its address to self
    //new_node->self = new_node;
    new_node->type = type;
    new_node->tkn = tkn;
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
    struct greennode *parent;
    //struct greennode *self;
    void *child; //points to a node within its code body
    void *next; //points to the next node out of its body
    void *prev;
    int color;
    int tkn;
};
typedef struct greennode *greenNode;

void greenaddtype(greenNode gn, int type){
    printf("%s now has %d return type\n",gn->name,type );
    gn->returnType = type;
}
void greenaddparam(int stack,int param_type){

}
/*
void make_add_green_node(stack_ptr st,int returnType,char name[15],int is_child){
    greenNode new_node = (greenNode) malloc(sizeof(struct greennode));
    strcpy(new_node->name,name);
    new_node->returnType = returnType;
    new_node->color = GREEN;
    int set_child = 0;
    if(is_child == 1){
        greenNode temp = st->stacktop;
        while(set_child == 0){
            if (temp->color == GREEN){
                //this node is the parent
                set_child = 1;
                temp->child = new_node;//make temps child the new_node
                new_node->parent = temp;//make new_nodes parent temp

                st->stacktop = new_node;//This line could be wrong
            }
            else if(temp->color == BLUE){
                //go back one node
                if(temp->prev == NULL){
                    printf("ERROR CANNOT ADD NODE AS A CHILD\n");
                }
                else{
                    temp = temp->prev;
                }
            }
        }
    }//END IS_CHILD check
    else{
        greenNode temp = st->stacktop;
        temp->next = new_node;
        new_node->prev = temp;
        st->stacktop = new_node;
    }

}
*/
/*
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
*/

stack_ptr st;
int main(){
    return 0;
}




#endif
