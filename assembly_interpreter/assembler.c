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
	int num;
	instruction inst;
	int one;
	int two;
	int three;
} memory;

int main(int argc, char *argv[]){

	int inst_list[100];
	int li = 0;

	memory *mem = malloc(sizeof(memory) * 100);

	FILE *fp = fopen(argv[1], "r");

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
		inst_list[li++] = mi;

		for(int i=0; i<NUM; i++){
			if(strcmp(s2, inst_str[i]) == 0){
				index = i;
				break;
			}
		}
		
		mem[mi].inst = index;
		
		s1 = strtok(s3, " ");
		if(s1 != NULL){
			mem[mi].one = atoi(s1);
			s2 = strtok(NULL, " ");
			if(s2 != NULL){
				mem[mi].two = atoi(s2);
				s3 = strtok(NULL, "\n");
				if(s3 != NULL){
					mem[mi].three = atoi(s3);
				}
			}

		}
		
		if(index == NUM){
			mem[mi].num = atoi(s2);
		}
		
	}

	int input;
	int a, b, c;
	for(int i=0; i<li; i++){
		switch(mem[inst_list[i]].inst){
			case READ:
				scanf("%d", &input);
				mem[mem[inst_list[i]].one].num = input;
				break;
			case PRINT:
				printf("%d\n", mem[mem[inst_list[i]].one].num);
				break;
			case ASSIGN:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				mem[a].num = mem[b].num;
				break;
			case DEFINE:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				mem[a].num = b;
				break;
			case ADD:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				c = mem[inst_list[i]].three;
				mem[a].num = mem[b].num + mem[c].num;
				break;
			case MINUS:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				c = mem[inst_list[i]].three;
				mem[a].num = mem[b].num - mem[c].num;
				break;
			case MOD:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				c = mem[inst_list[i]].three;
				mem[a].num = mem[b].num % mem[c].num;
				break;
			case EQ:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				c = mem[inst_list[i]].three;
				if(mem[b].num == mem[c].num){
					mem[a].num = 1;
					break;
				}
				mem[a].num = 0;
				break;
			case LESS:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				c = mem[inst_list[i]].three;
				if(mem[b].num < mem[c].num){
					mem[a].num = 1;
					break;
				}
				mem[a].num = 0;
				break;
			case JUMP:
				a = mem[inst_list[i]].one;
				b = mem[inst_list[i]].two;
				if(mem[a].num != 0){
					i = b - 1;
				}
				break;
			case TERM:
				exit(0);
		}
	}
	
	return 0;
}
