//Author: Carson Ellsworth
//Date:   08-31-2019
//Class:  CS4013
//proj:   project 0
#ifndef LEXANALYZE_H
#define LEXANALYZE_H


#include <stdio.h>
#include <string.h>
#include "../lexeme.c"
#include "machines.c"
#include "../symbolNode.c"


symbolNode restable;
symbolNode idtable;

void read_print_line(FILE *input, FILE *listF, FILE *tokenF){
    //file and output should be two open files upon the function call
    char buffer[72];
    char *fptr=buffer;
    char *bptr=buffer;
    restable = createNode(NULL,"restablehead");
    idtable = createNode(NULL,"idtablehead");
    //creates a reserved word table
    createTable(restable);
    traverseList(restable);

    if(input == NULL || listF == NULL){
        printf("File not opened\n");
    }
    else{
        int linecnt = 0;
        //load new line into buffer
        fprintf(tokenF, "Line No.   Lexeme        TOKEN-TYPE    ATTRIBUTE\n");
        while( fgets(buffer, sizeof(buffer), input) != NULL){
            fptr=buffer;
            bptr=buffer;
            linecnt++;


            //make listing file
            fprintf(listF,"%d    %s",linecnt,buffer);

            while(whiteSpace(&fptr,&bptr).attr.val != WSPACE_NL){
                struct Lexeme idLM = idres(&fptr,&bptr,restable,idtable);
                if(idLM.tkn != MNOTREC) {
                    continue;
                }
                else if(idLM.tkn == LEXERROR){
                    continue;
                }
                struct Lexeme relopLM = relop(&fptr,&bptr);
                if(relopLM.tkn != MNOTREC) {
                    continue;
                }
                else if(relopLM.tkn == LEXERROR){
                    continue;
                }
                struct Lexeme realLM = realM(&fptr,&bptr);
                if(realLM.tkn != MNOTREC) {
                    continue;
                }
                else if(realLM.tkn == LEXERROR){
                    fprintf(tokenF, "testing\n");
                    continue;
                }
                struct Lexeme intLM = intM(&fptr,&bptr);
                if(intLM.tkn != MNOTREC) {
                    continue;
                }
                else if(intLM.tkn == LEXERROR){
                    continue;
                }
                struct Lexeme catchLM = catchallM(&fptr,&bptr);
                if(catchLM.tkn != MNOTREC) {
                    if(catchLM.tkn == LEXERROR ){
                        fprintf(listF,"LEXERR:    Unrecognized Symbol:    %c\n",*(fptr - 1));
                    }
                    continue;
                }
            }
        }
        traverseList(idtable);
    }
}


#endif
