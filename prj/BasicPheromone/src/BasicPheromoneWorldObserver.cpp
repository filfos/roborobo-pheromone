/*
 *  BasicPheromoneWorldObserver.cpp
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */

#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"

#include "World/World.h"


BasicPheromoneWorldObserver::BasicPheromoneWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
	stepCounter = 0;
	score = 0;
	
	
	tempForeground = IMG_Load("data/pheromone/rand1.png");
	
	
// 	Environment 1 1300x700
// 	createFood(100, 105, 20);
// 	createFood(120, 535, 20);
// 	createFood(570, 350, 20);
// 	createFood(500, 90, 20);
// 	createFood(1080, 70, 20);
// 	createFood(680, 640, 20);
// 	createFood(1130, 620, 20);
	
// 	Environment 1 1500x900
// 	createFood(120, 120, 30);
// 	createFood(620, 114, 30);
// 	createFood(1390, 94, 30);
// 	createFood(704, 452, 30);
// 	createFood(148, 678, 30);
// 	createFood(828, 834, 30);
// 	createFood(1414, 810, 30);
	randomizeFood(1500, 900, 10, 30);
	
	
	
	
	
// 	Environment 2
// 	createFood(1085, 93, 20);
// 	createFood(357, 82, 20);
// 	createFood(106, 122, 20);
// 	createFood(579, 317, 20);
// 	createFood(120, 601, 20);
// 	createFood(614, 602, 20);
// 	createFood(1040, 609, 20);
	
	
// 	Environment 3
// 	createFood(637, 87 ,20);
// 	createFood(237, 285, 20);
// 	createFood(21, 616, 20);
// 	createFood(595, 613, 20);
	
	noofFoodFound = 0;
	
	for (Uint8 i = 0; i < foodList.size(); i++)
	{
	  foundFood.push_back(0);
	  foodFoundTimestamps.push_back(0);
	  foodIdTimestamps.push_back(0);
	}
	
	noofPheromoneTimesteps = 30;
	
	//condition must be greater than noof agents.
	for (int i = 0; i < 10; i++)
	{
	  pheromoneTimer.push_back(noofPheromoneTimesteps);
	}
	
}

BasicPheromoneWorldObserver::~BasicPheromoneWorldObserver()
{
	// nothing to do.
}

void BasicPheromoneWorldObserver::reset()
{
	// nothing to do.
}

void BasicPheromoneWorldObserver::step()
{
  WorldObserver::step();
  
  if (noofFoodFound == foodList.size())
  {
    for (Uint8 i = 0; i < foodList.size(); i++)
    {
      std::cout << (int)foodFoundTimestamps.at(i) << std::endl;
    }
    std::cout << std::endl;
    std::cout << "--------------" << std::endl;
    for (Uint8 i = 0; i < foodList.size(); i++)
    {
      std::cout << i+1 << " : " << (int)foodIdTimestamps.at(i) << std::endl;
    }
    std::cout << "--------------" << std::endl;
  }
  else
  {
    score++;
  }
  
  stepCounter++;

  if (stepCounter == 10)
  {
    stepCounter = 0;
    secretePheromones();
  // secretePheromones(50, 1000, 100); //interval, total time, max radius
  }
  stepAllPheromones();
  checkForFood();
  stepAllFoods();

  
}

void BasicPheromoneWorldObserver::stepAllPheromones()
{
    int deleteNr = 0;
    if (useFoodPheromones)
    {
      for (Uint32 i = 0; i < pheromoneQueue.size(); i++)
      {
	if (pheromoneQueue.at(i)->getIntensity() == 0)
	  deleteNr++;
	else
	  pheromoneQueue.at(i)->step();
    
      }
      if (deleteNr > 0)
      {
      //TODO FIXME pheromoneQueue.erase(pheromoneQueue.begin(), pheromoneQueue.begin()+deleteNr);
      }
    }
    
    if (useDefaultPheromones)
    {
      for (Uint32 i = 0; i < pheromoneDefaultQueue.size(); i++)
      {
	if (pheromoneDefaultQueue.at(i)->getIntensity() == 0)
	{
	  //do nothing
	}
	else
	  pheromoneDefaultQueue.at(i)->step();
      }
    }
    
    
    
}

