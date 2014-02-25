/*
 *  BasicPheromoneControlArchitecture.cpp
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */

#include "BasicPheromone/include/BasicPheromoneControlArchitecture.h"
//#include "World/World.h"
#include "BasicPheromone/include/Food.h"

#include "Observers/WorldObserver.h"
#include "World/World.h"
#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include <math.h>


BasicPheromoneControlArchitecture::BasicPheromoneControlArchitecture( RobotAgentWorldModel *__wm ) : BehaviorControlArchitecture ( __wm )
{
    _wm = (BasicPheromoneAgentWorldModel*)__wm;

    _wm->testFoo();
    _pSensor = new BasicPheromoneSensor(__wm);
    stepCounter = 0;
    stagnationRotation = 0;

    isRotating = false;
    isReversing = false;
    doneReversing = false;
    
    red = 0;
    green = 0;
    blue = 0;
    
    ignorePheromones = true;
    releasePheromones = true;
    
    mStatus = DEFAULT;
      
    goalDeg = 0;
    reverseCounter = 0;

}

BasicPheromoneControlArchitecture::~BasicPheromoneControlArchitecture()
{
	// nothing to do.
}

void BasicPheromoneControlArchitecture::reset()
{
	// nothing to do.
}

void BasicPheromoneControlArchitecture::step()
{
	_pSensor->update();
	
	blue = _pSensor->foodFound() == 1 ? 255 : 0;
	
	green = releasePheromones ? 255 : 0;
	
	_wm->setRobotLED_colorValues(red, green, blue);
	
	
	mStatus = determineBehaviour();
	
	if (mStatus == WALL_STAGNATION)
	  wallStagnation();
	
	else if (mStatus == PHEROMONE_STAGNATION)
	  pheromoneStagnation();
	
	else if (mStatus == PHEROMONE_AVOIDANCE)
	  pheromoneReaction();
	
	else if (mStatus == DEFAULT)
	  if (!ignorePheromones)
	     wallAndPheromoneAvoidance();
	  else 
	    wallAvoidance();
	else
	  std::cout << "This should never print " << std::endl;
	
// 	std::cout << _wm->_sensors[0][0] << std::endl;
// 	std::cout << _wm->_sensors[0][1] << std::endl;
// 	std::cout << _wm->_sensors[0][2] << std::endl;
// 	std::cout << _wm->_sensors[0][3] << std::endl;
// 	std::cout << _wm->_sensors[0][4] << std::endl;
// 	std::cout << _wm->_sensors[0][5] << std::endl;
// 	std::cout << _wm->_sensors[0][6] << std::endl;
// 	std::cout << "---------------------" << std::endl;
	
	// SHOW ON-SCREEN LIGHT SENSOR LOCATION
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[0][2], _pSensor->_sensor[0][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[1][2], _pSensor->_sensor[1][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[2][2], _pSensor->_sensor[2][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[3][2], _pSensor->_sensor[3][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[4][2], _pSensor->_sensor[4][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[5][2], _pSensor->_sensor[5][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[6][2], _pSensor->_sensor[6][3], 2, 0, 255, 255, 255);
// 	filledCircleRGBA(gBackgroundImage, _pSensor->_sensor[7][2], _pSensor->_sensor[7][3], 2, 0, 255, 255, 255);

    // PRINT DISTANCE SENSORS
//     std::cout << "         " << _wm->_sensors[2][5] << std::endl;
//     std::cout << "   " << _wm->_sensors[1][5] <<"          " << _wm->_sensors[3][5] << std::endl;
//     std::cout << "" << _wm->_sensors[0][5] <<"                " << _wm->_sensors[4][5] << std::endl;
//     std::cout << "   " << _wm->_sensors[7][5] <<"          " << _wm->_sensors[5][5] << std::endl;
//     std::cout << "         " << _wm->_sensors[6][5] << std::endl;
//     std::cout << "-----------------------------" << std::endl;
      
}


void BasicPheromoneControlArchitecture::pheromoneReaction()
{
  if (!isRotating)
  {
    std::cout << "FRONT: " << _pSensor->_sensor[FRONT][LIGHT] << std::endl;
    
    Uint8 bestDirection = 10;
    int val = -1;
    
    for (Uint8 i = 0; i < 8; i++)
    {
      int temp = 0;
      temp += _pSensor->_sensor[(i+7)%8][LIGHT];
      temp += _pSensor->_sensor[i][LIGHT];
      temp += _pSensor->_sensor[(i+9)%8][LIGHT];
      
      if (temp > val)
      {
	val = temp;
	bestDirection = i;
      }
    }
    stagnationRotation = getAngleOfLightSensor(bestDirection);
  }
  stopAndRotate(stagnationRotation);
}


//For 8-sensor CHiRP-robot
void BasicPheromoneControlArchitecture::wallAvoidance()
{
  releasePheromones = true;
  //Slow down if wall is detected straight ahead
  _wm->_desiredTranslationalValue =  + 2 - 2*(( (double)gSensorRange - (_wm->_sensors[N][5] ) ) / (double)gSensorRange);
  
  if (_wm->_sensors[N][5] < (double)gSensorRange*(2/3) && _wm->_sensors[NE][5] == (double)gSensorRange &&  _wm->_sensors[NW][5] == (double)gSensorRange)
    	_wm->_desiredRotationalVelocity = 0.4 - (double)(rand()%10)/10.*0.2;

  
	// Compare distance to walls on both sides
	double rot = 1;
	if ( _wm->_sensors[W][5] + _wm->_sensors[NW][5] <  _wm->_sensors[NE][5] + _wm->_sensors[E][5] )
	{
	//robot body radius = 16, thus right triangle created by sensors have hypotenuse 80
	// and adjacent side: 80*cos(45) = 40*sqrt(2). To weigh them equally the ratio 80/40*sqrt(2) = 1.41
	//is used reduce the importance of side sensors (right/east and left/west).
		rot += (( (double)gSensorRange - _wm->_sensors[NW][5] ) / (double)gSensorRange);
		rot += (( (double)gSensorRange - (_wm->_sensors[W][5]))/ (double)gSensorRange);
		std::cout << "WEST "<< rot <<std::endl;
		_wm->_desiredRotationalVelocity = rot;
	}	
	else
	{
		if ( _wm->_sensors[NE][5] + _wm->_sensors[E][5] < 2*gSensorRange )
		{
		  rot += (( (double)gSensorRange - _wm->_sensors[NE][5]) / (double)gSensorRange);
		  rot += (( (double)gSensorRange - (_wm->_sensors[E][5]))/ (double)gSensorRange);
		  std::cout << "EAST " << rot << std::endl;
		  _wm->_desiredRotationalVelocity = -rot;
		}
		else
		{
			// Straighten out
			if ( _wm->_desiredRotationalVelocity > 0 )
			{
				_wm->_desiredRotationalVelocity -= 0.5;
			}
			else
			{
				if ( _wm->_desiredRotationalVelocity < 0) 
				{
					_wm->_desiredRotationalVelocity += 0.5;
				}
				else
				{
					// Add small random rotation
					_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
				}
			}
		}
	}			
}

void BasicPheromoneControlArchitecture::wallAndPheromoneAvoidance()
{
	releasePheromones = true;

	//Slow down if wall is detected straight ahead
// 	if (_pSensor->_sensor[FRONT][LIGHT] > 0)
// 	{
// 		_wm->_desiredTranslationalValue = + 2 - 2*( 1.0 - (_pSensor->_sensor[FRONT][LIGHT]) ) ;
// 	}
// 	else
// 	{
	_wm->_desiredTranslationalValue =  + 2 - 2*(( (double)gSensorRange - (_wm->_sensors[N][5] ) ) / (double)gSensorRange);
// 	}
	  
	// Compare distance to walls on both sides
	if ( _wm->_sensors[W][5] + _wm->_sensors[NW][5] <  _wm->_sensors[NE][5] + _wm->_sensors[E][5] ) 
		_wm->_desiredRotationalVelocity = +2;
		  
	else if (_pSensor->_sensor[LEFT][LIGHT] + _pSensor->_sensor[FRONT_LEFT][LIGHT]  >
		  _pSensor->_sensor[RIGHT][LIGHT] + _pSensor->_sensor[FRONT_RIGHT][LIGHT]
	)
	{
		_wm->_desiredRotationalVelocity = +2;
		std::cout << "DAFUK" << std::endl;
	}
	else
		if ( _wm->_sensors[NE][5] + _wm->_sensors[E][5] < 2*gSensorRange ) 
			_wm->_desiredRotationalVelocity = -2;
		else if (_pSensor->_sensor[RIGHT][LIGHT] + _pSensor->_sensor[FRONT_RIGHT][LIGHT] + _pSensor->_sensor[BACK_RIGHT][LIGHT] < 0)
			_wm->_desiredRotationalVelocity = -2;
		else
			if ( _wm->_desiredRotationalVelocity > 0 ) 
				_wm->_desiredRotationalVelocity -= 0.5;
			else
				if ( _wm->_desiredRotationalVelocity < 0) 
					_wm->_desiredRotationalVelocity += 0.5;
				else
					_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
}


void BasicPheromoneControlArchitecture::foodReaction(int x, int y)
{  
 // std::cout << "Found food at  x:" << x << "   y: " << y <<  std::endl;
 // _wm->setRobotLED_colorValues(0, 0, 255);
}



void BasicPheromoneControlArchitecture::wallStagnation()
{
  releasePheromones = false;
  if (!doneReversing)
  {
      reverse(40);
  }
  else
  {
    if (!isRotating)
    {
      isReversing = false;
      std::cout << "CONTINUING " << std::endl;
      Uint8 bestDirection = 10;
      int val = -1;
      for (Uint8 i = 0; i < 8; i++)
      {
	int temp = 0;
	temp += _wm->_sensors[(i+7)%8][5];
	temp += _wm->_sensors[i][5];
	temp += _wm->_sensors[(i+9)%8][5];
	
	std::cout << "temp "<< (int)i << "  " << temp << std::endl;
	
	
	if (temp > val)
	{
	  val = temp;
	  bestDirection = i;
	}
      }
    
      stagnationRotation = getAngleOfDistanceSensor(bestDirection);
    }
    
    stopAndRotate(stagnationRotation);
  }
}


void BasicPheromoneControlArchitecture::pheromoneStagnation()
{
  //some code already in pheromoneReaction().
    //should inhibit more abrubt turns. Maybe even stop completely.
    
    //IDEA
    /*
     * similar check to wall stagnation (possibly higher threshold)
     * 	if little movement and there is alot of pheromones present;
     * 		ignore pheromones for X time steps.
     * 
     * check location at time 0. them location at time 100.
     * draw circle from first location. if the cartesian
     * coordinates from the second is within the circle, then
     * ignore pheromones for a short duration.
     * 
     * Ways to continuously increase/decrease the size of the circle.
     */
}


void BasicPheromoneControlArchitecture::reverse(int duration)
{  
    _wm->_desiredRotationalVelocity = 0;
    _wm->_desiredTranslationalValue =  - 2 + 2*(( (double)gSensorRange - (_wm->_sensors[S][5] ) ) / (double)gSensorRange);
    
 
  
	// Compare distance to walls on both sides
	if ( _wm->_sensors[W][5] + _wm->_sensors[SW][5] <  _wm->_sensors[SE][5] + _wm->_sensors[E][5] ) 
		_wm->_desiredRotationalVelocity = +2;
	else
		if ( _wm->_sensors[SE][5] + _wm->_sensors[E][5] < 2*gSensorRange ) 
			_wm->_desiredRotationalVelocity = -2;
		else
			if ( _wm->_desiredRotationalVelocity > 0 ) 
				_wm->_desiredRotationalVelocity -= 0.5;
			else
				if ( _wm->_desiredRotationalVelocity < 0) 
					_wm->_desiredRotationalVelocity += 0.5;
				else
					_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
	
	   if (reverseCounter == duration)
    {
      reverseCounter = 0;
      doneReversing = true;
      _wm->_desiredTranslationalValue = 0;
      return;
    } 
    doneReversing = false;
    isReversing = true;
    reverseCounter++;
	
}


/* Input in range <-180, 180>
 Returns true when done*/
void BasicPheromoneControlArchitecture::stopAndRotate(int deg)
{
  if (!isRotating)
  {
    goalDeg = _wm->_agentAbsoluteOrientation + deg;
    
    if (goalDeg > 180)
      goalDeg -= 360;
    if (goalDeg < -180)
      goalDeg += 360;
    isRotating = true;
  }
  
  _wm->_desiredTranslationalValue = 0;
  if (deg > 0)
    _wm->_desiredRotationalVelocity = 3; //2max ?
  else if (deg < 0)
    _wm->_desiredRotationalVelocity = -3;
  
 // std::cout <<"input: "<< deg << "    actual orientation " << _wm->_agentAbsoluteOrientation << "  /  " << goalDeg << std::endl;
  
  if ( (goalDeg+360) <= (_wm->_agentAbsoluteOrientation+362)
    && (goalDeg+360) >= (_wm->_agentAbsoluteOrientation+358)
  )
  {
    _wm->_desiredRotationalVelocity = 0;
    isRotating = false;
    doneReversing = false;
  }
}

int BasicPheromoneControlArchitecture::getAngleOfLightSensor(int sensorId)
{  
  switch(sensorId)
  {
    case FRONT:
      return 0;
    case FRONT_RIGHT:
      return 45;
    case RIGHT:
      return 90;
    case BACK_RIGHT:
      return 135;
    case BACK:
      return 180;
    case BACK_LEFT:
      return -135;
    case LEFT:
      return -90;
    case FRONT_LEFT:
      return -45; 
    default:
      return -1;
  }
}

int BasicPheromoneControlArchitecture::getAngleOfDistanceSensor(int sensorId)
{  
  switch(sensorId)
  {
    case N:
      return 0;
    case NE:
      return 45;
    case E:
      return 90;
    case SE:
      return 135;
    case S:
      return 180;
    case SW:
      return -135;
    case W:
      return -90;
    case NW:
      return -45; 
    default:
      return -1;
  }
}


int BasicPheromoneControlArchitecture::determineBehaviour()
{
  if (isRotating || isReversing) // Continue previous action
{
//   std::cout << "	Status: continue same action"  << std::endl;
    return mStatus;
} 
  if (checkWallStagnation())
  {
//     std::cout << "	status: wall stagnation" << std::endl;
    return WALL_STAGNATION;
  }
  
  if (checkPheromoneStagnation())
  {
//     std::cout << "	status: pheromone stagnation" << std::endl;
    return PHEROMONE_STAGNATION;
  }
  
  if (checkPheromoneDetected())
  {
//     std::cout << "	status: pheromone avoidance" << std::endl;
    return PHEROMONE_AVOIDANCE;
  }
  
//   std::cout << "	status: default" << std::endl;
  return DEFAULT;
}

bool BasicPheromoneControlArchitecture::checkWallStagnation()
{
  lastLocations.push_back(new Coordinate(_wm->_xReal, _wm->_yReal));
  ++stepCounter;
  
  if (stepCounter == 100)
  {
    // calculate standard deviation of cartesian coordinates
    double meanX = 0.0;
    double meanY = 0.0;
    
    double stdX = 0.0;
    double stdY = 0.0;
    
    for (Uint16 i = 0; i < stepCounter; i++)
    {
      meanX += lastLocations.at(i)->x();
      meanY += lastLocations.at(i)->y();
    }
    meanX /= stepCounter;
    meanY /= stepCounter;
    
    for (Uint16 i = 0; i < stepCounter; i++)
    {
      double x = lastLocations.at(i)->x();
      stdX += (x-meanX)*(x-meanX);
      
      double y = lastLocations.at(i)->y();
      stdY += (y-meanY)*(y-meanY);
    }
    
    stdX /= stepCounter;
    stdY /= stepCounter;
    
    stdX = sqrt(stdX);
    stdY = sqrt(stdY);
    
    std::cout << "Wall Stagnation limit 20:    Value:   "    << stdX + stdY << std::endl;
    lastLocations.clear();
    stepCounter = 0;
    if (stdX + stdY < 20)
    {
      std::cout << "|||||||||| STAGNATION |||||||||" << std::endl;
      return true;
    }
  }
  return false;
}
bool BasicPheromoneControlArchitecture::checkPheromoneStagnation()
{
  return false;
}
bool BasicPheromoneControlArchitecture::checkPheromoneDetected()
{
  double mean = 0;
  for (int i = 0; i < 8; i++)
    mean += _pSensor->_sensor[i][LIGHT];
  
  mean /= 8;
  
  if (mean > 0)
  {
    return false; //TODO Change to true;
  }
 
  return false;
}

double BasicPheromoneControlArchitecture::getWallAttackAngle()
{
/*  
   if (_wm->_sensors[N][5] < (double)gSensorRange && _wm->_sensors[NE][5] < (double)gSensorRange)
  {
    double b = _wm->_sensors[NE][5] + 16; //add radius of robot body
    double c = _wm->_sensors[N][5] + 16;
    
    double a = sqrt( b*b + c*c - 2*b*c*cosine(45) );
    
    double bAngle = arcsine( ( b*sine(45) / a ) );
    
    std::cout << "attack angle RIGHT  "<< bAngle << std::endl;
  }
  
  else if (_wm->_sensors[N][5] < (double)gSensorRange && _wm->_sensors[NW][5] < (double)gSensorRange)
  {
    double b = _wm->_sensors[NW][5] + 16;
    double c = _wm->_sensors[N][5] + 16;
    
    double a = sqrt( b*b + c*c - 2*b*c*cosine(45) );
    
    double cAngle = arcsine( ( c*sine(45) / a ) );
    
    std::cout << "attack angle LEFT   "<< cAngle << std::endl;
  }
  */
 /* 
  if (_wm->_sensors[NE][5] < (double)gSensorRange && _wm->_sensors[E][5] < (double)gSensorRange)
  {
    double b = _wm->_sensors[NE][5];
    double c = _wm->_sensors[E][5];
    
    double a = sqrt( b*b + c*c - 2*b*c*cosine(45) );
    
    double bAngle = arcsine( ( b*sine(45) / a ) ) - 45;
    
    std::cout << "attack angle "<< bAngle << std::endl;
  }
  
  else if (_wm->_sensors[NW][5] < (double)gSensorRange && _wm->_sensors[W][5] < (double)gSensorRange)
  {
    double b = _wm->_sensors[NW][5];
    double c = _wm->_sensors[W][5];
    
    double a = sqrt( b*b + c*c - 2*b*c*cosine(45) );
    
    double bAngle = arcsine( ( b*sine(45) / a ) );
    
    std::cout << "attack angle "<< bAngle << std::endl;
  }
  */
  return -1;
}

double BasicPheromoneControlArchitecture::sine(double theta)
{
  
  return sin(theta*pi/180);
}

double BasicPheromoneControlArchitecture::cosine(double theta)
{
  return cos(theta*pi/180);
  
}
 
double BasicPheromoneControlArchitecture::arcsine(double ratio)
{
  return asin(ratio)*180/pi;
}
 
double BasicPheromoneControlArchitecture::maxLim (double a, double lim)
{
  if (a > lim) 
    return lim;
  
  return a;
}
 
 