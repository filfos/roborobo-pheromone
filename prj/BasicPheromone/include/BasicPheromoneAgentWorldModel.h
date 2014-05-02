/*
 *  BasicPheromoneAgentWorldModel.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */


#ifndef BASICPHEROMONEAGENTWORLDMODEL_H
#define BASICPHEROMONEAGENTWORLDMODEL_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

#include <vector>

class World;

class BasicPheromoneAgentWorldModel : public RobotAgentWorldModel
{	
	private: 
		
		
	protected:
	  						
		
  
	
	public:

		// refer to super class to check what data / methods are available.		
		
		BasicPheromoneAgentWorldModel();
		virtual ~BasicPheromoneAgentWorldModel();
		
		
		World* getWorld();
		
};


#endif


