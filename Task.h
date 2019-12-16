#ifndef TASK_H
#define TASK_H

class Task
{
public:
	void(*func)(void*);
	void* params;

	Task(void(*func)(void*), void* params);

	Task(const Task&);

	~Task();
};

#endif
