#ifndef __TEST_SOCKETS_H__
#define __TEST_SOCKETS_H__

#include <queue>

#include "TcpSocket.h"
#include "SocketHandler.h"
#include "Thread.h"
#include "StdoutLog.h"

typedef struct _AsyncStruct
{
	std::string			filename;
} AsyncStruct;

typedef struct _ImageInfo
{
	AsyncStruct *asyncStruct;
} ImageInfo;

class SocketProtocol
{
public:
	virtual void OnReceiveData(const char *buf,size_t len)
	{
		
	}
};

class MyHandler : public SocketHandler
{
public:
	MyHandler(StdLog * pLog)
	:SocketHandler(pLog)
	,m_quit(false)
	{
		
	}
	
	virtual ~MyHandler() 
	{
		
	}
	
	void SetQuit()
	{ 
		m_quit = true;
	}
	
	bool Quit()
	{ 
		return m_quit;
	}
	
private:
	bool m_quit;
};

class TestSocket : public TcpSocket
{
public:
	TestSocket(ISocketHandler& pHandler)
	:TcpSocket(pHandler)
	,m_pTarget(NULL)
	{
		//SetLineProtocol();
	}

	void OnRawData(const char *buf,size_t len);

	void OnConnect();

	void OnConnectFailed();
	
public:
	
	SocketProtocol * m_pTarget;
	
};



//////////////////////////////////////////


class TestThread : public Thread , public SocketProtocol
{
public:
	TestThread(SocketProtocol * pTarget);

	virtual ~TestThread();

	void Run();
	
	void send(const std::string &str);
	
	void OnReceiveData(const char *buf,size_t len);

private:
	StdoutLog     * m_plog;
	MyHandler     * m_Handler;
	TestSocket    * m_pTS;
	
	std::queue<AsyncStruct*>		*s_pAsyncStructQueue;
	std::queue<ImageInfo*>			*s_pImageQueue;

};

#endif