/*
 *  AgentWorldInterface.h
 *  roborobo
 *
 *  Created by Nicolas on 29/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ROBOTAGENTWORLDINTERFACEDANGERWORLD_H
#define ROBOTAGENTWORLDINTERFACEDANGERWORLD_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"
#include "World/RobotAgentWorldInterface.h"

/* An AgentWorldInterface provides a single static class for applying world constrained dynamics onto agent dynamics
   Example: 
	add noise to effector values to compute "true" effector values
	add noise to sensor values
	modify "true" effector to comply with sliding, rugged terrain, etc.
*/
class RobotAgentWorldInterfaceDangerWorld : public RobotAgentWorldInterface
{

	public: 
		static void prepareInternalDynamics ();
		static void applyInternalDynamics( RobotAgentWorldModel* );
};

#endif
