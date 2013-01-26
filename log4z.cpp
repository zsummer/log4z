
/*
 * Log4z License
 * -----------
 * 
 * Log4z is licensed under the terms of the MIT license reproduced below.
 * This means that Log4z is free software and can be used for both academic
 * and commercial purposes at absolutely no cost.
 * 
 * 
 * ===============================================================================
 * 
 * Copyright (C) 2012 YaweiZhang <yawei_zhang@foxmail.com>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ===============================================================================
 * 
 * (end of COPYRIGHT)
 */

#include "log4z.h"

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
//#include <locale>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <map>

#ifdef WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <io.h>
#include <shlwapi.h>
#include <process.h>
#pragma comment(lib, "shlwapi")
#pragma warning(disable:4996)
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#endif




_ZSUMMER_BEGIN
_ZSUMMER_LOG4Z_BEGIN





void SleepMillisecond(unsigned int ms)
{
#ifdef WIN32
	::Sleep(ms);
#else
	usleep(1000*ms);
#endif
}

unsigned int GetTimeMillisecond()
{
#ifdef WIN32
	return ::GetTickCount();
#else
	struct timeval tm;
	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000 + (tm.tv_usec/1000));
#endif
}


bool TimeToTm(time_t t, tm * tt)
{
#ifdef WIN32
	if (localtime_s(tt, &t) == 0)
	{
		return true;
	}
	return false;
#else
	if (localtime_r(&t, tt) != NULL)
	{
		return true;
	}
	return false;
#endif
}

time_t TmToTime(tm * tt)
{
	return mktime(tt);
}

