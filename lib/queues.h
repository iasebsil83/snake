#ifndef QUEUES_H
#define QUEUES_H






// ---------------- DEFINITIONS ----------------

//structures
typedef struct QUEUE_ITEM {
	struct QUEUE_ITEM* next;
	void*              data;
} queue_item;
typedef struct QUEUE {
	unsigned int length;
	queue_item*  first;
} queue;






// ---------------- INSTANTIATION ----------------

//create - clear
queue* queue_create();






// ---------------- USE ----------------

//get - set
void* queue_get(queue* q, unsigned int index);

//add - remove
void  queue_append(queue* q, void* data);






#endif
