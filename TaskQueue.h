#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <windows.h>

#include "Task.h"

#define SPINS 4000

class Node
{
public:
	Task* data;
	Node* prev;
	Node* next;
};

class TaskQueue
{
private:
	Node* first, * last;
	CRITICAL_SECTION csection;

public:
	TaskQueue();

	TaskQueue(const TaskQueue&);

	Task* Front();

	void Enqueue(Task* indata);

	void Dequeue();

	bool Empty();

	~TaskQueue();
};

#endif

