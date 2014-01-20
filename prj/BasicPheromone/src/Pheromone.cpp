/*
    Filip Fossum 18.10.2013
*/


#include "BasicPheromone/include/Pheromone.h"
#include "World/World.h"

Pheromone::Pheromone(World *__world, int __robotId)
{
  _robotId = __robotId;
  _world = __world;
  
  Pheromone::init();
}

Pheromone::Pheromone(World *__world, int __robotId, int __interval, int __lifetime, int __maxDiffusion)
{
  _robotId = __robotId;
  _world = __world;
  _interval = __interval;
  _lifetime = __lifetime;
  _maxDiffusion = __maxDiffusion;
  
  Pheromone::init();
}

Pheromone::~Pheromone()
{
  // Do nothing
}

void Pheromone::init()
{
  _stepCounter = -20;
  a = 255;
  _radius = 0;
  RobotAgent *robot = _world->getAgent(_robotId);  
  robot -> getCoord(_x, _y);
  filledCircleRGBA(gBackgroundImage, _x, _y, _radius, r, g, b, a);
}

void Pheromone::diffuse()
{
   // std::cout << _stepCounter << std::endl;
  if (_stepCounter == _interval)
  {
  //  SDL_FillRect(gBackgroundImage, NULL, 0xffffffff);
    _stepCounter = 0;
    
    evaporate();
    
    //_radius+=3;
    
    
    
    filledCircleRGBA(gBackgroundImage, _x, _y, _radius, 255, 255, 255, 255);
    filledCircleRGBA(gBackgroundImage, _x, _y, _radius, r, g, b, a);
  }
  
}

void Pheromone::evaporate()
{
 // std::cout << "alpha " << a << std::endl;
 
 
   double norm_a = (double)a / 255.0d;
   norm_a = norm_a *  exp((log(0.5)/_lifetime )*_interval);
   a = norm_a*255;
  //a -=15;
   _radius = ceil((1-norm_a)*_maxDiffusion);
  if (a == 0)
  {
    a=0;
   // filledCircleRGBA(gBackgroundImage, _x, _y, _radius, 255, 255, 255, 255);

  }
}


void Pheromone::step()
{
  _stepCounter++;
  diffuse();
}

int Pheromone::getIntensity()
{
  return a;
}



  /*
  radius++;
  //std::cout << radius << std::endl;
  
  if (radius < 100)
  {
    //TOP AND BOTTOM
    int y1 = sourceCoord.y - radius;
    int y2 = sourceCoord.y + radius;
    for(int x = sourceCoord.x - radius; x <= sourceCoord.x + radius; x++)
    {
      pixelColor(gBackgroundImage, x, y1, color);
      pixelColor(gBackgroundImage, x, y2, color);
    }
    
    //LEFT AND RIGHT
    int x1 = sourceCoord.x + radius;
    int x2 = sourceCoord.x + radius;
    for(int y = sourceCoord.y - radius + 1; y <= sourceCoord.y + radius - 1; y++)
    {
      pixelColor(gBackgroundImage, x1, y, color);
      pixelColor(gBackgroundImage, x2, y, color);
    }
  }
  
}
*/
