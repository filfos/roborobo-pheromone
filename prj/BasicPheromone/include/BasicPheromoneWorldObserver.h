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
#include "BasicPheromone/include/BasicPheromoneAgentWorldModel.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

class World;

class BasicPheromoneWorldObserver : public WorldObserver
{
	protected:		
		SDL_Surface* tempForeground;
		
		int stepCounter;
		
		//Pheromone properties
		int lifetime;
		int interval;
		
		int cellSize;
		
		double evaporationFactor;
		
		std::vector< std::vector<int> > intensities;
		std::vector< std::vector<int> > intensityBuffer;
		std::vector< std::vector<bool> > wallMap;

		
 		std::vector< std::vector<int> > movementHistory;

		std::vector< std::vector<bool> > visitedCells;
		
		int cellCheckCounter;
		double cellVisitedAverage;
		int noofTimesCellCounted;
		
		void countVisitedCells();
		
		void addCurrentCells();
  
  
		bool firstLoop;
		bool isUsingPheromones;
		
		void printAgentLocations();
				
		void drawIntensities();
		
		void createWallMap();
		
		/* -------- For Celleular Automata ------- */
		void updateIntensityMap();
		
		
		double eightNeighbourMean(int x, int y);
		double maxNeighbour(int x, int y);
		
// 		void addBufferedValues();
				
// 		bool foodPlaced;
		bool wallUpdated;
		
		Uint8 getAveragedIntensity(int x, int y);
		
// 		void updatePheromones();
		void diffuse();
		void evaporate();
		
		int getMax(int a, int b, int c, int d);
		
		
	public:
		BasicPheromoneWorldObserver( World *__world );
		~BasicPheromoneWorldObserver();
				
		void reset();
		void step();
		
		/* Return intensity at (x, y) converted to unsigned 8 bit integer */
		Uint8 getIntensityAt(int x, int y);
		
		/* Returns the unmodified intensity value at (x, y) */
		int getActualIntensityAt(int x, int y);
		
		void setIntensityAt(int x, int y, Uint8 value);
		
		void setActualIntensityAt(int x, int y, int value);
		
		/* Increases intensity at (x,y) with value. Overflow will result in 255 */
		void increaseIntensityAt(int x, int y, Uint8 value);
		/* Reduces intensity at (x,y) with value. Overflow will result in 0 */
		void reduceIntensityAt(int x, int y, Uint8 value);
		
		/* Increase or reduce the existing intensity value at (x, y) with the given value */
		void modifyIntensityAt(int x, int y, int value);
		
		void activatePheromone(int x, int y, int intensity);
		
		
		bool isPheromonesInUse();
		
		
		private:
		int untouchedStepCounter;
		std::ofstream dispersionFile;
		std::ofstream tilesFoundFile;
		std::ofstream spawnLocationFile;
		bool shouldWriteSpawnLocations;
		void writeDispersionToFile();
		void writeToTilesFoundFile();
		void writeAgentLocation();
		std::string generateSpawnFileNumber();
		
		int c1;
		int c2;
};

#endif

