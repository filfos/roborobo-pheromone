/*
 *  BasicWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicProject/include/BasicAgentWorldModel.h"

#include "World/World.h"

BasicAgentWorldModel::BasicAgentWorldModel()
{
	_sensors = NULL;
}

BasicAgentWorldModel::~BasicAgentWorldModel()
{
	if ( _sensors != NULL )
		delete[] _sensors;
}
