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
	_wo = (BasicPheromoneWorldObserver*)_wm->getWorld()->getWorldObserver();
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
  int x, y;
  x = _wm->getXReal();
  y = _wm->getYReal();
  
//   if (getRobotLED_blueValue() == 255)
  if (_wm->getRobotLED_greenValue() == 255)
    _wo->activatePheromone(x, y, 255);
   
}


