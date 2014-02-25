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
#include "Pheromone.h"

#include <vector>

class World;

class BasicPheromoneAgentWorldModel : public RobotAgentWorldModel
{	
	private: 
		int _robotNeighborhoodCounter; // count the number of other robots within a given radius.
		
		
	protected:
	  
		std::vector<Pheromone*> pheromones;
		double intensities[1500][900];
		
  
	
	public:

		// refer to super class to check what data / methods are available.		
		
		BasicPheromoneAgentWorldModel();
		virtual ~BasicPheromoneAgentWorldModel();
		
		void testFoo();
};


#endif


