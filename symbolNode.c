#ifndef SYMBOLNODE_H
#define SYMBOLNODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexeme.c"

/*
this will be the data structure and functionality used to work the symboltable

*/

struct symbolNode{
    //int tkn;
    struct Lexeme *lex;
    //struct symbolNode *tbl;
    struct symbolNode *next;
    char name[];
};

typedef struct symbolNode *symbolNode;
typedef struct Lexeme *Lex;

symbolNode createNode(Lex lex/*int tkn*/, char name[]/*,symbolNode tbl*/){
    symbolNode newNode = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    int counter = 0;
    for(int i =0; i < strlen(name); i++){
        newNode->name[i]=name[i];
        counter++;
    }
    newNode->name[counter] = '\0';
    //newNode->tkn = tkn;
    newNode->lex = lex;
    //newNode->tbl = NULL;
    newNode->next = NULL;
    return newNode;
}

symbolNode addNode(symbolNode head, char name[],symbolNode tbl, int tkn, int attr){
    symbolNode temp = (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    symbolNode p;//= (symbolNode)malloc(sizeof(symbolNode) + sizeof(char[strlen(name)]));
    Lex l = (Lex)malloc(sizeof(Lex));
    l->tkn = tkn;
    if(tbl == NULL){
        l->attr.val = attr;
    }
    else{
        l->attr.ptr = tbl;
    }
    temp = createNode(l,name);//this will be added to the back of the list
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
        if(p->lex == NULL){}
        else{
            printf("name: %s ||Lex->tkn: %d\n", p->name,p->lex->tkn);
        }
        p = p->next;
    }
    if(p->lex == NULL){}
    else{
        printf("name: %s ||Lex->tkn: %d\n", p->name,p->lex->tkn);
    }
}



symbolNode isWord(symbolNode head,char name[]){
    //go through and find the node with the string matching name
    symbolNode curNode = head;
    int result;
    while(curNode->next != NULL){
        result = strcmp(curNode->name,name);
        if(result == 0){
            //found node, this means a new node cannot be added
            return curNode;
        }
        curNode = curNode->next;
    }
    result = strcmp(curNode->name,name);
    if(result == 0){
        //found node, this means a new node cannot be added
        return curNode;
    }
    return NULL; //node with name name not found, therefore it is a valid node to add
}

#endif