std::string TimeToString(time_t t)
{
	char m[128] = {0};
	tm tt;
	if (TimeToTm(t, &tt))
	{
		sprintf(m, "%d-%02d-%02d %02d:%02d:%02d", tt.tm_year+1900, tt.tm_mon+1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
	}
	else
	{
		sprintf(m, "0000-00-00 00:00:00");
	}
	return m;
}


void TrimString(std::string & str, int flag = 2)
{
	if (str.length() == 0)
	{
		return ;
	}
	if (flag == 0 || flag == 2)
	{
		std::string::size_type pos = str.find_first_not_of(' ');
		if (pos != std::string::npos)
		{
			str = str.substr(pos, std::string::npos);
		}
	}
	if (flag == 1 || flag == 2)
	{
		std::string::size_type pos = str.find_last_not_of(' ');
		if (pos != std::string::npos)
		{
			str = str.substr(0, pos+1);
		}
	}
}


void FixWinSep(std::string &path)
{
	if (path.length() == 0)
	{
		return;
	}
	for (std::string::iterator iter = path.begin(); iter != path.end(); ++iter)
	{
		if (*iter == '\\')
		{
			*iter = '/';
		}
	}
}

void FixPath(std::string &path)
{
	if (path.length() == 0)
	{
		return;
	}
	FixWinSep(path);
	if (path.at(path.length()-1) != '/')
	{
		path += "/";
	}
}
void TrimXmlContent(std::string &content)
{
	if (content.length() == 0)
	{
		return ;
	}
	std::string dest;
	std::string::size_type pos1 = 0;
	std::string::size_type pos2 = 0;
	do 
	{
		pos2 = content.find("<!--", pos1);
		if (pos2 == std::string::npos)
		{
			dest.append(content.substr(pos1, std::string::npos));
			break;
		}

		dest.append(content.substr(pos1, pos2 - pos1));
		pos1 = pos2;
		pos2 = content.find("-->",pos1+4);
		if (pos2 == std::string::npos)
		{
			break;
		}
		pos1 = pos2+3;
	} while (1);
	content = dest;
}

bool GetXmlParam(std::string content, std::string param, std::vector<std::string> & data)
{
	TrimXmlContent(content);
	data.clear();
	std::string preParam = "<";
	preParam += param;
	preParam += ">";
	std::string suffParam = "</";
	suffParam += param;
	suffParam += ">";

	std::string::size_type pos1 = 0;
	while(1)
	{
		pos1 = content.find(preParam, pos1);
		if (pos1 == std::string::npos)
		{
			break;
		}
		pos1 += preParam.length();
		std::string::size_type pos2 = content.find(suffParam, pos1);
		if (pos2 == std::string::npos)
		{
			break;
		}

		data.push_back(content.substr(pos1, pos2-pos1));
		TrimString(data.back());
	}
	return true;
}
bool GetXmlParam(std::string content, std::string param, std::string & data)
{
	data.clear();
	std::vector<std::string> vct;
	GetXmlParam(content, param, vct);
	if (vct.size() > 0)
	{
		data = vct.at(0);
		return true;
	}
	return false;
}
bool GetXmlParam(std::string content, std::string param, int & data)
{
	std::string str;
	if (!GetXmlParam(content, param, str))
	{
		return false;
	}
	if (str.length() == 0)
	{
		return false;
	}
	data = atoi(str.c_str());
	return true;
}
bool GetXmlParam(std::string content, std::string param, bool & data)
{
	std::string str;
	if (!GetXmlParam(content, param, str))
	{
		return false;
	}
	if (str.length() == 0)
	{
		return false;
	}
	int n = atoi(str.c_str());
	if (n == 0)
	{
		data = false;
	}
	else
	{
		data = true;
	}
	return true;
}


bool IsDirectory(std::string path)
{
#ifdef WIN32
	return PathIsDirectoryA(path.c_str()) ? true : false;
#else
	DIR * pdir = opendir(path.c_str());
	if (pdir == NULL)
	{
		return false;
	}
	else
	{
		closedir(pdir);
		pdir = NULL;
		return true;
	}
#endif
	return false;
}


bool CreateDir(std::string path)
{
#ifdef WIN32
	return CreateDirectoryA(path.c_str(), NULL) ? true : false;
#else
	return (mkdir(path.c_str(), S_IRWXU|S_IRWXG|S_IRWXO) == 0);
#endif
	return false;
} 


bool CreateRecursionDir(std::string path)
{
	if (path.length() == 0) return true;
	std::string sub;
	char lastchar=0;
	FixPath(path);

	std::string::size_type pos = path.find('/');
	while (pos != std::string::npos)
	{
		std::string cur = path.substr(0, pos-0);
		if (cur.length() > 0 && !IsDirectory(cur))
		{
			if (!CreateDir(cur))
			{
				return false;
			}
		}
		pos = path.find('/', pos+1);
	}

	return true;
}


std::string GetMainLoggerName()
{
	std::string name;
	name = "MainLog";
#ifdef WIN32
	
	char buf[260] = {0};
	if (GetModuleFileNameA(NULL, buf, 259) > 0)
	{
		name = buf;
	}
	std::string::size_type pos = name.rfind("\\");
	if (pos != std::string::npos)
	{
		name = name.substr(pos+1, std::string::npos);
	}
	pos = name.rfind(".");
	if (pos != std::string::npos)
	{
		name = name.substr(0, pos-0);
	}
#else
	pid_t id = getpid();
	char buf[260];
	sprintf(buf, "/proc/%d/cmdline", (int)id);
	std::fstream i;
	i.open(buf, std::ios::in);
	if (!i.is_open())
	{
		return name;
	}
	std::string line;
	std::getline(i, line);
	i.close();
	if (line.length() > 0)
	{
		name = line;
	}
	std::string::size_type pos = name.rfind("/");
	if (pos != std::string::npos)
	{
		name = name.substr(pos+1, std::string::npos);
	}
#endif
	return name;
}



#ifdef WIN32

const static WORD cs_sColor[LOG_LEVEL_FATAL+1] = {
	0,
	FOREGROUND_BLUE|FOREGROUND_GREEN,
	FOREGROUND_GREEN|FOREGROUND_RED,
	FOREGROUND_RED,
	FOREGROUND_GREEN,
	FOREGROUND_RED|FOREGROUND_BLUE};
#else

const static char cs_strColor[LOG_LEVEL_FATAL+1][50] = { 
	"\e[0m",
	"\e[34m\e[1m",//hight blue
	"\e[33m", //yellow
	"\e[31m", //red
	"\e[32m", //green
	"\e[35m"};
#endif

void ShowColorText(const char *text, int level = LOG_LEVEL_DEBUG)
{
	if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_FATAL) goto showfail;
	if (level == LOG_LEVEL_DEBUG) goto showfail;
#ifndef WIN32
	printf("%s%s\e[0m", cs_strColor[level], text);
#else
	HANDLE hStd = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStd == INVALID_HANDLE_VALUE) goto showfail;

	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	if (!GetConsoleScreenBufferInfo(hStd, &oldInfo)) goto showfail;

	if (SetConsoleTextAttribute(hStd, cs_sColor[level]))
	{
		printf(text);
		SetConsoleTextAttribute(hStd, oldInfo.wAttributes);
	}
	else
	{
		goto showfail;
	}
