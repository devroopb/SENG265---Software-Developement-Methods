/*
Devroop Banerjee
V00837868
SENG 265
Assignment 4
*/

/*
 * UVic SENG 265, Fall 2016,  A#4
 *
 * This will contain the bulk of the work for the fourth assignment. It
 * provide similar functionality to the class written in Python for
 * assignment #3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formatter.h"

int FT;
int LM;
int LW;
int LS;
int line_length = 0;
int file;
char* temp;

void error(){
	fprintf(stderr,"Something went wrong while trying to allocate memory\n");
	exit(1);
}

int extract(char* line){
	int length = strlen(line) + 1;
	char *grab = (char*)calloc(length, sizeof(char));
	if(grab == NULL){
		error();
	}
	strncpy(grab, line, length);
	strtok(grab, " \n");
	
	if(!strncmp(grab, ".FT", length)){
		temp = strtok(NULL, " \n");
		if(!strncmp(temp, "on", 3)){
			FT = 1;
		}else if(!strncmp(temp, "off", 3)){
			FT = 0;
		}
		line_length = 0;
		return 1;
	}
	
	if(!strncmp(grab, ".LS", length)){
		LS = atoi(strtok(NULL, " \n"));
		return 1;
	}
	
	if(!strncmp(grab, ".LW", length)){
		LW = atoi(strtok (NULL, " \n"));
		FT = 1;
		return 1;
	}
	
	if(!strncmp(grab, ".LM", length)){
		temp = strtok(NULL, " \n");
		if(!strncmp(temp, "+", 1)){
			temp++;
			LM += atoi(temp);
		}else if(!strncmp(temp, "-", 1)){
			temp++;
			LM -= atoi(temp);
		}else{
			LM = atoi(temp);
		}
		if (LM >= LW - 20){
			LM = LW - 20;
		}
		return 1;
	}
	
	grab = (char*)realloc(grab, (length+1)*sizeof(char));
	free(grab);
	return 0;
}

char **format_file(FILE *infile){
	file = 1;
	char *line = NULL;
	int num_lines = 0;
	size_t length = 0;
	ssize_t x = getline(&line, &length, infile);
	char **out = calloc(1, sizeof(char*));
	if(out == NULL){
		error();
	}
	
	while(x != -1){
		line[strlen(line)-1] = '\0';
		out = (char**)realloc(out, (num_lines + 1) * sizeof(char*));
		if(out == NULL){
			error();
		}
		out[num_lines] = calloc(x + 1, sizeof(char));
		if(out[num_lines] == NULL){
			error();
		}
		strcpy(out[num_lines], line);
		num_lines++;
	}
	char **result = format_lines(out, num_lines);
	return result;
}

char **format_lines(char **lines, int num_lines){
#ifdef DEBUG
	char **result = (char **)malloc(sizeof(char *)*2);
	if (result == NULL){
		error();
	}
	result[0] = (char *)malloc(sizeof(char)*80);
	if (result[0] == NULL){
		error();
	}
	strncpy(result[0], "(machine-like voice) EXTERMINATE THEM!", 79);
	result[1] = (char *)malloc(sizeof(char)*2);
	if (result[1] == NULL){
		error();
	}
	result[1][0] = '\0';
	result = (char**)calloc(1, sizeof(char*));
	if(result == NULL){
		error();
	}
	
	free(result);

	int size = 0;
	int i;
	for(i = 0; i < num_lines; i++){
		if(!strncmp(lines[i], "\n", 1) && FT){
			size++;
			result = (char**)realloc(result, (size + 1)*sizeof(char*));
			if(result == NULL){
				error();
			}
			result[size] = (char*)calloc(2, sizeof(char));
			if(result[size] == NULL){
				error();
			}
			strncpy(result[size], "\n", 2);
			line_length = LW;
			continue;
		}else if(extract(lines[i])){
			continue;
		}else if(FT){
			char *grab = (char*)calloc(strlen(lines[i]) + 1, sizeof(char));
			if(grab == NULL){
				error();
			}
			strncpy(grab, lines[i], strlen(lines[i])-1);
			char* tokenizer = strtok(grab, "\t \n");

			while(tokenizer){
				if((line_length + strlen(tokenizer) + 1) > LW){
					strncat(result[size], "\n", 2);
					size++;
					result = (char**)realloc(result, (size + 1)*sizeof(char*));
					if(result == NULL){
						error();
					}
					line_length = 0;
				}else if(line_length !=0){
					strncat(result[size], " ", 1);
					line_length++;
				}
				if(line_length == 0){
					result[size] = (char*)calloc(LW + 2, sizeof(char));
					if(result[size] == NULL){
						error();
					}
					int z;
					for(z = 0; z < LM; z++){
						strncat(result[size], " ", 1);
						line_length++;
					}
				}
				strncat(result[size], tokenizer, strlen(tokenizer));
				line_length += strlen(tokenizer);
				tokenizer = strtok(NULL, " \n");
				continue;
			}
			free(grab);
			continue;
		}else{
			if(line_length < LW){
				strncat(result[size], "\n", 2);
				size++;
			}
			result = (char**)realloc(result, (size + 1)*sizeof(char*));
			if(result == NULL){
				error();
			}
			result[size] = (char*)calloc(LW + 2, sizeof(char));
			if(result[size] == NULL){
				error();
			}
			strncpy(result[size], lines[i], strlen(lines[i])+1);
			line_length = LW + 1;
			size++;
			continue;
		}
	}
	if(FT){
		size++;
		result = (char**)realloc(result, (size + 1)*sizeof(char*));
		if(result == NULL){
			error();
		}
		result[size] = (char*)calloc(2, sizeof(char));
		if(result[size] == NULL){
			error();
		}
		strncpy(result[size], "\n", 1);
		size++;
	}
	if(file){
		for(i = 0; i < num_lines; i++){
			free((lines + i));
		}
		free(lines);
	}
#endif
	return result;
}
