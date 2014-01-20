/*
 *  EvolvingRobotAgentWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 25/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "WorldModels/EvolvingRobotAgentWorldModel.h"

EvolvingRobotAgentWorldModel::EvolvingRobotAgentWorldModel()
{
}

EvolvingRobotAgentWorldModel::~EvolvingRobotAgentWorldModel()
{
}

/*std::vector<double>* EvolvingRobotAgentWorldModel::getGenome() // return pointer to original
{
	return &_genome;
}*/

////void EvolvingRobotAgentWorldModel::setGenome( std::vector<double> &__genome ) // give pointer to original __genome
/*void EvolvingRobotAgentWorldModel::setGenome( std::vector<double> __genome ) // copy content of original __genome
{
	_genome = __genome;
}*/
		
bool EvolvingRobotAgentWorldModel::getNewGenomeStatus()
{
	return _isNewGenome;
}

void EvolvingRobotAgentWorldModel::setNewGenomeStatus( bool __status )
{
	_isNewGenome = __status;
}
		
