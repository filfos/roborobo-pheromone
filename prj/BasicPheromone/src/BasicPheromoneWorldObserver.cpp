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
	wallUpdated = false;
	
	firstLoop = true;
	
	c1 = 0;
	c2 = 0;
	
//Increase interval to reduce diffusion
//Increase lifetime to reduce evaporation
	lifetime = 200;
	interval = 20;
		
	cellSize = 20;
	
	
	//CHANGE TO DISABLE PHEROMONES COMPLETELY
	//when True: READ spawnlocations. False: WRITE
	isUsingPheromones = true;
	shouldWriteSpawnLocations = false;
		
	cellCheckCounter = -10; //-30
	cellVisitedAverage = 0.0;
	noofTimesCellCounted = 0;
	
	
	std::string spawnNumber = generateSpawnFileNumber();
			
	evaporationFactor = exp((log(0.5)/lifetime )*interval);
	if (gBatchMode)
	{
	  std::string tiles = "output/batch/tiles"+gStartTime+".dat";
	  std::string disp = "output/batch/dispersion"+gStartTime+".dat";
	  	  
	  tilesFoundFile.open(tiles.c_str());
	  dispersionFile.open(disp.c_str());
	  
	  if (shouldWriteSpawnLocations)
	  {
	    std::string spawn = "output/spawn/"+spawnNumber+"spawn";
	    spawnLocationFile.open(spawn.c_str());
	  }
	}
	else
	{
	  dispersionFile.open("output/other/dispersion_distance.txt");
	  tilesFoundFile.open("output/other/tiles_found.txt");
	  spawnLocationFile.open("output/other/spawn_locations.txt");
	}
	
				
	intensities.resize(gScreenWidth);
	intensityBuffer.resize(gScreenWidth);
	wallMap.resize(gScreenWidth);
	if (!gBatchMode)
	{
	  movementHistory.resize(gScreenWidth);
	}
	
	int h = gScreenHeight/cellSize;
	int w = gScreenWidth/cellSize;
	
	std::cout << h << "   " << w << std::endl;
	
	visitedCells.resize(w);

	for (int i = 0; i < gScreenWidth; i++)
	{
	  intensities[i].resize(gScreenHeight);
	  intensityBuffer[i].resize(gScreenHeight);
	  wallMap[i].resize(gScreenHeight);
	  if (!gBatchMode)
	  {
	    	movementHistory[i].resize(gScreenHeight);
	  }
	}
	
	for (int i = 0; i < w ; i++)
	{
	  visitedCells[i].resize(h);
	}
	for (int x = 0; x < w; x++)
	  for (int y = 0; y < h; y++)
	    visitedCells[x][y] = false;
	  
	if (!gBatchMode)
	{
	  for (int x = 0; x < gScreenWidth; x++)
	    for (int y = 0; y < gScreenHeight; y++)
	      movementHistory[x][y] = 0;
	}
	
}

BasicPheromoneWorldObserver::~BasicPheromoneWorldObserver()
{
	// nothing to do.
}

void BasicPheromoneWorldObserver::reset()
{
	// nothing to do
}

void BasicPheromoneWorldObserver::step()
{
  WorldObserver::step();
  createWallMap();
  
  if (firstLoop)
  {
    if (gBatchMode && shouldWriteSpawnLocations)
    {
	writeAgentLocation();
    }
    else
    {
        printAgentLocations();
    }

    firstLoop = false;
  }
  
  untouchedStepCounter++;
  stepCounter++;  
  
  if (isUsingPheromones)
  {
    if (stepCounter == interval)
    {
      //-------
      diffuse();
//       evaporate();

      //-----
      drawIntensities();
      
      stepCounter = 0;
    }
  }
  
  ++cellCheckCounter;
  if (cellCheckCounter >= 0) // Have to delay execution until agents are initialized
  {
    addCurrentCells();
    writeDispersionToFile();
    writeToTilesFoundFile();
    if (!gBatchMode)
    {
      addToMovementHistory();
    }

  }
  if (!gBatchMode)
  {
    if (cellCheckCounter == 10000)//20500)//6850)
    {
      std::cout << "DONE.. " << std::endl;
      countVisitedCells();
      clearPheromoneVisuals();
      colorVisitedCells();
      displayMovementHistory();
      drawGrid();
      
//       cellCheckCounter = 0;
      gPauseMode=true;
    } 
  }
}

