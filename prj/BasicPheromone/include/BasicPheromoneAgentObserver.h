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

#include "BasicPheromoneAgentWorldModel.h"
#include "Observers/AgentObserver.h"

class BasicPheromoneAgentObserver : public AgentObserver
{
	protected:
		BasicPheromoneAgentWorldModel	*_wm;

	public:
		//Initializes the variables
		BasicPheromoneAgentObserver( );
		BasicPheromoneAgentObserver( RobotAgentWorldModel *__wm );
		~BasicPheromoneAgentObserver();
				
		void reset();
		void step();
		
};


#endif

