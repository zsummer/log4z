
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




LoggerId g_idFromConfig;
LoggerId g_idDynamic;


int main(int argc, char *argv[])
{

	ILog4zManager::GetInstance()->ConfigFromFile("../config.cfg");

	g_idFromConfig = ILog4zManager::GetInstance()->GetLoggerFromName("FileConfig");
	g_idDynamic = ILog4zManager::GetInstance()->DynamicCreateLogger("Dynamic");
	
	
	ILog4zManager::GetInstance()->ChangeLoggerLevel(g_idFromConfig, LOG_LEVEL_INFO);

	//start log4z
	ILog4zManager::GetInstance()->Start();

	//virtual the main logic in project.
	while(1)
	{
		LOG_DEBUG(g_idFromConfig, "FileConfig DEBUG");
		LOG_DEBUG(g_idDynamic, "idDynamic DEBUG");
		LOG_ALARM(g_idFromConfig, "FileConfig ALARM");
		LOGI("main thread trace msg ...");
#ifdef WIN32
		::Sleep(rand()%3000);
#else
		usleep((rand()%3000)*1000);
#endif

	}

	printf("press anykey to exit.");
	getchar();
	return 0;
}



