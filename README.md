Welcome to the log4z wiki!  
# Introduction:    

Log4z is an open source C++ lightweight & cross platform log library. It provides in a C++ application log and trace debug function for 7*24h service program.   

log4z是一款开源的轻量级的跨平台日志库 主要针对7*24小时服务器程序的日志输出与跟踪调试, 支持64/32位的debian, redhat, centos, suse, windows等多操作系统.  
***
### log4z提供多日志分流机制, 用户可通过配置文件或者在程序中动态创建的方式生成多个日志器进行不同的日志输出工作. 
> 例如 在一个完整的后端服务中, 可以创建一个日志器进行打印DB操作产生的警告与错误日志, 然后再创建一个日志器进行打印网络底层可能产生的网络错误信息, 而逻辑部分则使用log4z默认的主日志器进行打印输出, 并且他们都是线程安全的.  

log4z的所有日志器都可以通过配置文件进行初始化设定或者通过SetXXX接口进行动态更改其属性, 这些属性包括日志器的文件名, 日志过滤等级, 屏幕输出控制等, 每个日志器对应一个日志输出文件, 每个文件每天自动更换一次 每月更换一次目录.  

log4z另外一个优秀的地方则是根据日志的等级不同予以不同的颜色展示, 日志级别越高 则颜色越醒目, 因此可以在调试程序时候非常舒适的找到警告或者错误日志.  

### 这些特性整理成条目大致如下:
    
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
日志文件的输出可以通过配置文件或者在程序启动时进行灵活的配置  
***  
### 示例配置文件如下:  
config.cfg   
> \#log4z  
>   
> \#section name is log name  
> \#path defalt is ./log/  
> \#level value must in "ALL, DEBUG, INFO, WARN, ERROR, ALARM, FATAL".  
> \#display defalt is true  
> [Main]  
> path=./MainLog/  
> level = ALL  
> display = true  
> 
> [Dynamic]  
> path = ./AdvacedLog  
> [FileConfig]  
> path = ./AdvacedLog  
> \#level=DEBUG  
> \#display=true  
>   
> [MySql]  
> path = ./Stress  
> [NetWork]  
> path = ./Stress  
> [Monitor]  
> path = ./Stress  
> \#  

  
***  
### 实际产生的路径如下  
* 配置文件中每个段(大括号括起来的字符串)对应一个实际的日志文件  
* 每个日志文件都可以单独配置其路径, 日志级别, 屏幕输出控制等  
* 每个路径下会创建一个月目录, 每个月更换一次,  格式为 xxxx_xx (年_月)   
* 每个日志文件每天更换一次, 每次启动程序更换一次, 格式为 name_年_月_日_进程ID.log  
***
bin  
├── AdvacedLog  
│   └── 2013_04  
│       ├── Dynamic_2013_04_25_002762.log  
│       ├── Dynamic_2013_04_27_002836.log  
│       ├── FileConfig_2013_04_25_002762.log  
│       └── FileConfig_2013_04_27_002836.log  
├── MainLog  
│   └── 2013_04  
│       ├── advanced_test_2013_04_25_002762.log  
│       ├── advanced_test_2013_04_27_002836.log  
│       └── stress_test_2013_04_25_002770.log  
├── Stress  
│   └── 2013_04  
│       ├── Monitor_2013_04_25_002770.log  
│       ├── MySql_2013_04_25_002770.log  
│       └── NetWork_2013_04_25_002770.log  

***   
### fast_test示例程序产生的日志内容
> 2013-04-27 22:54:26 LOG_ALARM -----------------  log4z thread started!   ----------------------------  
> 2013-04-27 22:54:26 LOG_ALARM  logger id=0 path=./log/ name=fast_test  
> 2013-04-27 22:54:26 LOG_DEBUG  *** hellow wolrd ***  ( fast_test.cpp ) : 16  
> 2013-04-27 22:54:26 LOG_INFO loginfo ( fast_test.cpp ) : 17  
> 2013-04-27 22:54:26 LOG_WARN log warning ( fast_test.cpp ) : 18  
> 2013-04-27 22:54:26 LOG_ERROR log err ( fast_test.cpp ) : 19  
> 2013-04-27 22:54:26 LOG_ALARM log alarm ( fast_test.cpp ) : 20  
> 2013-04-27 22:54:26 LOG_FATAL log fatal ( fast_test.cpp ) : 21  
  
对应的linux下截图  
![fast_output.png](https://raw.github.com/zsummer/wiki-pic/master/log4z/fast_output.png)  
  
# How to use log4z  
### 最简使用示例  
1. 首先  启动log4z日志系统  
`ILog4zManager::GetInstance()->Start();`  
2. 现在开始已经可以在任意线程内很优雅的使用  
以下这些接口将会把日志内容以不同级别(LOGD DEBUG, LOGI INFO ...)录入到主日志文件中:  
`LOGD(" *** " << "hellow wolrd" <<" *** ");`  
`LOGI("loginfo");`  
`LOGW("log warning");`  
`LOGE("log err");`  
`LOGA("log alarm");`  
`LOGF("log fatal");`  
  
3. 结束操作:  不需要做任何销毁操作, 在程序正常退出的情况下(例如 非崩溃 非系统异常断电关机) log4z会安全的自我销毁.   
  
###  通过文件配置或者动态创建多个日志记录器  
* 动态创建日志器的方式获取ID  
`LoggerId idDynamic = ILog4zManager::GetInstance()->CreateLogger("Dynamic");`  
* 通过配置文件获取日志器ID  
`ILog4zManager::GetInstance()->Config("config.cfg");`  
`LoggerId idFromConfig = ILog4zManager::GetInstance()->FindLogger("FileConfig");`  
* 如果动态创建的日志器和配置文件中的日志器重复, 那么靠后的操作将覆盖前面的配置, ID不变.  
* 启动log4z  
`ILog4zManager::GetInstance()->Start();`  
* 把日志记录到指定ID  
`LOG_DEBUG(idFromConfig, "FileConfig DEBUG");`  
`LOG_INFO(idDynamic, "idDynamic INFO");`  
* 结束操作: 自动安全销毁 不需要额外的结束操作.  
  
  
  
***  
  
auther: 张亚伟  
=======  
QQ Group: 19811947  
Web Site: www.zsummer.net  
mail: yawei_zhang@foxmail.com  
github: https://github.com/zsummer  








