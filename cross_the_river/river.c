#include <stdio.h>
#include <stdlib.h>

typedef enum {
	left,
	right
} loc;

typedef struct s{
	loc person;
	loc wolf;
	loc sheep;
	loc cabbage;
	struct s *prev;
} state;

int compare (state a, state b){
	if(a.person == b.person && a.wolf == b.wolf && a.sheep == b.sheep && a.cabbage == b.cabbage){
		return 1;
	}
	return 0;
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

int find(queue *q, state s){
	node *n = q->front;
	while(n != NULL){
		if(compare(s, *n->data)){
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

state* set(int p, int w, int s, int c){
	state *ss = malloc(sizeof(state));
	ss->person = p;
	ss->wolf = w;
	ss->sheep = s;
	ss->cabbage = c;
	ss->prev = NULL;
	return ss;
}

queue *initial(int person, int wolf, int sheep, int cabbage){
	int idx = 0;
	
	queue *q = malloc(sizeof(queue));
	int l[4] = {person, wolf, sheep, cabbage};
	for(int i=0; i<4; i++){
		if(person == l[i]){
			l[i] = !l[i];
			if(!((l[1] == l[2] && l[0] != l[1]) ||(l[2] == l[3] && l[0] != l[2]))){
       	 		state *st = set(l[0], l[1], l[2], l[3]);
        		push(q, st);
    	    }
			if(i != 0){
				l[i] = !l[i];
			}
		}
	}
	return q;
}

void image(int n[]){
	char * content[4] = {"person", "wolf", "sheep", "cabbage"};

	for(int i=0; i<4; i++){
		if(n[i]==left){
			printf("%s\t|\t|\t\n",content[i]);
		}
		else{
			printf("\t|\t|%s\n",content[i]);
		}
	}
	printf("------------------------\n");

}

int main(){

	state first = {left, left, left, left, NULL};
    state bingo = {right, right, right,right, NULL};

	queue q = {NULL};
	push(&q, &first);
	state *ns, *finish, *nadj;
	queue visited = {NULL};
	push(&visited, &first);
	
	while(!qisempty(&q)){
		ns = pop(&q);
				push(&visited, ns);
		if(compare(*ns, bingo)){
			finish = ns;
			break;
		}

		queue *adj = initial(ns->person, ns->wolf, ns->sheep, ns->cabbage);
	
		while(!(qisempty(adj))){
			nadj = pop(adj);
	 		if(!find(&visited, *nadj)){
				push(&q, nadj);
				nadj->prev = ns;
			}	
		}
	}

	printf("\n\n<person, wolf, sheep, cabbage>\n\n");
	printf("------------------------\n");

	state *n = finish, *nn = NULL, *nnn;

    while(n != NULL){
        nnn = nn;
        nn = n;
        n = n->prev;
        nn->prev = nnn;
    }

    while(nn != NULL){
        int num[4] = {nn->person, nn->wolf, nn->sheep, nn->cabbage};
        image(num);
        nn = nn->prev;
    }
	
	printf("\n!!CROSSED THE RIVER!!\n\n\n");	

	return 0;
}
