#ifndef RESWORD_H
#define RESWORD_H

#include <stdlib.h>
#include <stdio.h>
#include "../symbolNode.c"

symbolNode resTable;

void createTable(symbolNode head){
    FILE *input = fopen("reservedword.txt","r");
    char buffer[72];
    char *fptr = buffer;
    char *bptr = buffer;
    if(input == NULL){
        printf("file not opened\n");
    }
    else{
        while( fgets(buffer, sizeof(buffer),input) != NULL){
            //go through the file line by line
            int tkn = 0;
            int attr = 0;
            int cntr = 0;
            int elcntr = 0;
            char elements[3][15];
            if(*fptr != '/') {
                while(*fptr != '\n'){
                    if(*fptr == ' '){ //' 'is the delimeter character
                        char word[15];
                        cntr = 0;
                        while(bptr != fptr){
                            word[cntr] = *bptr;
                            bptr++;
                            cntr++;
                        }
                        word[cntr]='\0';
                        //printf("word: %s\n", word);
                        strcpy(elements[elcntr],word);
                        elcntr++;

                    }

                    fptr++;
                }//end NL while

                //printf("elements 0:%s elements 1:%s elements 2:%s\n", elements[0] , elements[1], elements[2]);
                char tmp[] = {'N','U','L','L'};
                if(strcmp(elements[0],tmp) != 0){
                    addNode(head,(int)elements[1],elements[0],(int)elements[2]);
                }
                for(int i = 0; i < 5; i++){
                    switch(i){
                        case 0:
                            elements[0][i] = 'N';
                            break;
                        case 1:
                            elements[0][i] = 'U';
                            break;
                        case 2: case 3:
                            elements[0][i] = 'L';
                            break;
                        case 4:
                            elements[0][i] = '\0';
                            break;
                        default:
                            break;
                    }
                }

            }//end comment catch line

            //reset buffer values
            fptr=buffer;
            bptr=buffer;

        }//end buffer grab while
    }//end else
}

/*
int main(void){
    resTable = createNode(0,"",0);
    createTable(resTable);
    return 0;
}*/

#endif
