Welcome to the log4z wiki!  
# Introduction:    

Log4z is an open source C++ lightweight & cross platform log library. It provides in a C++ application log and trace debug function for 7*24h service program. Can work in 64/32 of debian, redhat, centos, suse, windows and other operating systems.   

log4z是一款开源的轻量级的跨平台日志库 主要针对7*24小时服务器程序的日志输出与跟踪调试, 支持64/32位的debian, redhat, centos, suse, windows等多操作系统.  
  
### log4z提供多日志分流机制, 用户可通过配置文件或者在程序中动态创建的方式生成多个日志器进行不同的日志输出工作.  [log4z provide multiple log triage mechanism, the user can configure the file or created dynamically in the program means for generating a plurality of different logger log output work]  
> 例如 在一个完整的后端服务中, 可以创建一个日志器进行打印DB操作产生的警告与错误日志, 然后再创建一个日志器进行打印网络底层可能产生的网络错误信息, 而逻辑部分则使用log4z默认的主日志器进行打印输出, 并且他们都是线程安全的.[For example, in a complete back-end service, you can create a logger for printing DB operation generated warnings and error logs, and then create a logger for printing network infrastructure network may produce an error message, and the logical part of the default using log4z master logger for print output, and they are thread-safe.]  

log4z的所有日志器都可以通过配置文件进行初始化设定或者通过SetXXX接口进行动态更改其属性, 这些属性包括日志器的文件名, 日志过滤等级, 屏幕输出控制等, 每个日志器对应一个日志输出文件, 每个文件每天自动更换一次 每月(可禁用)更换一次目录.[log4z all logger can be set via the configuration file to initialize or through SetXXX interface to dynamically change its properties, which include the log's file name, log filter level, the screen output control, etc., each corresponding to a logger log output files, each file automatically replaced once a day, once a month(option) replacement directory.]  

log4z另外一个优秀的地方则是根据日志的等级不同予以不同的颜色展示, 日志级别越高 则颜色越醒目, 因此可以在调试程序时候非常舒适的找到警告或者错误日志.[Log4z is another good place is according to different log level to different colors, higher log level more eye-catching color, so it can be in the debugger when very comfortable to find a warning or error log.]  

### 这些特性整理成条目大致如下[These feature into entry is as follows]:
    
1. MIT open source license,very liberal.  
   MIT开源授权 无限制使用. 
2. lightweight and cross platform.  
   轻量级, 跨平台 可在linux与windows上共同使用. 
3. complete priority level control.  
   完整的日志优先级控制.
4. multi log output,  thread safe.  
   多日志分流 线程安全.  
5. complete file's configure.  
   可通过配置文件全程配置.  
6. display to screen with different color.  
   屏幕日志为彩色输出, 信息简约整洁.  
 
# The  log output file  
日志文件的输出可以通过配置文件或者在程序启动时进行灵活的配置[The output log file through a configuration file or when the program starts a flexible configuration]  
***  
### 示例配置文件如下[For example]:  
\#log4z   
\#section name is log name  
\#path: default is ./log/  
\#level: must in "ALL, DEBUG, INFO, WARN, ERROR, ALARM, FATAL". that is log output level.  
\#display: default is true, that is display to screen.  
\#monthdir default is false, that is no month directory.  
    
\#[Main]  
\#path=./MainLog/  
\#level = ALL  
\#display = true  
\#monthdir = false  
  
[FileConfig]  
path = ./AdvacedLog  
[MySql]  
path = ./Stress  
[NetWork]  
path = ./Stress  
[Monitor]  
path = ./Stress  
***    
### 产生的路径如下[Following the path]  
* 配置文件中每个段(大括号括起来的字符串)对应一个实际的日志文件[Each section of the configuration file (string bracketed) corresponds to an actual log file]  
* 每个日志文件都可以单独配置其路径, 日志级别, 屏幕输出控制等[Each log file can be individually configured its path, log level, the screen output control]  
* (option)每个路径下会创建一个月目录, 每个月更换一次,  格式为 xxxx_xx (年_月) [Each path will create a directory change once a month, every month, the format of xxxx_xx (years _ month)]   
* 每个日志文件每天更换一次, 每次启动程序更换一次, 格式为 name_年月日_时分_进程ID.log [Each log file once a day, each time you start the program change time, the format for name_ years month day _ hour min _ processID.log]  
***  
bin  
├── AdvacedLog  
│   ├── FileConfig_20130902_0650_019005.log  
│   └── FileConfig_20130902_0650_019013.log  
├── log  
│   ├── advanced_test_20130902_0650_019005.log  
│   ├── advanced_test_20130902_0650_019013.log  
│   ├── Dynamic_20130902_0650_019005.log  
│   ├── Dynamic_20130902_0650_019013.log  
│   ├── fast_test_20130902_0650_019003.log  
│   └── stress_test_20130902_0650_019007.log  
└── Stress  
│   ├── Monitor_20130902_0650_019007.log  
│   ├── MySql_20130902_0650_019007.log  
│   └── NetWork_20130902_0650_019007.log  
***   
  
### fast_test示例程序产生的日志内容[The fast_test sample program generated log content]  
> 2013-04-27 22:54:26 LOG_ALARM -----------------  log4z thread started!   ----------------------------  
> 2013-04-27 22:54:26 LOG_ALARM  logger id=0 path=./log/ name=fast_test  
> 2013-04-27 22:54:26 LOG_DEBUG  *** hellow wolrd ***  ( fast_test.cpp ) : 16  
> 2013-04-27 22:54:26 LOG_INFO loginfo ( fast_test.cpp ) : 17  
> 2013-04-27 22:54:26 LOG_WARN log warning ( fast_test.cpp ) : 18  
> 2013-04-27 22:54:26 LOG_ERROR log err ( fast_test.cpp ) : 19  
> 2013-04-27 22:54:26 LOG_ALARM log alarm ( fast_test.cpp ) : 20  
> 2013-04-27 22:54:26 LOG_FATAL log fatal ( fast_test.cpp ) : 21  
  
对应的linux下截图[The corresponding Linux screenshot]  
![fast_output.png](https://raw.github.com/zsummer/wiki-pic/master/log4z/fast_output.png)  
  
# How to use log4z  
### 最简使用示例[The simplest example]  
1. 启动log4z日志系统[Start the log4z log system]  
`ILog4zManager::GetInstance()->Start();`  
2. 现在开始已经可以在任意线程内很优雅的使用[Now already can be used in arbitrary thread is elegant]  
以下这些接口将会把日志内容以不同级别(LOGD DEBUG, LOGI INFO ...)录入到主日志文件中:  
`LOGD(" *** " << "hellow wolrd" <<" *** ");`  
`LOGI("loginfo");`  
`LOGW("log warning");`  
`LOGE("log err");`  
`LOGA("log alarm");`  
`LOGF("log fatal");`  
  
3. 结束操作:  不需要做任何销毁操作, 在程序正常退出的情况下(例如 非崩溃 非系统异常断电关机) log4z会安全的自我销毁[The end of operation: don't need to do any destruction in operation, from the normal procedures circumstances (such as non collapse not abnormal power off) log4z safety self destruction.].   
  
### 通过文件配置或者动态创建多个日志记录器[Through configuration files or create multiple dynamic logger]  
* 动态创建日志器的方式获取ID[Gets the ID dynamic create log device]  
`LoggerId idDynamic = ILog4zManager::GetInstance()->CreateLogger("Dynamic");`  
* 通过配置文件获取日志器ID[Gets the log ID through the configuration file]  
`ILog4zManager::GetInstance()->Config("config.cfg");`  
`LoggerId idFromConfig = ILog4zManager::GetInstance()->FindLogger("FileConfig");`  
* 如果动态创建的日志器和配置文件中的日志器重复, 那么靠后的操作将覆盖前面的配置, ID不变[If the log device log is created dynamically and in the configuration file to repeat, so on the operation will cover in front of the configuration, ID invariant].  
* 启动log4z[Start log4z]  
`ILog4zManager::GetInstance()->Start();`  
* 把日志记录到指定ID[The log record to the specified ID]  
`LOG_DEBUG(idFromConfig, "FileConfig DEBUG");`  
`LOG_INFO(idDynamic, "idDynamic INFO");`  
* 结束操作: 自动安全销毁 不需要额外的结束操作[The end of operation: automatic safety destruction does not need additional end].  
    
# auther: 张亚伟 YaweiZhang  
Web Site: www.zsummer.net  
mail: yawei_zhang@foxmail.com  
github: https://github.com/zsummer  
