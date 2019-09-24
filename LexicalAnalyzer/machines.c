#ifndef MACHINES_H
#define MACHINES_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "../lexeme.c"
#include "../symbolNode.c"
#include "resword.c"

struct Lexeme MN = {MNOTREC,{MNOTREC}};
char letter[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char digit[] = {'0','1','2','3','4','5','6','7','8','9'};
int letterSize = sizeof(letter);
int digitSize = sizeof(digit);
int parenOpen = 0;
int parenClose = 0;
int brackOpen = 0;
int brackClose = 0;
symbolNode head;

struct Lexeme whiteSpace(char **fptr,char **bptr){
    struct Lexeme NL = {WSPACE, {WSPACE_NL}};
    struct Lexeme BL = {WSPACE, {WSPACE_BL}};
    struct Lexeme TB = {WSPACE, {WSPACE_TB}};

    while(**fptr == ' '){
        //stay in loop and advance till item seen
        (*fptr)++;
        *bptr =  *fptr;
    }

    if(**fptr == '\n'){
        //get newline
        printf("NL returned\n");
        return NL;
    }

    //*bptr = *fptr; //could be source of problem
    printf("MACHINE NOT RECOGNIZED WS\n");
    return MN;
}//END WHITESPACE MACHINE









struct Lexeme relop(char **fptr,char **bptr){
    struct Lexeme LT = {RELOP, {RELOP_LT}};
    struct Lexeme LE = {RELOP, {RELOP_LE}};
    struct Lexeme GT = {RELOP, {RELOP_GT}};
    struct Lexeme GE = {RELOP, {RELOP_GE}};
    struct Lexeme EQ = {RELOP, {RELOP_EQ}};
    struct Lexeme NE = {RELOP, {RELOP_NE}};

    //this tree takes us to LT,LE,NE
    if(**fptr == '<'){
            (*fptr)++;
            if(**fptr == '='){
                (*fptr)++;
                *bptr = *fptr;
                printf("Less than or equal returned\n");
                return LE;
            }
            else if(**fptr == '>'){
                (*fptr)++;
                *bptr = *fptr;
                printf("Not Equal returned\n");
                return NE;
            }
            else{
                //(*fptr)++;
                *bptr = *fptr;
                printf("Less than returned\n");
                return LT;
            }
    }

    //this tree takes us to GT,GE
    if(**fptr == '>'){
            (*fptr)++;
            if(**fptr == '='){
                (*fptr)++;
                *bptr = *fptr;
                printf("Greater than or Equal returned\n");
                return GE;
            }
            else{
                *bptr = *fptr;
                printf("Greater than returned\n");
                return GT;
            }
    }

    //this tree takes us to EQ
    if(**fptr == '='){
        (*fptr)++;
        *bptr = *fptr;
        printf("Equal returned\n");
        return EQ;
    }
    *bptr = *fptr;
    printf("MACHINE NOT RECOGNIZED RELOP\n");
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








struct Lexeme idres(char **fptr, char **bptr){
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

    struct Lexeme idtl = {LEXERROR,IDTOOLONG};
    struct Lexeme id = {ID,0};//0 is tmp value, this actually needs to be a pointer

    int counter = 0;//counter to make hard things simple


    if(match(**fptr,letter,letterSize)){
        //**fptr is a letter
        //(*fptr)++;//buffer address pointer
        //counter++;//increase counter
        while(match(**fptr,letter,letterSize) || match(**fptr,digit,digitSize)){
            //string[counter] = **fptr;
            (*fptr)++;
            counter++;
        }
        //checks to see if the created string would be too long
        if(counter > 0){
            if(counter > 10){
                printf("IDTOOLONG\n");
                *bptr = *fptr;
                return idtl;
            }
            //while MNOTREC and is less than 10
            char string[counter+1];//temp string to build up identifiers
            for(int i = 0; i <= counter; i++){
                string[i] = **bptr;
                (*bptr)++;
            }

            string[counter] = '\0';

            int result = isResword(head,string);
            if( result == 0){
                //good to add to symbolNode list
                addNode(head,ID,string,0);//this last value should be a pointer
            }

            else{
                printf("already existing identifier: %s\n", string);
            }
            printf("identifier: %s\n", string );
            return id;
        }
    }
    printf("MACHINE NOT RECOGNIZED IDRES\n");
    return MN;
}





struct Lexeme realM(char **fptr, char **bptr){
    struct Lexeme realData = {INTEGER};
    realData.attr.ptr = NULL;
    struct Lexeme realerr = {LEXERROR,{REALTOOLONG}};
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
            if(rcounter > 0){
                if(**fptr == 'E'){
                    (*fptr)++;
                    epresent = 1;
                    while(match(**fptr,digit,digitSize)){
                        (*fptr)++;
                        ecounter++;
                    }
                }
            }
        }
        else{
            (*fptr) = (*bptr);
            printf("MACHINE NOT RECOGNIZED REALM\n");
            return MN;
        }

        if(fcounter > 5){
            printf("REALFRONTTOOLONG\n");
            return realerr;//real front too long
        }

        if(rcounter > 5){
            printf("REALBACKTOOLONG\n");
            return realerr;
        }

        if(ecounter > 2){
            printf("REALEXPONENTTOOBIG\n");
            return realerr;
        }

        if(fcounter+rcounter > 10){
            printf("REALTOOLONG\n");
            return realerr;//real too long
        }
        int counter = fcounter + rcounter + ecounter + epresent;

        char realstr[counter + 1];//takes into account the dot and E if present
        int strcntr = 0;
        while(*bptr != *fptr){
            realstr[strcntr] = **bptr;
            (*bptr)++;
            strcntr++;
        }
        realstr[strcntr] = '\0';
        printf("real: %s\n", realstr);
        return realData;
    }
    return MN;
}






