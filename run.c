#ifndef RUN_H
#define RUN_H

#include <stdio.h>
#include "lexeme.c"
#include "symbolNode.c"
#include "LexicalAnalyzer/lexAnalyze.c"



int main() {

	//char buffer[72];
	FILE *file = fopen("textfiles/program.txt", "r");
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
