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


symbolNode head;

void read_print_line(FILE *input, FILE *output){
    //file and output should be two open files upon the function call
    char buffer[72];
    char *fptr=buffer;
    char *bptr=buffer;
    head = reswordSetup();
    createTable(head);

    if(input == NULL || output == NULL){
        printf("File not opened\n");
    }
    else{
        int linecnt = 0;
        //load new line into buffer
        while( fgets(buffer, sizeof(buffer), input) != NULL){
            fptr=buffer;
            bptr=buffer;
            linecnt++;


            //make listing file
            fprintf(output,"%d    %s",linecnt,buffer);

            while(whiteSpace(&fptr,&bptr).attr.val != WSPACE_NL){
                struct Lexeme relopLM = relop(&fptr,&bptr);
                if(relopLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexeme idLM = idres(&fptr,&bptr,head);
                if(idLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexeme realLM = realM(&fptr,&bptr);
                if(realLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexeme intLM = intM(&fptr,&bptr);
                if(intLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexeme catchLM = catchallM(&fptr,&bptr);
                if(catchLM.tkn != MNOTREC) {
                    if(catchLM.tkn == LEXERROR ){
                        fprintf(output,"LEXERR:    Unrecognized Symbol:    %c\n",*(fptr - 1));
                    }
                    continue;
                }
            }
        }
    }
}


#endif
