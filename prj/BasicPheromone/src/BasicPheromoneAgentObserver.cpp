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
	
	currentIndex = 0;
	stepCounter = 0;
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
   ++stepCounter;
   
   if (stepCounter == 10)
   {
     Uint8 initialIntensity = _wm->getRobotLED_greenValue();
     _wm->secretePheromone(20, 300, 70, initialIntensity);
     stepCounter = 0;
   }
  // Update active pheromones in queue
  for (int i = currentIndex; i < _wm->getPheromoneQueueSize(); i++)
  {
    Pheromone *p = _wm->getPheromone(i);
    if (p->isActive())
    {
	p->step();
	if (p->isUpdated())
	  updateIntensities(p); 

    }
    else
    {
	currentIndex = i;
	continue;
    }
  }
}


void BasicPheromoneAgentObserver::updateIntensities(Pheromone *p)
{
  int x0, y0;
  int radius;
  p->getCenter(x0, y0);
  p->getCurrentRadius(radius);
  int sqrdRadius = radius*radius;
  int sqrdOldRadius = p->getOldRadius()*p->getOldRadius();
  
  for (int x = x0-radius; x < x0 + radius; x++)
  {
    if (x < 0) continue;
    if (x >= gScreenWidth) break;
    
    for (int y = y0-radius; y < y0 + radius; y++)
    {
      if (y < 0) continue;
      if (y >= gScreenHeight) break;

      int sqrdDistance = p->distanceFromCenter(x, y);
      if (sqrdDistance < sqrdOldRadius)
      {
	/* Reduce intensity of existing pheromone */
	_wo->modifyIntensityAt(x, y, -p->getEvaporated());

      }
      else if (sqrdDistance < sqrdRadius)
      {
	/* Pheromone expands into new territory */	
	_wo->modifyIntensityAt(x, y, p->getIntensity());

      }
    }
  }   
}


