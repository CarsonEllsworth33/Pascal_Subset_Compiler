#include <stdio.h>
#include "../lexeme.c"
#include <regex.h>


struct Lexeme tmp = {66,{0}};
struct Lexeme MN = {MNOTREC,{MNOTREC}};

struct Lexeme whiteSpace(char **fptr,char **bptr){
    struct Lexeme NL = {WSPACE, {WSPACE_NL}};
    struct Lexeme BL = {WSPACE, {WSPACE_BL}};
    struct Lexeme TB = {WSPACE, {WSPACE_TB}};

    if(**fptr == ' '){
        //stay in loop and advance till item seen
        printf("blank returned\n");
        (*fptr)++;
        return BL;
    }
    if(**fptr == '\n'){
        //get newline
        printf("NL returned\n");
        return NL;
    }
    *bptr = *fptr;
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
                printf("Less than or equal returned\n");
                return LE;
            }
            else if(**fptr == '>'){
                (*fptr)++;
                printf("Not Equal returned\n");
                return NE;
            }
            else{
                //fptr--;
                printf("Less than returned\n");
                return LT;
            }
    }

    //this tree takes us to GT,GE
    if(**fptr == '>'){
            (*fptr)++;
            if(**fptr == '='){
                (*fptr)++;
                printf("Greater than or Equal returned\n");
                return GE;
            }
            else{
                //fptr--;
                printf("Greater than returned\n");
                return GT;
            }
    }

    //this tree takes us to EQ
    if(**fptr == '='){
        (*fptr)++;
        printf("Equal returned\n");
        return EQ;
    }
    printf("Machine not Recognized returned\n");
    return MN;
}//END RELOP MACHINE


int match(char **fptr, char compArr[]){
    int arrlen = 0;
    if(compArr[0] == 'a'){
        arrlen = 56;
    }
    else if(compArr[0] == '0'){
        arrlen = 10;
    }
    for(int i = 0; i < arrlen; i++){
        if(**fptr == compArr[i]) return 1;
    }
    return 0;
}

struct Lexeme idres(char **fptr, char **bptr){
    //const char* letter = "[a-zA-Z]";
    char letter[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char digit[] = {'0','1','2','3','4','5','6','7','8','9'};
    struct Lexeme idtl = {LEXERROR,IDTOOLONG};
    struct Lexeme id = {ID,0};//0 is tmp value, this actually needs to be a pointer
    int counter = 1;
    char string[10];
    if(match(fptr,letter)){
        //**fptr is a letter
        string[counter] = **fptr;
        (*fptr)++;//buffer address pointer
        counter++;//increase counter
        while(match(fptr,letter) || match(fptr,digit)){
            if(counter > 10){
                printf("IDTOOLONG\n");
                return idtl;
            }
            string[counter] = **fptr;
            (*fptr)++;
            counter++;
        }
        printf("%s\n", string);
        //add string to symbol table or reject (if same word already exists).
        return id;
    }
    return MN;
}



void realM(){}
void intM(){}

//these might fit better in the catch all machine
void mulop(){}
void addop(){}
//


int main(){
    //this is a test main to try out the machines once they are ready
    char buffer[72];
    char *fptr = buffer;
    char *bptr = buffer;
    FILE *input = fopen("todo.txt","r");

    fgets(buffer, sizeof(buffer), input);

//process till EOF
while(fgets(buffer,sizeof(buffer),input) != NULL){
    fptr=buffer;
    bptr=buffer;

    while(whiteSpace(&fptr,&bptr).attr.val != WSPACE_NL){
        if(whiteSpace(&fptr,&bptr).attr.val != WSPACE_BL){
            relop(&fptr,&bptr);
        }
    }

    /*
    while(whiteSpace(fptr,bptr).attr.val == WSPACE_BL){
        //keep whitespacing
    }
    //LOAD NEW LINE INTO BUFFER
    while(whiteSpace(fptr,bptr).attr.val == WSPACE_NL){

        fgets(buffer, sizeof(buffer), input);
        fptr = buffer;
        bptr = buffer;

    }
    */
}
    char test[11] = "abcdefghijk";
    char *cptr = test;
    printf("f pointer: %p\nf value: %c\nb pointer: %p\nb value: %c\n",fptr,*fptr,bptr,*fptr);
    idres(&cptr,&bptr);
    return 0;
}
