/*
    Filip Fossum 18.10.2013
*/


#include "BasicPheromone/include/Food.h"
#include "World/World.h"



int Food::_foodCount = 1;

Food::Food(Sint16 x_center, Sint16 y_center, Sint16 radius)
{  
    _isInitialized = false;
    _isActive = false;
    
    _x = x_center;
    _y = y_center;
    _radius = radius;

    _id = getFoodCount();
}

Food::~Food()
{
    //nothing to do
}

void Food::step()
{
  if (!_isInitialized )
  {
    _isActive = true;
    _isInitialized = true;
    filledCircleColor(gZoneImage, _x, _y, _radius*2, green);
  }
  if (_isActive)
  {
    filledCircleColor(gBackgroundImage, _x, _y, _radius, green);
  }
}

void Food::hide()
{
    filledCircleColor(gBackgroundImage, _x, _y, _radius, white);
    filledCircleColor(gZoneImage, _x, _y, _radius*2, white);
    _isActive = false;
}

int Food::getId(int x, int y)
{ //add 3000 for safety. Won't work if food sources are very close to eachother.
  if ( ((x - _x)*(x - _x) + (y - _y)*(y - _y)) <= _radius*_radius + 3000 )
  {
    return _id;
  }
  return 0;
  
}