#endif

	return;

showfail:
	printf(text);
}





class CLock
{
public:
	CLock();
	virtual ~CLock();

public:
	void Lock();
	void UnLock();


private:
#ifdef WIN32
	CRITICAL_SECTION m_crit;
#else
	pthread_mutex_t  m_crit;
#endif
};


CLock::CLock()
{
#ifdef WIN32
	InitializeCriticalSection(&m_crit);
#else
	//m_crit = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_crit, &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}


CLock::~CLock()
{
#ifdef WIN32
	DeleteCriticalSection(&m_crit);
#else
	pthread_mutex_destroy(&m_crit);
#endif
}


void CLock::Lock()
{
#ifdef WIN32
	EnterCriticalSection(&m_crit);
#else
	pthread_mutex_lock(&m_crit);
#endif
}


void CLock::UnLock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_crit);
#else
	pthread_mutex_unlock(&m_crit);
#endif
}

class CAutoLock
{
public:
	explicit CAutoLock(CLock & lk):m_lock(lk)
	{
	}
	~CAutoLock()
	{
		m_lock.UnLock();
	}
	inline void Lock()
	{
		m_lock.Lock();
	}


private:
	CLock & m_lock;
};


class CSem
{
public:
	CSem()
	{
#ifdef WIN32
		m_hSem = NULL;
#else
		m_bCreate = false;
#endif

	}
	virtual ~CSem()
	{
#ifdef WIN32
		if (m_hSem != NULL)
		{
			CloseHandle(m_hSem);
			m_hSem = NULL;
		}
#else
		if (m_bCreate)
		{
			m_bCreate = false;
			sem_destroy(&m_semid);
		}
#endif
	}
public:
	//linux下忽略参数 maxcount
	bool Create(int initcount)
	{
		if (initcount < 0)
		{
			initcount = 0;
		}
#ifdef WIN32
		if (initcount > 64)
		{
			return false;
		}
		m_hSem = CreateSemaphore(NULL, initcount, 64, NULL);
		if (m_hSem == NULL)
		{
			return false;
		}
#else
		if (sem_init(&m_semid, 0, initcount) != 0)
		{
			return false;
		}
		m_bCreate = true;
#endif
		return true;

	}

	//timeout <= 0 为直到有信号才返回 否则超时也会返回
	bool Wait(int timeout = 0)
	{
#ifdef WIN32
		if (timeout <= 0)
		{
			timeout = INFINITE;
		}
		if (WaitForSingleObject(m_hSem, timeout) != WAIT_OBJECT_0)
		{
			return false;
		}
#else
		if (timeout <= 0)
		{
			return (sem_wait(&m_semid) == 0);
		}
		else
		{
			timespec ts;
			ts.tv_sec += time(NULL) + timeout/1000;
			ts.tv_nsec += (timeout%1000)*1000000;
			return (sem_timedwait(&m_semid, &ts) == 0);
		}
#endif
		return true;
	}
	//发送一个信号
	bool Post()
	{
#ifdef WIN32
		return ReleaseSemaphore(m_hSem, 1, NULL) ? true : false;
#else
		return (sem_post(&m_semid) == 0);
#endif
	}
private:
#ifdef WIN32
	HANDLE m_hSem;
#else
	sem_t m_semid;
	bool  m_bCreate;
#endif
};


class CThread
{
public:
	CThread();
	virtual ~CThread();

public:
	bool Start();
	virtual void Run() = 0;
	bool Wait();
	inline unsigned long long GetThreadID() {return m_hThreadID;};
private:
	unsigned long long m_hThreadID;
};


CThread::CThread()
{
	m_hThreadID = 0;
}
CThread::~CThread()
{

}
#ifdef WIN32

static unsigned int WINAPI  ThreadProc(LPVOID lpParam)
{
	CThread * p = (CThread *) lpParam;
	p->Run();
	_endthreadex(0);
	return 0;
}
#else

static void * ThreadProc(void * pParam)
{
	CThread * p = (CThread *) pParam;
	p->Run();
	return NULL;
}
#endif


