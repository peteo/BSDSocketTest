//
//  HelloWorldScene.cpp
//  BSDSocketTest
//
//  Created by Peteo on 11-12-30.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

//#include "TestSocket.h"

#include "HelloWorldScene.h"

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
	CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Thonburi", 20);
	pLabel->setTag(100);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	pLabel->setPosition( ccp(size.width / 2, size.height / 2) );
	this->addChild(pLabel);

	// add "HelloWorld" splash screen"
	//CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");
	//pSprite->setPosition( ccp(size.width/2, size.height/2) );
	//this->addChild(pSprite, 0);

	//schedule(schedule_selector(HelloWorld::gameLoop),0);

	//m_pTest = new TestThread(this);
	
	m_pNetManager = new NetManager();
	m_pNetManager->m_pTarget = this;
	m_pNetManager->init();
	
	return true;
}

void HelloWorld::gameLoop(ccTime dt)
{
	
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	//CCDirector::sharedDirector()->end();
	
	//test send
	m_pNetManager->addRequest("root\r\n",NULL,NULL);
	m_pNetManager->addRequest("root\r\n",NULL,NULL);
	m_pNetManager->addRequest("help\r\n",NULL,NULL);
}


void HelloWorld::OnReceiveData(const char *buf,size_t len)
{
	CCLOG("HelloWorld::OnRawData:%s\n",buf);
	
	CCLabelTTF* pLabel = (CCLabelTTF*)getChildByTag(100);
	if(pLabel)
	{
		pLabel->setString(buf);
	}
}
 
 

