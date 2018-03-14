#include "headers/mtezq.h"


MTQueue* mt_initQueue(void (* free_function)(void*))
{
	MTQueue* new_queue = malloc(sizeof(MTQueue));
	if(new_queue != NULL)
	{
		new_queue->abs_front = NULL;
		new_queue->front = NULL;
		new_queue->back = NULL;
		new_queue->length = 0;
		new_queue->abs_length = 0;
		new_queue->free_function = free_function;
#ifdef WIN32_LEAN_AND_MEAN
		InitializeCriticalSection(&new_queue->lock);
#else
		pthread_mutex_init(&new_queue->lock, NULL);
#endif
	}
	return new_queue;
}


MTQueue* mt_convertQueue(Queue* queue)
{
	MTQueue* new_queue = mt_initQueue(queue->free_function);
	if(new_queue != NULL)
	{
		new_queue->abs_front = queue->abs_front;
		new_queue->front = queue->front;
		new_queue->back = queue->back;
		new_queue->length = queue->length;
		new_queue->abs_length = queue->abs_length;
	}
	return new_queue;
}


error_t mt_enqueue(MTQueue* queue, void* data)
{
	if(queue != NULL)
	{
		QueueNode* new_node = malloc(sizeof(QueueNode));
		if(new_node == NULL)
		{
			return 1;
		}
		
		new_node->data = data;
#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		if(queue->abs_length == 0)
		{
			new_node->next = NULL;
			new_node->prev = NULL;
			queue->abs_front = new_node;
			queue->front = new_node;
			queue->back = queue->front;
		}
		else
		{
			new_node->prev = queue->back;
			new_node->next = queue->back->next;
			
			if(queue->back->next != NULL)
			{
				queue->back->next->prev = new_node;
			}
			queue->back->next = new_node;
			queue->back = new_node;
			if(queue->length == 0)
			{
				queue->front = new_node;
			}
		}
		queue->length++;
		queue->abs_length++;
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return 0;
	}
	else
	{
		return 2;
	}
	
}


error_t mt_priorityEnqueue(MTQueue* queue, void* data)
{
	if(queue != NULL)
	{
		QueueNode* new_node = malloc(sizeof(QueueNode));
		if(new_node == NULL)
		{
			return 1;
		}

#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		new_node->data = data;
		if(queue->abs_length == 0)
		{
			new_node->next = NULL;
			new_node->prev = NULL;
			queue->abs_front = new_node;
			queue->front = new_node;
			queue->back = queue->front;
		}
		else if(queue->length == 0)
		{
			//means the queue is reset, so just normal queue instead so we don't confuse the total length
			new_node->prev = queue->back;
			new_node->next = queue->back->next;
			if(queue->back->next != NULL)
			{
				queue->back->next->prev = new_node;
			}
			queue->back->next = new_node;
			queue->back = new_node;
			queue->front = new_node;
			
		}
		else
		{
			new_node->prev = queue->front->prev;
			new_node->next = queue->front;
			
			if(queue->front == queue->abs_front)
			{
				queue->abs_front = new_node;
			}
			else
			{
				queue->front->prev->next = new_node;
			}
			
			queue->front->prev = new_node;
			queue->front = new_node;
			
		}
		queue->length++;
		queue->abs_length++;
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return 0;
	}
	else
	{
		return 2;
	}
}


error_t mt_resetQueue(MTQueue* queue)
{
	if(queue != NULL)
	{
#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		queue->front = queue->back;
		queue->length = 0;
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return 0;
	}
	else
	{
		return 2;
	}
	
}


error_t mt_restartQueue(MTQueue* queue)
{
	if(queue != NULL)
	{
#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		queue->front = queue->abs_front;
		queue->length = queue->abs_length;
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return 0;
	}
	else
	{
		return 2;
	}
}


