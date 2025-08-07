// ---------------- IMPORTATIONS ----------------

//standard
#include <stdio.h>
#include <stdlib.h>

//only for memcpy()
#include <string.h>

//own header
#include "queues.h"








/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Queues [0.1.1] ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                 Queues by I.A.

        Queues is an utility program that allows you to create and manipulate
    queues of any type (fixed type recommended).

        Theses queues are working as simple linked lists. Any kind of data
    can be stored inside them : different types / sizes / allocation places
    are allowed.

        ANY ITEM ADDED IS ALLOCATED INSIDE QUEUE IN HEAP. Make sure to use
    the correct queue_remove() or queue_pop() calls to free this data.

        As last point, some functions are using recursivity to work (for
    optimization). So there is ONE rule to keep in mind :

        ------------------------------------------
        /!\ DO NOT TURN A QUEUE INTO A CYCLE* ! /!\
        ------------------------------------------

    *(a cycle is a queue which has its last element pointing to its first)

    DISCLAIMER about mixed types :
            If you want to use different types in the same queue, you will
        have to remember the type each value refers to.
        (the first one is a float, the second one is an int, ...)

    22/01/2021 > [0.1.0] :
    - Created queues.c/.h.

    26/07/2023 > [0.1.1] :
    - Reformed the whole concept by adding queue_item structure.
      (Simplificated & optimized)

    BUGS : .
    NOTES : .

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                  By I.A.
******************************************************************************************

    LICENCE :

    C_Queues
    Copyright (C) 2021 Sebastien SILVANO

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.

    If not, see <https://www.gnu.org/licenses/>.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */








// ---------------- INSTANTIATION ----------------

//create - clear
queue* queue_create(){ //data used will no longer stay inside queue, it will be copied

	//create queue
	queue* q = malloc(sizeof(queue));
	if(q == NULL){
		puts("FATAL ERROR > queues.c : queue_create() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//set initial values
	q->length = 0;
	q->first  = NULL;

	return q;
}








// ---------------- USE ----------------

//check integrity
static char queue_check(queue* q, unsigned int index){

	//NULL queue
	if(q == NULL){
		puts("RUNTIME ERROR > queues.c : queue_check() : Queue is NULL.");
		return 1;
	}

	//out of bound
	if(index >= q->length){
		printf("RUNTIME ERROR > queues.c : queue_check() : Index '%u' is out of bound ('%u' elements).\n", index, q->length);
		return 1;
	}

	return 0;
}

//get - set
static queue_item* queue_getItem(queue* q, unsigned int index){
	if(queue_check(q, index)){
		return NULL;
	}

	//iterate until the correct q is found
	queue_item* current = q->first;
	for(unsigned int ui=0; ui < index; ui++){

		//internal error (optional)
		#ifdef INTERNAL_ERRORS
		if(current == NULL){
			puts("INTERNAL ERROR : queues.c : queue_get() : Next element should exist but is NULL.\n");
			return NULL;
		}
		#endif

		current = current->next;
	}

	return current;
}

void* queue_get(queue* q, unsigned int index){
	if(queue_check(q, index)){
		return NULL;
	}

	//get queue item
	queue_item* qi = queue_getItem(q, index);

	//internal error (optional)
	#ifdef INTERNAL_ERRORS
	if(qi == NULL){
		printf("INTERNAL ERROR > queues.c : queue_get() : Could not found queue_item at index %u.\n", index);
		return NULL;
	}
	#endif

	//return its data
	return qi->data;
}




//add - remove
void queue_append(queue* q, void* data){
	if(q == NULL){
		puts("RUNTIME ERROR > queues.c : queue_append() : Queue is NULL.");
		return;
	}

	//create new item
	queue_item* qi_new = malloc(sizeof(queue_item));
	if(qi_new == NULL){
		puts("FATAL ERROR > queues.c : queue_append() : Computer refuses to give more memory.");
		exit(EXIT_FAILURE);
	}

	//associate given data
	qi_new->data = data;

	//attach next element (no element)
	qi_new->next = NULL;

	//attach to existing queue : case 1 : empty queue
	if(q->first == NULL){
		q->first = qi_new;
	}

	//attach to existing queue : case 2 : get the latest element
	else{
		queue_item* last = q->first;
		while(last->next != NULL){ last = last->next; }
		last->next = qi_new;
	}

	//increase length
	q->length++;
}
