/*
Devroop Banerjee
V00837868
SENG 265
Assignment 1
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINES 500
#define MAX_CHARS 132

int FT;
int LM;
int LW;
int LS;
int line_length;
char line[MAX_CHARS];
char out[MAX_CHARS*MAX_LINES];
char formatted_string[MAX_CHARS];




int check_if_command_exists(char* line){
	char roughwork[MAX_CHARS];
	strncpy(roughwork, line, MAX_CHARS);
	strtok(roughwork, " \n");
	
	if(!strncmp(roughwork,".LW", MAX_CHARS)){
		LW = atoi(strtok(NULL, " \n"));		
		FT = 1;
		return 1;
	}
	if(!strncmp(roughwork,".LM", MAX_CHARS)){
		LM = atoi(strtok(NULL, " \n"));
		return 1;
	}
	if(!strncmp(roughwork,".FT", MAX_CHARS)){
		char* on_or_off = strtok(NULL, " \n");
		if(!strncmp(on_or_off, "on", 3)){
			FT = 1;
		}else{
			FT = 0;		
		}
		return 1;
	}
	if(!strncmp(roughwork,".LS", MAX_CHARS)){
		LS = atoi(strtok(NULL, " \n"));
		return 1;
	}	
	return 0;
}




void transform_line(char* line){
	char* string = strtok(line, " \n");

	while(string){
		if(line_length + strlen(string) >= LW){
			strncat(formatted_string, "\n", 1);					//Adds a new line
			line_length = 0;
			if(LS == 2){
				strncat(formatted_string, "\n\n", 2);
				line_length = 0;
			}else if(LS == 1){
				strncat(formatted_string, "\n", 1);
				line_length = 0;
			}else if(LS == 0){
				continue;
			}
			if(LM){
				while(line_length < LM){
					strncat(formatted_string, " ", 1);
					line_length++;
				}
			}
		}else if(line_length >= LM+1){
			strncat(formatted_string, " " , 1);
			line_length++;
    		}
		line_length += strlen(string);
		strncat(formatted_string, string, MAX_CHARS);
		string = strtok(NULL, " \n");	
	}
	return;
}




void execute_commands(char* line){
	if(LM){
		while(line_length < LM){
			strncat(formatted_string, " ", 1);
			line_length++;
		}
	}
	if(FT && !strncmp(line, "\n", 1)){
		strncat(out, "\n\n", MAX_CHARS);
		line_length = 0;
	}
	if(FT){
		transform_line(line);
		strncat(out, formatted_string, MAX_CHARS);
		strncpy(formatted_string, "", MAX_CHARS);
	}else{
		strncat(out, line, MAX_CHARS);
	}	
	return;
}




int main(int argc, char* argv[]){
	FILE* x;
	x = fopen(argv[1], "r");
	
	if(x == NULL){
		return 1;
	}

	while(fgets(line, MAX_CHARS, x)){								//Read file line by line		
		if(check_if_command_exists(line)){
			continue;
		}
		execute_commands(line);
	}

	if(FT){
		strncat(out, "\n", MAX_CHARS);
	}

	printf("%s", out);
	exit(0);
}



























