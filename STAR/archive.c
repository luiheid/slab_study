#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

void archive(FILE *fp, char origin[], DIR *dir){
    DIR *dd;
    struct dirent *d; 
	unsigned int length = strlen(origin);
	
	fwrite(&length,1,sizeof(length),fp);
	fwrite(origin,1,strlen(origin),fp);

	struct stat sfile;
	stat(origin, &sfile);
	fwrite(&sfile.st_mode,sizeof(mode_t),1,fp);
	
	unsigned int a = 0;
	fwrite(&a,1,sizeof(a),fp);
    while(d = readdir(dir)){
		if(!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")){
			continue;
		}   
		char path_or[strlen(origin)+strlen(d->d_name)+1];
		strcpy(path_or, origin);
		strcat(path_or, "/");
		strcat(path_or, d->d_name);

		if((dd = opendir(path_or)) != NULL){
			
			archive(fp, path_or, dd);
		}  
		else{
			length = strlen(path_or);
		
			fwrite(&length,1,sizeof(length),fp);
			fwrite(path_or,1,strlen(path_or),fp);

			stat(path_or, &sfile);
			fwrite(&sfile.st_mode,sizeof(mode_t),1,fp);

			FILE *fp1 = fopen(path_or, "rb");

			long size;

			fseek(fp1,0,SEEK_END);
			size = ftell(fp1);
			rewind(fp1);

			unsigned int size_u = size;
			fwrite(&size_u,1,sizeof(size_u),fp);

			char *buffer = malloc(size);

			fread(buffer, 1, size, fp1);
			fwrite(buffer, 1, size, fp);
			fclose(fp1);

			free(buffer);
		}   
	}   
	closedir(dir);
}

void extract(char file[]){
	FILE *fp = fopen(file, "rb");
	unsigned int u = 0;
	mode_t m;
	while(fread(&u, 4, 1, fp)){
		char path[u+1];
		fread(path, 1, u, fp);
		path[u]='\0';
		fread(&m,sizeof(mode_t),1,fp);

		mode_t m2 = S_IFMT;
		fread(&u, 4, 1, fp);
		if((m & m2) == S_IFDIR){
			mkdir(path, m);
		}   
		else{
			FILE *fp1 = fopen(path, "wb");

			char *buffer = malloc(u);

			fread(buffer, 1, u, fp);
			fwrite(buffer, 1, u, fp1);
			free(buffer);
			fclose(fp1);

			chmod(path, m);
		}
	}
	fclose(fp);
}

void list(char file[]){
	FILE *fp = fopen(file,"rb");
	unsigned int u;
	mode_t m;
	while(fread(&u,4,1,fp)){
		char path[u+1];
		fread(path,1,u,fp);
		path[u] = '\0';

		printf("%s\n",path);

		fread(&m,sizeof(mode_t),1,fp);
		fread(&u,4,1,fp);
		fseek(fp,u,SEEK_CUR);
	}
	fclose(fp);
}

int main(int argc, char *argv[]){
	char *command[3] = {"archive", "list", "extract"};
	int index = -1;

	for(int i=0; i<3; i++){
		if(!strcmp(argv[1],command[i])){
			index = i;
		}
	}

	if(index == -1){
		goto err;
	}

	switch(index){
		case 0:
			if(argc != 4){
				goto err;
			}
			DIR *dp = opendir(argv[3]);
			if(dp == NULL){
				fprintf(stderr, "Not a existing directory");
				exit(0);
			}
			FILE *fp = fopen(argv[2], "wb");
			
			archive(fp, argv[3], dp);

			fclose(fp);
			break;

		case 1:
			if(argc != 3){
				goto err;
			}
			list(argv[2]);
			break;

		case 2:
			if(argc != 3){
				goto err;
			}
			extract(argv[2]);
	}

	return 0;

err:
	fprintf(stderr, "command format is not right\n");
	return 0;
}
