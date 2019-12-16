#include <iostream>
#include <locale>
#include <windows.h>
#include <fstream>
#include <thread>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include "ThreadPool.h"
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <string>


std::vector<std::string> data;
std::vector<std::vector<std::string>> sep_data;

bool GetDataFromFile()
{
	std::string filename;
	std::cout << "Enter filename - ";
	std::cin >> filename;
	std::cout << "\n";
	std::cout << "reading file..";
	std::cout << "\n";
	std::ifstream reader(filename.c_str());
	if (!reader.is_open())
	{
		reader.close();
		return false;
	}

	while (!reader.eof())
	{
		
		std::string tmp_s;
		getline(reader, tmp_s, '\n');
		data.push_back(tmp_s);
	}
	reader.close();
	return true;
}

short GetThreadCapacity()
{
	short thread_quantity, max_thread = std::thread::hardware_concurrency();
	std::cout << "max number of thread - " << max_thread << std::endl;
	std::cout << "enter threads count - ";
	std::cin >> thread_quantity;
	std::cout << "\n";
	if ((thread_quantity > data.size()) || (thread_quantity > max_thread) || (thread_quantity <= 0))
		return -1;
	return thread_quantity;
}

void SortPiece(void* arr_piece)
{
	std::vector<std::string>* obj = (std::vector<std::string>*)arr_piece;
	std::sort(obj->begin(), obj->end());
}

int GetMinValueIndex(std::vector<std::string> indata)
{
	std::string min_value;
	int index;
	for (int i = 0; i < indata.size(); i++)
		if (indata[i] != "")
		{
			min_value = indata[i];
			index = i;
			break;
		}

	for (int i = 0; i < indata.size(); i++)
		if (indata[i] != "")
			if (strcmp(min_value.c_str(), indata[i].c_str()) > 0)
				if (min_value > indata[i])
				{
					min_value = indata[i];
					index = i;
				}
	return index;
}
bool CorrectBuff(std::vector<std::string> indata)
{
	for (int i = 0; i < indata.size(); i++)
		if (indata[i] != "")
			return true;
	return false;
}

void FinalSort()
{
	int result_size = data.size();
	data.clear();
	std::vector<int> counter;
	counter.reserve(sep_data.size());
	for (int i = 0; i < sep_data.size(); i++)
		counter.push_back(0);
	std::vector<std::string> buff;

	while (1)
	{
		buff.clear();
		for (int i = 0; i < sep_data.size(); i++)
			if (sep_data[i].size() > counter[i])
				buff.push_back(sep_data[i][counter[i]]);
			else
				buff.push_back("");
		if (!CorrectBuff(buff))
			break;
		int minvalue_index = GetMinValueIndex(buff);
		data.push_back(buff[minvalue_index]);
		counter[minvalue_index]++;
	}
}

void CreateTasks(std::vector<std::string> indata, short quantity, TaskQueue* result)
{
	bool isEven = indata.size() % quantity == 0;
	int standard_string_capacity = indata.size() / quantity;
	if (!isEven)
		standard_string_capacity = indata.size() / quantity + 1;
	int global_counter = 0;
	for (int i = 0; i < quantity; i++)
	{
		sep_data[i].clear();
		int tmp_counter = 0;
		while (standard_string_capacity != tmp_counter)
		{
			if (global_counter == indata.size())
				break;
			sep_data[i].push_back(indata[global_counter]);
			global_counter++;
			tmp_counter++;
		}
		Task* newtask = new Task(&SortPiece, (void*)& sep_data[i]);
		result->Enqueue(newtask);
	}
}

int main()
{
	setlocale(LC_ALL, "russian");
	if (!GetDataFromFile())
	{
		std::cout << "file not founded." << std::endl;
		system("pause");
		return 1;
	}
	//std::cout << "__________________" << std::endl;
	//std::cout << "Indata from file:\n\n";
	//for (int i = 0; i < data.size(); i++)
	//	std::cout << data[i] << std::endl;
	//std::cout << "\n";
	short thread_quantity = GetThreadCapacity();
	if (thread_quantity == -1)
	{
		std::cout << "invalid thread count value." << std::endl;
		system("pause");
		return 1;
	}
	sep_data.resize(thread_quantity);
	TaskQueue* taskqueue = new TaskQueue();
	CreateTasks(data, thread_quantity, taskqueue);
	ThreadPool* threads = new ThreadPool(thread_quantity, taskqueue);
	threads->WaitAll();
	delete(threads);
	delete(taskqueue);
	FinalSort();
	std::cout << "DONE\n" << std::endl;
	std::cout << "enter output filename: " << std::endl;
	
	std::string outputFile;
	std::cin >> outputFile;
	std::ifstream fo(outputFile);
	if (!fo.good()) {
		std::cout << "Can't find output file" << std::endl;
		getchar();
		getchar();
		return 0;
	}
	else
	{
		std::cout << "writing data to file.." << std::endl;
		std::ofstream stream(outputFile);
		for (std::string str : data) {
			const char* cstr = str.c_str();
			stream.write(cstr, strlen(cstr));
		}
        stream.close();	
		std::cout << "done" << std::endl;
	}
	
	data.clear();
	system("pause");
	return 0;
}
