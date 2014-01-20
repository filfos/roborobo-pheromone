/*
 *  BehaviorControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"

BehaviorControlArchitecture::BehaviorControlArchitecture(  ) 
{
	// nothing to do.
}


BehaviorControlArchitecture::BehaviorControlArchitecture( RobotAgentWorldModel *__wm ) 
{
	_wm = __wm;
}

BehaviorControlArchitecture::~BehaviorControlArchitecture()
{
	// nothing to do.
}

