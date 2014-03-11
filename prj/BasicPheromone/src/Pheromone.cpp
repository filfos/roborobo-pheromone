/*
    Filip Fossum 18.10.2013
*/


#include "BasicPheromone/include/Pheromone.h"
#include "World/World.h"

Pheromone::Pheromone(World *__world, int __robotId)
{
  _robotId = __robotId;
  _world = __world;
  //_wo = _world->getWorldObserver();
  
  Pheromone::init();
}

Pheromone::Pheromone(World *__world, int __robotId, int __interval, int __lifetime, int __maxDiffusion)
{
  _robotId = __robotId;
  _world = __world;
  _interval = __interval;
  _lifetime = __lifetime;
  _maxDiffusion = __maxDiffusion;
  _initialIntensity = 255;
  _evaporated = 0;
  
  Pheromone::init();
}

Pheromone::Pheromone(World *__world, int __robotId, int __interval, int __lifetime, int __maxDiffusion, Uint8 __initialIntensity)
{
  _robotId = __robotId;
  _world = __world;
  _interval = __interval;
  _lifetime = __lifetime;
  _maxDiffusion = __maxDiffusion;
  _initialIntensity = __initialIntensity;
  _evaporated = 0;
  Pheromone::init();
}


Pheromone::~Pheromone()
{
  // Do nothing
}

void Pheromone::init()
{
  _stepCounter = -50;
  a = _initialIntensity; //FIXME Only seems to work when this is 255
  _radius = 0;
  _isUpdated = false;
  RobotAgent *robot = _world->getAgent(_robotId);  
  robot -> getCoord(_x, _y);
  _x += x_offset;
  _y += y_offset;
  filledCircleRGBA(gBackgroundImage, _x, _y, _radius, r, g, b, a);
  
 
}

void Pheromone::diffuse()
{
  if (_stepCounter == _interval)
  {
    _stepCounter = 0;
    _isUpdated = true;    
    evaporate();
  }
  else
  {
    _isUpdated = false;
  }
  
}

void Pheromone::evaporate()
{ 
   _evaporated = a; //was +=
 
   double norm_a = (double)a / 255.0;
   norm_a = norm_a *  exp((log(0.5)/_lifetime )*_interval);
   a = norm_a*255;
   
   _evaporated -= a;
   _oldRadius = _radius;
   
   double radiusFactor = 1 - ((double)a / (double)_initialIntensity);
   //1-norm_a
   _radius = ceil(radiusFactor*_maxDiffusion);
  if (a == 0)
  {
    a=0;
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

bool Pheromone::isActive()
{
  return a != 0 ? true : false;
}

void Pheromone::getCenter(int &x, int &y)
{
  x = _x;
  y = _y;
}

void Pheromone::getCurrentRadius(int &radius)
{
  radius = _radius;
}

// NB: Squared answer.
int Pheromone::distanceFromCenter(int x, int y)
{
  int dx = x - _x;
  int dy = y - _y;
  
  dx *= dx;
  dy *= dy;
  
  return dx + dy;
}

Uint8 Pheromone::getAlpha(int x, int y)
{
    Uint8 r, g, b;
    Uint32 pixel = getPixel32(gBackgroundImage, x, y ); 
    SDL_GetRGB(pixel, gBackgroundImage->format, &r, &g, &b);
    
    //Simulate alpha channel
    return 255-g;
}

Uint8 Pheromone::getEvaporated()
{
  return _evaporated;
}

int Pheromone::getOldRadius()
{
  return _oldRadius;
}

bool Pheromone::isUpdated()
{
  return _isUpdated;
}

/*
void Pheromone::drawPixels(int radius)
{
  int sqrdRadius = radius*radius;
  
  for (int x = _x-radius; x < _x+radius || x >= gScreenWidth; x++)
  {
    if (x < 0) continue;
    for (int y = _y-radius; y < _y+radius || y >= gScreenHeight; y++)
    {
      if (y < 0) continue;
      
      if (distanceFromCenter(x, y) < sqrdRadius)
      {
	Uint8 alpha = getAlpha(x, y);
	alpha = alpha + a > 255 ? 255 : alpha + a;
	//TEST If overflow
	pixelRGBA(gBackgroundImage, x, y, r, g, b, a);
      }
    }
  }
  
}*/

//TODO Revert
/*
void Pheromomone::drawCircle()
{
  int x0;
  int y0;
  
  int radius;
  int maxRadius;
  
  int x = radius;
  int y = 0;
  
  int radiusError =  1-x;
  
  while (x >= y)
  {
    DrawPixel(x + x0, y + y0);
    DrawPixel(y + x0, x + y0);
    DrawPixel(-x + x0, y + y0);
    DrawPixel(-y + x0, x + y0);
    DrawPixel(-x + x0, -y + y0);
    DrawPixel(-y + x0, -x + y0);
    DrawPixel(x + x0, -y + y0);
    (y + x0, -x + y0);
    
    y++;
    
    if (radiusError < 0)
      radiusError += 2*y + 1;
    else
    {
      x--;
      radiusError += 2*(y - x + 1);
    }
  } 
}
*/

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
