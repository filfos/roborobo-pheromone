/*
 *  AgentWorldInterface.h
 *  roborobo
 *
 *  Created by Nicolas on 29/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ROBOTAGENTWORLDINTERFACE_H
#define ROBOTAGENTWORLDINTERFACE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

/* An AgentWorldInterface provides a single static class for applying world constrained dynamics onto agent dynamics
   Example: 
	add noise to effector values to compute "true" effector values
	add noise to sensor values
	modify "true" effector to comply with sliding, rugged terrain, etc.
*/
class RobotAgentWorldInterface
{
	public: 
		static void prepareInternalDynamics (); // called once per world iteration, by world->updateWorld(...)
		static void applyInternalDynamics( RobotAgentWorldModel* ); // called for each agent
};

#endif
