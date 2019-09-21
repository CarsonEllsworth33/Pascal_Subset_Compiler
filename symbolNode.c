#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
this will be the data structure and functionality used to work the symboltable

*/

struct symbolNode{
    int tkn;
    struct symbolNode *next;
    char name[];
};

typedef struct symbolNode *symbolNode;

symbolNode createNode(int tkn, char name[]){
    symbolNode newNode = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));

    int counter = 0;
    for(int i =0; i < strlen(name); i++){
        newNode->name[i]=name[i];
        counter++;
    }
    newNode->name[counter] = '\0';
    newNode->tkn = tkn;
    newNode->next = NULL;
    //printf("node name: %s\n", newNode->name);
    return newNode;
}

symbolNode addNode(symbolNode head, int tkn, char name[]){
    symbolNode temp = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    symbolNode p = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    temp = createNode(tkn,name);//this will be added to the back of the list
    if(head == NULL){
        head = temp;
    }
    else{
        p = head;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}



int isResword(symbolNode head,char name[]){
    //go through and find the node with the string matching name
    symbolNode curNode = head;
    int result;
    while(curNode->next != NULL){
        result = strcmp(curNode->name,name);
        if(result == 0){
            //found node, this means a new node cannot be added
            return 1;
        }
        curNode = curNode->next;
    }
    result = strcmp(curNode->name,name);
    if(result == 0){
        //found node, this means a new node cannot be added
        return 1;
    }
    return 0; //node with name name not found, therefore it is a valid node to add
}


symbolNode reswordSetup(void){
    symbolNode head = createNode(0,"temp");
    addNode(head,50,"program");
    addNode(head,51,"var");
    addNode(head,53,"array");
    addNode(head,54,"of");
    addNode(head,55,"integer");
    addNode(head,56,"real");
    addNode(head,58,"function");
    addNode(head,60,"begin");
    addNode(head,61,"end");
    addNode(head,68,"if");
    addNode(head,69,"then");
    addNode(head,70,"else");
    addNode(head,71,"while");
    addNode(head,72,"do");
    addNode(head,73,"not");
    return head;
}
