/*
 *  RobotAgent.h
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ROBOTAGENT_H
#define ROBOTAGENT_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Agents/Agent.h"

#include "Utilities/SDL_gfxPrimitives.h"
#include "Utilities/SDL_gfxPrimitivesExtra.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "Observers/Observer.h"
#include "Observers/WorldObserver.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include "WorldModels/RobotAgentWorldModel.h"
#include "World/World.h"
#include "World/RobotAgentWorldInterface.h"

// *** begin: add new includes here ***

#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include "WorldModels/EvolvingRobotAgentWorldModelDangerWorld.h"

#include "BehaviorControlArchitectures/PerceptronBehaviorControlArchitecture.h"

#include "Observers/Observer.h"
//#include "OnePlusOne-Online/include/OnePlusOneOnlineGeneticsEngineAgentObserver.h"
//#include "OnePlusOne-Online/include/StrongCausalityExperimentAgentObserver.h"
#include "Observers/AgentObserver.h"

#include "World/RobotAgentWorldInterfaceDangerWorld.h"

// *** end ***


class World;

class RobotAgent : public Agent
{
	
	private:
	
		BehaviorControlArchitecture *_behavior;
		AgentObserver *_agentObserver;
	
		RobotAgentWorldModel *_wm;

		// real displacement (x/y format, delta applied to real coordinates (_xReal,_yReal))
		double _xDeltaReal;
		double _yDeltaReal;
	
		double _lastAgentAbsoluteOrientation; // backup in case of collision

		int _iterations;
					
	public:
		//Initializes the variables
		RobotAgent( World *__world );
		~RobotAgent();

		void reset();

		void stepBehavior();
		void stepBehavior(Uint8* __keyboardStates);

		void callObserver(); // call the embedded genetic engine -- always called before stepBehavior.

		void applyDynamics(); // use transl/rot delta to compute x/y delta.

		//Moves the agent
		void move( int __recursiveIt = 0 );
		
		void show();

		bool isCollision();

		void setCoordReal (int __x, int __y); // agent is centered on point
		
		void registerAgent();
		void unregisterAgent();
		
		void behaviorUpdate_remoteController(Uint8* __keyStates);

		void initController(double* __params); // set up controller with parameters (e.g. NN weights)
		void resetFitness(); // prepare fitness for start-up (init or reset)
		double getFitnessValue(); // return actual fitness computed for this agent
		void updateFitness(); // step-update of the fitness - performs updates needed at this time step -- automatically called through world.updateWorld

		BehaviorControlArchitecture* getBehavior();
		AgentObserver* getObserver();
		WorldObserver* getWorldObserver();

		void displayInfo(); // display input/output values on console. (used for inspect agent mode, cf. help on console) -- keep it simple: one line only.

		RobotAgentWorldModel* getWorldModel();
	
		
		// return a list of agents in the vincinity (ie. in the communication area)
		// the content of this list depends on the max communication range (see properties file).
		// works only if communication network is active.
		std::vector<int> getNeighbors(); 
		
		/*
		// controller step
		void step();
		void step(Uint8* __keyStates);
    

		//Shows the dot on the screen
		void show();
    
		//Sets the camera over the dot
		void set_camera();
	
		void getCoord(int &coord_x, int &coord_y)
		{
			coord_x=x;
			coord_y=y;
		}
	
		{
			x = __x - gDotWidth/2 ; 
			y = __y - gDotHeight/2 ;
		}
		*/
};



#endif /* ROBOTAGENT_H */


