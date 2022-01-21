#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	start=0,
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
// --- NOFIX 

int load_game (char * filename) {
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

	if(cars[0].x2 == 5){
		printf("It is already solved\n");
		goto err;
	}

	fclose(fp);
	return 0;
	
err:
	fclose(fp);
	return 1;
}

void display () {
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


int update_cells (car_t *s) {
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.;

	for(int i=0; i<n_cars; i++){
		if(s[i].dir == vertical){
			for(int j=s[i].y1; j>=s[i].y2; j--){
				if(cells[j][s[i].x1]!=0){
					printf("Not a valid data\n");
					return 1;
				}
				cells[j][s[i].x1]=s[i].id;
			}
		}
		else /* (s[i].dir == horizontal) */ {
			for(int j=s[i].x1; j<=s[i].x2; j++){
				if(cells[s[i].y1][j]!=0){
					printf("Not a valid data\n");
					return 1;
				}
				cells[s[i].y1][j]=s[i].id;
			}
		}
	}
	return 0;

}

int notvalidmove(car_t *s, int id, int op){
	if(!(0 <= id && id <= n_cars)){
		return 1;
	}
	if ( (s[id].dir == vertical && (op == left || op == right)) || (s[id].dir == horizontal && (op == up || op == down))){
		return 1;	       
	}
	if (op==left){
		if(s[id].x1 == 0 || cells[s[id].y1][s[id].x1-1] != 0){
			return 1;
		}
	}
	if (op==right){
		if(s[id].x2 == 5 || cells[s[id].y1][s[id].x2+1] != 0){
			return 1;
		}
	}   
	if (op==up){
		if(s[id].y1 == 5 || cells[s[id].y1+1][s[id].x1] != 0){
			return 1;
		}
	}   
	if (op==down){
		if(s[id].y2 == 0 || cells[s[id].y2-1][s[id].x1] != 0){
			return 1;
		}   
	}   
	return 0;
}

void move (car_t *s, int id, int op) {
	if (op==left){
		s[id].x1--;
		s[id].x2--;
	}
	if (op==right){
		s[id].x1++;
		s[id].x2++;	
	}   
	if (op==up){
		s[id].y1++;
		s[id].y2++;
	}   
	if (op==down){
		s[id].y1--;
		s[id].y2--;
	}   
}

typedef struct s{
	car_t *scars;
	struct s *prev;
} state;

int compare (car_t *a, car_t *b){
	for(int i=0; i<n_cars; i++){
		if(!(a[i].x1 == b[i].x1 && a[i].x2 == b[i].x2 && a[i].y1 == b[i].y1 && a[i].y2 == b[i].y2 && a[i].id == b[i].id)){
			return 0;
		}
	}
	return 1;
}

typedef struct n{
    state *data;
	struct n *next;
} node;

typedef struct {
    node *front;
	node *rear;
} queue;

int qisempty(queue *q){
	if(q->front == NULL){
		return 1;
	}
	return 0;
}

state *pop(queue *q){

	if(qisempty(q)){
		fprintf(stderr,"The queue is empty\n");
		return NULL;
	}
	node *t = q->front;
	q->front = q->front->next;
	return t->data;
}

int find(queue *q, state *s){
	node *n = q->front;
	while(n != NULL){
		if(compare(s->scars, n->data->scars)){
			return 1;
		}
		n = n->next;
	}
	return 0;
}

void push(queue *q, state *s){
	node *newnode = malloc(sizeof(node));
	newnode->data = s;
	newnode->next = NULL;
	if(qisempty(q)){            
		q->front = newnode;
		q->rear = newnode;
	}
    else{
		q->rear->next = newnode;
		q->rear = newnode;
	}
}


state *makestate(car_t *c){
	state *s = malloc(sizeof(state));
	s->scars = c;
	s->prev = NULL;
	return s;
}

void copycars(car_t *s1, car_t *s2){
	for(int i=0; i<n_cars; i++){
		s1[i].x1 = s2[i].x1;
		s1[i].x2 = s2[i].x2;
		s1[i].y1 = s2[i].y1;
		s1[i].y2 = s2[i].y2;
		s1[i].span = s2[i].span;
		s1[i].id = s2[i].id;
		s1[i].dir = s2[i].dir;
	}
}

car_t *setcars(car_t *s){
	car_t *c = malloc(sizeof(car_t) * n_cars);
	copycars(c, s);
	return c;
}

queue *initial(car_t *s){
	update_cells(s);
	queue *q = malloc(sizeof(queue));
	q->front=NULL;

	for(int i=0; i<n_cars; i++){
		for(int j=1; j<5; j++){
			if(!notvalidmove(s, i, j)){
				car_t *c = setcars(s);
				move(c, i, j);
				state *st = makestate(c);
				push(q, st);
			}
		}
	}
	return q;
}

state* reverse(state* head){
	state *n = head, *nn = NULL, *nnn; 
	while(n!=NULL){
		nnn = nn;
		nn = n;
		n = n->prev;
		nn->prev = nnn;		
	}
	return nn;
}

int main(){
	int i=0;

	char buf[128] ;

	scanf(" %s", buf) ;
	if(load_game(buf)){
		exit(0);
	}
	printf("\n == start ==\n\n");

	queue q = {NULL, NULL};

	state *s = makestate(cars);
	push(&q, s);
	state *ns, *finish, *nadj;
	queue visited = {NULL, NULL};
	push(&visited, s);
	
	while(!qisempty(&q)){
		ns = pop(&q);
		push(&visited, ns);
		i++;
		if(ns->scars[0].x2 == 5){
			finish = ns;
			break;
		}
		queue *adj = initial(ns->scars);      
		while(!(qisempty(adj))){
			nadj = pop(adj);
	 		if(!find(&visited, nadj)){

				nadj->prev = ns;
				push(&q, nadj);
			}
		}
	}
    
	finish = reverse(finish);

	while(finish != NULL){
		printf("-----------\n\n");
		update_cells(finish->scars);
        display();
		finish = finish->prev;
    }

	printf(" == DONE ==\n\n");
	printf("%d\n", i);
	return 0;
}
