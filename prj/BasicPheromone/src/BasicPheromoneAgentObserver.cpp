/*
 *  BasicPheromoneAgentObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicPheromone/include/BasicPheromoneAgentObserver.h"



BasicPheromoneAgentObserver::BasicPheromoneAgentObserver( )
{
}

BasicPheromoneAgentObserver::BasicPheromoneAgentObserver( RobotAgentWorldModel *__wm )
{
	_wm = (BasicPheromoneAgentWorldModel*)__wm;
}

BasicPheromoneAgentObserver::~BasicPheromoneAgentObserver()
{
	// nothing to do.
}

void BasicPheromoneAgentObserver::reset()
{
	// nothing to do.
}

void BasicPheromoneAgentObserver::step()
{
	// nothing to do.
}

