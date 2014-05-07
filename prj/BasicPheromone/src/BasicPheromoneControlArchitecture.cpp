/*
 *  BasicPheromoneControlArchitecture.cpp
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */

#include "BasicPheromone/include/BasicPheromoneControlArchitecture.h"
#include "Observers/WorldObserver.h"
#include "World/World.h"
#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"
#include "WorldModels/RobotAgentWorldModel.h"

#include <math.h>


BasicPheromoneControlArchitecture::BasicPheromoneControlArchitecture( RobotAgentWorldModel *__wm ) : BehaviorControlArchitecture ( __wm )
{
    _wm = (BasicPheromoneAgentWorldModel*)__wm;
    BasicPheromoneWorldObserver* wo = (BasicPheromoneWorldObserver*)_wm->getWorld()->getWorldObserver();
    
    isUsingPheromones = wo->isPheromonesInUse();
    
//     std::cout << wo->isPheromonesInUse() << " 0=wall avoidance, 1=pheromone "  << std::endl;
    
    _pSensor = new BasicPheromoneSensor(__wm);
    stepCounter = 0;
    pheromoneCounter = 0;
    stagnationRotation = 0;
    pheromoneIgnoreCounter = 50;

    isRotating = false;
    isReversing = false;
    doneReversing = false;
    isPheromoneStagnated = false;
    
    red = 0;
    green = 0;
    blue = 0;
    
    ignorePheromones = false;
    releasePheromones = true;
    
    mStatus = DEFAULT;
      
    goalDeg = 0;
    reverseCounter = 0;
    
    stepsInPoo = 0;
    
    pLeftCounter = 0;
    pRightCounter = 0;
    
    pLeftIgnoreCounter = 0;
    pRightIgnoreCounter = 0;
    
    pStagnationLeft = false;
    pStagnationRight = false;

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
	
	if (isUsingPheromones)
	{
	  green = releasePheromones ? 255 : 0;
	}
	else
	{
	  green = 0;
	}
	
	_wm->setRobotLED_colorValues(red, green, blue);
	  
	
	mStatus = determineBehaviour();
	
	if (mStatus == WALL_STAGNATION)
	  wallStagnation();
	
// 	else if (mStatus == PHEROMONE_STAGNATION)
// 	  pheromoneStagnation();
	
	else if (mStatus == DEFAULT)
// 	  if (!ignorePheromones)
	     wallAndPheromoneAvoidance();
// 	  else 
// 	    wallAvoidance();
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



//For 8-sensor CHiRP-robot
void BasicPheromoneControlArchitecture::wallAvoidance()
{
  releasePheromones = true;
  
//   std::cout << "ONLY WALL" << std::endl;
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
	//is used reduce the importance of side sensors (right/east and left/west)... or maybe not after all
		rot += (( (double)gSensorRange - _wm->_sensors[NW][5] ) / (double)gSensorRange);
		rot += (( (double)gSensorRange - (_wm->_sensors[W][5]))/ (double)gSensorRange);
// 		std::cout << "WEST "<< rot <<std::endl;
		_wm->_desiredRotationalVelocity = rot;
	}	
	else
	{
		if ( _wm->_sensors[NE][5] + _wm->_sensors[E][5] < 2*gSensorRange )
		{
		  rot += (( (double)gSensorRange - _wm->_sensors[NE][5]) / (double)gSensorRange);
		  rot += (( (double)gSensorRange - (_wm->_sensors[E][5]))/ (double)gSensorRange);
// 		  std::cout << "EAST " << rot << std::endl;
		  _wm->_desiredRotationalVelocity = -rot;
		}
		else
		{
			_wm->_desiredRotationalVelocity += .1;
			// Straighten out
			if ( _wm->_desiredRotationalVelocity > 0 )
			{
				_wm->_desiredRotationalVelocity -= 0.25;
			}
			else
			{
				if ( _wm->_desiredRotationalVelocity < 0) 
				{
					_wm->_desiredRotationalVelocity += 0.25;
				}
				else
				{
					// Add small random rotation
					_wm->_desiredRotationalVelocity = 0;//0.01 - (double)(rand()%10)/10.*0.02;
				}
			}
		}
	}			
}