bool CThread::Start()
{
#ifdef WIN32
	unsigned long long ret = _beginthreadex(NULL, 0, ThreadProc, (void *) this, 0, NULL);

	if (ret == -1 || ret == 1  || ret == 0)
	{
		return false;
	}
	m_hThreadID = ret;
#else
	pthread_t ptid = 0;
	int ret = pthread_create(&ptid, NULL, ThreadProc, (void*)this);
	if (ret != 0)
	{
		return false;
	}
	m_hThreadID = ptid;

#endif
	return true;
}



bool CThread::Wait()
{
#ifdef WIN32
	if (WaitForSingleObject((HANDLE)m_hThreadID, INFINITE) != WAIT_OBJECT_0)
	{
		return false;
	}
#else
	if (pthread_join((pthread_t)m_hThreadID, NULL) != 0)
	{
		return false;
	}
#endif
	return true;
}




static const char *const LOG_STRING[]=
{
	"LOG_DEBUG",
	"LOG_INFO",
	"LOG_WARN",
	"LOG_ERROR",
	"LOG_ALARM",
	"LOG_FATAL",
};
struct LogData
{
	LoggerId _id;		//dest logger id
	int	_level;	//log level
	time_t _time;		//create time
	char _content[LOG_BUF_SIZE]; //content
};

struct LoggerInfo 
{
	std::string _path;
	std::string _name;
	int  _level; //filter level
	bool _display; //display to screen 
	bool _enable;
	std::fstream	_handle; //file handle.
	LoggerInfo(){_display = false; _enable = false; _level = LOG_LEVEL_DEBUG;}
};


#ifndef LOG4Z_ASSERT
#define LOG4Z_ASSERT(x) {\
if(!(x))\
	assert(0);\
}
#endif




class CLogerManager : public CThread, public ILog4zManager
{
public:
	CLogerManager()
	{
		m_bRuning = false;
		m_uptimes = 0;
		for (unsigned int i=0; i<LOGGER_MAX; i++)
		{
			m_loggers[i]._level = LOG_LEVEL_DEBUG;
			m_loggers[i]._display = true;
			m_loggers[i]._enable = false;
		}
		m_lastId = -1;
		m_main = DynamicCreateLogger("", "", LOG_LEVEL_DEBUG, true);
		LOG4Z_ASSERT(m_main == 0);
	}
	~CLogerManager()
	{
		Stop();
	}


	std::string GetExampleConfig()
	{
		return ""
			"<!--at current version, configure can't support xml Comments.-->\n" 
			"<!--logger id must in the region [0,LOGGER_MAX) -->\n"
			"<logger>\n"
			"\t<path>./log/</path> <!--#path-->\n"
			"\t<name>test</name> <!--#name-->\n"
			"\t<level>0</level> <!--#DEBUG WARN ERROR ALARM FATAL-->\n"
			"\t<display>1</display> <!--#display to screent-->\n"
			"</logger>\n";
	}

	bool ConfigFromFile(std::string cfg)
	{
		std::string content;
		std::ifstream f;
		f.open(cfg.c_str(), std::ios_base::in);
		content.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

		std::vector<std::string> vctLogger;
		GetXmlParam(content, "logger", vctLogger);
		for (unsigned int i=0; i<vctLogger.size(); i++)
		{
			int lid = 0;
			LoggerInfo l;

			GetXmlParam(vctLogger[i], "path", l._path);
			GetXmlParam(vctLogger[i], "name", l._name);
			GetXmlParam(vctLogger[i], "level", l._level);
			GetXmlParam(vctLogger[i], "display", l._display);
			DynamicCreateLogger(l._path, l._name, l._level, l._display);
		}
		return true;
	}

	virtual LoggerId GetLoggerFromName(std::string name)
	{
		CAutoLock l(m_idLock);
		std::map<std::string, LoggerId>::iterator iter;
		iter = m_ids.find(name);
		if (iter != m_ids.end())
		{
			return iter->second;
		}
		return -1;
	}

	LoggerId GetMainLogger()
	{
		return m_main;
	}
	virtual bool	ConfigMainLogger(std::string path,std::string name,int nLevel,bool display)
	{
		TrimString(path);
		if (path.length() == 0)
		{
			path = "./log/";
		}
		else
		{
			FixPath(path);
		}
		if (name.length() == 0)
		{
			name = GetMainLoggerName();
		}
		CAutoLock l(m_idLock);
		m_loggers[m_main]._path = path;
		m_loggers[m_main]._name = name;
		m_loggers[m_main]._level = nLevel;
		m_loggers[m_main]._enable = true;
		m_loggers[m_main]._display = display;
		return true;

	}


