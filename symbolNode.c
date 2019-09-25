#ifndef SYMBOLNODE_H
#define SYMBOLNODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
this will be the data structure and functionality used to work the symboltable

*/

struct symbolNode{
    int tkn;
    int attr;
    struct symbolNode *next;
    char name[];
};

typedef struct symbolNode *symbolNode;

symbolNode createNode(int tkn, char name[],int attr){
    symbolNode newNode = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));

    int counter = 0;
    for(int i =0; i < strlen(name); i++){
        newNode->name[i]=name[i];
        counter++;
    }
    newNode->name[counter] = '\0';
    newNode->tkn = tkn;
    newNode->attr = attr;
    newNode->next = NULL;
    //printf("node name: %s\n", newNode->name);
    return newNode;
}

symbolNode addNode(symbolNode head, int tkn, char name[],int attr){
    symbolNode temp = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    symbolNode p;//= (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    temp = createNode(tkn,name,attr);//this will be added to the back of the list
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

void traverseList(symbolNode head){
    symbolNode p;
    p = head;
    while(p->next != NULL){
        printf("name: %s\n", p->name);
        p = p->next;
    }
    printf("name: %s\n", p->name);
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


symbolNode headNodeSetup(void){
    symbolNode head = createNode(0,"temp",0);
    return head;
}

#endif
