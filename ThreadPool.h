#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "TaskQueue.h"

#define SPINS 4000

class ThreadPool
{
public:

	ThreadPool(int thread_quantity, TaskQueue* task_queue);
	void WaitAll();
	~ThreadPool();
private:
	HANDLE* threads;
	CRITICAL_SECTION csection;
	int thread_quantity;
	static DWORD WINAPI DoThreadWork(LPVOID obj);
};

#endif