std::string BasicPheromoneWorldObserver::generateSpawnFileNumber()
{
  std::string directory = "/home/filip/roborobo-pheromone/output/spawn";

  DIR *dp;
  int i = -2;
  struct dirent *ep;     
  dp = opendir (directory.c_str());

  if (dp != NULL)
  {
    while ((ep = readdir (dp)) != NULL )
    {
      i++;
    }

    (void) closedir (dp);
  }
  else
  {
    perror ("Couldn't open the directory");
  }
  
//   std::cout << "Using directory: " << directory << " for spawn locations" << std::endl;
//   printf("There's %d files in the current directory.\n", i);
  
  std::stringstream ss;
  ss << i;
    
  return ss.str();
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
  
//   std::cout << "D : " <<c1++ << std::endl;
  dispersionFile << avg_distance << ",";
  
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
//   std::cout << "T : " << c2++ << std::endl;
  tilesFoundFile << noofTilesVisited << ",";
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
      
//       visitedCells[x][y] = false;
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
//      x += 15;
//      y += 15;
    _world->getAgent(i)->getCoord(x, y);
    visitedCells[x/cellSize][y/cellSize] = true;
  }
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
					intensities[x][y-1])*evaporationFactor;
	
      }
    }
  }
  intensities = intensityBuffer;
}

void BasicPheromoneWorldObserver::evaporate()
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
	intensities[x][y]*=evaporationFactor;
      }
    }
  }
}




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
  
  sum += intensities[x][y+1]*1; //N
  sum += intensities[x][y-1]*1; //S
  sum += intensities[x+1][y]*1; //E
  sum += intensities[x-1][y]*1; //W
  
  sum += intensities[x+1][y+1]*1; //NE
  sum += intensities[x+1][y-1]*1; //SE
  sum += intensities[x-1][y-1]*1; //SW
  sum += intensities[x-1][y+1]*1; //NW
    
  return sum/8;
}

void BasicPheromoneWorldObserver::activatePheromone(int x, int y, int intensity)
{
  
  intensities[x][y] = intensity;
  
//   for (int i = 1; i <= 1; i++)
//   {
//   
//   intensities[x+i][y] = intensity;
//   intensities[x-i][y] = intensity;
//   intensities[x][y+i] = intensity;
//   intensities[x][y-i] = intensity;
// 
//   intensities[x+i][y+i] = intensity;
//   intensities[x+i][y-i] = intensity;
//   intensities[x-i][y+i] = intensity;
//   intensities[x-i][y-i] = intensity;
//   }
}


void BasicPheromoneWorldObserver::printAgentLocations()
{
  for (int i = 0; i < gAgentCounter; i++)
  {
    RobotAgentWorldModel *wm = _world->getAgent(i)->getWorldModel();
    
    std::cout << "\nagent["<<i<<"].x = "<<wm->_xReal << std::endl;
    std::cout << "agent["<<i<<"].y = "<<wm->_yReal << std::endl;
    std::cout << "agent["<<i<<"].orientation = "<<wm->_agentAbsoluteOrientation<< std::endl;
  }
}

