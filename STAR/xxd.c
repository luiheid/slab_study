#include <stdio.h>
#include <stdlib.h>

int main(){
	char filename[20];
	long size;
	char * buffer;

	scanf(" %s", filename);
	FILE *fp = fopen(filename, "rb");
	fseek(fp,0L,SEEK_END);
	size = ftell(fp);
	rewind(fp);

	buffer = malloc(sizeof(char)*size);

	fread(buffer, sizeof(char), size, fp);

	for(int i=0; i<size/16 +1; i++){
		printf("%08x: ",i*16);
		for(int j=0; j<16; j++){
			if(j % 2 == 0){
				printf(" ");
			}
			if(i * 16 + j >= size){
				printf("  ");
				continue;
			}
			printf("%02x", buffer[i*16 + j]);
		}
		printf(" ");
		for(int j=0; j<16; j++){
			if(i * 16 + j > size){
				printf("\n");
				exit(0);
			}
			if(buffer[i*16 + j] == '\n'){
				buffer[i*16 + j] = '.';
			}
			printf("%c", buffer[i*16 + j]);
		}
		printf("\n");
	}
}
