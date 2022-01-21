#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* NOFIX --- */

typedef enum {
	start,
	left,
	right,
	up,
	down,
	quit,
	N_op 
} commands ;

typedef enum {
	vertical,
	horizontal 
} direction ;

typedef struct {
	int id ;
	int y1, y2 ;	// y1: the minimum of y, y2: the maximum of y
	int x1, x2 ;	// x1: the minimum of x, x2: the maximum of x
	int span ;		// the number of cells 
	direction dir ;	// the direction of the car
} car_t ;

char * op_str[N_op] = {
	"start",
	"left",
	"right",
	"up",
	"down",
	"quit"
} ;

int n_cars = 0 ;
car_t * cars = 0x0 ;
int cells[6][6] ; // cells[Y][X]
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]

/* --- NOFIX */


commands
get_op_code (char * s)
{
	// return the corresponding number for the command given as s.
	// FIXME
	for(int i=0; i<N_op; i++){
		if(strcmp(s,op_str[i])==0)
			return i;
	}
	return N_op;
}

int
load_game (char * filename)
{
	//FIXME
	// load_game returns 0 for a success, or return 1 for a failure.
	// Use fopen, getline, strtok, atoi, strcmp
	// Note that the last character of a line obtained by getline may be '\n'.
	FILE *fp = fopen(filename,"r");

	if(fp == NULL)
	{
		printf("No such file\n\n");
		return 1;
	}


	char buf[128];
	char file[3][12];

	fscanf(fp," %d",&n_cars);

	if(n_cars < 2 || 36 < n_cars){
		printf("First line of file is not a proper number\n");
		goto err;
	}

	cars = (car_t*)malloc(sizeof(car_t)*n_cars);

	for(int i=0; i<n_cars; i++){
		fscanf(fp, " %s", buf);
		strcpy(file[0], strtok(buf,":"));
		strcpy(file[1], strtok(NULL,":"));
		strcpy(file[2], strtok(NULL,":"));

		if(file[1]==NULL || file[2]==NULL){
			fprintf(stderr,"File Format must be \"location:direction:span\"\n");
			goto err;
		}
		if(strcmp("vertical",file[1])!=0 && strcmp("horizontal",file[1])!=0){
			printf("File direction is not right\n");
			goto err;
		}

		cars[i].id = i+1;
		if((cars[i].span = atoi(file[2]))>6){
			printf("span is not proper\n");
			goto err;
		}

		if(strcmp("vertical",file[1])==0) 
			cars[i].dir = vertical;
		else
			cars[i].dir = horizontal;

		if(cars[i].dir == vertical){
			cars[i].y1 = file[0][1]-'1';
			cars[i].y2 = cars[i].y1 - cars[i].span + 1;
			cars[i].x1 = file[0][0] - 'A';
			cars[i].x2 = cars[i].x1;
		}
		else{
                         cars[i].y1 = file[0][1]-'1';
                         cars[i].y2 = cars[i].y1;
                         cars[i].x1 = file[0][0] - 'A';
                         cars[i].x2 = cars[i].x1 + cars[i].span - 1;
                }
		if(cars[i].x1<0 || 5<cars[i].x1 || cars[i].x2<0 || 5<cars[i].x2 || cars[i].y1<0 || 5<cars[i].y1 || cars[i].y2<0 || 5<cars[i].y2){
                	printf("cars are not in a proper location\n");
                	goto err;
                }

			
	}

	if(cars[0].x2==5){
		printf("It is already solved\n");
		goto err;
	}

	fclose(fp);
	return 0;
	
err:
	fclose(fp);
	return 1;
}

void
display ()
{
	/* The beginning state of board1.txt must be shown as follows: 
 	 + + 2 + + +
 	 + + 2 + + +
	 1 1 2 + + +
	 3 3 3 + + 4
	 + + + + + 4
	 + + + + + 4
	*/

	//FIXME
	
	for(int i=5; i>=0; i--){
		for(int j=0; j<6; j++){
			if(cells[i][j]==0) 
				printf("%s","+ ");
			else
				printf("%d ", cells[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int 
update_cells ()
{
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.

	//FIXME
	// return 0 for sucess
	// return 1 if the given car information (cars) has a problem

	for(int i=0; i<n_cars; i++){
		if(cars[i].dir==vertical){
			for(int j=cars[i].y1; j>=cars[i].y2; j--){
				if(cells[j][cars[i].x1]!=0){
                                        printf("Not a valid data\n");
                                        return 1;
                                }
				cells[j][cars[i].x1]=cars[i].id;
			}
		}
		else /* (cars[i].dir == horizontal) */ {
			for(int j=cars[i].x1; j<=cars[i].x2; j++){
				if(cells[cars[i].y1][j]!=0){
                                        printf("Not a valid data\n");
                                        return 1;
                                }
				cells[cars[i].y1][j]=cars[i].id;
			}
		}
	}
	return 0;

}

int
move (int id, int op) 
{
	//FIXME
	// move returns 1 when the given input is invalid.
	// or return 0 for a success.

	// Update cars[id].x1, cars[id].x2, cars[id].y1 and cars[id].y2
	//   according to the given command (op) if it is possible.

	// The condition that car_id can move left is when 
	//  (1) car_id is horizontally placed, and
	//  (2) the minimum x value of car_id is greather than 0, and
	//  (3) no car is placed at cells[cars[id].y1][cars[id].x1-1].
	// You can find the condition for moving right, up, down as
	//   a similar fashion.
	
	if(!(0<id && id<=n_cars)){
		printf("Not a valid id\n\n");
		return 1;
	}

	id--;

	if ( (cars[id].dir==vertical && (op==1 || op==2)) || (cars[id].dir!=vertical && (op==3 | op==4))){
		printf("That direction is unavailable to move\n\n");
		return 1;	       
	}
	
	if (op==left){
		if(cars[id].x1==0 || cells[cars[id].y1][cars[id].x1-1]!=0){
			printf("You can't move it to left\n\n");
			return 1;
		}
		cars[id].x1--;
		cars[id].x2--;
	}
	if (op==right){
                if(cars[id].x2==5 || cells[cars[id].y1][cars[id].x2+1]!=0){
                        printf("You can't move it to right\n\n");
                        return 1;
                }
	     	cars[id].x1++;
                cars[id].x2++;	
        }   
	if (op==up){
                if(cars[id].y1==5 || cells[cars[id].y1+1][cars[id].x1]!=0){
                        printf("You can't move it to up\n\n");
                        return 1;
                }
	     	cars[id].y1++;
                cars[id].y2++;
        }   
	if (op==down){
                if(cars[id].y2==0 || cells[cars[id].y2-1][cars[id].x1]!=0){
                        printf("You can't move it to down\n\n");
                        return 1;
                }   
		cars[id].y1--;
                cars[id].y2--;
        }   
	return 0;

}

int
main ()
{
	char buf[128] ;
	int op ;
	int id ;
	int error;

	while (1) {
		scanf("%s", buf) ;
		switch (op = get_op_code(buf)) {
			case start:
				scanf("%s", buf) ;
				if(load_game(buf)) 
					break ;
				update_cells();
				display() ;
				break ;
			case left:
			case right:
			case up:
			case down:
				scanf("%d", &id) ;
				if(move(id, op))
					break ;
				if(update_cells())
					break ;
				display() ;
				if(cells[3][5]==1){
         		       		printf("done\n") ;
               				exit(0) ;
				}
				break ;
			//FIXME
			case quit:
				exit(0) ;
			case N_op:
				printf("Not a valid command\n") ;
				scanf("%[^\n]128s", buf) ;
		}
	}
}
