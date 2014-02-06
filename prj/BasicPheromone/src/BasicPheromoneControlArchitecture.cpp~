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

BasicPheromoneControlArchitecture::BasicPheromoneControlArchitecture( RobotAgentWorldModel *__wm ) : BehaviorControlArchitecture ( __wm )
{
    _pSensor = new BasicPheromoneSensor(__wm);
    _pLeftCounter = 0;
    _pRightCounter = 0;
  //  std::cout << FRONT << std::endl;
	// nothing to do
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
	
	//std::cout << "sensor range " << _pSensor->sensorRange << std::endl;
	//_wm->setRobotLED_colorValues(0,0,0);
	
	//if (_pheromoneCounter > 0) _pheromoneCounter--;
	//else _wm->setRobotLED_colorValues(0,0,0);
	
	
	if (_pSensor->foodFound() == 1)
	{
	  _wm->setRobotLED_colorValues(0,0,255);
	//  _pheromoneCounter = 5;
	}
	
	std::cout << "Distance:  " << _pSensor->distanceToPheromone(FRONT) << std::endl;
	
	pheromoneReaction();

	/*
	_wm->_desiredTranslationalValue = + 1 -( (double)_pSensor->sensorRange - _pSensor->_sensor[FRONT][PROX] ) / (double)_pSensor->sensorRange;  
	
	if ( _pSensor->_sensor[FRONT_LEFT][PROX] + _pSensor->_sensor[FRONT][PROX] + _pSensor->_sensor[FRONT_RIGHT][PROX] < (double)_pSensor->sensorRange*1/3)
	{
	  if (_pSensor->_sensor[FRONT_LEFT][PROX] < _pSensor->_sensor[FRONT_RIGHT][PROX])
	  {
	    _wm->_desiredRotationalVelocity = +5;
// 	    std::cout << "TUUUUURN RIIIIIGHT" << std::endl;
	  }
	  else
	  {
	    _wm->_desiredRotationalVelocity = -5;
// 	    std::cout << "TUUUUURN LEEEEEFT" << std::endl;
	  }
	}
	
	if ( _pSensor->_sensor[LEFT][PROX] + _pSensor->_sensor[FRONT_LEFT][PROX] < _pSensor->_sensor[RIGHT][PROX] + _pSensor->_sensor[FRONT_RIGHT][PROX])
	{
	  //Turn Right
// 	  std::cout << "Turning RIGHT" << std::endl;
	  _wm->_desiredRotationalVelocity = +2;
	}
	else
	{
	  if (_pSensor->_sensor[RIGHT][PROX] + _pSensor->_sensor[FRONT_RIGHT][PROX] < _pSensor->sensorRange)
	  {
	    //Turn Left
//     	  std::cout << "Turning LEFT" << std::endl;
	    _wm->_desiredRotationalVelocity = -2;
	  }
	  else
	  {
	    if (_wm->_desiredRotationalVelocity > 0)
	    {
	      //Straighten
// 	      std::cout << "Straighten LEFT" << std::endl;
	      _wm->_desiredRotationalVelocity -= 0.5;
	    }
	    else
	    {
	      if (_wm->_desiredRotationalVelocity < 0)
	      {
		//Straighten
// 		std::cout << "Straighten RIGHT" << std::endl;
		_wm->_desiredRotationalVelocity += 0.5;
	      }
	      else
	      {
// 		std::cout << "Random" << std::endl;
		_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
	      }
	    }
	  }
	}
	*/
	//_wm->_desiredTranslationalValue = 10;
	
	wallAvoidance();
	
	// a basic obstacle avoidance behavior (E-puck sensor layout)
	/*
	_wm->_desiredTranslationalValue =  + 1 - ( (double)gSensorRange - ((_wm->_sensors[2][5]+_wm->_sensors[3][5])/2) )  / (double)gSensorRange; 
	if ( _wm->_sensors[0][5] + _wm->_sensors[1][5] + _wm->_sensors[2][5] < _wm->_sensors[3][5] + _wm->_sensors[4][5] + _wm->_sensors[5][5] ) 
		_wm->_desiredRotationalVelocity = +5;
	else
		if ( _wm->_sensors[3][5] + _wm->_sensors[4][5] + _wm->_sensors[5][5] < 3*gSensorRange ) 
			_wm->_desiredRotationalVelocity = -5;
		else
			if ( _wm->_desiredRotationalVelocity > 0 ) 
				_wm->_desiredRotationalVelocity--;
			else
				if ( _wm->_desiredRotationalVelocity < 0) 
					_wm->_desiredRotationalVelocity++;
				else
					_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
				
	*/
}