/*
void BasicPheromoneWorldObserver::stepAllPheromones()
{
    std::deque<Pheromone*>::iterator ph;
    for (ph = pheromoneQueue.begin(); ph != pheromoneQueue.end(); )
    {
      if ((*ph)->getIntensity() == 0)
      {
	std::cout << "queue size " << pheromoneQueue.size() << std::endl;
// 	if (pheromoneQueue.size() == 1)
// 	{
// 	  pheromoneQueue.clear();
// 	  break;
// 	}
	pheromoneQueue.erase(ph);
      }
      else
      {
	std::cout << "step" << std::endl;
	(*ph)->step();
	++ph;
      }
    }
    
//     std::vector<int> deleteIndices;
//     for (int i = 0; i < pheromoneQueue; i++)
//     {
//       if (pheromoneQueue.at(i)->getIntensity() == 0)
// 	deleteIndices.push_back();
//       else
// 	pheromoneQueue.at(i)->step();
//     }

}
*/

/*
void BasicPheromoneWorldObserver::secretePheromones()
{
  for (int i = 0; i != gAgentCounter; i++)
  {					// Interval, lifetime, maxradius	
    Pheromone *p = new Pheromone(_world, i, 25, 300, 50);
    pheromoneQueue.push_back(p);
  }
}
*/
void BasicPheromoneWorldObserver::secretePheromones(int interval, int lifetime, int maxDiffusion)
{
  for (int i = 0; i != gAgentCounter; i++)
  {
    if (_world->getAgent(i)->getWorldModel()->getRobotLED_blueValue() == 255)
    {
      if (pheromoneTimer.at(i) > 0)
      {
	Pheromone *p = new Pheromone(_world, i, interval, lifetime, maxDiffusion);
	pheromoneQueue.push_back(p);
	
	pheromoneTimer.at(i)--;
      }
      else
      {
	pheromoneTimer.at(i) = noofPheromoneTimesteps;
	_world->getAgent(i)->getWorldModel()->setRobotLED_colorValues(0,0,0);
      }
    }
    
  }
  
}


void BasicPheromoneWorldObserver::secretePheromones()
{
  for (int i = 0; i != gAgentCounter; i++)
  {
    if (_world->getAgent(i)->getWorldModel()->getRobotLED_blueValue() == 255 && useFoodPheromones)
    {
      if (pheromoneTimer.at(i) > 0)
      {
	Pheromone *p = new Pheromone(_world, i, foodInterval, foodLifetime, foodMaxDiffusion);
	pheromoneQueue.push_back(p);
	
	pheromoneTimer.at(i)--;
      }
      else
      {
	pheromoneTimer.at(i) = noofPheromoneTimesteps;
	_world->getAgent(i)->getWorldModel()->setRobotLED_colorValues(0,0,0);
      }
    }
    if (useDefaultPheromones)
    {
      if (pheromoneTimer.at(i) > 0)
      {
	Pheromone *p = new Pheromone(_world, i, defaultInterval, defaultLifetime, defaultMaxDiffusion);
	pheromoneDefaultQueue.push_back(p);
	// Potentially create a new queue for default pheromones.
	
	pheromoneTimer.at(i)--;
      }
      else
      {
	//TODO is this the "time" between timesteps? separate for default pheromones?
	pheromoneTimer.at(i) = noofPheromoneTimesteps;
      }
    }
  }
  
}


void BasicPheromoneWorldObserver::createFood(Sint16 x_center, Sint16 y_center, Sint16 radius)
{
  
  Food *f = new Food(x_center, y_center, radius);
  foodList.push_back(f);  
  
}


void BasicPheromoneWorldObserver::checkForFood()
{
  for (int i = 0; i != gAgentCounter; i++)
  {
    if(_world->getAgent(i)->getWorldModel()->getRobotLED_blueValue() == 255)
    {
      int x, y;
      _world->getAgent(i)->getCoord(x, y);
      for (Uint8 j = 0; j < foodList.size(); j++)
      {
	int id = foodList.at(j)->getId(x, y);
	//std::cout << "id: " << id << std::endl;
	if (id != 0 && foundFood.at(id-1) == 0)
	{
	  foodList.at(id-1)->hide();
	  foundFood.at(id-1) = 1;
	  noofFoodFound++;
	 // std::cout << noofFoodFound << std::endl;
	  foodFoundTimestamps.at(noofFoodFound-1) = score;
	  foodIdTimestamps.at(id-1) = score;
	}
      }
    }
  }
}

