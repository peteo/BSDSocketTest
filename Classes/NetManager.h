/*
 *  NetManager.h
 *  BSDSocketTest
 *
 *  Created by Peteo on 12-1-11.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 *  网络管理模块
 */

#ifndef __NETMANAGER_H__
#define __NETMANAGER_H__

#include "cocos2d.h"

#include "SocketHandler.h"
#include "StdoutLog.h"
#include "TcpSocket.h"

USING_NS_CC;

class SocketReceiveProtocol
{
public:
	virtual void OnReceiveData(const char *buf,size_t len)
	{
		
	}
};

/** Socket句柄 */
class DWSocketHandler : public SocketHandler
{
public:
	DWSocketHandler(StdLog * pLog)
	:SocketHandler(pLog)
	,m_quit(false)
	{
		
	}
	
	virtual ~DWSocketHandler() 
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


/** Socket实例 */
class DWSocket : public TcpSocket
{
public:
	DWSocket(ISocketHandler& pHandler)
	:TcpSocket(pHandler)
	{
		//SetLineProtocol();
	}
	
	void OnRawData(const char *buf,size_t len);
	
	void OnConnect();
	
	void OnConnectFailed();
};


/** 网络管理实例 */
class NetManager : public SelectorProtocol, public CCObject
{
public:
	
	typedef enum NETMANAGER_STATUS
	{
		NETMANAGER_Connect = 0,
		NETMANAGER_RawData,
		NETMANAGER_ConnectFailed,
		NETMANAGER_STATUS_COUNT
	}NETMANAGER_STATUS;
	
public:
	
	NetManager();
	
	virtual ~NetManager();
	
	virtual bool init();
	
	/** 网络时间片，处理socket线程收到的队列包 */
	void NetTick(ccTime dt);
	
	/** 发送网络请求 */
	void addRequest(const char *path, SelectorProtocol *target, SEL_CallFuncO selector);
	
	CC_SYNTHESIZE_READONLY(DWSocketHandler*,m_pSocketHandler,SocketHandler);
	
private:
	StdoutLog			* m_pLog;
	DWSocket			* m_pSocket;
	
public:
	
	SocketReceiveProtocol * m_pTarget;
	
};

#endif
