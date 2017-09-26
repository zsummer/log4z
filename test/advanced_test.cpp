
#include "../log4z.h"
#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#if WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#endif
using namespace zsummer::log4z;




LoggerId logid_fromfile;
LoggerId logid_dynamic;

bool g_quit;
void signalFunc(int id)
{
    g_quit = false;
#ifdef WIN32
    signal(id, &signalFunc);
#endif
}
int main(int argc, char *argv[])
{
    g_quit = true;
    signal(SIGINT, &signalFunc);

    ILog4zManager::getRef().config("config.cfg");
    logid_dynamic = ILog4zManager::getRef().createLogger("dynamic");
    logid_fromfile = ILog4zManager::getRef().findLogger("fromfile");
    //start log4z
    ILog4zManager::getRef().start();

    //configure the output behaviour
    ILog4zManager::getRef().setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID, true);
    ILog4zManager::getRef().setLoggerDisplay(logid_dynamic, true);
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_DEBUG);
    ILog4zManager::getRef().setLoggerLevel(logid_dynamic, LOG_LEVEL_DEBUG);
    ILog4zManager::getRef().setLoggerMonthdir(logid_dynamic, true);
    ILog4zManager::getRef().setLoggerFileLine(LOG4Z_MAIN_LOGGER_ID, false);
    ILog4zManager::getRef().setLoggerName(LOG4Z_MAIN_LOGGER_ID, "main");
    ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./");
    ILog4zManager::getRef().setLoggerLimitsize(logid_dynamic, 1);
    ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID, true);


    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_TRACE);
    LOGT("set LOG_LEVEL_TRACE ----------------------------");
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_DEBUG);
    LOGT("set LOG_LEVEL_TRACE ============================");

    char buf[100];
    for (size_t j = 0; j < 100000; j++)
    {
        Log4zStream ls(buf, 100);

        for (size_t i = 0; i < 100; i++)
        {
            int r = rand() % 14;
            switch (r)
            {
            case 0:
                ls << 'c';
                break;
            case 1:
                ls << UINT8_MAX;
                break;
            case 2:
                ls << INT16_MIN;
                break;
            case 3:
                ls << INT16_MAX;
                break;
            case 4:
                ls << UINT16_MAX;
                break;
            case 5:
                ls << INT32_MIN;
                break;
            case 6:
                ls << INT32_MAX;
                break;
            case 7:
                ls << UINT32_MAX;
                break;
            case 8:
                ls << INT64_MIN;
                break;
            case 9:
                ls << INT64_MAX;
                break;
            case 10:
                ls << UINT64_MAX;
                break;
            case 11:
                ls << (float)pow(rand()%100, rand()%20) *((rand()%2 == 0? -1.0 : 1.0));
                break;
            case 12:
                ls << (double)pow(rand() % 100, rand() % 200)*((rand() % 2 == 0 ? -1.0 : 1.0));
                break;
            default:
                ls << "8";
                break;
            }
            if (ls.getCurrentLen() > 100)
            {
                LOGE("overwrite ls.getCurrentLen()=" << ls.getCurrentLen());
                return 1;
            }
        }
       
    }

    //hot update configure
    ILog4zManager::getRef().setAutoUpdate(10);
    time_t now = time(NULL);
    while(g_quit &&  time(NULL) - now < 10)
    {
        LOG_TRACE(logid_fromfile, "fromfile LOG_TRACE");
        LOG_DEBUG(logid_fromfile, "fromfile LOG_DEBUG");
        LOG_INFO(logid_fromfile, "fromfile LOG_INFO");
        LOG_WARN(logid_fromfile, "fromfile LOG_WARN");
        LOG_ERROR(logid_fromfile, "fromfile LOG_ERROR");
        LOG_ALARM(logid_fromfile, "fromfile LOG_ALARM");
        LOG_FATAL(logid_fromfile, "fromfile LOG_FATAL");

        LOG_TRACE(logid_dynamic, "dynamic LOG_TRACE");
        LOG_DEBUG(logid_dynamic, "dynamic LOG_DEBUG");
        LOG_INFO(logid_dynamic, "dynamic LOG_INFO");
        LOG_WARN(logid_dynamic, "dynamic LOG_WARN");
        LOG_ERROR(logid_dynamic, "dynamic LOG_ERROR");
        LOG_ALARM(logid_dynamic, "dynamic LOG_ALARM");
        LOG_FATAL(logid_dynamic, "dynamic LOG_FATAL");

        LOGT("main LOGT");
        LOGD("main LOGD");
        LOGI("main LOGI");
        LOGW("main LOGW");
        LOGE("main LOGE");
        LOGA("main LOGA");
        LOGF("main LOGF");
        LOGF(" = = = = = = = = = = = = = = =  = = = = = ");
#ifdef WIN32
        ::Sleep(rand()%3000);
#else
        usleep((rand()%3000)*1000);
#endif
    }


    LOGA("main quit .. hit 'enter' to exit.");
    getchar();
    return 0;
}



