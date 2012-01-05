#ifndef __TEST_SOCKETS_H__
#define __TEST_SOCKETS_H__

#include "TcpSocket.h"
#include "SocketHandler.h"
#include "Thread.h"
#include "StdoutLog.h"

class MyHandler : public SocketHandler
{
public:
	MyHandler(StdLog *p) : SocketHandler(p),m_done(false),m_quit(false) {}
	~MyHandler() {}

	void List(TcpSocket *p) {
		for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); it++)
		{
			Socket *p0 = (*it).second;
#ifdef ENABLE_POOL
			if (dynamic_cast<ISocketHandler::PoolSocket *>(p0))
			{
				p -> Send("PoolSocket\n");
			}
			else
#endif
			if (dynamic_cast<TcpSocket *>(p0))
			{
				p -> Send("TcpSocket\n");
			}
			else
			{
				p -> Send("Some kind of Socket\n");
			}
		}
	}
	void SetQuit() { m_quit = true; }
	bool Quit() { return m_quit; }
	void CheckHtml() {
		if (m_done)
		{
			if (m_ok)
				printf("Html OK:\n%s\n", m_html.c_str());
			else
				printf("Html Failed\n");
			m_done = false;
		}
	}

	std::string m_html;
	bool m_ok;
	bool m_done;

private:
	bool m_quit;
};

class TestSocket : public TcpSocket
{
public:
	TestSocket(ISocketHandler& h) : TcpSocket(h)
	{
		//SetLineProtocol();
	}

	void OnRawData(const char *buf,size_t len)
	{
		printf("OnRawData:%s\n",buf);
	}

	void OnConnect()
	{
		printf("TestSocket connected\n");
	}

	void OnConnectFailed()
	{
		printf("TestSocket::OnConnectFailed\n");
		SetCloseAndDelete();
	}
};

class TestThread : public Thread
{
public:
	TestThread();

	virtual ~TestThread();

	void Run();
	
	void send(const std::string &str);

private:
	StdoutLog     * m_plog;
	MyHandler     * m_Handler;
	TestSocket    * m_pTS;

};

#endif