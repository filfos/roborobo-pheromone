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
#include "BasicPheromone/include/BasicPheromoneAgentWorldModel.h"

#include "Coordinate.h"

class BasicPheromoneControlArchitecture : public BehaviorControlArchitecture
{
	public:
		//Initializes the variables
		BasicPheromoneControlArchitecture( RobotAgentWorldModel *__wm );
		~BasicPheromoneControlArchitecture();
		
		void reset();
		void step();
		
	
	protected:
		BasicPheromoneAgentWorldModel* _wm;
		
		void pheromoneReaction();
		void wallAvoidance();
		void wallAndPheromoneAvoidance();
		
		void foodReaction(int x, int y);
		
		void pheromoneStagnation();
		void wallStagnation();
		
		int determineBehaviour();
		
		bool checkWallStagnation();
		bool checkPheromoneStagnation();
		bool checkPheromoneDetected();
		
		void reverse(int duration);
		void stopAndRotate(int deg);
		
		double getWallAttackAngle();
		
		double cosine(double theta);
		double sine(double theta);
		
		double arcsine(double ratio);
		
		double maxLim (double a, double lim);
		
		const static double pi = 3.141592;
		
		bool isNotUsingPheromones;

		
		BasicPheromoneSensor *_pSensor;
		int stepCounter;
		int pheromoneCounter;
		int stagnationRotation;
		int pheromoneIgnoreCounter;
		
		bool isPheromoneStagnated;
		
		int red, green, blue;
		
		bool isRotating;
		bool isReversing;
		bool doneReversing;
		
		bool ignorePheromones;
		bool releasePheromones;
		
		int mStatus;
				
		int goalDeg;
		int reverseCounter;
		
		
		int getAngleOfLightSensor(int sensorId);
		int getAngleOfDistanceSensor(int sensorId);
		
		
		//std::vector<std::vector<int> > lastLocations;
		std::vector< Coordinate* > lastLocations;
		
		
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
		  PROX = 4, //NOT IN USE
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
		
		
		
		enum Status {
		  WALL_STAGNATION = 0,
		  PHEROMONE_STAGNATION = 1,
		  PHEROMONE_AVOIDANCE = 2,
		  DEFAULT = 3 
		};
		

};


#endif