	virtual LoggerId DynamicCreateLogger(	std::string path,
							std::string name,
							int nLevel,
							bool display)
	{
		TrimString(path);
		if (path.length() == 0)
		{
			path = "./log/";
		}
		else
		{
			FixPath(path);
		}
		if (name.length() == 0)
		{
			name = GetMainLoggerName();
		}
		
		CAutoLock l(m_idLock);
		m_lastId++;
		if (m_lastId >= LOGGER_MAX)
		{
			LOG4Z_ASSERT(0);
			return -1;
		}
		if (m_ids.find(name) != m_ids.end())
		{
			LOG4Z_ASSERT(0);
			return -1;
		}
		m_ids.insert(std::pair<std::string, LoggerId>(name, m_lastId));
		
		m_loggers[m_lastId]._path = path;
		m_loggers[m_lastId]._name = name;
		m_loggers[m_lastId]._level = nLevel;
		m_loggers[m_lastId]._enable = true;
		m_loggers[m_lastId]._display = display;
		return m_lastId;
	}

	bool ChangeLoggerLevel(LoggerId nLoggerID, int nLevel)
	{
		if (nLoggerID <0 || nLoggerID >= LOGGER_MAX || nLevel < LOG_LEVEL_DEBUG || nLevel >LOG_LEVEL_FATAL) return false;
		m_loggers[nLoggerID]._level = nLevel;
		return true;
	}
	bool ChangeLoggerDisplay(LoggerId nLoggerID, bool enable)
	{
		if (nLoggerID <0 || nLoggerID >= LOGGER_MAX) return false;
		m_loggers[nLoggerID]._display = enable;
		return true;
	}

	bool PushLog(LoggerId id, int level, const char * log)
	{
		if (id < 0 || id >= LOGGER_MAX)
		{
			return false;
		}
		if (!m_loggers[id]._enable)
		{
			return false;
		}
		if (level < m_loggers[id]._level)
		{
			return true;
		}

		LogData * pLog = new LogData;
		pLog->_id =id;
		pLog->_level = level;
		pLog->_time = time(NULL);
		if (strlen(log) >= LOG_BUF_SIZE)
		{
			memcpy(pLog->_content, log, LOG_BUF_SIZE);
			pLog->_content[LOG_BUF_SIZE-1] = '\0';
		}
		else
		{
			memcpy(pLog->_content, log, strlen(log)+1);
		}
		CAutoLock l(m_lock);
		l.Lock();
		m_logs.push_back(pLog);
		return true;
	}

	bool Start()
	{
		m_bRuning = true;
		m_semaphore.Create(0);
		bool ret = CThread::Start();
		assert(ret);
		return ret && m_semaphore.Wait(3000);
	}
	bool Stop()
	{
		if (m_bRuning == true)
		{
			m_bRuning = false;
			Wait();
			return true;
		}
		return false;
	}

protected:
	time_t GetNextUptime(time_t tt)
	{
		tm t;
		TimeToTm(tt, &t);
		return tt - t.tm_hour*3600 - t.tm_min*60 - t.tm_sec + 24*3600;
	}
	bool IsSameDay(time_t t1, time_t t2)
	{
		tm tm1;
		tm tm2;
		TimeToTm(t1, &tm1);
		TimeToTm(t2, &tm2);
		if ( tm1.tm_year == tm2.tm_year
			&& tm1.tm_yday == tm2.tm_yday)
		{
			return true;
		}
		return false;
	}
	bool OpenLogger(LoggerId id, time_t now)
	{
		if (id < 0 || id >m_lastId || id >= LOGGER_MAX)
		{
			return false;
		}
		LoggerInfo * pLogger = &m_loggers[id];
		if (pLogger->_handle.is_open())
		{
			pLogger->_handle.close();
		}

		tm t;
		TimeToTm(now, &t);
		char buf[100];
		sprintf(buf, "%04d_%02d", t.tm_year+1900, t.tm_mon+1);
		std::string path = pLogger->_path + buf + "/";
		if (!IsDirectory(path))
		{
			CreateRecursionDir(path);
		}

		sprintf(buf, "%s_%04d_%02d_%02d.log", pLogger->_name.c_str(), t.tm_year+1900, t.tm_mon+1, t.tm_mday);
		path += buf;
		pLogger->_handle.open(path.c_str(), std::ios::app|std::ios::out|std::ios::binary);
		if (!pLogger->_handle.is_open())
		{
			return false;
		}
		return true;
	}

