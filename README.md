Welcome to the log4z wiki!  
# Introduction:    

Log4z is an open source C++ lightweight & cross platform log library. It provides in a C++ application log and trace debug function for 7*24h service program. Can work in 64/32 of debian, redhat, centos, suse, windows and other operating systems.   

log4z是一款开源的轻量级的跨平台日志库 主要针对7*24小时服务器程序的日志输出与跟踪调试, 支持64/32位的debian, redhat, centos, suse, windows等多操作系统.  
  


# feature:
    
1. MIT open source license,very liberal.  
   MIT开源授权 授权的限制非常小. 
2. Lightweight and cross platform.  
   轻量级, 跨linux & windows平台.
3. Complete priority filter control.  
   完整的日志优先级过滤控制.
4. Multi-Logger output, rolling file,  thread safe.  
   多日志分流,滚动日志, 多线程安全.  
5. Display to screen with different color.  
   屏幕日志为彩色输出, 信息简约整洁.  
6. Support configure from file and hot update, or can fast work without any config.
   可通过配置文件配置并进行配置热更新,也可以不做任何配置直接快速使用.
7. Support format-style and stream-style write log.  
   支持格式化形式和流形式输入日志.  

# The  log file  
日志文件的输出可以通过配置文件或者在程序启动时进行灵活的配置[The output log file through a configuration file or when the program starts a flexible configuration]  
### file name format / 文件命名格式:  
LogName_YearMonthDayHourMinute_ProcessID_BlockNumber.log    
日志名_年月日时分_进程ID_文件滚动分块编号.log  
  
***   
E:\GITHUB\LOG4Z\PROJECT\BIN  
│──advanced_test.exe  
│──config.cfg  
│──fast_test.exe  
│──stress_test.exe  
│  
├─AdvacedLog  
│──│───FileConfig_2013100921_003852_000.log  
│──│───FileConfig_2013101313_005920_000.log  
│  
├─log  
│──│──advanced_test_2013100921_003852_000.log  
│──│──advanced_test_2013101313_005920_000.log    
│──│──fast_test_2013100921_003855_000.log  
│──│──fast_test_2013101313_006160_000.log  
│──│──stress_test_2013101313_007196_000.log  
│──│  
│──└─2013_10  
│─────│────Dynamic_2013100921_003852_000.log  
│─────│────Dynamic_2013101313_005920_000.log  
│  
└─Stress  
───│────NetWork_2013101313_007196_000.log  
───│────NetWork_2013101313_007196_001.log  
───│────NetWork_2013101313_007196_002.log  
───│────NetWork_2013101313_007196_003.log  
───│────NetWork_2013101313_007196_004.log  
***  
### 日志内容[log content]  
![fast_output_linux.png](https://raw.github.com/zsummer/wiki-pic/master/log4z/fast_output_linux.png)  
![fast_output_windows.png](https://raw.github.com/zsummer/wiki-pic/master/log4z/fast_output_windows.png)  

# How to use log4z  
![hellow_world.png](https://raw.github.com/zsummer/wiki-pic/master/log4z/hellow_world.png)  

# configure file view  
![config.png](https://raw.github.com/zsummer/wiki-pic/master/log4z/config.png)  
