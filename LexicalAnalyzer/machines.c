#include <stdio.h>
#include "../lexeme.c"


struct Lexeme tmp = {66,{0}};

struct Lexeme whiteSpace(char *fptr,char *bptr){
    struct Lexeme NL = {WSPACE, {WSPACE_NL}};
    struct Lexeme BL = {WSPACE, {WSPACE_BL}};
    struct Lexeme TB = {WSPACE, {WSPACE_TB}};
    struct Lexeme MN = {MNOTREC, {MNOTREC}};

    if(*fptr == ' '){
        //stay in loop and advance till item seen
        fptr++;
        return BL;
    }
    if(*fptr == '\n'){
        //get newline
        return NL;
    }
    *bptr = *fptr;
    return MN;
}//END WHITESPACE MACHINE

struct Lexeme relop(char *fptr,char *bptr){
    struct Lexeme LT = {RELOP, {RELOP_LT}};
    struct Lexeme LE = {RELOP, {RELOP_LE}};
    struct Lexeme GT = {RELOP, {RELOP_GT}};
    struct Lexeme GE = {RELOP, {RELOP_GE}};
    struct Lexeme EQ = {RELOP, {RELOP_EQ}};
    struct Lexeme NE = {RELOP, {RELOP_NE}};
    struct Lexeme MN = {MNOTREC, {MNOTREC}};

    //this tree takes us to LT,LE,NE
    if(*fptr == '<'){
            fptr++;
            if(*fptr == '='){
                return LE;
            }
            else if(*fptr == '>'){
                return NE;
            }
            else{
                fptr--;
                return LT;
            }
    }

    //this tree takes us to GT,GE
    if(*fptr == '>'){
            fptr++;
            if(*fptr == '='){
                return GE;
            }
            else{
                fptr--;
                return GT;
            }
    }

    //this tree takes us to EQ
    if(*fptr == '='){
        fptr++;
        return EQ;
    }

    return MN;
}//END RELOP MACHINE


int main(){
    //this is a test main to try out the machines once they are ready
    char buffer[72];
    char *fptr = buffer;
    char *bptr = buffer;
    FILE *input = fopen("todo.txt","r");

    fgets(buffer, sizeof(buffer), input);

    while(whiteSpace(fptr,bptr).attr.val == WSPACE_BL){

    }
    //LOAD NEW LINE INTO BUFFER
    while(whiteSpace(fptr,bptr).attr.val == WSPACE_NL){

        fgets(buffer, sizeof(buffer), input);
        fptr = buffer;
        bptr = buffer;

    }
    printf("f pointer: %p\nf value: %c\nb pointer: %p\nb value: %c\n",fptr,*fptr,bptr,*fptr);

    return 0;
}
