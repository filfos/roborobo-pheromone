/*
 *  BasicPheromoneWorldObserver.cpp
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */

#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"

#include "World/World.h"

// #include <iostream>
// #include <fstream>

BasicPheromoneWorldObserver::BasicPheromoneWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
	untouchedStepCounter = 0;
	stepCounter = 0;
	score = 0;
	wallUpdated = false;
	foodPlaced = false;
	isDone = false;
		
	cellCheckCounter = -30;
	cellVisitedAverage = 0.0;
	noofTimesCellCounted = 0;
	
	evaporationFactor = exp((log(0.5)/lifetime )*interval);
	
	dispersionFile.open("dispersion_distance.txt");
	tilesFoundFile.open("tiles_found.txt");
			
	intensities.resize(gScreenWidth);
	intensityBuffer.resize(gScreenWidth);
	wallMap.resize(gScreenWidth);
	movementHistory.resize(gScreenWidth);
	
	int h = gScreenHeight/cellSize;
	int w = gScreenWidth/cellSize;
	
	std::cout << h << "   " << w << std::endl;
	
	visitedCells.resize(w);
// 	particleMap.resize(gScreenWidth);

	for (int i = 0; i < gScreenWidth; i++)
	{
	  intensities[i].resize(gScreenHeight);
	  intensityBuffer[i].resize(gScreenHeight);
	  wallMap[i].resize(gScreenHeight);
	  movementHistory[i].resize(gScreenHeight);
// 	  particleMap[i].resize(gScreenHeight);
	}
	
	for (int i = 0; i < w ; i++)
	{
	  visitedCells[i].resize(h);
	}
	for (int x = 0; x < w; x++)
	  for (int y = 0; y < h; y++)
	    visitedCells[x][y] = false;
	  
	for (int x = 0; x < gScreenWidth; x++)
	  for (int y = 0; y < gScreenHeight; y++)
	    movementHistory[x][y] = 0;
	
// 	for (int x = 0; x < gScreenWidth; x++)
// 	{
// 	  for (int y = 0; y < gScreenHeight; y++)
// 	  {
// 	    Particle *p = new Particle();
// 	    particleMap[x][y] = p; 
// 	    
// 	  }
// 	}

/* RAND 1  OLD ????*/	
//  createFood( 1365, 845,20);
//  createFood( 375, 170, 20 );
//  createFood( 1298, 507, 20 );
//  createFood( 1403, 181, 20);
//  createFood( 169, 636, 20 );
//  createFood( 786, 94, 20 );
//  createFood( 1386, 291, 20 );
//  createFood( 1098, 679, 20);
//  createFood( 1119, 285, 20 );
//  createFood( 308, 284, 20 );


/* RAND 1 */
//  createFood( 376, 170,20);
//  createFood( 308, 286, 20 );
//  createFood( 170, 636, 20 );
//  createFood( 786, 96, 20);
//  createFood( 1121, 289, 20 );
//  createFood( 1098, 677, 20 );
//  createFood( 1405, 180, 20 );
//  createFood( 1389, 294, 20);
//  createFood( 1300, 508, 20 );
//  createFood( 1368, 846, 20 );

/* RAND 2 */
//  createFood( 1311, 461, 20 );
//  createFood( 1473, 709, 20 );
//  createFood( 1426, 70, 20 );
//  createFood( 466, 728, 20 );
//  createFood( 235, 852, 20 );
//  createFood( 489, 80, 20 );
//  createFood( 1323, 238, 20 );
//  createFood( 430, 273, 20 );
//  createFood( 930, 288, 20 );
//  createFood( 87, 707, 20 );

/* RAND 3 */
//  createFood( 785, 514, 20 );
//  createFood( 526, 798, 20 );
//  createFood( 1431, 235, 20 );
//  createFood( 259, 575, 20 );
//  createFood( 71, 849, 20 );
//  createFood( 326, 307, 20);
//  createFood( 808, 170, 20 );
//  createFood( 1221, 737, 20 );
//  createFood( 652, 715, 20 );
//  createFood( 1074, 139, 20 );

/* RAND 4 */
//  createFood( 1431, 474, 20 );
//  createFood( 1227, 604, 20 );
//  createFood( 1209, 289, 20 );
//  createFood( 1268, 674, 20 );
//  createFood( 734, 704, 20 );
//  createFood( 155, 264, 20 );
//  createFood( 1000, 734, 20 );
//  createFood( 139, 48, 20 );
//  createFood( 1045, 314, 20 );
//  createFood( 912, 102, 20 );

