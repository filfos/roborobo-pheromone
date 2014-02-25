/*
 *  BasicPheromoneWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicPheromone/include/BasicPheromoneAgentWorldModel.h"

#include "World/World.h"

BasicPheromoneAgentWorldModel::BasicPheromoneAgentWorldModel()
{
	_sensors = NULL;
	std::cout<<  "HELLO WORLD" << std::endl;
	intensities[1][1] = 2;
}

BasicPheromoneAgentWorldModel::~BasicPheromoneAgentWorldModel()
{
	if ( _sensors != NULL )
		delete[] _sensors;
}


void BasicPheromoneAgentWorldModel::testFoo()
{
  std::cout << "THIS IS A TEST " << std::endl;
  std::cout << intensities[1][1]++ << std::endl;
  
}