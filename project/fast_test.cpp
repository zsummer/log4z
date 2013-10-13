
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
	LOGD(" *** " << "hellow world" <<" *** ");
	LOGI("loginfo");
	LOGW("log warning");
	LOGE("log err");
	LOGA("log alarm");
	LOGF("log fatal");
#ifdef WIN32
	wchar_t *wstr = L"UNICODE¿í×Ö½Ú×Ö·û´®";
	LOGF(wstr);
#endif
	LOGD("char:" <<'c'
		<< ", unsigned char:" << (unsigned char) 'c'
		<< ", short:" << (short) -1
		<< ", unsigned short:" << (unsigned short) -1
		<< ", int:" << (int) -1
		<< ", unsigned int:" << (unsigned int) -1
		<< ", long:" << (long) -1
		<< ", unsigned long:" << (unsigned long) -1
		<< ", long long:" << (long long) -1
		<< ", unsigned long long:" << (unsigned long long) -1
		<< ", float:" << (float) -1.234567
		<< ", double:" << (double) -2.34566
		<< ", std::string:" << std::string("fffff")
		<< ", void *:" << ( int *) argv
		<< ", const void*:" << (const int *) argv
		<< ", constant:" << 1000 
		<< ", constant:" << 100.12345678
		<< ", bool:" << (bool) true
		<< ", show hex data:" << BinaryBlock("1234567890abcdefghigklmnopqrstuvwxyz_zyw_zsummer_log4z", 50));

	LOGA("main quit ...");
	return 0;
}

