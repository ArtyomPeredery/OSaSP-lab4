#include "ThreadPool.h"

ThreadPool::ThreadPool(int thread_quantity, TaskQueue* task_queue)
{

	threads = (HANDLE*)malloc(sizeof(HANDLE) * thread_quantity);
	this->thread_quantity = thread_quantity;
	InitializeCriticalSection(&csection);
	for (int i = 0; i < thread_quantity; i++)
		threads[i] = CreateThread(NULL, 0, &ThreadPool::DoThreadWork, (LPVOID)task_queue, CREATE_SUSPENDED, NULL);
	EnterCriticalSection(&csection);
	for (int i = 0; i < thread_quantity; i++)
		ResumeThread(threads[i]);
	LeaveCriticalSection(&csection);
}

void ThreadPool::WaitAll()
{
	WaitForMultipleObjects(thread_quantity, threads, TRUE, INFINITE);
}

ThreadPool::~ThreadPool()
{
	DeleteCriticalSection(&csection);
}

DWORD WINAPI ThreadPool::DoThreadWork(LPVOID obj)
{
	TaskQueue* tasks = (TaskQueue*)obj;
	while (1)
	{
		Task* tmp = tasks->Front();
		if (tmp == NULL)
			ExitThread(0);
		tmp->func(tmp->params);
	}
	return 0;

}