void BasicPheromoneControlArchitecture::wallAndPheromoneAvoidance()
{
	releasePheromones = true;
// 	std::cout << " WALL AND PHEROMONE" << std::endl;
	int leftMax = _pSensor->getIndexOfMaxLeftLightSensor();	
	int rightMax = _pSensor->getIndexOfMaxRightLightSensor();
	
	double leftMaxPhero = _pSensor->getLeftLightMax();
	double rightMaxPhero = _pSensor->getRightLightMax();
	
	double maxSpeed = (double)gMaxTranslationalSpeed;
	double maxRot = (double)gMaxRotationalSpeed;
	double rot = maxRot/2;
	
	
// 	double pLeftRot = -(((_pSensor->_sensor[RIGHT][LIGHT] + _pSensor->_sensor[FRONT_RIGHT][LIGHT])/2) *maxRot);
// 	double pRightRot = +(((_pSensor->_sensor[LEFT][LIGHT] + _pSensor->_sensor[FRONT_LEFT][LIGHT])/2)*maxRot);
// 	double pRot = pLeftRot + pRightRot;
	
// 	std::cout << pLeftRot << "   " << pRightRot << std::endl;
	
	bool isNearWall = false;
	for (int i = 0; i < 8; i++)
	{
	  if (_wm->_sensors[i][5] < gSensorRange)
	  {
	    isNearWall = true;
	    break;
	  }
	}
	
	checkPheromoneStagnation();

	_wm->_desiredTranslationalValue =  + maxSpeed - maxSpeed*(( (double)gSensorRange - (_wm->_sensors[N][5] ) ) / (double)gSensorRange);
	
// 	_wm->_desiredTranslationalValue = gMaxTranslationalSpeed - (_pSensor->_sensor[FRONT][LIGHT] / (double)gMaxTranslationalSpeed);
	
	//Random Rotation when only front sensor detects an object
// 	if (_wm->_sensors[N][5] < (double)gSensorRange*(2/3) && _wm->_sensors[NE][5] == (double)gSensorRange &&  _wm->_sensors[NW][5] == (double)gSensorRange)
//     	_wm->_desiredRotationalVelocity = 0.4 - (double)(rand()%10)/10.*0.2;

	
// 	if (_pSensor->_sensor[FRONT][LIGHT] > 0)
// 	  _wm->_desiredTranslationalValue =  + 2 - 1*(( 255.0 - _pSensor->_sensor[FRONT][LIGHT] ) / 255.0);
	  
	//WALL TURN RIGHT
	if ( _wm->_sensors[W][5] + _wm->_sensors[NW][5] <  _wm->_sensors[NE][5] + _wm->_sensors[E][5] ) 
	{
		rot += (( (double)gSensorRange - _wm->_sensors[NW][5] ) / (double)gSensorRange);
		rot += (( (double)gSensorRange - (_wm->_sensors[W][5]))/ (double)gSensorRange);
		_wm->_desiredRotationalVelocity = rot;
// 		pLeftCounter--;
	}
	//PHEROMONE SHIFT RANDOM
// 	else if (leftMax != -1 && _pSensor->_sensor[leftMax][LIGHT] == _pSensor->_sensor[rightMax][LIGHT] && !isNearWall && !ignorePheromones)
// 	{
// 	 _wm->_desiredRotationalVelocity = maxRot - (double)(rand()%10)/10.*rot;
// // 	 releasePheromones=false;
// // 	  std::cout << "  CENTER   " << std::endl;//stepsInPoo++ << std::endl;
// 	}
	//PHEROMONE TURN RIGHT
	else if (!isNearWall && !pStagnationRight &&  leftMaxPhero > rightMaxPhero )//_pSensor->_sensor[leftMax][LIGHT] > _pSensor->_sensor[rightMax][LIGHT] )//_pSensor->_sensor[leftMax][LIGHT] > _pSensor->_sensor[rightMax][LIGHT] && !isNearWall && !pStagnationRight)
	{
	  _wm->_desiredRotationalVelocity = rot+rot*(_pSensor->_sensor[leftMax][LIGHT]);
// 	  _wm->_desiredRotationalVelocity = pRightRot;
	  pDirectionCounter++;
	  pRightCounter++;
// 	  releasePheromones=false;
// 	  std::cout << "LEFT " << pRightCounter <<  std::endl;

	}
	else
		//WALL TURN LEFT
		if ( _wm->_sensors[NE][5] + _wm->_sensors[E][5] < 2*gSensorRange ) 
		{
		    rot += (( (double)gSensorRange - _wm->_sensors[NE][5]) / (double)gSensorRange);
		    rot += (( (double)gSensorRange - (_wm->_sensors[E][5]))/ (double)gSensorRange);
		    _wm->_desiredRotationalVelocity = -rot;
// 		    pRightCounter--;

		}
		//PHEROMONE TURN LEFT
		else if (!isNearWall && !pStagnationLeft && rightMaxPhero > leftMaxPhero )//_pSensor->_sensor[rightMax][LIGHT] > 0.01) //_pSensor->_sensor[rightMax][LIGHT] > 0.01 && _pSensor->_sensor[rightMax][LIGHT] != _pSensor->_sensor[leftMax][LIGHT] && !isNearWall && !pStagnationLeft)
		{
			_wm->_desiredRotationalVelocity = -rot - (rot*(_pSensor->_sensor[rightMax][LIGHT]));
// 			_wm->_desiredRotationalVelocity = pLeftRot;

			pDirectionCounter--;
			pLeftCounter++;
// 			releasePheromones=false;
// 			std::cout << "      RIGHT " << pLeftCounter << std::endl;
		}
		else
		{
			pDirectionCounter = 0;
			pLeftCounter = pLeftCounter < 0 ? 0 : pLeftCounter--;
			pRightCounter = pRightCounter < 0 ? 0 : pRightCounter--;
			
			_wm->_desiredRotationalVelocity = 0;// .01;
			//STRAIGHTEN OUT
// 			if ( _wm->_desiredRotationalVelocity > 0 ) 
// 				_wm->_desiredRotationalVelocity -= 0.1;
// 			else
// 				if ( _wm->_desiredRotationalVelocity < 0) 
// 					_wm->_desiredRotationalVelocity += 0.1;
// 				else
// 					_wm->_desiredRotationalVelocity = 0;// 0.1 - (double)(rand()%10)/10.*0.2;
				
		}
}



