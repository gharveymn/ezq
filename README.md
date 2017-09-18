# ezq

This is a simple queueing library for C which allows for malloc-and-forget functionality.

## Usage

This system is made for data objects which are malloced, so in the simplest case where we have a first order malloced item we write

```C
	//NULL means this queue will use free(void*) when flushing
	Queue* queue = initQueue(NULL);
```

which is the case of a queue storing strings or something. Now if the data objects your queue will store has nested mallocs you can pass a function which will free each one of those objects into the initializer. For example, 

```C
typedef struct
{
	int* vvv_big_array;
} Object;

int main(void)
{
	Queue* queue = initQueue(freeObj);

	//...

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
```
Once this is done the resposibility is no longer yours to free the individual objects. To pass objects into the queue in this case we could write something like 
```C
//...
int main(void)
{
	Queue* queue = initQueue(freeObj);
	Object* my_obj = malloc(sizeof(Object));
	initializeObj(my_obj);
	enqueue(queue, obj);

	//and to dequeue just write
	Object* your_obj = dequeue(queue);

	//this does not free the queue node
	//and none of the nodes are freed until cleanQueue, 
	//flushQueue, or freeQueue are called.

	//cleanQueue cleans up already dequeued nodes but does not
	//affect ones that are still available

	//flushQueue frees all nodes in the queue regardless of use

	//freeQueue frees the entire queue object. Be sure to
	//call this at the end of execution

	freeQueue(queue);
}
//...
```
where freeing the objects occurs when `freeQueue(queue)` executes. One can also priority enqueue and peek the front and back ends of the queue.
