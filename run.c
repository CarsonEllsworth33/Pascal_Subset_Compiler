#ifndef RUN_H
#define RUN_H

#include <stdio.h>
#include "lexeme.c"
#include "symbolNode.c"
#include "LexicalAnalyzer/lexAnalyze.c"



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

	if(file != NULL){
		read_print_line(file,list,token);
	}
	else{
		printf("program.txt file not found\n");
	}

	fclose(file);
	fclose(list);
	fclose(token);

	struct Lexeme tmp = getLexeme(10);
	printf("getLexeme tkn: %d\ngetLexeme attr: %d\n",tmp.tkn,tmp.attr.val);
	return 0;
}

#endif
