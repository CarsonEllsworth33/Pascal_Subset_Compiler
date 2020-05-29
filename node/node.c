#ifndef NODE_H
#define NODE_H
/*
What needs to be done:
    proper working nodes in terms of creation addressing printing
    node scope structure

What is done:
    rough creation of node structure as well as some functionality
    nodes can be created and added to a blue node stack

Possible problems:
    does the blue node need to be able to have children nodes?
    answer: No not if blue nodes are made children of the function nodes they are apart of
    solution: blue nodes need to be able to be children
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexeme.c"

#define GREEN 101 //function or program
#define BLUE 100 //declared variable
#define YELLOW 99 //function parameter/variable

/*
All functions that have parameters require the first parameter to be treated as a child node
*/
char np_name[15];

struct node{
    int type;
    int color;
    char name[15];
    struct node *next;
    struct node *prev;
    struct node *child;
    struct node *parent;
};
typedef struct node *node_ptr;

struct node_stack{
    struct node *stackbase;
    struct node *stacktop;
};
typedef struct node_stack *stack_ptr;


/*
this just adds a node by moving the stack top pointer arround
*/
void add_make_node(stack_ptr st, char name[15], int type, int color,int down_scope){
    node_ptr new_node = (node_ptr)malloc(sizeof(struct node));//create new_node
    strcpy(new_node->name,name);
    new_node->color = color;
    new_node->type = type;

    if (st->stackbase == NULL){
        printf("Setting up Stack\n" );
        st->stackbase = new_node;
        st->stacktop = new_node;
    }
    else if(down_scope){
        st->stacktop->child = new_node;
        new_node->parent = st->stacktop;
        st->stacktop = new_node;
    }
    else{
        st->stacktop->next = new_node;
        new_node->prev = st->stacktop;
        st->stacktop = new_node;
    }
}

/*
This is the method used for creating new nodes for the stack
*/
int check_add_node(stack_ptr st,char name[15],int type, int color,int down_scope){
    //only check within local scope
        node_ptr temp = st->stacktop;
        if (st->stacktop != NULL){
            while(strcmp(temp->name,name)!=0){
                //not same keep searching
                if(temp->prev == NULL){
                    if(temp->parent != NULL){
                        printf("reached base of scope, valid to add node\n");
                        add_make_node(st,name,type,color,down_scope);
                        return 0;
                    }
                    else{
                        if(temp == st->stackbase){
                            printf("reached base node, valid to add node\n");
                            add_make_node(st,name,type,color,down_scope);
                            return 0;
                        }
                        else{
                            //throw
                        }
                    }
                }
                else{
                    temp = temp->prev;
                }
            }

            //found node with same name say no node no
            printf("Found Node with same name within scope, cannot create new node\n");
            return -1;
        }
        else{
            add_make_node(st,name,type,color,down_scope);
            return 0;
        }
}




node_ptr pop_node(stack_ptr st){
    node_ptr cur = st->stacktop;
    node_ptr prev;
    if(cur != NULL){
        if (cur->prev == NULL){
            printf("end of scope, going up scope level\n");
            if(cur->parent != NULL) {
                prev = cur->parent;
                prev->child = NULL;
                st->stacktop = prev;
            }
            else {
                printf("No parent node\n");
                st->stacktop = NULL;
                st->stackbase = NULL;
            }
            printf("poped node: %s\n",cur->name);
            return cur;
        }
        else {
            prev = cur->prev;
            prev->next = NULL;
            st->stacktop = prev;
            printf("poped node: %s\n",cur->name);
            return cur;
        }
    }
    else{
        printf("Stack is empty!! Returning NULL\n");
        return st->stacktop;
    }
}

void up_scope(stack_ptr st){
    node_ptr temp = st->stacktop;
    if(temp == NULL) {
        printf("Stack is empty\n" );
        return;
    }
    while (temp->parent == NULL){
        if(temp == st->stackbase){
            printf("Stackbase reached\n");
            return;
        }
        temp=temp->prev;
    }
    temp = temp->parent;
    st->stacktop = temp;
    printf("scope up new stacktop: %s\n",temp->name);
}

