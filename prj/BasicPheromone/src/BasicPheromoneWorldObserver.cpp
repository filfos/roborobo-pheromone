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
	wallUpdated = false;
	foodPlaced = false;
		
	
	evaporationFactor = exp((log(0.5)/lifetime )*interval);
	
	std::cout << "kabooom" << std::endl;
	
// 	pheromoneMap[1][1] = 2;
	
// 	int pheromoneMap[1500][900] = {{0}};
// 	int pheromoneBuffer[1500][900] = {{0}};
	
	
	intensities.resize(gScreenWidth);
	intensityBuffer.resize(gScreenWidth);
	wallMap.resize(gScreenWidth);

	for (int i = 0; i < gScreenWidth; i++)
	{
	  intensities[i].resize(gScreenHeight);
	  intensityBuffer[i].resize(gScreenHeight);
	  wallMap[i].resize(gScreenHeight);
	}
	
// 	for (int x = 0; x < gScreenWidth; x++)
// 	{
// 	  for (int y = 0; y < gScreenHeight; y++)
// 	  {
// 	    Particle *p = new Particle();
// 	    particleMap[x][y] = p; 
// 	    
// 	  }
// 	}

	
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

// 	randomizeFood(gScreenWidth, gScreenHeight, 10, 30);
	
	
	
	
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
  createWallMap();
  randomizeFood(gScreenWidth, gScreenHeight, 10, 30);
  
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
  

//   if (stepCounter % 5 == 0)
//     updateIntensityMap();
//   
//   if (stepCounter == 20)
//   {
//     stepCounter = 0;    
//     drawIntensities();
//   }
  if (stepCounter == 10)
  {
//     updatePheromones();
    diffuse();
    evaporate();
    drawIntensities();
    stepCounter = 0;
  }
  
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
    if (_world->getAgent(i)->getWorldModel()->getRobotLED_greenValue() == 255 && useDefaultPheromones)
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


void BasicPheromoneWorldObserver::createWallMap()
{
  if (!wallUpdated)
  {
    wallUpdated = true;
    for (int y = 0; y < gScreenHeight; y++)
	{
// 	  std::cout << std::endl;
	  for (int x = 0; x < gScreenWidth; x++)
	  {
	    Uint8 r, g, b;	  
	    Uint32 pixel = getPixel32(gForegroundImage, x, y );
	    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
	    
	    if (x == 2 || y == 2 || x == (gScreenWidth-2) || y == (gScreenHeight-2))
	    {
	      wallMap[x][y] = 1;
	      continue;
	    }
	    
	    if (r == 0 && b == 0 && g == 0)
	    { 
	      wallMap[x][y] = true;
	    }
	    else
	    {
	      wallMap[x][y] = false;
	    }
	  }
	}
  }
  
}