void BasicPheromoneControlArchitecture::pheromoneReaction()
{
  double mean = 0;
  for (int i = 0; i < 8; i++)
    mean += _pSensor->_sensor[i][LIGHT];
  mean /= 8;
  
  if (mean == 0)
    return;
    
 // _wm -> _desiredTranslationalValue = + 2 - 2*( _pSensor->sensorRange)
  
  
  if (mean > 0)
  {
    double right = 0;
    right += _pSensor->_sensor[FRONT_RIGHT][LIGHT];
    right += _pSensor->_sensor[RIGHT][LIGHT];
    right += _pSensor->_sensor[BACK_RIGHT][LIGHT];
    
    double left = 0;
    left += _pSensor->_sensor[FRONT_LEFT][LIGHT];
    left += _pSensor->_sensor[LEFT][LIGHT];
    left += _pSensor->_sensor[BACK_LEFT][LIGHT];
    
   // if (_pheromoneCounter)
    int lim = 100;
    
    if (left > right)
    {
      if (_pRightCounter < lim)
      {
	_wm->_desiredRotationalVelocity = 5;
      }
      ++_pRightCounter;
      if (_pRightCounter > 1.5*lim)
      {
	_pRightCounter = 0;
	std::cout << "reset RIGHT" << std::endl;
      }

    }
    else 
    {
      if (_pLeftCounter < 100)
      {
	_wm->_desiredRotationalVelocity = -5;
      }
      ++_pLeftCounter;
      if (_pLeftCounter > 1.5*lim)
      {
	_pLeftCounter = 0;
	std::cout << "reset LEFT" << std::endl;
      }
    }
  }
  
 // std::cout << "pheromone" << std::endl;
}


//For 8-sensor CHiRP-robot
void BasicPheromoneControlArchitecture::wallAvoidance()
{
  //Slow down if wall is detected straight ahead
  _wm->_desiredTranslationalValue =  + 2 - 2*(( (double)gSensorRange - (_wm->_sensors[N][5] ) ) / (double)gSensorRange);
  
	// Compare distance to walls on both sides
	if ( _wm->_sensors[W][5] + _wm->_sensors[NW][5] <  _wm->_sensors[NE][5] + _wm->_sensors[E][5] ) 
		_wm->_desiredRotationalVelocity = +2;
	else
		if ( _wm->_sensors[NE][5] + _wm->_sensors[E][5] < 2*gSensorRange ) 
			_wm->_desiredRotationalVelocity = -2;
		else
			if ( _wm->_desiredRotationalVelocity > 0 ) 
				_wm->_desiredRotationalVelocity -= 0.5;
			else
				if ( _wm->_desiredRotationalVelocity < 0) 
					_wm->_desiredRotationalVelocity += 0.5;
				else
					_wm->_desiredRotationalVelocity = 0.01 - (double)(rand()%10)/10.*0.02;
				
  /*
  std::cout << "0 :: " << _wm->_sensors[W][5] << std::endl;
  std::cout << "1 :: " << _wm->_sensors[NW][5] << std::endl;
  std::cout << "2 :: " << _wm->_sensors[N][5] << std::endl;
  std::cout << "3 :: " << _wm->_sensors[NE][5] << std::endl;
  std::cout << "4 :: " << _wm->_sensors[E][5] << std::endl;
  std::cout << "5 :: " << _wm->_sensors[SE][5] << std::endl;
  std::cout << "6 :: " << _wm->_sensors[S][5] << std::endl;
  std::cout << "7 :: " << _wm->_sensors[SW][5] << std::endl;
*/
}


void BasicPheromoneControlArchitecture::foodReaction(int x, int y)
{  
 // std::cout << "Found food at  x:" << x << "   y: " << y <<  std::endl;
 // _wm->setRobotLED_colorValues(0, 0, 255);
}











