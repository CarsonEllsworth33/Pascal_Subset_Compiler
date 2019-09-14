#include <stdio.h>
#include <stdlib.h>
#include "../lexeme.c"
#include <regex.h>


struct Lexeme tmp = {66,{0}};
struct Lexeme MN = {MNOTREC,{MNOTREC}};
char letter[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char digit[] = {'0','1','2','3','4','5','6','7','8','9'};

struct Lexeme whiteSpace(char **fptr,char **bptr){
    struct Lexeme NL = {WSPACE, {WSPACE_NL}};
    struct Lexeme BL = {WSPACE, {WSPACE_BL}};
    struct Lexeme TB = {WSPACE, {WSPACE_TB}};

    while(**fptr == ' '){
        //stay in loop and advance till item seen
        printf("blank returned\n");
        (*fptr)++;
        *bptr =  *fptr;
    }

    if(**fptr == '\n'){
        //get newline
        printf("NL returned\n");
        return NL;
    }

    //*bptr = *fptr; //could be source of problem
    printf("Machine Not Recognized WS\n");
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
    printf("Machine not Recognized RE\n");
    return MN;
}//END RELOP MACHINE





int match(char **fptr, char compArr[]){
    int arrlen = 0;
    if(compArr[0] == 'a'){
        arrlen = 57;
    }
    else if(compArr[0] == '0'){
        arrlen = 11;
    }
    for(int i = 0; i < arrlen; i++){
        if(**fptr == compArr[i]) return 1;
    }
    return 0;
}//END MATCH








struct Lexeme idres(char **fptr, char **bptr){
    //const char* letter = "[a-zA-Z]";
    //user defined letter and digit alphabet

    struct Lexeme idtl = {LEXERROR,IDTOOLONG};
    struct Lexeme id = {ID,0};//0 is tmp value, this actually needs to be a pointer

    int counter = 0;//counter to make hard things simple

    if(match(fptr,letter)){
        //**fptr is a letter
        (*fptr)++;//buffer address pointer
        counter++;//increase counter
        while(match(fptr,letter) || match(fptr,digit)){
            //string[counter] = **fptr;
            (*fptr)++;
            counter++;
        }
        //checks to see if the created string would be too long
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
        //put something or another in token file
        printf("counter size %d\n", counter );
        printf("%s\n", string);
        printf("Machine not Recognized IDRES\n");
    }
    //add string to symbol table or reject
    //(if same word already exists).

    return id;
}







void realM(char **fptr, char **bptr){

}






void intM(char **fptr, char **bptr){
    struct Lexeme intData = {INTEGER,NULL};
    int counter = 0;

    if(match(fptr,digit)){

    }
}



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

    //fgets(buffer, sizeof(buffer), input);


//process till EOF
while(fgets(buffer,sizeof(buffer),input) != NULL){
    fptr=buffer;
    bptr=buffer;
    printf("current buffer line: %s\n", buffer);

    while(whiteSpace(&fptr,&bptr).attr.val != WSPACE_NL){

        relop(&fptr,&bptr);
        idres(&fptr,&bptr);
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
    //printf("f pointer: %p\nf value: %c\nb pointer: %p\nb value: %c\n",fptr,*fptr,bptr,*fptr);
    return 0;
}
