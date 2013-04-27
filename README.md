Welcome to the log4z wiki!  
# Introduction:    

Log4z is an open source C++ lightweight & cross platform log library. It provides in a C++ application log and trace debug function for 7*24h service program.   

log4z是一款开源的轻量级的跨平台日志库 主要针对7*24小时服务器程序的日志输出与跟踪调试, 支持64/32位的debian, redhat, centos, suse, windows等多操作系统.  
***
log4z提供多日志分流机制, 用户可通过配置文件或者在程序中动态创建的方式生成多个日志器进行不同的日志输出工作. 
> 例如 在一个完整的后端服务中, 可以创建一个日志器进行打印DB操作产生的警告与错误日志, 然后再创建一个日志器进行打印网络底层可能产生的网络错误信息, 而逻辑部分则使用log4z默认的主日志器进行打印输出, 并且他们都是线程安全的.  

log4z的所有日志器都可以通过配置文件进行初始化设定或者通过SetXXX接口进行动态更改其属性, 这些属性包括日志器的文件名, 日志过滤等级, 屏幕输出控制等, 每个日志器对应一个日志输出文件, 每个文件每天自动更换一次 每月更换一次目录.  

log4z另外一个优秀的地方则是根据日志的等级不同予以不同的颜色展示, 日志级别越高 则颜色越醒目, 因此可以在调试程序时候非常舒适的找到警告或者错误日志.  

这些特性整理成条目大致如下:
    
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
 

   
# How to use log4z  



auther: 张亚伟 
=======  
QQ Group: 19811947  
Web Site: www.zsummer.net  
mail: yawei_zhang@foxmail.com  
github: https://github.com/zsummer  