void* mt_dequeue(MTQueue* queue)
{
	if(queue != NULL)
	{
#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		if(queue->front != NULL && queue->length > 0)
		{
			void* to_return = queue->front->data;
			QueueNode* new_front = queue->front->next;
			queue->front = new_front;
			queue->length--;
#ifdef WIN32_LEAN_AND_MEAN
			LeaveCriticalSection(&queue->lock);
#else
			pthread_mutex_unlock(&queue->lock);
#endif
			return to_return;
		}
		else
		{
#ifdef WIN32_LEAN_AND_MEAN
			LeaveCriticalSection(&queue->lock);
#else
			pthread_mutex_unlock(&queue->lock);
#endif
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}


void* mt_peekQueue(MTQueue* queue, int queue_location)
{
#ifdef WIN32_LEAN_AND_MEAN
	EnterCriticalSection(&queue->lock);
#else
	pthread_mutex_lock(&queue->lock);
#endif
	QueueNode* ret = NULL;
	if(queue->front != NULL && queue->length > 0)
	{
		if(queue_location == QUEUE_FRONT)
		{
			ret = queue->front->data;
		}
		else if(queue_location == QUEUE_BACK)
		{
			ret = queue->back->data;
		}
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return ret;
	}
	else
	{
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return ret;
	}
}


error_t mt_mergeQueue(MTQueue* new_queue, Queue** queues, size_t num_queues)
{
	if(new_queue != NULL)
	{
		size_t i,j;
		for(i = 0; i < num_queues; i++)
		{
			size_t q_len = queues[i]->length;
			for(j = 0; j < q_len; j++)
			{
				mt_enqueue(new_queue, dequeue(queues[i]));
			}
			restartQueue(queues[i]);
		}
		return 0;
	}
	else
	{
		return 2;
	}
}


error_t mt_mergeMTQueue(MTQueue* new_queue, MTQueue** queues, size_t num_queues)
{
	if(new_queue != NULL)
	{
		size_t i,j;
		for(i = 0; i < num_queues; i++)
		{
			size_t q_len = queues[i]->length;
			for(j = 0; j < q_len; j++)
			{
				mt_enqueue(new_queue, mt_dequeue(queues[i]));
			}
			mt_restartQueue(queues[i]);
		}
		
		return 0;
	}
	else
	{
		return 2;
	}
}


error_t mt_flushQueue(MTQueue* queue)
{
	if(queue != NULL)
	{
#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		while(queue->abs_length > 0)
		{
			QueueNode* next = queue->abs_front->next;
			if(queue->abs_front->data != NULL && queue->free_function != NULL)
			{
				queue->free_function(queue->abs_front->data);
			}
			free(queue->abs_front);
			queue->abs_front = next;
			queue->abs_length--;
		}
		queue->abs_front = NULL;
		queue->front = NULL;
		queue->back = NULL;
		queue->length = 0;
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return 0;
	}
	else
	{
		return 2;
	}
}


error_t mt_cleanQueue(MTQueue* queue)
{
	//move the absolute front to the same position as front and free up the queue objects along the way
	if(queue != NULL)
	{
#ifdef WIN32_LEAN_AND_MEAN
		EnterCriticalSection(&queue->lock);
#else
		pthread_mutex_lock(&queue->lock);
#endif
		while(queue->abs_front != queue->front)
		{
			QueueNode* next = queue->abs_front->next;
			if(queue->abs_front->data != NULL)
			{
				queue->free_function(queue->abs_front->data);
			}
			queue->abs_front->prev = NULL;
			queue->abs_front->data = NULL;
			free(queue->abs_front);
			queue->abs_front = next;
			queue->abs_length--;
		}
#ifdef WIN32_LEAN_AND_MEAN
		LeaveCriticalSection(&queue->lock);
#else
		pthread_mutex_unlock(&queue->lock);
#endif
		return 0;
	}
	else
	{
		return 2;
	}
}


void mt_freeQueue(MTQueue* queue)
{
	if(queue != NULL)
	{
		mt_flushQueue(queue);
#ifdef WIN32_LEAN_AND_MEAN
		DeleteCriticalSection(&queue->lock);
#else
		pthread_mutex_destroy(&queue->lock);
#endif
		free(queue);
	}
	//mt_queue being NULL is not an error
}


