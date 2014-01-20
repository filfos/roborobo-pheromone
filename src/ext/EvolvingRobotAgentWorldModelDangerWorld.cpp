/*
 *  EvolvingRobotAgentWorldModelDangerWorld.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 22/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "WorldModels/EvolvingRobotAgentWorldModelDangerWorld.h"

// radius (to the square) within which neighborhood is considered (in pixels)
double EvolvingRobotAgentWorldModelDangerWorld::_gInfluenceRadius = 10; 

// nb of robots needed to avoid velocity penalization in danger zone
int EvolvingRobotAgentWorldModelDangerWorld::_gRobotDensityThreshold = 2; 

// (maximum) penalization if no neighbors
double EvolvingRobotAgentWorldModelDangerWorld::_gMaximumVelocityPenalizationFactor = 0.5; 

// initialization state -- should be true after first call.
bool EvolvingRobotAgentWorldModelDangerWorld::_gLoadedProperties = false;


EvolvingRobotAgentWorldModelDangerWorld::EvolvingRobotAgentWorldModelDangerWorld()
{
}

EvolvingRobotAgentWorldModelDangerWorld::~EvolvingRobotAgentWorldModelDangerWorld()
{
}