void BasicPheromoneWorldObserver::checkForFood()
{
  for (int i = 0; i != gAgentCounter; i++)
  {
    if(_world->getAgent(i)->getWorldModel()->getRobotLED_blueValue() == 255)
    {
      int x, y;
      _world->getAgent(i)->getCoord(x, y);
      //correct error in Roborobo's coordinates:
      x += 15;
      y += 15;
      for (Uint8 j = 0; j < foodList.size(); j++)
      {
	int id = foodList.at(j)->getId(x, y);
	
	if (id != 0 && foundFood.at(id-1) == 0)
	{
	  foodList.at(id-1)->hide();
	  foundFood.at(id-1) = 1;
	  noofFoodFound++;
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


void BasicPheromoneWorldObserver::drawIntensities()
{ 
  SDL_LockSurface(gBackgroundImage);
  for (Uint32 x = 1; x < intensities.size()-1; x+=1)
  {
    for (Uint32 y = 1; y < intensities[0].size()-1; y+=1)
    {   
// 	intensities[x][y] = intensityBuffer[x][y];
// 	intensityBuffer[x][y] = 0;
// 	intensities[x][y] = intensities[x][y]*0.95;
	Uint8 intens = eightNeighbourMean(x, y); //getAveragedIntensity(x, y);
	Uint32 color = SDL_MapRGB(gBackgroundImage->format, 255, 255-intens, 255-intens );
	Uint8* pixel =(Uint8*) gBackgroundImage->pixels;
	
	
	pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;      
    }
  }
  SDL_UnlockSurface(gBackgroundImage);
}

void BasicPheromoneWorldObserver::updatePheromones()
{
  for (int y = 1; y < gScreenHeight-1; y++)
  {
    for (int x = 1; x < gScreenWidth-1; x++)
    {
      if (intensities[x][y] == 0 || wallMap[x][y] == 1)
      {
	continue;
      }
      else
      {
// 	evaporationFactor = 0.999;
      if(intensities[x][y] > 255) intensities[x][y] = 255;
      int intens = intensities[x][y]*0.126;
      intensityBuffer[x+1][y] += intens;
//       intensityBuffer[x+1][y] /= 2;
      intensityBuffer[x][y+1] += intens;
//       intensityBuffer[x][y+1] /= 2;
      intensityBuffer[x-1][y] += intens;
//       intensityBuffer[x-1][y] /= 2;
      intensityBuffer[x][y-1] += intens;
      
      intensityBuffer[x+1][y+1] += intens;
            intensityBuffer[x+1][y-1] += intens;
      intensityBuffer[x-1][y+1] += intens;
      intensityBuffer[x-1][y-1] += intens;

//       intensityBuffer[x][y-1] /= 2;
//       intensityBuffer[x][y] += intens;
//       intensityBuffer[x][y] /= 2;
      }
    }
  }
}

void BasicPheromoneWorldObserver::diffuse()
{
  for (int y = 1; y < gScreenHeight-1; y++)
  {
    for (int x = 1; x < gScreenWidth-1; x++)
    {
      if (wallMap[x][y] == 1)
      {
	continue;
      }
      else
      {
	intensityBuffer[x][y] = getMax(intensities[x+1][y], 
					intensities[x-1][y], 
					intensities[x][y+1], 
					intensities[x][y-1] );
	
      }
    }
  }
  intensities = intensityBuffer;
}

void BasicPheromoneWorldObserver::evaporate()
{
  for (int y = 0; y < gScreenHeight; y++)
  {
    for (int x = 0; x < gScreenWidth; x++)
    {
      intensities[x][y] = intensities[x][y]*0.95;
    }
  }
}

	/*
	 * Wednesday: Spawn a finite pool of e.g. 2000. Diffusion is done 
	 * by spreading portions of the pool. if a value above 255 is
	 * seen; draw 255, but retain the original value.
	 * 
	 *
	int sum = 0;
	sum += factor*intensities[x+1][y];
	sum += factor*intensities[x-1][y];
	sum += factor*intensities[x][y+1];
	sum += factor*intensities[x][y-1];

	sum += factor*intensities[x+1][y+1];
	sum += factor*intensities[x-1][y-1];
	sum += factor*intensities[x+1][y-1];
	sum += factor*intensities[x-1][y+1];
	
	intensityBuffer[x][y] += sum/8;
      }
    }
  }
}
        */
int BasicPheromoneWorldObserver::getMax(int a, int b, int c, int d)
{
  int max = a > b ? a : b;
  max = max > c ? max : c;
  max = max > d ? max : d;
    
  return max;
}

double BasicPheromoneWorldObserver::eightNeighbourMean(int x, int y)
{  
  double sum = 0;
  
//   sum += intensities[x][y]*;
  
  sum += intensities[x][y+1]*4; //N
  sum += intensities[x][y-1]*4; //S
  sum += intensities[x+1][y]*4; //E
  sum += intensities[x-1][y]*4; //W
  
  sum += intensities[x+1][y+1]*1; //NE
  sum += intensities[x+1][y-1]*1; //SE
  sum += intensities[x-1][y-1]*1; //SW
  sum += intensities[x-1][y+1]*1; //NW
    
  return sum/20;
}

void BasicPheromoneWorldObserver::activatePheromone(int x, int y, int intensity)
{
  intensities[x][y] = intensity;
  
  for (int i = 1; i <= 1; i++)
  {
  
  intensities[x+i][y] = intensity;
  intensities[x-i][y] = intensity;
  intensities[x][y+i] = intensity;
  intensities[x][y-i] = intensity;

  intensities[x+i][y+i] = intensity;
  intensities[x+i][y-i] = intensity;
  intensities[x-i][y+i] = intensity;
  intensities[x-i][y-i] = intensity;
  }
}

void BasicPheromoneWorldObserver::randomizeFood(int imageWidth, int imageHeight, int noofFood, int thresholdRadius)
{
  if (!foodPlaced)
  {
      foodPlaced = true;
      int count = 0;
      std::vector<std::vector<int> > coords(2, std::vector<int>(noofFood));  
      
      srand(time(0));
      
      for (int i = 0; i != noofFood; i++)
      {  
	int x = (rand() % imageWidth) + 1;
	int y = (rand() % imageHeight) + 1;
	
	if ( !foodNearby(x, y, thresholdRadius, coords))
	{

	  bool obstacleOverlap = true;
	  while(obstacleOverlap)
	  {
	    std::vector<int> loc(2);
	    loc = insideObstacle(x, y, 30);
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
      
      noofFoodFound = 0;
  } 
  if (foodPlaced)
  {
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
    Uint32 pixel = getPixel32(gForegroundImage, x, y-i ); 
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    if (r == 0 && b == 0 && g == 0)
      black[0] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x+i, y-i );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
      black[1] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x+i, y );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
      black[2] += 1;      
    else
      break;
  } 
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x+i, y+i );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
      black[3] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x, y+i );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
      black[4] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x-i, y+i );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
      black[5] += 1;      
    else
      break;
  }
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x-i, y );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
      black[6] += 1;      
    else
      break;
  } 
  
  for (int i = 0; i < radius; i++)
  {
    Uint32 pixel = getPixel32(gForegroundImage, x-i, y-i );
    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
    
    if (r == 0 && b == 0 && g == 0)
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
  
  
  std::vector<int> newLoc(2);
  newLoc[0] = _x;
  newLoc[1] = _y;  
      
  return newLoc;    
}

Uint8 BasicPheromoneWorldObserver::getIntensityAt(int x, int y)
{
  if (intensities[x][y] > 255)
    return 255;
  else if (intensities[x][y] < 0)
    return 0;
    
  return intensities[x][y];
}

int BasicPheromoneWorldObserver::getActualIntensityAt(int x, int y)
{
  return intensities[x][y];
}

void BasicPheromoneWorldObserver::setIntensityAt(int x, int y, Uint8 value)
{
  if (intensities[x][y] > 255)
    intensities[x][y] = 255;
  else if (intensities[x][y] < 0)
    intensities[x][y] = 0;
  else  
    intensities[x][y] = value;  
}

void BasicPheromoneWorldObserver::setActualIntensityAt(int x, int y, int value)
{
  intensities[x][y] = value;
}

void BasicPheromoneWorldObserver::increaseIntensityAt(int x, int y, Uint8 value)
{
  int val = (int)intensities[x][y] + value;
  if (val > 255)
    val = 255;
  
  intensities[x][y] = val;

}
void BasicPheromoneWorldObserver::reduceIntensityAt(int x, int y, Uint8 value)
{
  int val = (int)intensities[x][y] - value;
  if (val < 0)
    val = 0;
  
  intensities[x][y] = val;
}

void BasicPheromoneWorldObserver::modifyIntensityAt(int x, int y, int value)
{
  intensities[x][y] += value;
}

/*
void BasicPheromoneWorldObserver::updateIntensityMap()
{
  if (!wallUpdated)
  {
    wallUpdated = true;
    for (int y = 0; y < gScreenHeight; y++)
	{
// 	  std::cout << std::endl;
	  for (int x = 0; x < gScreenWidth; x++)
	  {
	    Uint8 r, g, b;	  
	    Uint32 pixel = getPixel32(gForegroundImage, x, y );
	    SDL_GetRGB(pixel, gForegroundImage->format, &r, &g, &b);
	    
	    if (r == 0 && b == 0 && g == 0)
	    { 
	      wallMap[x][y] = true;
	    }
	    else
	    {
	      wallMap[x][y] = false;
	    }
	  }
	}
  }
  
  for (int x = 2; x < gScreenWidth-2; x++)
  {
    for (int y = 2; y < gScreenHeight-2; y++)
    {
      if (wallMap[x][y] == true)
      {
	intensityBuffer[x][y] = 0; //continue;
      }
            
     double mean = eightNeighbourMean(x,y);
      
      intensityBuffer[x][y] = mean;
    
      
    }
  }
  addBufferedValues();
}
	*/

void BasicPheromoneWorldObserver::updateIntensityMap()
{
   
//   /* Translation Phase */
//   for (int x = 1; x < gScreenWidth-1; x++)
//   {
//     for (int y = 1; y < gScreenHeight-1; y++)
//     {
//       Particle *p = particleMap[x][y];
//       int intens = p->getIntensity();
//       intensities[x][y] = intens;
//       intens = eightNeighbourMean(x, y);
//      
//       /* TRANSLATION AND WALL REFLECTION */
//       if (p->e)
//       {
// 	p->e = 0;
// 	if (wallMap[x+1][y])
// 	  p->w = 1;
// 	else
// 	{
// 	  particleMap[x+1][y]->eNext = 1;
// 	  particleMap[x+1][y]->setIntensity(intens);
// 	}
// 	//---- Diagonal
// 	if (p->s)
// 	{
// 	  if (!wallMap[x+1][y+1])
// 	  {
// 	    particleMap[x+1][y+1]->eNext;
// 	    particleMap[x+1][y+1]->sNext;
// 	    particleMap[x+1][y+1]->setIntensity(intens);
// 	  }
// 	}
// 	if (p->n)
// 	{
// 	  if (!wallMap[x+1][y-1])
// 	  {
// 	    particleMap[x+1][y-1]->eNext;
// 	    particleMap[x+1][y-1]->sNext;
// 	    particleMap[x+1][y-1]->setIntensity(intens);
// 	  }
// 	}
// 	
// 	
//       //--- 
//       }
//       
//       if (p->w)
//       {
// 	p->w = 0;
// 	if (wallMap[x-1][y])
// 	  p->e = 1;
// 	else
// 	{
// 	  particleMap[x-1][y]->wNext = 1;
// 	  particleMap[x-1][y]->setIntensity(intens);
// 	}
// 	
// 	//---- Diagonal
// 	if (p->s)
// 	{
// 	  if (!wallMap[x-1][y+1])
// 	  {
// 	    particleMap[x-1][y+1]->eNext;
// 	    particleMap[x-1][y+1]->sNext;
// 	    particleMap[x-1][y+1]->setIntensity(intens);
// 	  }
// 	}
// 	if (p->n)
// 	{
// 	  if (!wallMap[x-1][y-1])
// 	  {
// 	    particleMap[x-1][y-1]->eNext;
// 	    particleMap[x-1][y-1]->sNext;
// 	    particleMap[x-1][y-1]->setIntensity(intens);
// 	  }
// 	}
// 	
// 	  //---
//       }   
//       
//       if (p->n)
//       {
// 	p->n = 0;
// 	if (wallMap[x][y+1])
// 	  p->s = 1;
// 	else
// 	{
// 	  particleMap[x][y+1]->nNext = 1;
// 	  particleMap[x][y+1]->setIntensity(intens);
// 	}
//       }   
//       
//       if (p->s)
//       {
// 	p->s = 0;
// 	if (wallMap[x][y-1])
// 	  p->n = 1;
// 	else
// 	{
// 	  particleMap[x][y-1]->sNext = 1;
// 	  particleMap[x][y-1]->setIntensity(intens);
// 	}
//       }
//       
// //       p->evaporate();
// //       p->swap();
// 
//     }
//   }
//   /* COLLISION */
//   for (int x = 1; x < gScreenWidth-1; x++)
//   {
//     for (int y = 1; y < gScreenHeight-1; y++)
//     {
//       Particle *p = particleMap[x][y];
//       
//       if (p->eNext && p->wNext && (!p->nNext && !p->sNext))
//       {
// 	p->nNext = 1;
// 	p->sNext = 1;
// // 	p->setIntensity(particleMap[x+1][y]->getIntensity() + particleMap[x-1][y]->getIntensity());
//       }
//       
//       if (p->nNext && p->sNext && (!p->eNext && !p->wNext))
//       {
// 	p->eNext = 1;
// 	p->wNext = 1;
// // 	p->setIntensity(particleMap[x][y+1]->getIntensity() + particleMap[x][y-1]->getIntensity());
// 
//       }
//       p->evaporate();
//       p->swap();
// //       intensities[x][y] = eightNeighbourMean(x, y);
//       
//     }
//   }  
}



Uint8 BasicPheromoneWorldObserver::getAveragedIntensity(int x, int y)
{
  double sum = 0;
  
  sum += getIntensityAt(x, y);
  sum += getIntensityAt(x+1, y);
  sum += getIntensityAt(x-1, y);
  sum += getIntensityAt(x, y+1);
  sum += getIntensityAt(x, y-1);
  
  sum += getIntensityAt(x+1, y+1);
  sum += getIntensityAt(x-1, y-1);
  sum +=getIntensityAt(x+1, y-1);
  sum += getIntensityAt(x-1, y+1);
  
  sum /= 9;
  
  return (Uint8)sum;
  
}



double BasicPheromoneWorldObserver::maxNeighbour(int x, int y)
{
  int count = 0;
  double ret = 0;
  for (int i = x-1; i < x+2; i++)
  {
    for (int j = y-1; j < y+2; j++)
    {
      if (i == x && j == y)
	continue;
      
      if (intensities[i][j] == 0)
	continue;
      
      count++;
      ret += intensities[i][j];
      
    }
  }
  
  return ret / count;
//   double max = 0;
//   
//   max =  intensities[x][y+1] > max ? intensities[x][y+1] : max; //N
//   max = intensities[x][y-1] > max ? intensities[x][y-1] : max; //S
//   max = intensities[x+1][y] > max ? intensities[x+1][y] : max; //E
//   max = intensities[x-1][y] > max ? intensities[x-1][y] : max; //W
//   
//   return max;
}


void BasicPheromoneWorldObserver::addBufferedValues()
{
  for (int x = 0; x < gScreenWidth; x++)
  {
    for (int y = 0; y < gScreenHeight; y++)
    {
//       intensities[x][y] = particleBuffer[x][y]->getIntensity();
//       particleBuffer[x][y]->reset();
    }
  }
}


/*
void BasicPheromoneWorldObserver::addBufferedValues()
{
  for (int x = 0; x < gScreenWidth; x++)
  {
    for (int y = 0; y < gScreenHeight; y++)
    {
      if (intensityBuffer[x][y] != 0)
      {
// 	mean = mean *  exp((log(0.5)/50 )*1000);
	intensities[x][y] = intensityBuffer[x][y]; // exp(log(intensityBuffer[x][y]));
// 	double mean = eightNeighbourMean(x,y);
// 	intensities[x][y] = mean;
	
	intensityBuffer[x][y] = 0;
      }
    }
  }
}*/