void BasicPheromoneControlArchitecture::wallStagnation()
{
  releasePheromones = false;
  pDirectionCounter=0;
  if (!doneReversing)
  {
      reverse(40);
  }
  else
  {
    if (!isRotating)
    {
      isReversing = false;
      Uint8 bestDirection = 10;
      int val = -1;
      for (Uint8 i = 0; i < 8; i++)
      {
	int temp = 0;
	temp += _wm->_sensors[(i+7)%8][5];
	temp += _wm->_sensors[i][5];
	temp += _wm->_sensors[(i+9)%8][5];
		
	
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

/*
void BasicPheromoneControlArchitecture::pheromoneStagnation()
{
  ignorePheromones = true;
  pheromoneIgnoreCounter--;
  

}*/


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
  
//   if (checkPheromoneStagnation())
//   {
//     std::cout << "	status: pheromone stagnation" << std::endl;
//     return PHEROMONE_STAGNATION;
//   }

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
    
//     std::cout << "Wall Stagnation limit 20:    Value:   "    << stdX + stdY << std::endl;
    lastLocations.clear();
    stepCounter = 0;
    
    if (stdX + stdY < 20)
    {
//       std::cout << "|||||||||| STAGNATION |||||||||" << std::endl;
      return true;
    }
  }
  return false;
}
void BasicPheromoneControlArchitecture::checkPheromoneStagnation()
{
  
//   std::cout << "      RIGHT " << pLeftCounter << std::endl;
//   	  std::cout << "LEFT " << pRightCounter <<  std::endl;

  
  int threshold = 50;
  int stagDuration = 10;
  if (!pStagnationLeft)
  {
    if (pLeftCounter >= threshold)
    {
      pLeftIgnoreCounter = stagDuration;
      pStagnationLeft = true;
//       std::cout << "STAG LEFT" << std::endl;
    }
  }
  else
  {
    pLeftIgnoreCounter--;
    pLeftCounter = 0;

  }
  if (pLeftIgnoreCounter == 0)
  {
      pStagnationLeft = false;

  }
  
  
  
  if (!pStagnationRight)
  {
    if (pRightCounter >= threshold)
    {
      pRightIgnoreCounter = stagDuration;
      pRightCounter = 0;
      pStagnationRight = true;
//       std::cout << "     STAG RIGHT " << std::endl;
    }
  }
  else
  {
    pRightIgnoreCounter--;
    pRightCounter = 0;

  }
  if (pRightIgnoreCounter <= 0)
  {
    pStagnationRight=false;
  }
  
//   if (!isPheromoneStagnated)
//   {
//   //   std::cout << pDirectionCounter << std::endl;
//     int threshold = 50;
//     if (pDirectionCounter >= threshold || pDirectionCounter <= -threshold)
//     {
//       if (pDirectionCounter > 0)
// 	std::cout << "    STAG RIGHT" << std::endl;
//       else if (pDirectionCounter < 0)
// 	std::cout << "STAG LEFT" << std::endl;
//       isPheromoneStagnated = true;
//       pheromoneIgnoreCounter = 200;
//       pDirectionCounter = 0;
//     }
//   }
//   else 
//   {
//     pheromoneIgnoreCounter--;
//     ignorePheromones = true;
// //     releasePheromones = false;
//   
//   if (pheromoneIgnoreCounter <= 0)
//   {
//     ignorePheromones = false;
//     isPheromoneStagnated = false;
// //     releasePheromones = true;
//     pDirectionCounter = 0;
//     std::cout << "ENDED" << std::endl;
//   }
//   }
}


 