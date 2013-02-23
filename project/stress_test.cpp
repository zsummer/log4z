#include "../log4z.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#if WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <stdlib.h>
#endif
using namespace zsummer::log4z;

//cross platform function:
void SleepMillisecond(unsigned int ms);
bool CreateThread(void(*run)());


//enum multi logger
enum ENUM_LOGGER
{
	L_MAIN=0, //the main logger, It away exist.
	L_MYSQL,	//the user-defined logger.
	L_NET,		//the user-defined logger.
	L_MONITER,	//the user-defined logger.
};
LoggerId g_logger[L_MONITER+1];

//stress attribute
char g_data[100];
const int FOR_MAX = 7;

void MysqlModuleTrace()
{
	while(1)
	{
		for (int i=0; i<FOR_MAX; i++)
		{
			LOG_DEBUG(g_logger[L_MYSQL], g_data);
		}
		if (rand()%100 < 5)
		{
			SleepMillisecond(5);
		}
	}
}
//virtual the Network module in a project.
void NetworkModuleTrace()
{
	while(1)
	{
		for (int i=0; i<FOR_MAX; i++)
		{
			LOG_DEBUG(g_logger[L_NET], g_data);
		}
		if (rand()%100 < 5)
		{
			SleepMillisecond(5);
		}
	}
}
//virtual the Moniter module in a project.
void MoniterModuleTrace()
{
	while(1)
	{
		for (int i=0; i<FOR_MAX; i++)
		{
			LOG_DEBUG(g_logger[L_MONITER], g_data);
		}
		if (rand()%100 < 5)
		{
			SleepMillisecond(5);
		}
	}
}
//////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	memset(g_data, '-', sizeof(g_data));
	g_data[sizeof(g_data)-1] = '\0';
	//add and configure logger
	ILog4zManager::GetInstance()->ConfigMainLogger("", "L_MAIN");
	g_logger[L_MAIN] = ILog4zManager::GetInstance()->GetMainLogger();

	g_logger[L_MYSQL] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_MYSQL");
	g_logger[L_NET] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_NET");
	g_logger[L_MONITER] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_MONITER");

	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_MYSQL], false);
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_NET], false);
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_MONITER], false);

	ILog4zManager::GetInstance()->Start();

	//create thread
	CreateThread(&MysqlModuleTrace);
	CreateThread(&NetworkModuleTrace);
	CreateThread(&MoniterModuleTrace);


	unsigned long long lastCount = 0;
	unsigned long long lastData = 0;
	while(1)
	{ 
		unsigned long long speedCount = ILog4zManager::GetInstance()->GetStatusTotalWriteCount() - lastCount;
		lastCount += speedCount;
		unsigned long long speedData = ILog4zManager::GetInstance()->GetStatusTotalWriteBytes() - lastData;
		lastData += speedData;
		LOGI("Stress Status:  Write Speed: " << speedCount/5 
			<< " n/s, Speed: " << speedData/1024/5 
			<< " KB/s, Waiting: " << ILog4zManager::GetInstance()->GetStatusWaitingCount()
			<< " n, Total Count: " << lastCount
			<< " n, Total Data: " << lastData);
		SleepMillisecond(5000);
	}

	printf("press anykey to exit.");
	getchar();
	return 0;
}


//////////////////////////////////////////////////////////////////////////
void SleepMillisecond(unsigned int ms)
{
#ifdef WIN32
	::Sleep(ms);
#else
	usleep(1000*ms);
#endif
}



//
#ifdef WIN32
 static unsigned int WINAPI  ThreadProc(LPVOID lpParam)
{
	((void(*)())(lpParam))();
	_endthreadex(0);
	return 0;
}
#else
static void * ThreadProc(void * pParam)
{
	((void(*)())(pParam))();
	return NULL;
}
#endif

bool CreateThread(void(*run)())
{
#ifdef WIN32
	unsigned long long ret = _beginthreadex(NULL, 0, ThreadProc,(void*) run, 0, NULL);

	if (ret == -1 || ret == 1  || ret == 0)
	{
		return false;
	}
#else
	pthread_t ptid = 0;
	int ret = pthread_create(&ptid, NULL, ThreadProc, (void*)run);
	if (ret != 0)
	{
		return false;
	}
#endif
	return true;
}

