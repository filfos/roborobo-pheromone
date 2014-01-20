/*
 *  BasicControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */


#ifndef BASICCONTROLARCHITECTURE_H
#define BASICCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"

#include "WorldModels/RobotAgentWorldModel.h"

class BasicControlArchitecture : public BehaviorControlArchitecture
{
	public:
		//Initializes the variables
		BasicControlArchitecture( RobotAgentWorldModel *__wm );
		~BasicControlArchitecture();
		
		void reset();
		void step();
};


#endif

