/*
 *  BasicPheromoneControlArchitecture.h
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */


#ifndef BASICPHEROMONECONTROLARCHITECTURE_H
#define BASICPHEROMONECONTROLARCHITECTURE_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"

#include "WorldModels/RobotAgentWorldModel.h"
#include "BasicPheromoneSensor.h"

class BasicPheromoneControlArchitecture : public BehaviorControlArchitecture
{
	public:
		//Initializes the variables
		BasicPheromoneControlArchitecture( RobotAgentWorldModel *__wm );
		~BasicPheromoneControlArchitecture();
		
		void reset();
		void step();
		
	
	protected:
		void pheromoneReaction();
		void wallAvoidance();
		void foodReaction(int x, int y);
		
		BasicPheromoneSensor *_pSensor;
		int _pLeftCounter;
		int _pRightCounter;
		
		
		//Sensor location of _pSensor first dimension
		enum sensorLocation {
		  FRONT,
		  FRONT_RIGHT, 
		  RIGHT, 
		  BACK_RIGHT, 
		  BACK, 
		  BACK_LEFT, 
		  LEFT, 
		  FRONT_LEFT
		};
		
		enum sensorValues {
		  PROX = 4,
		  LIGHT = 5
		};
		
		enum proxSensors {
		  W = 0,
		  NW = 1,
		  N = 2,
		  NE = 3, 
		  E = 4, 
		  SE = 5,
		  S = 6,
		  SW = 7
		};


};


#endif