void BasicPheromoneWorldObserver::writeAgentLocation()
{
  for (int i = 0; i < gAgentCounter; i++)
  {
    RobotAgentWorldModel *wm = _world->getAgent(i)->getWorldModel();
    
    spawnLocationFile << i << ",";
    spawnLocationFile << wm->_xReal << ",";
    spawnLocationFile << wm->_yReal << ",";
    spawnLocationFile << wm->_agentAbsoluteOrientation << "\n";
  }
  spawnLocationFile.close();
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



bool BasicPheromoneWorldObserver::isPheromonesInUse()
{
  return isUsingPheromones;
}

void BasicPheromoneWorldObserver::addToMovementHistory()
{
  for (int i = 0; i < gAgentCounter; i++)
  {
    int x, y;
    _world->getAgent(i)->getCoord(x, y);
//     x += 15;
//     y += 15;
    
    movementHistory[x][y] += 1;
  }
}

void BasicPheromoneWorldObserver::drawIntensities()
{ 
  SDL_LockSurface(gBackgroundImage);
  for (Uint32 x = 1; x < intensities.size()-1; x+=1)
  {
    for (Uint32 y = 1; y < intensities[0].size()-1; y+=1)
    {   
	Uint8 intens = intensities[x][y];//*eightNeighbourMean(x, y); //getAveragedIntensity(x, y);
	Uint32 color = SDL_MapRGB(gBackgroundImage->format, 255, 255-intens, 255-intens );
	Uint8* pixel =(Uint8*) gBackgroundImage->pixels;
	
	
	pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;      
	
    }
  }
  SDL_UnlockSurface(gBackgroundImage);
}


void BasicPheromoneWorldObserver::displayMovementHistory()
{
  
  SDL_LockSurface(gBackgroundImage);
  for (Uint32 y = 0; y < movementHistory[0].size(); y++)
  {
    for (Uint32 x = 0; x < movementHistory.size(); x++)
    {
	if (movementHistory[x][y] > 0)
	{
	  Uint32 color = SDL_MapRGB(gBackgroundImage->format, 255, 0, 0);
	  Uint8* pixel =(Uint8*) gBackgroundImage->pixels;

	  pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	  *((Uint32*)pixel) = color;
	}
    }
  }
  SDL_UnlockSurface(gBackgroundImage);  
}

void BasicPheromoneWorldObserver::drawGrid()
{
  SDL_LockSurface(gBackgroundImage);
  
  for (Uint32 x = 0; x < (Uint32)gScreenWidth; x++)
  {
    for (Uint32 y = cellSize; y < (Uint32)gScreenHeight; y+=cellSize)
      {
   
//       std::cout << x << "  " << y << std::endl;
	Uint32 color = SDL_MapRGB(gBackgroundImage->format, 0, 0, 0 );
	Uint8* pixel =(Uint8*) gBackgroundImage->pixels;

	pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;
    }
  }
  
  
  for (Uint32 y = 0; y < (Uint32)gScreenHeight; y++)
  {
    for (Uint32 x = cellSize; x < (Uint32)gScreenWidth; x+=cellSize)
      {
	Uint32 color = SDL_MapRGB(gBackgroundImage->format, 0, 0, 0 );
	Uint8* pixel =(Uint8*) gBackgroundImage->pixels;

	pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;
    }
  }
  SDL_UnlockSurface(gBackgroundImage);

}
void BasicPheromoneWorldObserver::colorVisitedCells()
{
  SDL_LockSurface(gBackgroundImage);
  for (Uint32 y = 0; y < visitedCells[0].size(); y++)
  {
    for (Uint32 x = 0; x < visitedCells.size(); x++)
    {
      
      if (visitedCells[x][y])
      {
	for (Uint32 _y = y*cellSize; _y < y*cellSize+cellSize; _y++)
	{
	  for (Uint32 _x = x*cellSize; _x < x*cellSize+cellSize; _x++)
	  {
	    Uint32 color = SDL_MapRGB(gBackgroundImage->format, 180, 255, 180 );
	    Uint8* pixel =(Uint8*) gBackgroundImage->pixels;
	
	    pixel += (_y * gBackgroundImage->pitch) + (_x * sizeof(Uint32));
	    *((Uint32*)pixel) = color;
	  }
	
	}
      }
    }
  }
  SDL_UnlockSurface(gBackgroundImage);

}

void BasicPheromoneWorldObserver::clearPheromoneVisuals()
{
  SDL_LockSurface(gBackgroundImage);
  for (Uint32 x = 0; x < (Uint32)gScreenWidth; x++)
  {
    for (Uint32 y = 0; y < (Uint32)gScreenHeight; y++)
    {   
	Uint32 color = SDL_MapRGB(gBackgroundImage->format, 255, 255, 255 );
	Uint8* pixel =(Uint8*) gBackgroundImage->pixels;
	
	pixel += (y * gBackgroundImage->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = color;      
	
    }
  }
  SDL_UnlockSurface(gBackgroundImage);
}