/* RAND 5 */	
//  createFood( 218, 536, 20 );
//  createFood( 693, 866, 20 );
//  createFood( 824, 263, 20 );
//  createFood( 32, 379, 20 );
//  createFood( 1251, 390, 20 );
//  createFood( 309, 680, 20 );
//  createFood( 668, 472, 20 );
//  createFood( 1306, 849, 20 );
//  createFood( 1149, 156, 20 );
//  createFood( 984, 627, 20 );

/* RAND 6 */
//  createFood( 540, 257, 20 );
//  createFood( 882, 571, 20 );
//  createFood( 1284, 361, 20 );
//  createFood( 448, 769, 20 );
//  createFood( 980, 317, 20 );
//  createFood( 1240, 854, 20 );
//  createFood( 1369, 26, 20 );
//  createFood( 52, 154, 20 );
//  createFood( 167, 637, 20 );
//  createFood( 143, 850, 20 );

	
 
}

BasicPheromoneWorldObserver::~BasicPheromoneWorldObserver()
{
	// nothing to do.
}

void BasicPheromoneWorldObserver::reset()
{
	// nothing to do.p
}

void BasicPheromoneWorldObserver::step()
{
  WorldObserver::step();
  createWallMap();
  randomizeFood(gScreenWidth, gScreenHeight, 0, 30);
  
  untouchedStepCounter++;
//   std::cout << untouchedStepCounter << std::endl;
  if (noofFoodFound == foodList.size() && !isDone)
  {
    isDone = true;
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
//     countVisitedCells();
//     displayMovementHistory(6000);
  

    
//     pause();
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
  if (stepCounter == interval)
  {
//     updatePheromones();
//     updateIntensityMap();

    //-------
    diffuse();
    
    
    
    
    
    
    
    
//     evaporate();

    //-----
    drawIntensities();
    
    stepCounter = 0;
  }
  
  ++cellCheckCounter;
  if (cellCheckCounter >= 0) // Have to delay execution until agents are initialized
  {
    addCurrentCells();
    addToMovementHistory();
    writeDispersionToFile();
    writeToTilesFoundFile();

  }
  if (cellCheckCounter == 6415)
  {
    countVisitedCells();
    displayMovementHistory(6000);
    cellCheckCounter = 0;
  }
  checkForFood();
  stepAllFoods();
  
}

void BasicPheromoneWorldObserver::addToMovementHistory()
{
  for (int i = 0; i < gAgentCounter; i++)
  {
    int x, y;
    _world->getAgent(i)->getCoord(x, y);
    x += 15;
    y += 15;
    
    movementHistory[x][y] += 1;    
  }
}
		
void BasicPheromoneWorldObserver::displayMovementHistory(int ms)
{
  
//   SDL_Surface *movement;
//   movement = SDL_CreateRGBSurfaceFrom(gEnvironmentImage->pixels, 1500, 900, fmt.BitsPerPixel, gEnvironmentImage->pitch, fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
  SDL_LockSurface(gBackgroundImage);
  for (Uint32 y = 0; y < movementHistory[0].size(); y++)
  {
    for (Uint32 x = 0; x < movementHistory.size(); x++)
    {
	int val = movementHistory[x][y];
	Uint8 intens = val > 0 ? 255 : 0;//val > 255 ? 255 : val;
	Uint32 color = SDL_MapRGB(gBackgroundImage->format, 255, 255-intens, 255-intens );
	Uint8* pixel =(Uint8*) gBackgroundImage->pixels;
	
	
	pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;      
    }
  }
  SDL_UnlockSurface(gBackgroundImage);  
  gPauseMode = true;
}

void BasicPheromoneWorldObserver::writeDispersionToFile()
{
  int x_mean = 0;
  int y_mean = 0;
  int x, y;
  
  /* Calculate new center point */
  for (int i = 0; i < gAgentCounter; i++)
  {
    _world->getAgent(i)->getCoord(x, y);
    x_mean += x;
    y_mean += y;
  }
  x_mean /= gAgentCounter;
  y_mean /= gAgentCounter;
  
  
  double x_avg = 0;
  double y_avg = 0;
  double avg_distance = 0;
  /* Calculate average distance from the center point */
  for (int i = 0; i < gAgentCounter; i++)
  {
    _world->getAgent(i)->getCoord(x, y);
    x_avg = pow(x - x_mean, 2.0);
    y_avg = pow(y - y_mean, 2.0);
    
    avg_distance += sqrt(x_avg + y_avg);
  }
  avg_distance /= gAgentCounter;
  
  dispersionFile << avg_distance << "\n";
  
}

void BasicPheromoneWorldObserver::writeToTilesFoundFile()
{
  int noofTilesVisited = 0;
  for (int x = 0; x < gScreenWidth/cellSize; x++)
  {
    for (int y = 0; y < gScreenHeight/cellSize; y++)
    {
      if (visitedCells[x][y])
	++noofTilesVisited;
    }
  }
  
  tilesFoundFile << noofTilesVisited << "\n";
}

void BasicPheromoneWorldObserver::countVisitedCells()
{
  int noofCells = 0;
  int noofVisitedCells = 0;
  
  for (int x = 0; x < gScreenWidth/cellSize; x++)
  {
    for (int y = 0; y < gScreenHeight/cellSize; y++)
    {
      ++noofCells;
      if (visitedCells[x][y])
	++noofVisitedCells;
      
      visitedCells[x][y] = false;
    }
  }
  ++noofTimesCellCounted;
  cellVisitedAverage += (double)noofVisitedCells/(double)noofCells;
  std::cout << "     Covered Cells / total cells = " << noofVisitedCells << " / " << noofCells << " = " << (double)noofVisitedCells/(double)noofCells << std::endl;
  std::cout << "     AVG Covered / Times Counted = " << cellVisitedAverage << " / " << noofTimesCellCounted << " = " << (double)cellVisitedAverage/(double)noofTimesCellCounted << std::endl;
  std::cout << "-----\n" << "fraction of visited Cell / total timesteps " << cellVisitedAverage << " / " << untouchedStepCounter << " =  " << (double)cellVisitedAverage / (double)untouchedStepCounter << std::endl;
  dispersionFile.close();
  tilesFoundFile.close();

}

void BasicPheromoneWorldObserver::addCurrentCells()
{
  for (int i = 0; i < gAgentCounter; i++)
  {
    int x, y;
     x += 15;
     y += 15;
    _world->getAgent(i)->getCoord(x, y);
    visitedCells[x/cellSize][y/cellSize] = true;
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
      intensityBuffer[x][y+1] += intens;
      intensityBuffer[x-1][y] += intens;
      intensityBuffer[x][y-1] += intens;
      
      intensityBuffer[x+1][y+1] += intens;
      intensityBuffer[x+1][y-1] += intens;
      intensityBuffer[x-1][y+1] += intens;
      intensityBuffer[x-1][y-1] += intens;

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
					intensities[x][y-1] )*evaporationFactor; //0.97
	
      }
    }
  }
  intensities = intensityBuffer;
}

