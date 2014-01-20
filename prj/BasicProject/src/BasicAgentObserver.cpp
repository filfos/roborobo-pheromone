/*
 *  BasicAgentObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicProject/include/BasicAgentObserver.h"



BasicAgentObserver::BasicAgentObserver( )
{
}

BasicAgentObserver::BasicAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (BasicAgentWorldModel*)__wm;
}

BasicAgentObserver::~BasicAgentObserver()
{
	// nothing to do.
}

void BasicAgentObserver::reset()
{
	// nothing to do.
}

void BasicAgentObserver::step()
{
	// nothing to do.
}

