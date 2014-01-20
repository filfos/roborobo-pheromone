/*
 *  PerceptronBehaviorControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef PERCEPTRONBEHAVIORCONTROLARCHITECTURE_H
#define PERCEPTRONBEHAVIORCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"



class PerceptronBehaviorControlArchitecture : public BehaviorControlArchitecture
{
	private:
		EvolvingRobotAgentWorldModel *_wm;
		std::vector<double> _parameters;

	public:
		//Initializes the variables
		PerceptronBehaviorControlArchitecture( RobotAgentWorldModel *__wm );
		~PerceptronBehaviorControlArchitecture();
		
		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

};


#endif