/*
this method searches a stack within a scope level to see if a node is present in the stact
returns pointer to node if found NULL otherwise
*/
node_ptr up_stream_node_find(stack_ptr st, char name[15]){
    node_ptr temp = st->stacktop;
    if (temp == NULL) return NULL;
    while (temp != st->stackbase){
        if(strcmp(temp->name,name) == 0){
            //congrats a node within scope level is Found
            return temp;
        }
        if(temp->prev == NULL){
            //go up to parent
            temp = temp->parent;
        }
        else{temp = temp->prev;}
    }
    temp = st->stackbase;
    if(strcmp(temp->name,name) == 0){
        //congrats a node within scope level is Found
        return temp;
    }
    return NULL;
}

char* near_parent(stack_ptr st, char name[15]){
    node_ptr temp = up_stream_node_find(st,name);
    if(temp == NULL){
        printf("UNABLE TO FIND NODE\n");
        return NULL;
    }
    while(temp->parent == NULL){
        if(temp->prev == NULL){
            printf("UNABLE TO FIND PARENT\n");
            return "NA";
        }
        temp=temp->prev;
    }
    temp = temp->parent;
    strcpy(np_name,temp->name);
    printf("TEST: np_name %s!!!!!!!!!!!!!!!\n", np_name);
    return np_name;
}


int get_id_type(stack_ptr st, char name[15]){
    node_ptr temp = up_stream_node_find(st,name);
    if(temp == NULL){
        printf("NO ID WITH NAME %s WITHIN SCOPE\n", name);
        return TYPEERR;
    }
    return temp->type;
}

int set_id_type(stack_ptr st, char name[15],int type){
    node_ptr temp = up_stream_node_find(st,name);
    if(temp == NULL){
        printf("NO ID WITH NAME %s\n",name);
        return -1;
    }
    temp->type = type;
    return 0;
}

int in_scope(stack_ptr st, char name[15]){
    node_ptr temp = up_stream_node_find(st,name);
    if(temp == NULL){
        return -1;//no node in scope
    }
    return 1;
}

int get_func_param_count(stack_ptr st, char name[15]){
    node_ptr temp = up_stream_node_find(st,name);
    int params = 0;
    if(temp == NULL){
        printf("no func with name %s\n",name );
        return -1;
    }
    if(temp->color != GREEN){
        printf("%s is not a function\n",name);
        return -2;
    }
    if(temp->child == NULL){
        return 0;
    }
    if(temp->child->color != YELLOW){
        return 0;
    }
    //the first child node must now be yellow
    temp = temp->child;
    params++;
    while(temp->color == YELLOW && temp->next != NULL){
        temp=temp->next;
        if(temp->color!=YELLOW){
            break;
        }
        params++;
    }
    return params;
}

int get_func_param_type(stack_ptr st, char name[15],int param_num){
    node_ptr temp = up_stream_node_find(st,name);
    if(temp == NULL){
        printf("no func with name %s\n",name );
        return -1;
    }
    int max_param_num = get_func_param_count(st,name);
    if(max_param_num == 0){
        return -2;//no parameters for function
    }
    if(max_param_num < param_num){
        return -3;//too many function parameters in function call
    }
    int itter=1;
    temp = temp->child;
    while(param_num != itter){
        temp = temp->next;
        itter++;
    }
    return temp->type;
}

void print_stack(stack_ptr st){
    node_ptr temp = st->stackbase;
    if(st->stackbase != NULL){
        printf("stackbase: %s\nstacktop: %s\n",st->stackbase->name,st->stacktop->name);
        while (temp->next != st->stacktop){
            if(temp->next == NULL) {
                if(temp->child != NULL){
                    printf("node name: %s type: %d color: %d\n",temp->name,temp->type,temp->color);
                    temp = temp->child;
                    continue;
                }
                else break;
            }
            printf("node name: %s type: %d color: %d\n",temp->name,temp->type,temp->color);
            temp = temp->next;
        }
        printf("node name: %s type: %d color: %d\n",temp->name,temp->type,temp->color);
        printf("node name: %s type: %d color: %d\n",st->stacktop->name,st->stacktop->type,st->stacktop->color);
    }
    else{
        printf("Empty Stack\n");
    }
}

#endif