void BasicPheromoneWorldObserver::stepAllFoods()
{
  for (Uint32 i = 0; i != foodList.size(); i++)
  {
    if (foundFood.at(i) == 0)
    {
      Food *f = foodList.at(i);
      f -> step();
    }
  }
}




void BasicPheromoneWorldObserver::randomizeFood(int imageWidth, int imageHeight, int noofFood, int thresholdRadius)
{
  int count = 0;
  std::vector<std::vector<int> > coords(2, std::vector<int>(noofFood));  
  
  srand(time(0));
  
  for (int i = 0; i != noofFood; i++)
  {
    int x = (rand() % imageWidth) + 1;
    int y = (rand() % imageHeight) + 1;
    
//     std::cout << " x " << x << std::endl;
//     std::cout << " y " << y << std::endl;
    
    if ( !foodNearby(x, y, thresholdRadius, coords))
    {
      
      bool obstacleOverlap = true;
      while(obstacleOverlap)
      {
	std::vector<int> loc(2);
	loc = insideObstacle(x, y, 20);
	x += loc[0];
	y += loc[1];
	
	obstacleOverlap = loc[0] != 0 && loc[1] != 0;
	
      }
       coords[0].push_back(x);
       coords[1].push_back(y);
       createFood(x, y, 20);      
       std::cout << "x : " << x << "  y  " << y << std::endl;
    }
    else
    {
      ++count;
      --i;
      std::cout << "Food number " << i << " retrying" << std::endl;
    }
    
    if (count > 100)
    {
      std::cout << "ABANDON SHIP" << std::endl;
      return;
    }

  }
  
  std::cout << " ------- FOOD LOCATIONS ------" << std::endl;
  for (Uint8 i = 0; i < coords[0].size(); i++)
    std::cout << " x:  " << coords[0][i] << "    y:  " << coords[1][i] << std::endl;
}

bool BasicPheromoneWorldObserver::foodNearby(int x, int y, int radius, std::vector<std::vector<int> > otherFood)
{
  for (Uint8 i = 0; i != otherFood.size(); i++)
  {
    if (pow((double)(x - otherFood[0][i]) , 2.0) + pow((double)(y - otherFood[1][i]) , 2.0)  <= pow((double)radius, 2.0))
      return true;
  }
  return false;
}

std::vector<int> BasicPheromoneWorldObserver::insideObstacle(int x, int y, int radius)
{
  std::vector<int> black(8);
  Uint8 r, g, b;
  
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x, y-i ); 
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    if (r == 255 && b == 255 && g == 255)
      black[0] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x+i, y-i );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[1] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x+i, y );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[2] += 1;      
    else
      break;
  } 
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x+i, y+i );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[3] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x, y+i );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[4] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x-i, y+i );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[5] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x-i, y );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[6] += 1;      
    else
      break;
  } 
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(tempForeground, x-i, y-i );
    SDL_GetRGB(pixel, tempForeground->format, &r, &g, &b);
    
    if (r == 255 && b == 255 && g == 255)
      black[7] += 1;      
    else
      break;
  }
  
  
  std::vector<int> temp(2);
  temp[0] = 0;
  temp[1] = 0;
  for (Uint8 i = 0; i < black.size(); i++)
  {
    if (black[i] > temp[1])
    {
      temp[0] = i;
      temp[1] = black[i];
    }
  }
  
  
  int _x = 0;
  int _y = 0;
  
  switch (temp[0])
  {
    case 0:
//       _x = 0;
      _y -= temp[1];
      break;
    case 1:
      _x += temp[1];
      _y -=temp[1];
      break;
    case 2:
      _x += temp[1]; 
//       _y = 0;
    break;
    case 3:
      _x += temp[1];
      _y += temp[1];
      break;
    case 4:
//       _x = 0;
      _y += temp[1];
      break;
    case 5:
      _x -= temp[1];
      _y -= temp[1];
      break;
    case 6:
      _x -= temp[1];
//       _y = 0;
      break;
    case 7:
      _x -= temp[1];
      _y += temp[1];
      break;
  }
  
  
//   std::cout << "x " << _x << "  y " << _y << std::endl;
  std::vector<int> newLoc(2);
  newLoc[0] = _x;
  newLoc[1] = _y;  
      
  return newLoc;    
}





