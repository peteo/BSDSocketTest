#include "TestSocket.h"

//#include <stack>
//#include <string>
//#include <cctype>


TestThread::TestThread(SocketProtocol * pTarget)
:Thread(false)
{
	m_plog     = new StdoutLog();
	m_Handler  = new MyHandler(m_plog);

	m_pTS      = new TestSocket(*m_Handler);
	//m_pTS->m_pTarget = pTarget;
	
	m_pTS->m_pTarget = this;
	
	s_pAsyncStructQueue = new std::queue<AsyncStruct*>();
	s_pImageQueue = new std::queue<ImageInfo*>();
	
	m_pTS->Open("210.51.22.209", 6001);
	m_Handler->Add(m_pTS);
	//h.Select(0,0);
	SetRelease(true);
}


TestThread::~TestThread()
{
	delete s_pAsyncStructQueue;
	delete s_pImageQueue;
	
	delete m_pTS;
	delete m_Handler;
	delete m_plog;
}

void TestThread::Run()
{
	m_Handler->Select(0,0);
	while (!m_Handler->Quit())
	{
		m_Handler->Select(1,0);
	}
}

void TestThread::send(const std::string &str)
{
	m_pTS->Send(str);
}

void TestThread::OnReceiveData(const char *buf,size_t len)
{
	
}

/////////////////////////////////////////////////////////////////

void TestSocket::OnRawData(const char *buf,size_t len)
{
	if(m_pTarget)
	{
		m_pTarget->OnReceiveData(buf, len);
	}
}

void TestSocket::OnConnect()
{
	printf("TestSocket connected\n");
}

void TestSocket::OnConnectFailed()
{
	printf("TestSocket::OnConnectFailed\n");
	SetCloseAndDelete();
}


