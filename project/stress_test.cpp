#include "../log4z.h"
#include <iostream>
#include <stdio.h>

#if WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <stdlib.h>
#endif
using namespace zsummer::log4z;

//cross platform function:
//sleep in linux or windows
void SleepMillisecond(unsigned int ms);
//create thread in linux or windows
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

//virtual the mysql module in a project.
void MysqlModuleTrace()
{
	while(1)
	{
		LOG_DEBUG(g_logger[L_MYSQL], "mysql trace some msg ...");
		LOG_DEBUG(g_logger[L_MYSQL], "mysql trace some msg ...");
		LOG_DEBUG(g_logger[L_MYSQL], "mysql trace some msg ...");
		LOG_DEBUG(g_logger[L_MYSQL], "mysql trace some msg ...");
		LOG_DEBUG(g_logger[L_MYSQL], "mysql trace some msg ...");
		LOG_DEBUG(g_logger[L_MYSQL], "mysql trace some msg ...");

		int rd = rand()%100;
		if (rd <2)
		{
			LOG_FATAL(g_logger[L_MYSQL], "mysql some time put the fatal msg ...");
		}
		if (rd <5)
		{
			LOG_ERROR(g_logger[L_MYSQL], "mysql some time put the error msg ...");
		}
		if (rd <= 30)
		{
			SleepMillisecond(rd%10 == 0);
		}
	}
}
//virtual the Network module in a project.
void NetworkModuleTrace()
{
	while(1)
	{
		LOG_DEBUG(g_logger[L_NET], "network trace some msg ...");
		LOG_DEBUG(g_logger[L_NET], "network trace some msg ...");
		int rd = rand()%100;
		if (rd <2)
		{
			LOG_FATAL(g_logger[L_NET], "network some time put the fatal msg ...");
		}
		if (rd <5)
		{
			LOG_ERROR(g_logger[L_NET], "network some time put the error msg ...");
		}
		if (rd <= 30)
		{
			SleepMillisecond(rd%10 == 0);
		}
	}
}
//virtual the Moniter module in a project.
void MoniterModuleTrace()
{
	while(1)
	{
		LOG_DEBUG(g_logger[L_MONITER], "network trace some msg ...");
		LOG_DEBUG(g_logger[L_MONITER], "network trace some msg ...");
		int rd = rand()%100;
		if (rd <2)
		{
			LOG_WARN(g_logger[L_MONITER], "network some time put the warning msg ...");
		}
		if (rd <5)
		{
			LOG_ALARM(g_logger[L_MONITER], "network some time put the alarm  msg ...");
		}
		if (rd <= 30)
		{
			SleepMillisecond(rd%10 == 0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	//add and configure logger
	ILog4zManager::GetInstance()->ConfigMainLogger("", "L_MAIN");
	g_logger[L_MAIN] = ILog4zManager::GetInstance()->GetMainLogger();
	g_logger[L_MYSQL] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_MYSQL");
	g_logger[L_NET] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_NET");
	g_logger[L_MONITER] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_MONITER");
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_MYSQL], false);
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_NET], false);
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_MONITER], false);
	//start log4z
	ILog4zManager::GetInstance()->Start();

	//create thread, it create the virtual module .
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

