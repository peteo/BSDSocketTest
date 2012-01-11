/*
 *  NetManager.cpp
 *  BSDSocketTest
 *
 *  Created by Peteo on 12-1-11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "NetManager.h"

#include <queue>

typedef struct _AsyncRequest
{
	char				*data;
	size_t				len;
	SelectorProtocol	*target;
	SEL_CallFuncO		selector;
	
	_AsyncRequest()
	{
		len			 = 0;
		data		 = NULL;
		target       = NULL;
		selector     = NULL;
	}
	virtual ~_AsyncRequest()
	{
		CC_SAFE_FREE(data);
	}
	
} AsyncRequest;

typedef struct _RawDataInfo
{
	//AsyncRequest *asyncRequest;
	char         *data;
	size_t	     len;
	
	_RawDataInfo()
	{
		len			 = 0;
		data		 = NULL;
		//asyncRequest = NULL;
	}
	virtual ~_RawDataInfo()
	{
		CC_SAFE_FREE(data);
	}
	
} RawDataInfo;

static pthread_t s_HandlerLoopThread;

static std::queue<AsyncRequest*>		*s_pAsyncRequestQueue;		//请求队列
static std::queue<RawDataInfo*>		    *s_pRawDataQueue;		    //接收处理队列

static pthread_mutex_t		s_AsyncRequestQueueMutex;
static pthread_mutex_t		s_RawDataQueueMutex;

//static sem_t s_sem;

static void* HandlerLoop(void* data)
{
	NetManager * pNetManager = (NetManager*)data;
	
	pNetManager->getSocketHandler()->Select(0,0);
	while (!pNetManager->getSocketHandler()->Quit())
	{
		pNetManager->getSocketHandler()->Select(1,0);
	}
	
	return 0;
}

NetManager::NetManager()
:m_pLog(NULL)
,m_pSocketHandler(NULL)
,m_pSocket(NULL)
,m_pTarget(NULL)
{
	
}

NetManager::~NetManager()
{
	//sem_destroy(&s_sem);
	
	pthread_mutex_destroy(&s_AsyncRequestQueueMutex);
	pthread_mutex_destroy(&s_RawDataQueueMutex);
	
	CC_SAFE_DELETE(s_pAsyncRequestQueue);
	CC_SAFE_DELETE(s_pRawDataQueue);
	
	CC_SAFE_DELETE(m_pLog);
	CC_SAFE_DELETE(m_pSocketHandler);
	CC_SAFE_DELETE(m_pSocket);
}

bool NetManager::init()
{
	m_pLog			  = new StdoutLog();
	m_pSocketHandler  = new DWSocketHandler(m_pLog);
	m_pSocket		  = new DWSocket(*m_pSocketHandler);
	
	s_pAsyncRequestQueue = new std::queue<AsyncRequest*>();
	s_pRawDataQueue		 = new std::queue<RawDataInfo*>();
	
	m_pSocket->Open("210.51.22.209", 6001);
	m_pSocketHandler->Add(m_pSocket);
	
	pthread_mutex_init(&s_AsyncRequestQueueMutex, NULL);
	//sem_init(&s_sem, 0, 0);
	pthread_mutex_init(&s_RawDataQueueMutex, NULL);
	
	pthread_create(&s_HandlerLoopThread,NULL,HandlerLoop,this);
	
	CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(NetManager::NetTick), this, 0, false);
	
	return true;
}

void NetManager::NetTick(ccTime dt)
{
	//首先处理发送请求
	AsyncRequest * pAsyncRequest = NULL;
	
	//sem_wait(&s_sem);
	
	std::queue<AsyncRequest*> *pQueue = s_pAsyncRequestQueue;
	
	pthread_mutex_lock(&s_AsyncRequestQueueMutex);
	if (pQueue->empty())
	{
		pthread_mutex_unlock(&s_AsyncRequestQueueMutex);
	}
	else
	{
		pAsyncRequest = pQueue->front();
		pQueue->pop();
		pthread_mutex_unlock(&s_AsyncRequestQueueMutex);
		
		CCLOG("thread 0x%x is sending %s", pthread_self(), pAsyncRequest->data);
		
		m_pSocket->Send(pAsyncRequest->data);
		
		CC_SAFE_DELETE(pAsyncRequest);
	}
	
	//处理接收的请求
	std::queue<RawDataInfo*> *RawDataQueue = s_pRawDataQueue;
	
	pthread_mutex_lock(&s_RawDataQueueMutex);
	if (RawDataQueue->empty())
	{
		pthread_mutex_unlock(&s_RawDataQueueMutex);
	}
	else
	{
		RawDataInfo *pRawDataInfo = RawDataQueue->front();
		RawDataQueue->pop();
		pthread_mutex_unlock(&s_RawDataQueueMutex);
		
		if(m_pTarget)
		{
			m_pTarget->OnReceiveData(pRawDataInfo->data,pRawDataInfo->len);
		}
		
		//AsyncRequest *pAsyncRequest = pRawDataInfo->asyncStruct;
		
		//SelectorProtocol *target = pAsyncStruct->target;
		//SEL_CallFuncO selector = pAsyncStruct->selector;
		//const char* filename = pAsyncStruct->filename.c_str();
		
		
		//if (target && selector)
		//{
			//(target->*selector)(texture);
			//target->selectorProtocolRelease();
		//}		
		
		CC_SAFE_DELETE(pRawDataInfo);
	}
	
	
	
}

void NetManager::addRequest(const char * path, SelectorProtocol *target, SEL_CallFuncO selector)
{
	if(path == NULL)
	{
		return;
	}
	
	if (target)
	{
		//target->selectorProtocolRetain();
	}
	
	// generate async struct
	AsyncRequest *data = new AsyncRequest();
	data->data     = strdup(path);
	data->target   = target;
	data->selector = selector;
	
	// add async struct into queue
	pthread_mutex_lock(&s_AsyncRequestQueueMutex);
	s_pAsyncRequestQueue->push(data);
	pthread_mutex_unlock(&s_AsyncRequestQueueMutex);
	
	//sem_post(&s_sem);
}





//////////////////////////////////////////////////////////////////

void DWSocket::OnRawData(const char *buf,size_t len)
{
	//CCLOG("DWSocket OnRawData[%s]",buf);
	
	// generate image info
	RawDataInfo *pRawDataInfo = new RawDataInfo();
	pRawDataInfo->len  = len;
	pRawDataInfo->data = (char*)malloc(len + 1);
	memset(pRawDataInfo->data,0,len + 1);
	memcpy(pRawDataInfo->data, buf, len);
	
	// put the image info into the queue
	pthread_mutex_lock(&s_RawDataQueueMutex);
	s_pRawDataQueue->push(pRawDataInfo);
	pthread_mutex_unlock(&s_RawDataQueueMutex);
}

void DWSocket::OnConnect()
{
	CCLOG("DWSocket OnConnect");
}

void DWSocket::OnConnectFailed()
{
	CCLOG("DWSocket::OnConnectFailed");
	SetCloseAndDelete();
}
