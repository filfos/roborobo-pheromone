/*
 *  EnergyPerceptronControlArchitecture.h
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ENERGYPERCEPTRONCONTROLARCHITECTURE_H
#define ENERGYPERCEPTRONCONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"



class EnergyPerceptronControlArchitecture : public BehaviorControlArchitecture
{
	private:
		EvolvingRobotAgentWorldModel *_wm;
		std::vector<double> _parameters;
		int _iteration;
		int _evaluationTime;
		int _maxEvaluation;
		double _driftEvaluationRate;

		float _energy;
		float _deltaEnergy;
		float _initialEnergy;

		double _angleToClosestEnergyPoint;
		double _distanceToClosestEnergyPoint;

		void searchClosestEnergyPoint(Point2d posRobot);

	public:
		//Initializes the variables
		EnergyPerceptronControlArchitecture( RobotAgentWorldModel *__wm );
		~EnergyPerceptronControlArchitecture();
		
		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

		float getEnergy();
		void setEnergy(float inValue);
		
		float getDeltaEnergy();
		void setDeltaEnergy(float inValue);

		float getInitialEnergy();
};


#endif

