#include <stdio.h>
#include "LexicalAnalyzer/lexAnalyze.c"
#include "lexeme.c"
#include "symbolNode.c"

int main() {

	char buffer[72];


	FILE *file = fopen("textfiles/program.txt", "r");
	FILE *line = fopen("textfiles/listing.txt","w");

	read_print_line(file,line);

	fclose(file);
	fclose(line);

	struct Lexeme tmp = getLexeme(10);
	printf("getLexeme tkn: %d\ngetLexeme attr: %d\n",tmp.lxm,tmp.attr.val);
	return 0;

}
