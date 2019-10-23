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
    int eofbreak = 0;
    restable = createNode(NULL,"restablehead");
    idtable = createNode(NULL,"idtablehead");
    //creates a reserved word table
    createTable(restable);
    //traverseList(restable);

    if(input == NULL || listF == NULL){
        printf("File not opened\n");
    }
    else{
        int linecnt = 0;
        //load new line into buffer
        fprintf(tokenF, "Line No.   Lexeme         TOKEN-TYPE    ATTRIBUTE\n");

        while(1){

            fgets(buffer, sizeof(buffer), input);

            if(feof(input)){
                fprintf(tokenF, "           EOF            %-2d (EOF)      %-d    (NULL)\n", EOF,0);
                break;
            }
            //this makes sure that a line greater than 70 characters will be broken up
            buffer[70]='\n';
            buffer[71]='\0';

            //set up pointers for buffer
            fptr=buffer;
            bptr=buffer;
            linecnt++;
            
            //make listing file
            fprintf(listF,"%d    %s",linecnt,buffer);

            while(1){

                struct Lexeme wsLM = whiteSpace(&fptr,&bptr);
                printf("wsLM tkn: %d wsLM attr: %d\n", wsLM.tkn,wsLM.attr.val);
                if(wsLM.tkn != MNOTREC){
                    if(wsLM.attr.val == WSPACE_NL){
                        break;
                    }
                    if(wsLM.tkn == LEXERROR){

                    }
                    continue;
                }


                struct Lexeme idLM = idres(&fptr,&bptr,restable,idtable);
                printf("idLM tkn: %d\n", idLM.tkn);
                //printf("linecnt: %d %s %d tkn %d val %p ptr\n",linecnt,idLM.word,idLM.tkn,idLM.attr.val,idLM.attr.ptr);
                if(idLM.tkn != MNOTREC){
                    if(idLM.tkn == LEXERROR){
                        fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (IDTOOLONG)\n",linecnt,idLM.word,idLM.tkn,"(LEXERROR)",0);
                        fprintf(listF, "LEXERR:     Identifier is greater than 10 characters %s\n",idLM.word);
                        continue;
                    }
                    else if(idLM.tkn != NOOP && strcmp(idLM.word,"NULL") != 0 ){
                        if(idLM.tkn == ID){
                            //printf( "ID adding word: %s | ptr: %p\n", idLM.word,idLM.attr.ptr);
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14p (ptr2symb tbl)\n",linecnt,idLM.word,idLM.tkn,"(ID)",idLM.attr.ptr);
                        }
                        else{
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (NULL)\n",linecnt,idLM.word,idLM.tkn,"(resword)",idLM.attr.val);
                        }

                    }
                    continue;
                }


                struct Lexeme relopLM = relop(&fptr,&bptr);
                printf("relopLM tkn: %d\n", relopLM.tkn);
                if(relopLM.tkn != MNOTREC) {
                    if(relopLM.tkn == LEXERROR){
                        continue;
                    }
                    else{
                        if(strcmp(relopLM.word,"=")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (RELOP_EQ)\n",linecnt,relopLM.word,relopLM.tkn,"(RELOP)",relopLM.attr.val);
                        else if(strcmp(relopLM.word,"<")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (RELOP_LT)\n",linecnt,relopLM.word,relopLM.tkn,"(RELOP)",relopLM.attr.val);
                        else if(strcmp(relopLM.word,"<=")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (RELOP_LE)\n",linecnt,relopLM.word,relopLM.tkn,"(RELOP)",relopLM.attr.val);
                        else if(strcmp(relopLM.word,"<>")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (RELOP_NE)\n",linecnt,relopLM.word,relopLM.tkn,"(RELOP)",relopLM.attr.val);
                        else if(strcmp(relopLM.word,">")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (RELOP_GT)\n",linecnt,relopLM.word,relopLM.tkn,"(RELOP)",relopLM.attr.val);
                        else if(strcmp(relopLM.word,"<=")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (RELOP_GE)\n",linecnt,relopLM.word,relopLM.tkn,"(RELOP)",relopLM.attr.val);
                    }
                    continue;
                }


                struct Lexeme realLM = realM(&fptr,&bptr);
                printf("realLM tkn: %d\n", realLM.tkn);
                if(realLM.tkn != MNOTREC) {
                    if(realLM.tkn == LEXERROR){
                        if(realLM.attr.val == REALFTOOLONG){
                            fprintf(listF, "LEXERR:     Real number front is greater than 5 digits: %s\n",realLM.word);
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (REALFRONTTOOLONG)\n",linecnt,realLM.word,realLM.tkn,"(LEXERROR)",realLM.attr.val);
                        }
                        else if(realLM.attr.val == REALBTOOLONG){
                            fprintf(listF, "LEXERR:     Real number back is greater than 5 digits: %s\n",realLM.word);
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (REALBACKTOOLONG)\n",linecnt,realLM.word,realLM.tkn,"(LEXERROR)",realLM.attr.val);
                        }
                        else if(realLM.attr.val == REALETOOLONG){
                            fprintf(listF, "LEXERR:     Real number exponent is greater than 2 digits: %s\n",realLM.word);
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (REALEXPONENTTOOLONG)\n",linecnt,realLM.word,realLM.tkn,"(LEXERROR)",realLM.attr.val);
                        }
                        continue;
                    }
                    else{
                        fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (REAL)\n",linecnt,realLM.word,realLM.tkn,"(LEXERROR)",realLM.attr.val);
                    }
                }


                struct Lexeme intLM = intM(&fptr,&bptr);
                printf("intLM tkn: %d\n", intLM.tkn);
                if(intLM.tkn != MNOTREC) {
                    if(intLM.tkn == LEXERROR){
                        if(intLM.attr.val == INTTOOLONG){
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (IntTooLong)\n",linecnt,intLM.word,intLM.tkn,"(LEXERROR)",intLM.attr.val);
                            fprintf(listF, "LEXERR:     Integer is greater than 10 digits: %s\n",intLM.word);
                        }
                        continue;
                    }
                    continue;
                }


                struct Lexeme catchLM = catchallM(&fptr,&bptr,input);
                printf("catchLM tkn: %d catchLM wrd: %s\n", catchLM.tkn,catchLM.word);
                if(catchLM.tkn != MNOTREC) {
                    if(catchLM.tkn == LEXERROR ){
                        if(catchLM.attr.val == UNKNOWNSYMBOL){
                            fprintf(listF,"LEXERR:      Unrecognized Symbol:    %c\n",*(fptr - 1));
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (Unrecog Symbol)\n",linecnt, catchLM.word,catchLM.tkn,"(LEXERROR)",catchLM.attr.val);
                        }
                        else if(catchLM.attr.val == PARENMISMATCH){
                            fprintf(listF,"LEXERR:      Parenthesis mismatch\n");
                        }
                        else if(catchLM.attr.val == BRACKMISMATCH){
                            fprintf(listF,"LEXERR:      Brackets mismatch\n");
                        }
                    }
                    /*
                    if(catchLM.tkn == EOF){
                        eofbreak = 1;
                        fprintf(tokenF, "           EOF            %-2d (EOF)      %-d    (NULL)\n", catchLM.tkn,catchLM.attr.val);
                        break;
                    }*/
                    else{
                        if(strcmp(catchLM.word,":=")==0)
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (ASSIGNOP)\n",linecnt, catchLM.word,catchLM.tkn,"(ASSIGNOP)",catchLM.attr.val);
                        else
                            fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (Symbol)\n",linecnt, catchLM.word,catchLM.tkn,"(SYMBOL)",catchLM.attr.val);
                    }
                    continue;
                }
            }
            if(eofbreak == 1){
                //printf("breaking hard\n");
                break;
            }

        }
    }
}


#endif