void BasicPheromoneWorldObserver::evaporate()
{
//   for (int y = 0; y < gScreenHeight; y++)
//   {
//     for (int x = 0; x < gScreenWidth; x++)
//     {
//       intensities[x][y] = intensities[x][y]*0.95;
//     }
//   }
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

/* LATTICE GAS */
//   Particle *p = particleMap[x][y];
//   p->setIntensity(intensity);
//   p->e = 1;
//   p->w = 1;
//   p->n = 1;
//   p->s = 1;
}


void BasicPheromoneWorldObserver::printAgentLocations()
{
  for (int i = 0; i < gAgentCounter; i++)
  {
    RobotAgentWorldModel *wm = _world->getAgent(i)->getWorldModel();
    
    std::cout << "\nagent["<<i<<"].x = "<<wm->_xReal << std::endl;
    std::cout << "agent["<<i<<"].y = "<<wm->_yReal << std::endl;
    std::cout << "agent["<<i<<"].orientation = "<<wm->_agentAbsoluteOrientation +180<< std::endl;
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
// 	  std::cout << "x : " << x << "  y  " << y << std::endl;
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
	std::cout << " createFood( " << coords[0][i] << ", " << coords[1][i] << ", 20 );" << std::endl;
      
      noofFoodFound = 0;
      printAgentLocations();
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
// 	p->setIntensity(particleMap[x+1][y]->getIntensity() + particleMap[x-1][y]->getIntensity());
//       }
//       
//       if (p->nNext && p->sNext && (!p->eNext && !p->wNext))
//       {
// 	p->eNext = 1;
// 	p->wNext = 1;
// 	p->setIntensity(particleMap[x][y+1]->getIntensity() + particleMap[x][y-1]->getIntensity());
// 
//       }
// //       p->evaporate();
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
