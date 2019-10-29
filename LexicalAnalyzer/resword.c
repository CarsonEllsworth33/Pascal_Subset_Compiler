#ifndef RESWORD_H
#define RESWORD_H

#include <stdlib.h>
#include <stdio.h>
#include "../symbolNode.c"

void createTable(symbolNode head){
    //the cwd is where run is executed from!!!!
    FILE *in = fopen("textfiles/reservedword.txt","r");
    char buffer[72];
    char *fptr = buffer;
    char *bptr = buffer;
    if(in == NULL){
        printf("file not opened\n");
    }
    else{
        while( fgets(buffer, sizeof(buffer),in) != NULL){
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

                char tmp[] = {'N','U','L','L'};

                if(strcmp(elements[0],tmp) != 0){

                    tkn = atoi(elements[1]);
                    attr = atoi(elements[2]);
                    //printf("elements[0] %-15s elements[1] %d elements[2] %d\n",elements[0],tkn,attr);
                    addNode(head, elements[0], NULL, tkn, attr);
                }
                strcpy(elements[0],"NULL");

            }//end comment catch line

            //reset buffer values
            fptr=buffer;
            bptr=buffer;

        }//end buffer grab while
    }//end else
    if(fclose(in) == 0){
        //printf("yay success\n");
    }
    else{
        printf("booo\n");
    }
}

#endif
