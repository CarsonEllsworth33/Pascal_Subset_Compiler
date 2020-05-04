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
int linecnt = 1;
char buffer[72];
char *fptr=buffer;
char *bptr=buffer;
int paren_count = 0;
struct Lexeme get_next_token(FILE *input,FILE *listF,FILE *tokenF){
    bptr=fptr;
    if(input == NULL || listF == NULL){
        printf("File not opened\n");
    }
    else{

        struct Lexeme wsLM = whiteSpace(&fptr,&bptr);
        if(wsLM.tkn != MNOTREC){
            if(wsLM.attr.val == WSPACE_NL){//here the machine recognizes newline char
                //get_new_line
                fgets(buffer, sizeof(buffer), input);
                if(feof(input)){
                    struct Lexeme eof_found = {EOF};
                    fprintf(tokenF, "           EOF            %-2d (EOF)      %-d    (NULL)\n", EOF,0);
                    exit(0);
                    return eof_found; //if get_token needs to return lexeme change this
                }
                if(paren_count > 0){
                    fprintf(listF,"Parenthesis mismatch missing %d ) characters\n",paren_count);
                }
                if(paren_count < 0){
                    fprintf(listF,"Parenthesis mismatch missing %d ( characters\n",paren_count*-1);
                }
                paren_count = 0;
                linecnt++;
                //print paren mis count error here
                fprintf(listF,"%d    %s",linecnt,buffer);
                fptr=buffer;
                bptr=buffer;
                //return wsLM.tkn;
                return get_next_token(input,listF,tokenF);
            }
            if(wsLM.tkn == LEXERROR){
                //NO lex errors for wsLM
            }
        }



        struct Lexeme idLM = idres(&fptr,&bptr,restable,idtable);
        if(idLM.tkn != MNOTREC){
            if(idLM.tkn == LEXERROR){
                fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (IDTOOLONG)\n",linecnt,idLM.word,idLM.tkn,"(LEXERROR)",0);
                fprintf(listF, "LEXERR:     Identifier is greater than 10 characters %s\n",idLM.word);
                //return idLM.tkn;
            }
            else if(idLM.tkn != NOOP && strcmp(idLM.word,"NULL") != 0 ){
                if(idLM.tkn == ID){
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14p (ptr2symb tbl)\n",linecnt,idLM.word,idLM.tkn,"(ID)",idLM.attr.ptr);
                    //return idLM.tkn;
                }
                else{
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (NULL)\n",linecnt,idLM.word,idLM.tkn,"(resword)",idLM.attr.val);
                    //return idLM.tkn;
                }
            }
            return idLM;
        }





        struct Lexeme relopLM = relop(&fptr,&bptr);
        //printf("relopLM tkn: %d\n", relopLM.tkn);
        if(relopLM.tkn != MNOTREC) {
            if(relopLM.tkn == LEXERROR){
                //NO LEX ERRORS POSSIBLE HERE
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
            //return relopLM.tkn;
            return relopLM;
        }




        struct Lexeme realLM = realM(&fptr,&bptr);
        //printf("realLM tkn: %d\n", realLM.tkn);
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
                else if(realLM.attr.val == LEADINGZERO){
                    fprintf(listF, "LEXERR:     Real number has leading zero: %s\n",realLM.word);
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (LEADINGZERO)\n",linecnt,realLM.word,realLM.tkn,"(LEXERROR)",realLM.attr.val);
                }
                else if(realLM.attr.val == TRAILINGZERO){
                    fprintf(listF, "LEXERR:     Real number has trailing zero: %s\n",realLM.word);
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (TRAILINGZERO)\n",linecnt,realLM.word,realLM.tkn,"(LEXERROR)",realLM.attr.val);
                }
                //return realLM.tkn;
                return realLM;
            }
            else{
                fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (REAL)\n",linecnt,realLM.word,realLM.tkn,"(REAL)",realLM.attr.val);
                //return realLM.tkn;
                return realLM;
            }
        }






        struct Lexeme intLM = intM(&fptr,&bptr);
        //printf("intLM tkn: %d\n", intLM.tkn);
        if(intLM.tkn != MNOTREC) {
            if(intLM.tkn == LEXERROR){
                if(intLM.attr.val == INTTOOLONG){
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (IntTooLong)\n",linecnt,intLM.word,intLM.tkn,"(LEXERROR)",intLM.attr.val);
                    fprintf(listF, "LEXERR:     Integer is greater than 10 digits: %s\n",intLM.word);
                }
                //return intLM.tkn;
            }
            else{
                fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (INTEGER)\n",linecnt,intLM.word,intLM.tkn,"(INTEGER)",intLM.attr.val);
            }
            //return intLM.tkn;
            return intLM;
        }





        struct Lexeme catchLM = catchallM(&fptr,&bptr,input);
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

            else{
                if(strcmp(catchLM.word,":=")==0){
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (ASSIGNOP)\n",linecnt, catchLM.word,catchLM.tkn,"(ASSIGNOP)",catchLM.attr.val);
                }
                else{
                    if(strcmp(catchLM.word,"(") == 0){
                        paren_count++;
                    }
                    if(strcmp(catchLM.word,")") == 0){
                        paren_count--;
                    }
                    fprintf(tokenF, "  %-3d      %-14s %-2d %-10s %-14d (Symbol)\n",linecnt, catchLM.word,catchLM.tkn,"(SYMBOL)",catchLM.attr.val);
                }
            }
            //return catchLM.tkn;
            return catchLM;
        }



    }//end top else

}


void read_print_line(FILE *input,FILE *listF,FILE *tokenF){
    struct Lexeme is_eof = get_next_token(input,listF,tokenF);
    while(is_eof.tkn != EOF){
        is_eof = get_next_token(input,listF,tokenF);
    }
}
/*///////////////////////////////////////////////////////////
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
///////////////////////////////////////////////////////////*/


#endif
