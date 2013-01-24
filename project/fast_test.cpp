
#include "../log4z.h"
#include <iostream>
#include <stdio.h>
using namespace zsummer::log4z;

int main(int argc, char *argv[])
{

	//start log4z
	ILog4zManager::GetInstance()->Start();

	//LOGD: LOG WITH level LOG_DEBUG
	//LOGI: LOG WITH level LOG_INFO
	//...
	LOGD(" *** " << "hellow wolrd" <<" *** ");
	LOGI("loginfo");
	LOGW("log warning");
	LOGE("log err");
	LOGA("log alarm");
	LOGF("log fatal");

	LOGA("press anykey to exit ...");
	getchar();
	return 0;
}

