#include "ezq.h"


#define arr_sz 10000
typedef struct
{
	int* vvv_big_array;
} Object;

void makeObj(Object* obj);
void initObj(Object* obj);
void freeObj(void* o);


int main()
{
	Object* obj;
	Queue* queue = initQueue(freeObj);
	
	for(int i = 0; i < 10; i++)
	{
		obj = malloc(sizeof(Object));
		makeObj(obj);
		enqueue(queue, obj);
	}
	
	for(int j = 0; j < 20; j++)
	{
		obj = malloc(sizeof(Object));
		makeObj(obj);
		priorityEnqueue(queue, obj);
	}
	
	dequeue(queue);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	priorityEnqueue(queue, obj);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	priorityEnqueue(queue, obj);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	enqueue(queue, obj);
	dequeue(queue);
	
	cleanQueue(queue);
	
	flushQueue(queue);
	
	dequeue(queue);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	priorityEnqueue(queue, obj);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	priorityEnqueue(queue, obj);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	enqueue(queue, obj);
	dequeue(queue);
	
	for(int j = 0; j < 20; j++)
	{
		obj = malloc(sizeof(Object));
		makeObj(obj);
		priorityEnqueue(queue, obj);
	}
	
	for(int i = 0; i < 10; i++)
	{
		obj = malloc(sizeof(Object));
		makeObj(obj);
		enqueue(queue, obj);
	}
	
	resetQueue(queue);
	
	obj = malloc(sizeof(Object));
	makeObj(obj);
	priorityEnqueue(queue, obj);
	obj = malloc(sizeof(Object));
	makeObj(obj);
	enqueue(queue, obj);
	dequeue(queue);
	
	for(int j = 0; j < 20; j++)
	{
		obj = malloc(sizeof(Object));
		makeObj(obj);
		priorityEnqueue(queue, obj);
	}
	
	for(int i = 0; i < 10; i++)
	{
		obj = malloc(sizeof(Object));
		makeObj(obj);
		enqueue(queue, obj);
	}
	
	cleanQueue(queue);
	
	for(int k = 0; k < 10; k++)
	{
		pullQueue(queue);
	}
	
	cleanQueue(queue);
	free(queue);
	
	return 0;
}


void makeObj(Object* obj)
{
	initObj(obj);
	obj->vvv_big_array = malloc(arr_sz * sizeof(int));
	if(obj->vvv_big_array == NULL)
	{
		exit(1);
	}
	
	for(int j = 0; j < arr_sz; j++)
	{
		obj->vvv_big_array[j] = j;
	}
}


void initObj(Object* obj)
{
	obj->vvv_big_array = NULL;
}


void freeObj(void* o)
{
	Object* obj = (Object*)o;
	if(obj->vvv_big_array != NULL)
	{
		free(obj->vvv_big_array);
	}
	free(obj);
}