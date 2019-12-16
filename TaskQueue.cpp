#include "TaskQueue.h"

TaskQueue::TaskQueue()
{
	InitializeCriticalSectionAndSpinCount(&csection, SPINS);
	first = NULL;
	last = NULL;
}

Task* TaskQueue::Front()
{
	EnterCriticalSection(&csection);
	if (Empty())
	{
		LeaveCriticalSection(&csection);
		return NULL;
	}
	if (first == last)
	{
		Node* tmp_return = first;
		first = NULL;
		last = NULL;
		LeaveCriticalSection(&csection);
		return tmp_return->data;
	}
	Node* tmp_last = last;
	last = last->prev;
	last->next = NULL;
	LeaveCriticalSection(&csection);
	return tmp_last->data;
}

void TaskQueue::Enqueue(Task* indata)
{
	EnterCriticalSection(&csection);
	Node* tmp = new Node();
	tmp->data = indata;
	tmp->next = NULL;
	tmp->prev = NULL;

	if (Empty())
	{
		first = tmp;
		last = tmp;
	}
	else
	{
		Node* tmp_first = first;
		first = tmp;
		first->next = tmp_first;
		tmp_first->prev = first;
	}
	LeaveCriticalSection(&csection);
}

void TaskQueue::Dequeue()
{
	Task* tmp = Front();
	delete(tmp);
}

bool TaskQueue::Empty()
{
	return ((first == NULL) && (last == NULL));
}

TaskQueue::~TaskQueue()
{
	while (!Empty())
		Dequeue();
	DeleteCriticalSection(&csection);
}
