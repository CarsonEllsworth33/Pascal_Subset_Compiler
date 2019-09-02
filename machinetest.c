#include <stdio.h>
#include "lexAnalyze.c"

int main() {

	char buffer[72];
	FILE *file = fopen("programexample.txt", "r");
	FILE *line = fopen("listing.txt","w");
	read_print_line(file,line);

	fclose(file);
	fclose(line);
	return 0;

}
