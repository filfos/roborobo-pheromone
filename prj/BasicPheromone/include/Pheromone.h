/*
    Filip Fossum 18.10.2013
*/


#ifndef PHEROMONE_H
#define PHEROMONE_H
#include <World/EnergyPoint.h>
#include <Agents/RobotAgent.h>

#include <cstdio>

class Pheromone 
{
    public :
      Pheromone(World *__world, int __robotId);
      Pheromone(World *__world, int __robotId, int __interval, int __lifetime, int __maxDiffusion);
      ~Pheromone();
      void init();
      void step();   
      
      int getIntensity();
      
    protected :
	int _x;
	int _y;
	int _radius;
	Uint16 _stepCounter;
	
	
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
  
	int _robotId;
	World *_world;
};

#endif // PHEROMONE_H
