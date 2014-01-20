/*
 *  BehaviorControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 19/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef BEHAVIORCONTROLARCHITECTURE_H
#define BEHAVIORCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"
#include "WorldModels/RobotAgentWorldModel.h"

class BehaviorControlArchitecture
{
	protected:
		RobotAgentWorldModel *_wm;

	public:
		//Initializes the variables
		BehaviorControlArchitecture( );
		BehaviorControlArchitecture( RobotAgentWorldModel *__wm );
		~BehaviorControlArchitecture();
				
		virtual void reset() = 0;
		virtual void step() = 0;
		
};


#endif

