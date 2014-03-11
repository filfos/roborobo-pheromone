/*
    Filip Fossum 18.10.2013
*/


#ifndef PHEROMONE_H
#define PHEROMONE_H
#include <World/EnergyPoint.h>
#include <Agents/RobotAgent.h>
//#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"

#include <cstdio>

class Pheromone 
{
    public :
      Pheromone(World *__world, int __robotId);
      Pheromone(World *__world, int __robotId, int __interval, int __lifetime, int __maxDiffusion);
      Pheromone(World *__world, int __robotId, int __interval, int __lifetime, int __maxDiffusion, Uint8 __initialIntensity);
      ~Pheromone();
      void init();
      void step();   
      
      int getIntensity();
      int distanceFromCenter(int x, int y);
      bool isActive();
      void getCenter(int &x, int &y);
      void getCurrentRadius(int &radius);
      
      /* Returns reduction in intensity since previous step */
      Uint8 getEvaporated();
      int getOldRadius();
      
      bool isUpdated();

      
      
    protected :
	int _x;
	int _y;
	int _radius;
	Uint16 _stepCounter;
	
	Uint8 _initialIntensity;
	
	Uint8 _evaporated;
	int _oldRadius;
	
	const static int x_offset = 15;
	const static int y_offset = 15;
	
	bool _isUpdated;
	
	
	//Number of time steps before complete evaporation
	int _lifetime;
	//Maximum radius for the pheromone
	int _maxDiffusion;
	//Number of ignored timesteps between diffusion and evaporation
	int _interval;
	
	const static Uint8 r = 255;
	const static Uint8 g = 0;
	const static Uint8 b = 0;
	Uint8 a;
	
	void diffuse();
	void evaporate();
	
	
// 	void drawPixels(int radius);
	Uint8 getAlpha(int x, int y);
  
	int _robotId;
	World *_world;
	//BasicPheromoneWorldObserver* _wo;
	
};

#endif // PHEROMONE_H
