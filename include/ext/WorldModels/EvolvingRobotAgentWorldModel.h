/*
 *  EvolvingRobotAgentWorldModel.h
 *  roborobo-online
 *
 *  Created by Nicolas on 25/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef EVOLVINGROBOTAGENTWORLDMODEL_H
#define EVOLVINGROBOTAGENTWORLDMODEL_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/RobotAgentWorldModel.h"

//class World;

class EvolvingRobotAgentWorldModel : public RobotAgentWorldModel
{
	protected:

		bool _isNewGenome;

	public:

		std::vector<double> _genome; // todo: accessing
		
		EvolvingRobotAgentWorldModel();
		~EvolvingRobotAgentWorldModel();
		
		//std::vector<double>* getGenome();
		//void setGenome( std::vector<double> __genome );
		
		bool getNewGenomeStatus();
		void setNewGenomeStatus( bool __status );
		
};


#endif

