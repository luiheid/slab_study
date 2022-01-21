#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
	READ,
	PRINT,
	ASSIGN,
	DEFINE,
	ADD,
	MINUS,
	MOD,
	EQ,
	LESS,
	JUMP,
	TERM,
	NUM
} instruction;

char *inst_str[NUM] = {
	"READ",
    "PRINT",
    "ASSIGN",
    "DEFINE",
    "ADD",
    "MINUS",
    "MOD",
    "EQ",
    "LESS",
    "JUMP",
	"TERM"
};

typedef struct {
	instruction inst;
	int one;
	int two;
	int three;
} Line;

typedef union {
	Line line;
	int num;
} memory;

int main(int argc, char *argv[]){

	memory *mem = calloc(100, sizeof(memory));

	FILE *fp = fopen(argv[1], "r");

	if(fp == NULL){
		fprintf(stderr, "FILE is not exist\n");
	}

	char str[30];
	while(fgets(str, 30, fp)){
		int index = NUM;

		char * s1 = strtok(str, ":");
		char * s2 = strtok(NULL, " ");
		char * s3 = strtok(NULL, "\n");

		if(s3 == NULL){
			s2 = strtok(s2, "\n");
		}

		int mi = atoi(s1);

		for(int i=0; i<NUM; i++){
			if(strcmp(s2, inst_str[i]) == 0){
				index = i;
				break;
			}
		}
		
		mem[mi].line.inst = index;
		
		s1 = strtok(s3, " ");
		if(s1 != NULL){
			mem[mi].line.one = atoi(s1);
			s2 = strtok(NULL, " ");
			if(s2 != NULL){
				mem[mi].line.two = atoi(s2);
				s3 = strtok(NULL, "\n");
				if(s3 != NULL){
					mem[mi].line.three = atoi(s3);
				}
			}

		}
		
		if(index == NUM){
			mem[mi].num = atoi(s2);
		}
		
	}

	fclose(fp);

	int input;
	int a, b, c;
	for(int i=0; i<100; i++){
		if(mem[i].line.inst == 0 && mem[i].line.one == 0){
			continue;
		}
		switch(mem[i].line.inst){
			case READ:
				scanf("%d", &input);
				mem[mem[i].line.one].num = input;
				break;
			case PRINT:
				printf("%d\n", mem[mem[i].line.one].num);
				break;
			case ASSIGN:
				a = mem[i].line.one;
				b = mem[i].line.two;
				mem[a].num = mem[b].num;
				break;
			case DEFINE:
				a = mem[i].line.one;
				b = mem[i].line.two;
				mem[a].num = b;
				break;
			case ADD:
				a = mem[i].line.one;
				b = mem[i].line.two;
				c = mem[i].line.three;
				mem[a].num = mem[b].num + mem[c].num;
				break;
			case MINUS:
				a = mem[i].line.one;
				b = mem[i].line.two;
				c = mem[i].line.three;
				mem[a].num = mem[b].num - mem[c].num;
				break;
			case MOD:
				a = mem[i].line.one;
				b = mem[i].line.two;
				c = mem[i].line.three;
				mem[a].num = mem[b].num % mem[c].num;
				break;
			case EQ:
				a = mem[i].line.one;
				b = mem[i].line.two;
				c = mem[i].line.three;
				if(mem[b].num == mem[c].num){
					mem[a].num = 1;
					break;
				}
				mem[a].num = 0;
				break;
			case LESS:
				a = mem[i].line.one;
				b = mem[i].line.two;
				c = mem[i].line.three;
				if(mem[b].num < mem[c].num){
					mem[a].num = 1;
					break;
				}
				mem[a].num = 0;
				break;
			case JUMP:
				a = mem[i].line.one;
				b = mem[i].line.two;
				if(mem[a].num != 0){
					i = b - 1;
				}
				break;
			case TERM:
				exit(0);
		}
	}
	
	free(mem);
	return 0;
}
