/*
 *  BasicPheromoneAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

 
#ifndef BASICPHEROMONEAGENTOBSERVER_H
#define BASICPHEROMONEAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "World/World.h"

#include "BasicPheromoneAgentWorldModel.h"
#include "Observers/AgentObserver.h"
#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"
#include "BasicPheromone/include/Pheromone.h"

class BasicPheromoneAgentObserver : public AgentObserver
{
	protected:
		BasicPheromoneAgentWorldModel	*_wm;
		BasicPheromoneWorldObserver *_wo;
		
		int currentIndex;
		Uint8 stepCounter;
		void updateIntensities(Pheromone *p);
		

	public:
		//Initializes the variables
		BasicPheromoneAgentObserver( );
		BasicPheromoneAgentObserver( RobotAgentWorldModel *__wm );
		~BasicPheromoneAgentObserver();
				
		void reset();
		void step();
		
};


#endif

