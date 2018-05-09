/*
 * UVic SENG 265, Fall 2016, A#4
 *
 * This will contain a solution to format265alt.c. In order to complete the
 * task of formatting a file, it must open the file and pass the result
 * to a routine in formatter.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include "formatter.h"

char **lines;
FILE *infile;
char *direct_input[512];
char **transformed;

void validity(){
	fprintf(stderr,"Invalid input!!\n");
	exit(1);
}

int main(int argc, char *argv[]) {
#ifdef DEBUG
	if(argc == 1){
		fgets(*direct_input, 512, stdin);
		transformed = format_lines(direct_input, 1);
	}else{
		if(argc != 2){
			validity();
		}
		infile = fopen(argv[1], "r");
		if(infile == NULL){
			validity();
		}
		transformed = format_file(infile);
		if(transformed == NULL){
			validity();
		}
	}
	for(lines = transformed; *lines != NULL; lines++){
		fprintf(stdout, "%s", *lines);
	}
	fclose(infile);
#endif
	exit(0);
}
