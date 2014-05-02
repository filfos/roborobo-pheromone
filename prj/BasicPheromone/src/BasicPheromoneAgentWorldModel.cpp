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
	_world = gWorld;
}

BasicPheromoneAgentWorldModel::~BasicPheromoneAgentWorldModel()
{
	if ( _sensors != NULL )
		delete[] _sensors;
}


World* BasicPheromoneAgentWorldModel::getWorld()
{
  return _world;
}