
#include "../log4z.h"
#include <iostream>
#include <stdio.h>
#include <signal.h>
#if WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#endif
using namespace zsummer::log4z;




LoggerId g_idFromConfig;
LoggerId g_idDynamic;

bool g_quit;
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

	
	g_idDynamic = ILog4zManager::GetInstance()->CreateLogger("Dynamic");

	ILog4zManager::GetInstance()->Config("config.cfg");
	g_idFromConfig = ILog4zManager::GetInstance()->FindLogger("FileConfig");
	//start log4z
	ILog4zManager::GetInstance()->Start();

	//设置屏幕输出
	ILog4zManager::GetInstance()->SetLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, true);
	ILog4zManager::GetInstance()->SetLoggerDisplay(g_idDynamic, true);
	//设置日志输出级别
	ILog4zManager::GetInstance()->SetLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_DEBUG);
	ILog4zManager::GetInstance()->SetLoggerLevel(g_idDynamic, LOG_LEVEL_DEBUG);
	//设置使用月目录
	ILog4zManager::GetInstance()->SetLoggerMonthdir(g_idDynamic, true);

	//virtual the main logic in project.
	while(g_quit)
	{
		LOG_DEBUG(g_idFromConfig, "FileConfig DEBUG");
		LOG_DEBUG(g_idDynamic, "idDynamic DEBUG");
		LOG_ALARM(g_idFromConfig, "FileConfig ALARM");
		LOG_INFO(g_idDynamic, "idDynamic INFO");
		LOGI("main thread trace msg ...");
#ifdef WIN32
		::Sleep(rand()%3000);
#else
		usleep((rand()%3000)*1000);
#endif

	}

	LOGA("main quit ..");
	return 0;
}



