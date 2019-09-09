//Author: Carson Ellsworth
//Date:   08-31-2019
//Class:  CS4013
//proj:   project 0

#include <stdio.h>
#include <string.h>
#include "../lexeme.c"
#include "machines.c"

void read_print_line(FILE *input, FILE *output){
    //file and output should be two open files upon the function call
    char buffer[72];

    if(input == NULL || output == NULL){
        printf("File not opened\n");
    }
    else{
        int linecnt = 0;
        //load new line into buffer
        while( fgets(buffer, sizeof(buffer), input) != NULL){
            linecnt++;
            //make listing file
            fprintf(output,"%d    %s",linecnt,buffer);
        }
    }
}

//make machines here