struct Lexeme intM(char **fptr, char **bptr){
    struct Lexeme intData = {INTEGER};
    intData.attr.ptr = NULL;
    struct Lexeme interr = {LEXERROR,{INTTOOLONG}};

    int counter = 0;

    while(match(**fptr,digit,digitSize)){
        (*fptr)++;
        counter++;
    }
    if(counter > 10){
        printf("INTEGERTOOLONG\n");
        *bptr = *fptr;
        return interr;//int too long
    }
    if(counter > 0){
        char intstr[counter + 1];
        for(int i = 0; i <= counter; i++){
            intstr[i] = **bptr;
            (*bptr)++;
        }
        intstr[counter] = '\0';
        printf("Int: %s\n", intstr);
        return intData;
    }
    printf("MACHINE NOT RECOGNIZED INT\n");
    return MN;
}





struct Lexeme catchallM(char **fptr,char **bptr){
    //Here we need to support addops(+ - or) mulops(* / div mod and)
    //assignop(:=) doubledot(..) dot(.)
    //parenthesis () and brackets [] need a counter for them
    //and handle any unrecognized symbol
    //div mod and or are all going to be recognized by the
    struct Lexeme plus;
    plus.tkn = ADDOP;
    plus.attr.val = ADDOP_PL;

    struct Lexeme minus;
    minus.tkn = ADDOP;
    minus.attr.val = ADDOP_MN;

    struct Lexeme times;
    times.tkn = MULOP;
    times.attr.val = MULOP_ML;

    /*struct Lexeme divI; // this is keyword div
    divI.tkn = MULOP;
    divI.attr.val = MULOP_DVI;*/

    struct Lexeme divF; // this is symbol: /
    divF.tkn = MULOP;
    divF.attr.val = MULOP_DVF;

    /*struct Lexeme mod;
    mod.tkn = MULOP;
    mod.attr.val = MULOP_MOD;*/

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

    struct Lexeme unrec;
    unrec.tkn = LEXERROR;
    unrec.attr.val = UNKNOWNSYMBOL;


    if(**fptr == '.'){
        (*fptr)++;
        if(**fptr == '.'){
            (*fptr)++;
            return dotdot;
        }
        else{
            return dot;
        }
    }

    if(**fptr == ':'){
        (*fptr)++;
        if(**fptr == '='){
            printf("assignop returned\n" );
            return assign;
        }
        else{
            //unrecognized symbol
            return unrec;
        }
    }

    if(**fptr == '*'){
        (*fptr)++;
        return times;
    }

    if(**fptr == '/'){
        (*fptr)++;
        return divF;
    }

    if(**fptr == '+'){
        (*fptr)++;
        return plus;
    }

    if(**fptr == '-'){
        (*fptr)++;
        return minus;
    }

    if(**fptr == '('){
        (*fptr)++;
        parenOpen++;
        return openParen;
    }

    if(**fptr == ')'){
        (*fptr)++;
        parenClose++;
        return closeParen;
    }

    if(**fptr == '['){
        (*fptr)++;
        brackOpen++;
        return openBrack;
    }

    if(**fptr == ']'){
        (*fptr)++;
        brackClose++;
        return closeBrack;
    }
    (*fptr)++;
    printf("MACHINE NOT RECOGNIZED CATCHALL\n");
    return unrec;
}


//these might fit better in the catch all machine
//







//this main is a tester function to check if the machins are working
/* Multi Line comment to disable main
int main(void){
    char buffer[72];
    char *fptr = buffer;
    char *bptr = buffer;
    FILE *input = fopen("todo.txt","r");
    head = createNode(0,"",0);
    createTable(head);
    traverseList(head);


//process till EOF
    while(fgets(buffer,sizeof(buffer),input) != NULL){
        fptr=buffer;
        bptr=buffer;


        while(whiteSpace(&fptr,&bptr).attr.val != WSPACE_NL){
            printf("fptr: %c\n", *fptr);

            if(relop(&fptr,&bptr).tkn != MN.tkn) {continue;}
            if(idres(&fptr,&bptr).tkn != MN.tkn) {continue;}
            if(realM(&fptr,&bptr).tkn != MN.tkn) {continue;}
            if(intM(&fptr,&bptr).tkn != MN.tkn) {continue;}
            if(catchallM(&fptr,&bptr).tkn != MN.tkn) {continue;}
            
        }

    }
    return 0;
}
*/

#endif
