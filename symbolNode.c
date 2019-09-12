#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
this will be the data structure and functionality used to work the symboltable

*/

typedef struct symbolNode{
    char name[10];
    int tkn;
    struct symbolNode *nextNode;
}symbolNode;

int createNode(char name[10],int tkn,symbolNode* nextNode){
    symbolNode *newNode = (symbolNode*)malloc(sizeof(symbolNode));

    if(newNode == NULL){
        printf("Error creating a new node.\n");
        exit(0);
    }
    strcpy(newNode->name,name);
    newNode->tkn = tkn;
    newNode->nextNode = nextNode;
    return 0;
}

struct symbolNode traverse(char name[10]){
    //go through and find the node with the string matching name
    struct symbolNode tmp = {"1234567890",0,NULL};
    return tmp;
}

int main(void){
    
}
