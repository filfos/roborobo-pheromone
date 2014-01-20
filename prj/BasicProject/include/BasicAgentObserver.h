/*
 *  BasicAgentObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

 
#ifndef BASICAGENTOBSERVER_H
#define BASICAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "BasicProject/include/BasicAgentWorldModel.h"

#include "Observers/AgentObserver.h"

class BasicAgentObserver : public AgentObserver
{
	protected:
		BasicAgentWorldModel	*_wm;

	public:
		//Initializes the variables
		BasicAgentObserver( );
		BasicAgentObserver( RobotAgentWorldModel *__wm );
		~BasicAgentObserver();
				
		void reset();
		void step();
		
};


#endif

