#include "../log4z.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#if WIN32
#include <Windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#endif
using namespace zsummer::log4z;

//! cross platform function:
void SleepMillisecond(unsigned int ms);
bool CreateThread(void(*run)());


//! multi logger id
LoggerId g_lgMySql;
LoggerId g_lgNet;
LoggerId g_lgMoniter;

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

//! stress coefficient
const int SWITCH_NUM = 100;

//! process quit.
bool g_quit;


void MultiThreadFunc()
{
	unsigned long long count = 0;
	while(g_quit)
	{
		count++;
		LOG_DEBUG(g_lgMySql, LOG_CONTENT);
		LOG_DEBUG(g_lgNet, LOG_CONTENT);
		LOG_DEBUG(g_lgMoniter, LOG_CONTENT);
		if (count%SWITCH_NUM == 0)
		{
			SleepMillisecond(10);
		}
	}
	LOGA("thread quit ... ");
}


void SignalFunc(int id)
{
	g_quit = false;
#ifdef WIN32
	signal(id, &SignalFunc);
#endif
}

int main(int argc, char *argv[])
{
	g_quit = true;
	signal(SIGINT, &SignalFunc);

	//! ---------
	g_lgMySql = ILog4zManager::GetInstance()->CreateLogger("MySql" );
	g_lgNet = ILog4zManager::GetInstance()->CreateLogger("NetWork" );
	g_lgMoniter = ILog4zManager::GetInstance()->CreateLogger("Monitor" );
	ILog4zManager::GetInstance()->Config("config.cfg");
	//! ---------
	ILog4zManager::GetInstance()->SetLoggerDisplay(g_lgMySql, false);
	ILog4zManager::GetInstance()->SetLoggerDisplay(g_lgNet, false);
	ILog4zManager::GetInstance()->SetLoggerDisplay(g_lgMoniter, false);

	//! ---------
	ILog4zManager::GetInstance()->Start();

	//! ---------
	CreateThread(&MultiThreadFunc);
	CreateThread(&MultiThreadFunc);
	CreateThread(&MultiThreadFunc);

	//! ---------
	unsigned long long lastCount = 0;
	unsigned long long lastData = 0;
	
	while(g_quit)
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

	//! wait all thread quit ...
	//! skip wait code ...
	LOGA("main quit ... ");
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

