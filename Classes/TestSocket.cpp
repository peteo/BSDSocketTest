#include "TestSocket.h"

TestThread::TestThread()
:Thread(false)
{
	m_plog     = new StdoutLog();
	m_Handler  = new MyHandler(m_plog);

	m_pTS      = new TestSocket(*m_Handler);
	
	m_pTS->Open("210.51.22.209", 6001);
	m_Handler->Add(m_pTS);
	//h.Select(0,0);
	SetRelease(true);
}


TestThread::~TestThread()
{
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