#ifndef RUN_H
#define RUN_H

#include <stdio.h>
#include "lexeme.c"
#include "symbolNode.c"
#include "LexicalAnalyzer/lexAnalyze.c"
#include "Parser/parser.c"


int main(int argc, char *argv[]) {
	FILE *file;
	if(argc >= 2){
		//grab file name from system args
		printf("cla %s\n",argv[1]);
		file = fopen(argv[1], "r");
	}
	else{
		file = fopen("textfiles/program.txt", "r");
	}
	FILE *list = fopen("textfiles/listing.txt","w");
	FILE *token = fopen("textfiles/token.txt","w");
	FILE *trace = fopen("textfiles/ptrace.txt","w");
	FILE *nodes = fopen("textfiles/nodes.txt","w");
	FILE *address = fopen("textfiles/addresses.txt","w");
	if(file != NULL){
		restable = createNode(NULL,"restablehead");
	    idtable = createNode(NULL,"idtablehead");
		createTable(restable);
		//traverseList(restable);
		fgets(buffer, sizeof(buffer), file);
		fprintf(list,"%d    %s",1,buffer);
		fprintf(token, "Line No.   Lexeme         TOKEN-TYPE    ATTRIBUTE\n");

		parse(file,list,token,trace,nodes,address);


		//read_print_line(file,list,token);
	}
	else{
		printf("program.txt file not found\n");
	}

	fclose(file);
	fclose(list);
	fclose(token);
	fclose(trace);
	fclose(nodes);
	fclose(address);

	return 0;
}

#endif
