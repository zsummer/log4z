
#include "../log4z.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
using namespace zsummer::log4z;

int main(int argc, char *argv[])
{

	//start log4z
	ILog4zManager::GetInstance()->Start();

	//LOGD: LOG WITH level LOG_DEBUG
	//LOGI: LOG WITH level LOG_INFO
	//...
	LOGI("begin test stream log input....");
	LOGD("stream input *** " << "hellow world" << " *** " << 123456 << " *** ");
	LOGI("stream input *** " << "hellow world" << " *** " << 123456 << " *** ");
	LOGW("stream input *** " << "hellow world" << " *** " << 123456 << " *** ");
	LOGE("stream input *** " << "hellow world" << " *** " << 123456 << " *** ");
	LOGA("stream input *** " << "hellow world" << " *** " << 123456 << " *** ");
	LOGF("stream input *** " << "hellow world" << " *** " << 123456 << " *** ");

	LOGFMTI("begin test format log input....");
	LOGFMTD("format input *** %s *** %d ***", "hellow world", 123456);
	LOGFMTI("format input *** %s *** %d ***", "hellow world", 123456);
	LOGFMTW("format input *** %s *** %d ***", "hellow world", 123456);
	LOGFMTE("format input *** %s *** %d ***", "hellow world", 123456);
	LOGFMTA("format input *** %s *** %d ***", "hellow world", 123456);
	LOGFMTF("format input *** %s *** %d ***", "hellow world", 123456);

	LOGI("begin test stream log utf-16 string input....");
#ifdef WIN32
	wchar_t *wstr = L"UNICODE¿í×Ö½Ú×Ö·û´®";
	LOGF(wstr);
#endif
	LOGI("begin test stream log all types input....");
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

	LOGI("begin test format log big string more than buff size input....");
	std::string str;
	str.resize(3000, 's');
	LOGFMT_DEBUG(LOG4Z_MAIN_LOGGER_ID, "%s", str.c_str());
	LOGI("begin test stream log big string more than buff size input....");
	LOGD(str);
	LOGA("main quit ...");

	return 0;
}

