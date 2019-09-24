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




void read_print_line(FILE *input, FILE *output){
    //file and output should be two open files upon the function call
    char buffer[72];
    char *fptr;
    char *bptr;

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
                printf("fptr: %c\n", *fptr);
                struct Lexene relopLM =relop(&fptr,&bptr);
                if(relopLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexene idLM = idres(&fptr,&bptr);
                if(idLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexene realLM = realM(&fptr,&bptr);
                if(realLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexene intLM = intM(&fptr,&bptr);
                if(intLM.tkn != MNOTREC) {
                    continue;
                }
                struct Lexene catchLM = catchallM(&fptr,&bptr);
                if(catchLM.tkn != MNOTREC) {
                    if(catchtkn.tkn == LEXERROR ){
                        fprintf(output,"LEXERR:    Unrecognized Symbol:    %c",*(fptr--));
                    }
                    continue;
                }
            }
        }
    }
}


#endif
