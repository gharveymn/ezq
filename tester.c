#include "src/headers/ezq.h"


#define arr_sz 10000
typedef struct
{
	int* vvv_big_array;
} Object;

Object* makeObj(void);
void initObj(Object* obj);
void freeObj(void* o);


int main()
{
	Object* obj;
	Queue* queue = initQueue(freeObj);
	
	for(int i = 0; i < 10; i++)
	{
		obj = makeObj();
		enqueue(queue, obj);
	}
	
	for(int j = 0; j < 20; j++)
	{
		obj = makeObj();
		priorityEnqueue(queue, obj);
	}
	
	dequeue(queue);
	obj = makeObj();
	priorityEnqueue(queue, obj);
	obj = makeObj();
	priorityEnqueue(queue, obj);
	obj = makeObj();
	enqueue(queue, obj);
	dequeue(queue);
	
	cleanQueue(queue);
	
	flushQueue(queue);
	
	dequeue(queue);
	obj = makeObj();
	priorityEnqueue(queue, obj);
	obj = makeObj();
	priorityEnqueue(queue, obj);
	obj = makeObj();
	enqueue(queue, obj);
	dequeue(queue);
	
	for(int j = 0; j < 20; j++)
	{
		obj = makeObj();
		priorityEnqueue(queue, obj);
	}
	
	for(int i = 0; i < 10; i++)
	{
		obj = makeObj();
		enqueue(queue, obj);
	}
	
	resetQueue(queue);
	
	obj = makeObj();
	priorityEnqueue(queue, obj);
	obj = makeObj();
	enqueue(queue, obj);
	dequeue(queue);
	
	for(int j = 0; j < 20; j++)
	{
		obj = makeObj();
		priorityEnqueue(queue, obj);
	}
	
	for(int i = 0; i < 10; i++)
	{
		obj = makeObj();
		enqueue(queue, obj);
	}
	
	cleanQueue(queue);
	free(queue);
	
	return 0;
}


Object* makeObj(void)
{
	Object* obj = malloc(sizeof(Object));
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
	
	return obj;
	
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