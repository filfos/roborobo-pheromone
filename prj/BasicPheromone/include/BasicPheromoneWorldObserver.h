/*
 *  BasicPheromoneWorldObserver.h
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */



#ifndef BASICPHEROMONEWORLDOBSERVER_H
#define BASICPHEROMONEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"

#include <vector>
#include <deque>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include "BasicPheromone/include/Pheromone.h"
#include "BasicPheromone/include/Food.h"
#include "BasicPheromone/include/BasicPheromoneAgentWorldModel.h"



class World;

class BasicPheromoneWorldObserver : public WorldObserver
{
	protected:
//		std::deque<Pheromone*> pheromoneQueue;
		std::vector<Pheromone*> pheromoneQueue;
		
		std::vector<Pheromone*> pheromoneDefaultQueue;

		std::vector<Food*> foodList;
		std::vector<int> pheromoneTimer;
		
		std::vector<int> foundFood;
		std::vector<int> foodFoundTimestamps;
		std::vector<int> foodIdTimestamps;
		int noofPheromoneTimesteps;
		Uint8 noofFoodFound;
		int score;
		
		SDL_Surface* tempForeground;
		
		int stepCounter;
		
		//Pheromone properties
		static const bool useFoodPheromones = false;
		static const bool useDefaultPheromones = true;
		
		static const int foodInterval = 50;
		static const int foodLifetime = 1000;
		static const int foodMaxDiffusion = 100;
		
		
		static const int defaultInterval = 20;
		static const int defaultLifetime = 300;		
		static const int defaultMaxDiffusion = 70;
		
		Uint8 intensities[1500][900];
		
		void randomizeFood(int imageWidth, int imageHeight, int noofFood, int thresholdRadius);
		bool foodNearby(int x, int y, int radius, std::vector<std::vector<int> > otherFood);

		std::vector<int> insideObstacle(int x, int y, int radius);

		
		void stepAllPheromones();
		void stepAllFoods();
		
		void secretePheromones();
		void secretePheromones(int interval, int lifetime, int maxDiffusion);
		
		void createFood(Sint16 x_center, Sint16 y_center, Sint16 radius);
		void checkForFood();
		
		
		
	public:
		BasicPheromoneWorldObserver( World *__world );
		~BasicPheromoneWorldObserver();
				
		void reset();
		void step();
		
		Uint8 getIntensityAt(int x, int y);
		void setIntensityAt(int x, int y, Uint8 value);
		
		
};

#endif

