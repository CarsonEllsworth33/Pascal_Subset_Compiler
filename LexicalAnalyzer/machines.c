#ifndef MACHINES_H
#define MACHINES_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "../lexeme.c"
#include "../symbolNode.c"
#include "resword.c"

char letter[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char digit[] = {'0','1','2','3','4','5','6','7','8','9'};
int letterSize = sizeof(letter);
int digitSize = sizeof(digit);
int parenOpen = 0;
int parenClose = 0;
int brackOpen = 0;
int brackClose = 0;

struct Lexeme MN = {MNOTREC,TYPEERR,"",{0}};
//MN.tkn = MNOTREC;
//MN.attr.val = MNOTREC;
//symbolNode head = reswordSetup();

struct Lexeme whiteSpace(char **fptr,char **bptr){
    struct Lexeme NL;
    NL.tkn = WSPACE;
    NL.attr.val = WSPACE_NL;
    struct Lexeme BL;
    BL.tkn = WSPACE;
    BL.attr.val = WSPACE_BL;
    struct Lexeme TB;
    TB.tkn = WSPACE;
    TB.attr.val = WSPACE_TB;
    struct Lexeme PE;
    PE.tkn = LEXERROR;
    PE.attr.val = PARENMISMATCH;
    struct Lexeme BE;
    BE.tkn = LEXERROR;
    BE.attr.val = BRACKMISMATCH;





    while(**fptr == ' '){
        //stay in loop and advance till item seen
        (*fptr)++;
        *bptr =  *fptr;
    }

    while(**fptr == '\t'){
        (*fptr)++;
        *bptr =  *fptr;
    }

    if(**fptr == '\n'){
        //get newline
        return NL;
    }

    //*bptr = *fptr; //could be source of problem
    //printf("MACHINE NOT RECOGNIZED WS\n");
    return MN;
}//END WHITESPACE MACHINE









struct Lexeme relop(char **fptr,char **bptr){
    struct Lexeme LT;
    LT.tkn = RELOP;
    LT.attr.val = RELOP_LT;
    struct Lexeme LE;
    LE.tkn = RELOP;
    LE.attr.val = RELOP_LE;
    struct Lexeme GT;
    GT.tkn = RELOP;
    GT.attr.val = RELOP_GT;
    struct Lexeme GE;
    GE.tkn = RELOP;
    GE.attr.val = RELOP_GE;
    struct Lexeme EQ;
    EQ.tkn = RELOP;
    EQ.attr.val = RELOP_EQ;
    struct Lexeme NE;
    NE.tkn = RELOP;
    NE.attr.val = RELOP_NE;

    //this tree takes us to LT,LE,NE
    if(**fptr == '<'){
        LT.word[0] = '<';
        LE.word[0] = '<';
        NE.word[0] = '<';
            (*fptr)++;
            if(**fptr == '='){
                LE.word[1]='=';
                LE.word[2]='\0';
                (*fptr)++;
                *bptr = *fptr;
                return LE;
            }
            else if(**fptr == '>'){
                NE.word[1]='>';
                NE.word[2]='\0';
                (*fptr)++;
                *bptr = *fptr;
                return NE;
            }
            else{
                LT.word[1]='\0';
                *bptr = *fptr;
                return LT;
            }
    }

    //this tree takes us to GT,GE
    if(**fptr == '>'){
        GT.word[0]='>';
        GE.word[0]='>';
            (*fptr)++;
            if(**fptr == '='){
                GE.word[1]='=';
                GE.word[2]='\0';
                (*fptr)++;
                *bptr = *fptr;
                //printf("Greater than or Equal returned\n");
                return GE;
            }
            else{
                GT.word[1]='\0';
                *bptr = *fptr;
                //printf("Greater than returned\n");
                return GT;
            }
    }

    //this tree takes us to EQ
    if(**fptr == '='){
        EQ.word[0] = '=';
        EQ.word[1]='\0';
        (*fptr)++;
        *bptr = *fptr;
        return EQ;
    }
    *bptr = *fptr;
    //printf("MACHINE NOT RECOGNIZED RELOP\n");
    return MN;
}//END RELOP MACHINE





int match(char fptr, char compArr[],int arrlen){

    for (int itter = 0; itter < arrlen; itter++){
        if(fptr == compArr[itter])
            {
                return 1;
            }
    }
    return 0;
}//END MATCH








struct Lexeme idres(char **fptr, char **bptr, symbolNode restable, symbolNode idtable){
    //const char* letter = "[a-zA-Z]";
    //user defined letter and digit alphabet
    /* RESERVED WORDS
    program
    array
    integer
    real
    function
    procedure //dont need to do
    begin
    end
    if
    then
    else
    while
    do
    not
    */

    struct Lexeme idtl;
    idtl.tkn = LEXERROR;
    idtl.attr.val = IDTOOLONG;

    struct Lexeme id;//0 is tmp value, this actually needs to be a pointer
    id.tkn = ID;
    id.attr.val = 0;

    int counter = 0;//counter to make hard things simple
    if(match(**fptr,letter,letterSize) && **fptr != '\n'){
        //**fptr is a letter
        //(*fptr)++;//buffer address pointer
        (*bptr) = (*fptr);
        while((match(**fptr,letter,letterSize) || match(**fptr,digit,digitSize)) && **fptr != '\n'){
            //string[counter] = **fptr;
            (*fptr)++;
            counter++;
        }
        //checks to see if the created string would be too long
        if(counter > 0){

            //while MNOTREC and is less than 10
            char string[counter+1];//temp string to build up identifiers

            for(int i = 0; i <= counter; i++){
                string[i] = **bptr;
                (*bptr)++;
            }

            string[counter] = '\0';

            if(counter > 10){
                //printf("IDTOOLONG\n");
                *bptr = *fptr;
                if(counter > 14){
                    for(int i = 0; i < 14; i++){
                        idtl.word[i]=string[i];
                    }
                    idtl.word[11]='.';
                    idtl.word[12]='.';
                    idtl.word[13]='.';
                    idtl.word[14]='\0';
                    return idtl;
                }
                strcpy(idtl.word,string);
                return idtl;
            }

            symbolNode result = isWord(restable,string);
            if( result == NULL){
                //not a resword
                result = isWord(idtable,string);
                if( result == NULL){
                    //not a resword or a id word, so it can be added as a new node
                    addNode(idtable,string,idtable,ID,0);
                    result = isWord(idtable,string);
                    if( result != NULL){
                        //this is if the word is already in the table
                        struct Lexeme lex = *result->lex;
                        //printf("lex->word %-15s lex->attr %d\n", lex.word,lex.attr.val);
                        return lex;
                    }
                    else{
                        printf("ERROR returning result->lex\n");
                    }
                }
                else{
                    //not resword but is an existing identifier
                    struct Lexeme lex = *result->lex;
                    //printf("lex->word %-15s lex->attr %d\n", lex.word,lex.attr.val);
                    return lex;
                }
            }
            else{
                //this is a resword
                struct Lexeme lex = *result->lex;
                //printf("idres|lex->word %-15s lex->attr %d\n", lex.word,lex.attr.val);
                return lex;
            }
        }
    }
    //printf("MACHINE NOT RECOGNIZED IDRES\n");
    return MN;
}





struct Lexeme realM(char **fptr, char **bptr){
    struct Lexeme realData;
    realData.tkn = NUM;
    realData.attr.val = 0;
    realData.type = TYPEREAL;
    struct Lexeme realerr;
    realerr.tkn = LEXERROR;

    int rcounter = 0;
    int fcounter = 0;
    int ecounter = 0;
    int epresent = 0;

    while(match(**fptr,digit,digitSize)){
        (*fptr)++;
        fcounter++;
    }
    if(fcounter > 0){
        if(**fptr == '.'){
            (*fptr)++;
            while(match(**fptr,digit,digitSize)){
                (*fptr)++;
                rcounter++;
            }
            //printf("rcounter %d\n",rcounter );
            if(rcounter > 0){
                if(**fptr == 'E'){
                    (*fptr)++;//advance past E
                    epresent = 1;
                    while(match(**fptr,digit,digitSize)){
                        (*fptr)++;
                        ecounter++;
                    }
                    if(ecounter == 0){
                        //backtrack fptr 1 and back out of it
                        (*fptr) = (*fptr) - 1;
                        epresent = 0;
                        //printf("REAL NO E fptr: %c bptr: %c\n", **fptr,**bptr);
                    }
                }
            }
            else if(rcounter == 0){
                (*fptr) = (*bptr);
                //printf("MACHINE NOT RECOGNIZED REALM\n");
                return MN;
            }
        }


        else{
            (*fptr) = (*bptr);
            //printf("MACHINE NOT RECOGNIZED REALM\n");
            return MN;
        }

        int counter = fcounter + rcounter + ecounter + epresent;
        //printf("real counter %d\n",counter );
        char realstr[counter + 1];//takes into account the dot and E if present
        int strcntr = 0;
        while(*bptr != *fptr){
            realstr[strcntr] = **bptr;
            (*bptr)++;
            strcntr++;
        }
        realstr[strcntr] = '\0';
        if(fcounter+rcounter+ecounter+epresent > 14){
            for(int i = 0; i < 14; i++){
                realerr.word[i]=realstr[i];
                realerr.word[11]='.';
                realerr.word[12]='.';
                realerr.word[13]='.';
                realerr.word[14]='\0';
            }
        }
        else{
            strcpy(realerr.word,realstr);
        }

        if(fcounter > 5){
            //printf("REALFRONTTOOLONG\n");
            realerr.attr.val = REALFTOOLONG;
            return realerr;//real front too long
        }

        if(rcounter > 5){
            //printf("REALBACKTOOLONG\n");
            realerr.attr.val = REALBTOOLONG;
            return realerr;
        }

        if(ecounter > 2){
            //printf("REALEXPONENTTOOBIG\n");
            realerr.attr.val = REALETOOLONG;
            return realerr;
        }
        if(realstr[0]=='0'){
            //printf("LEADINGZERO\n" );
            realerr.attr.val = LEADINGZERO;
            return realerr;
        }
        if(realstr[fcounter+rcounter]=='0'){
            //printf("TRAILINGZERO\n");
            realerr.attr.val = TRAILINGZERO;
            return realerr;
        }
        strcpy(realData.word,realstr);
        return realData;
    }
    return MN;
}






struct Lexeme intM(char **fptr, char **bptr){
    struct Lexeme intData;
    intData.tkn = NUM;
    intData.attr.ptr = NULL;
    intData.type = TYPEINT;

    struct Lexeme interr;
    interr.tkn = LEXERROR;
    interr.attr.val = INTTOOLONG;

    int counter = 0;
    while(match(**fptr,digit,digitSize)){
        (*fptr)++;
        counter++;
        //printf("int %c\n",**fptr);
    }
    //printf("int breakout on %c\n",**fptr);
    char intstr[counter + 1];
    for(int i = 0; i <= counter; i++){
        intstr[i] = **bptr;
        (*bptr)++;
    }
    intstr[counter] = '\0';
    if(counter > 14){
        for(int i = 0;i<14;i++){
            interr.word[i]=intstr[i];
            interr.word[11]='.';
            interr.word[12]='.';
            interr.word[13]='.';
            interr.word[14]='\0';
        }
    }
    else{
        strcpy(interr.word,intstr);
    }
    if(counter > 10){
        printf("INTEGERTOOLONG\n");
        //*bptr = *fptr;
        return interr;//int too long
    }
    if(counter > 0){
        //printf("Int: %s\n", intstr);
        strcpy(intData.word,intstr);
        return intData;
    }
    //printf("MACHINE NOT RECOGNIZED INT\n");
    return MN;
}





struct Lexeme catchallM(char **fptr,char **bptr, FILE *input){
    //Here we need to support addops(+ - or) mulops(* / div mod and)
    //assignop(:=) doubledot(..) dot(.)
    //parenthesis () and brackets [] need a counter for them
    //and handle any unrecognized symbol
    //div mod and or are all going to be recognized by the

    /*
    incomplete features:
    watching for mismatch count
    of paren and brackets
    */

    struct Lexeme plus;
    plus.tkn = ADDOP;
    plus.attr.val = ADDOP_PL;

    struct Lexeme comma;
    comma.tkn = COMMA;
    comma.attr.val = 0;

    struct Lexeme colon;
    colon.tkn = COLON;
    colon.attr.val = 0;

    struct Lexeme semicolon;
    semicolon.tkn = SEMICOLON;
    semicolon.attr.val = 0;

    struct Lexeme minus;
    minus.tkn = ADDOP;
    minus.attr.val = ADDOP_MN;

    struct Lexeme times;
    times.tkn = MULOP;
    times.attr.val = MULOP_ML;

    struct Lexeme divF; // this is symbol: /
    divF.tkn = MULOP;
    divF.attr.val = MULOP_DVF;

    struct Lexeme assign;
    assign.tkn = ASSIGNOP;
    assign.attr.val = 0;

    struct Lexeme dotdot;
    dotdot.tkn = DOTDOT;
    assign.attr.val = 0;

    struct Lexeme dot;
    dot.tkn = DOT;
    assign.attr.val = 0;

    struct Lexeme openParen;
    openParen.tkn = PAREN;
    openParen.attr.val = PAREN_OPEN;

    struct Lexeme closeParen;
    closeParen.tkn = PAREN;
    closeParen.attr.val = PAREN_CLOSE;

    struct Lexeme openBrack;
    openBrack.tkn = BRACK;
    openBrack.attr.val = BRACK_OPEN;

    struct Lexeme closeBrack;
    closeBrack.tkn = BRACK;
    closeBrack.attr.val = BRACK_CLOSE;

    struct Lexeme eoflex;
    eoflex.tkn = EOF;
    eoflex.attr.val = 0;

    struct Lexeme unrec;
    unrec.tkn = LEXERROR;
    unrec.attr.val = UNKNOWNSYMBOL;

    struct Lexeme badparen;
    badparen.tkn = LEXERROR;
    badparen.attr.val = PARENMISMATCH;

    struct Lexeme badbrack;
    badbrack.tkn = LEXERROR;
    badbrack.attr.val = BRACKMISMATCH;

    if(**fptr == '.'){
        dot.word[0] = **fptr;
        dotdot.word[0] = **fptr;
        (*fptr)++;
        if(**fptr == '.'){
            dotdot.word[1] = **fptr;
            dotdot.word[2] = '\0';
            (*fptr)++;
            *bptr = *fptr;
            return dotdot;
        }
        else{
            dot.word[1] = '\0';
            *bptr = *fptr;
            return dot;
        }
    }

    else if(**fptr == ':'){
        colon.word[0] = **fptr;
        assign.word[0] = **fptr;
        (*fptr)++;
        if(**fptr == '='){
            assign.word[1] = **fptr;
            assign.word[2] = '\0';
            (*fptr)++;
            //printf("assignop returned\n" );
            *bptr = *fptr;
            return assign;
        }
        else{
            colon.word[1] = '\0';
            *bptr = *fptr;
            return colon;
        }
    }

    else if(**fptr == ','){
        comma.word[0] = **fptr;
        comma.word[1] = '\0';
        (*fptr)++;
        *bptr = *fptr;
        return comma;
    }

    else if(**fptr == ';'){
        semicolon.word[0] = **fptr;
        semicolon.word[1] = '\0';
        (*fptr)++;
        *bptr = *fptr;
        return semicolon;
    }

    else if(**fptr == '*'){
        times.word[0] = **fptr;
        times.word[1] = '\0';
        (*fptr)++;
        *bptr = *fptr;
        return times;
    }

    else if(**fptr == '/'){
        divF.word[0] = **fptr;
        divF.word[1] = '\0';
        (*fptr)++;
        *bptr = *fptr;
        return divF;
    }

    else if(**fptr == '+'){
        plus.word[0] = **fptr;
        plus.word[1] = '\0';
        (*fptr)++;
        *bptr = *fptr;
        return plus;
    }

    else if(**fptr == '-'){
        minus.word[0] = **fptr;
        minus.word[1] = '\0';
        (*fptr)++;
        *bptr = *fptr;
        return minus;
    }

    else if(**fptr == '('){
        openParen.word[0] = **fptr;
        openParen.word[1] = '\0';
        (*fptr)++;
        parenOpen++;
        *bptr = *fptr;
        /*
        if(parenOpen < parenClose){
            badparen.word[0] = *(*fptr - 1);
            badparen.word[1] = '\0';
            return badparen;
        }
        */
        return openParen;
    }

    else if(**fptr == ')'){
        closeParen.word[0] = **fptr;
        closeParen.word[1] = '\0';
        (*fptr)++;
        parenClose++;
        *bptr = *fptr;
        /*
        if(parenOpen < parenClose){
            badparen.word[0] = *(*fptr - 1);
            badparen.word[1] = '\0';
            return badparen;
        }
        */
        return closeParen;
    }

    else if(**fptr == '['){
        openBrack.word[0] = **fptr;
        openBrack.word[1] = '\0';
        (*fptr)++;
        brackOpen++;
        *bptr = *fptr;
        /*
        if(brackOpen < brackClose){
            badbrack.word[0] = *(*fptr - 1);
            badbrack.word[1] = '\0';
            return badbrack;
        }
        */
        return openBrack;
    }

    else if(**fptr == ']'){
        closeBrack.word[0] = **fptr;
        closeBrack.word[1] = '\0';
        (*fptr)++;
        brackClose++;
        *bptr = *fptr;
        /*
        if(brackOpen < brackClose){
            badbrack.word[0] = *(*fptr - 1);
            badbrack.word[1] = '\0';
            return badbrack;
        }
        */
        return closeBrack;
    }
    (*fptr)++;

    unrec.word[0] = *(*fptr - 1);
    unrec.word[1] = '\0';

    //printf("MACHINE NOT RECOGNIZED CATCHALL\n");
    *bptr = *fptr;
    return unrec;
}

#endif
