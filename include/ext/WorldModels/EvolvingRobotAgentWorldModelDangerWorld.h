/*
 *  EvolvingRobotAgentWorldModelDangerWorld.h
 *  roborobo-online
 *
 *  Created by Nicolas on 22/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef EVOLVINGROBOTAGENTWORLDMODELDANGERWORLD_H
#define EVOLVINGROBOTAGENTWORLDMODELDANGERWORLD_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/EvolvingRobotAgentWorldModel.h"

//class World;

class EvolvingRobotAgentWorldModelDangerWorld : public EvolvingRobotAgentWorldModel
{
	public:
		//Initializes the variables
		EvolvingRobotAgentWorldModelDangerWorld();
		~EvolvingRobotAgentWorldModelDangerWorld();

	
		static double 	_gInfluenceRadius;			
		static int 		_gRobotDensityThreshold; 	
		static double 	_gMaximumVelocityPenalizationFactor; 
		static bool		_gLoadedProperties;
	
};


#endif

