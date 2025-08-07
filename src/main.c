// ---------------- IMPORTATIONS ----------------

//system
#include <stdio.h>
#include <stdlib.h>

//queues
#include "../lib/queues.h"

//multithreading
#include "../lib/threads.h"

//useful
#include <stdbool.h>
#include <unistd.h>
#include <time.h> //for random

//temporization
#include <math.h>






// ---------------- DEFINITIONS ----------------

//window
const int WINDOW_WIDTH  = 40;
const int WINDOW_HEIGHT = 20;

//directions
const char UP          = '\x00';
const char RIGHT       = '\x01';
const char DOWN        = '\x02';
const char LEFT        = '\x03';
const char INVALID_DIR = '\x09';

//speed
const int STEP = 1;

//allowed radius for getting point
const int ALLOWED_RADIUS = 1;






// ---------------- INITIALIZATION ----------------

//dimensions
const int WINDOW_WIDTH_2  = WINDOW_WIDTH/2;
const int WINDOW_HEIGHT_2 = WINDOW_HEIGHT/2;

//input
char key[2] = "__"; //initial value doesn't matter

//execution control
bool end = false;






// ---------------- TOOLS ----------------

//user input
void* inputLoop(void* args) {
	while(!end) { read(STDIN_FILENO, &key, 2); }
	return NULL;
}

int randint(int min, int max) { return min + (rand() % (max-min)); }



//game element
typedef struct {
	int  x;
	int  y;
	char dir;
} xyd;

xyd* xyd_new(int x, int y, char dir) {
	xyd* a = malloc(sizeof(xyd));
	a->x   = x;
	a->y   = y;
	a->dir = dir;
	return a;
}


//display
void clear() { printf("\x1b[H\x1b[2J\x1b[3J"); }

void printFrame() {
	char* text = malloc( (WINDOW_WIDTH+1)*WINDOW_HEIGHT);
	int index  = 0;

	//upper line
	for(int a=0; a < WINDOW_WIDTH; a++) { text[index++] = '#'; }
	text[index++] = '\n';

	//middle lines
	for(int a=0; a < WINDOW_HEIGHT-2; a++) {
		text[index++] = '#';
		for(int b=0; b < WINDOW_WIDTH-2; b++) { text[index++] = ' '; }
		text[index++] = '#';
		text[index++] = '\n';
	}

	//lower line
	for(int a=0; a < WINDOW_WIDTH; a++) { text[index++] = '#'; }

	//print result
	text[index] = '\0';
	puts(text);
}

void draw(int x, int y, char c) { printf("\x1b[%d;%dH%c", y+1, x+1, c); }






// ---------------- EXECUTION ----------------

//main
int main(int argc, char** argv) {

	//init random
	srand(time(NULL));

	//main elements
	queue* snake = queue_create();
	int point[2] = {WINDOW_WIDTH_2,WINDOW_HEIGHT_2};

	//initial state
	xyd* initialChunk = xyd_new(WINDOW_WIDTH_2,WINDOW_HEIGHT_2,RIGHT);
	queue_append(snake, initialChunk, sizeof(xyd));

	//input thread
	thread* inputThread = thread_create(inputLoop, NULL);
	thread_start(inputThread);

	//main loop
	while(!end) {
		xyd* head = queue_get(snake, 0);

		//input affects direction
		switch(key[0]) {
			case 'z': head->dir = UP;    break;
			case 'q': head->dir = LEFT;  break;
			case 's': head->dir = DOWN;  break;
			case 'd': head->dir = RIGHT; break;
		}

		//every dot moves according to its own direction
		for(int a=0; a < snake->length; a++) {
			xyd* current = queue_get(snake, a);
			switch(current->dir) {
				case UP:    current->y -= STEP; break;
				case RIGHT: current->x += STEP; break;
				case DOWN:  current->y += STEP; break;
				case LEFT:  current->x -= STEP; break;
			}
		}

		//death condition
		if(snake->length != 1) {
			xyd* afterHead = queue_get(snake, 1);
			if(abs(afterHead->dir - head->dir) == 2) { end = true; }
		}

		//transfer directions from head to tail
		int lastIndex = snake->length -1;
		for(int a=0; a < snake->length; a++) {
			if(a != lastIndex) {
				xyd* current  = queue_get(snake, lastIndex-a);
				xyd* previous = queue_get(snake, lastIndex-a-1);
				current->dir  = previous->dir; //transfert the direction of the point before us but in the reverse order
			}
		}

		//get point => increase length
		if(head->x <= point[0]+ALLOWED_RADIUS+1 && head->x >= point[0]-ALLOWED_RADIUS && head->y <= point[1]+ALLOWED_RADIUS+1 && head->y >= point[1]-ALLOWED_RADIUS) {
			point[0] = randint(ALLOWED_RADIUS, WINDOW_WIDTH -ALLOWED_RADIUS); //new position
			point[1] = randint(ALLOWED_RADIUS, WINDOW_HEIGHT-ALLOWED_RADIUS);
			xyd* lastChunk = queue_get(snake, lastIndex);

			//new chunk
			queue_append(
				snake,
				xyd_new(lastChunk->x, lastChunk->y, INVALID_DIR), //invalid direction for the moment, it must not move the 1st time, then it will be updated next iteration
				sizeof(xyd)
			);
		}

		//out of screen
		if(head->x < 0 || head->x >= WINDOW_WIDTH || head->y < 0 || head->y >= WINDOW_HEIGHT) { end = true; }

		//display rfresh
		clear();
		printFrame();

		//display dot
		draw(point[0], point[1], '@');

		//display snake
		for(int a=0; a < snake->length; a++) {
			xyd* current = queue_get(snake, a);
			draw(current->x, current->y, 'o');
		}
		//printf("KEY[%c%c]\n", key[0], key[1]);
		fflush(stdout);

		//temporization
		usleep( 2000000.0/(5.0*log(1.0 + snake->length)) );
	}
	puts("You died");

	//success
	return EXIT_SUCCESS;
}
