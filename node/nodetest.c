#ifndef NODETEST_H
#define NODETEST_H
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

#define GREEN 101
#define BLUE 100

/*
All functions that have parameters require the first parameter to be treated as a child node
*/


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
    else{
        if(down_scope /*&& color == GREEN*/){
            node_ptr temp = st->stacktop;//top stack node
            while(temp->color != GREEN){
                temp=temp->prev;
                if(temp->prev==NULL){
                    printf("NO GREEN NODE PRESENT IN STACK NODE NOT ADDED\n");
                    break;
                }
                printf("in while\n");
            }

            if(temp==NULL);//Do nothing
            else{
                //temp is now a green node
                printf("adding a child node\n");
                //this is the equivalent of going into a scope level
                temp->child = new_node;
                new_node->parent = temp;
                //new scope aquired
                st->stacktop = new_node;
            }

        }
        else{
            st->stacktop->next = new_node;
            new_node->prev = st->stacktop;
            st->stacktop = new_node;
            printf("stacktop name in else: %s\n", st->stacktop->name);
        }
    }
}

/*
This is the method used for creating new nodes for the stack
*/
void check_add_node(stack_ptr st,char name[15],int type, int color,int down_scope){
        node_ptr temp = st->stacktop;
        if (st->stacktop != NULL){
            while(strcmp(temp->name,name)!=0){
                //not same keep searching
                if(temp->prev == NULL){
                    if(temp->parent != NULL){
                        temp = temp->parent;
                    }
                    else{
                        if(temp == st->stackbase){
                            printf("reached base node, valid to add node\n");
                            add_make_node(st,name,type,color,down_scope);
                            return;
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
        }
        else{
            add_make_node(st,name,type,color,down_scope);
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
this method searches a stack within a scope level to see if a node is present in the stackt
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

int get_id_type(stack_ptr st, char name[15]){
    node_ptr temp = up_stream_node_find(st,name);
    if(temp == NULL){
        printf("NO ID WITH NAME %s\n", name);
        return -1;
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
                    printf("node name: %s\n",temp->name);
                    temp = temp->child;
                    continue;
                }
                else break;
            }
            //printf("temp name: %s\n",temp->name);
            temp = temp->next;
        }
        printf("last node on stack: %s\n",st->stacktop->name);
    }
    else{
        printf("Empty Stack\n");
    }
}


/*
struct node_stack stack;
stack_ptr st = &stack;
int main(){
    printf("running\n");
    check_add_node(st,"fun1",TYPEINT,GREEN,0);
    printf("Done1\n" );
    check_add_node(st,"x0",TYPEINT,BLUE,1);
    printf("Done2\n" );
    check_add_node(st,"fun2",TYPEINT,GREEN,0);
    printf("Done3\n");
    check_add_node(st,"x1",TYPEINT,BLUE,1);
    printf("Done4\n" );
    check_add_node(st,"fun3",TYPEINT,GREEN,0);
    printf("Done5\n" );
    check_add_node(st,"fun4",TYPEINT,GREEN,0);
    printf("Done6\n" );
    print_stack(st);
    node_ptr temp = up_stream_node_find(st,"fun1");
    if(temp != NULL){
        printf("%s node found\n", temp->name );
    }
    up_scope(st);
    return 0;
}
*/
#endif