	bool PopLog(LogData *& log)
	{
		CAutoLock l(m_lock);
		l.Lock();
		if (m_logs.empty())
		{
			return false;
		}
		log = m_logs.front();
		m_logs.pop_front();
		return true;
	}
	virtual void Run()
	{

		LogData * pLog = NULL;
		std::string text;
		text.reserve(2000);

		std::string str = "-----------------  log4z thread started!   ----------------------------";
		PushLog(GetMainLogger(), LOG_LEVEL_ALARM, str.c_str());
		for (int i=0; i<LOGGER_MAX; i++)
		{
			if (m_loggers[i]._enable)
			{
				std::stringstream ss;
				ss <<" logger id=" <<i 
				   <<" path=" <<m_loggers[i]._path
				   <<" name=" <<m_loggers[i]._name
				   <<" level=" << m_loggers[i]._level
				   <<" display=" << m_loggers[i]._display;
				PushLog(GetMainLogger(), LOG_LEVEL_ALARM, ss.str().c_str());
			}
		}
		
		m_semaphore.Post();



		int needFlush[LOGGER_MAX];
		while (true)
		{
			memset(needFlush, 0, sizeof(needFlush));

			while(PopLog(pLog))
			{
				if (!m_loggers[pLog->_id]._enable || pLog->_level <m_loggers[pLog->_id]._level  )
				{
					delete pLog;
					pLog = NULL;
					continue;
				}
				//update file
				if (!IsSameDay(pLog->_time, m_uptimes-1))
				{
					m_uptimes = GetNextUptime(pLog->_time);
					for (unsigned int i=0; i<LOGGER_MAX; i++)
					{
						if (m_loggers[i]._enable)
						{
							OpenLogger(i, pLog->_time);
						}
					}
				}
				//check file handle
				if (!m_loggers[pLog->_id]._handle.is_open() 
					|| !m_loggers[pLog->_id]._handle.good())
				{
					OpenLogger(pLog->_id, pLog->_time);
					if (!m_loggers[pLog->_id]._handle.is_open())
					{
						m_loggers[pLog->_id]._enable = false;
						delete pLog;
						pLog = NULL;
						continue;
					}
				}

				//record
				text = TimeToString(pLog->_time);
				text += " ";
				text += LOG_STRING[pLog->_level];
				text += " ";
				text += pLog->_content;
				text += "\r\n";
				m_loggers[pLog->_id]._handle.write(text.c_str(), (std::streamsize)text.length());
				needFlush[pLog->_id] ++;
				//print to screen
				if (m_loggers[pLog->_id]._display)
				{
					ShowColorText(text.c_str(), pLog->_level);
				}
				delete pLog;
				pLog = NULL;
			}

			//flush
			for (unsigned int i=0; i<LOGGER_MAX; i++)
			{
				if (m_loggers[i]._enable && needFlush[i] > 0)
				{
					m_loggers[i]._handle.flush();
				}
			}
			//stopped
			if (!m_bRuning)
			{
				break;
			}
			//delay. 
			SleepMillisecond(200);
		}

		for (unsigned int i=0; i<LOGGER_MAX; i++)
		{
			if (m_loggers[i]._enable)
			{
				m_loggers[i]._handle.close();
			}
		}
		m_bRuning = false;
	}

private:

	//thread status
	bool		m_bRuning;
	//log4z start wait for thread start
	CSem		m_semaphore;
	
	//update log file on everyday
	time_t		m_uptimes;

	//safe get logger id
	CLock m_idLock;
	std::map<std::string, LoggerId> m_ids;
	LoggerId	m_lastId;
	
	//main logger
	LoggerId	m_main;
	
	//all logger objects
	LoggerInfo m_loggers[LOGGER_MAX];

	//log queue, thread safe
	std::list<LogData *> m_logs;
	CLock	m_lock;
};




ILog4zManager * ILog4zManager::GetInstance()
{
	static CLogerManager m;
	return &m;
}







_ZSUMMER_LOG4Z_END
_ZSUMMER_END

