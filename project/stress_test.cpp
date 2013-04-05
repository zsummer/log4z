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

#define  LOG_CONTENT "char:" <<'c'\
<< ", unsigned char:" << (unsigned char) 'c'\
<< ", short:" << (short) -1\
<< ", unsigned short:" << (unsigned short) -1\
<< ", int:" << (int) -1\
<< ", unsigned int:" << (unsigned int) -1\
<< ", long:" << (long) -1\
<< ", unsigned long:" << (unsigned long) -1\
<< ", long long:" << (long long) -1\
<< ", unsigned long long:" << (unsigned long long) -1\
<< ", float:" << (float) -1.234567\
<< ", double:" << (double) -2.34566\
<< ", std::string:" << std::string("fffff")\
<< ", void *:" << ( int *) 32423324\
<< ", const void*:" << (const int *) 32423324\
<< ", constant:" << 1000 \
<< ", constant:" << 100.12345678\
<< ", bool:" << (bool) true;

const int SWITCH_NUM = 100;

void MultiThreadFunc()
{
	unsigned long long count = 0;
	while(1)
	{
		count++;
		LOG_DEBUG(g_logger[L_MYSQL], LOG_CONTENT);
		LOG_DEBUG(g_logger[L_NET], LOG_CONTENT);
		LOG_DEBUG(g_logger[L_MONITER], LOG_CONTENT);
		if (count%SWITCH_NUM == 0)
		{
			SleepMillisecond(10);
		}
	}
}


int main(int argc, char *argv[])
{

	//add and configure logger
	ILog4zManager::GetInstance()->ConfigMainLogger("", "L_MAIN");
	g_logger[L_MAIN] = ILog4zManager::GetInstance()->GetMainLogger();
	g_logger[L_MYSQL] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_MYSQL" );
	g_logger[L_NET] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_NET" );
	g_logger[L_MONITER] = ILog4zManager::GetInstance()->DynamicCreateLogger("", "L_MONITER" );

	//not display
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_MYSQL], false);
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_NET], false);
	ILog4zManager::GetInstance()->ChangeLoggerDisplay(g_logger[L_MONITER], false);

	//start
	ILog4zManager::GetInstance()->Start();

	//create thread
	CreateThread(&MultiThreadFunc);
	CreateThread(&MultiThreadFunc);
	CreateThread(&MultiThreadFunc);

	unsigned long long lastCount = 0;
	unsigned long long lastData = 0;
	while(1)
	{ 
		//stats
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

