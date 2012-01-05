//
//  HelloWorldScene.h
//  BSDSocketTest
//
//  Created by Peteo on 11-12-30.
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class TestThread;

class HelloWorld : public CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static CCScene* scene();
	
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);

	void gameLoop(ccTime dt);
	
	TestThread * m_pTest;
};

#endif // __HELLOWORLD_SCENE_H__
