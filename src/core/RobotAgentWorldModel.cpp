/*
 *  RobotAgentWorldModel.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "WorldModels/RobotAgentWorldModel.h"

#include "World/World.h"

RobotAgentWorldModel::RobotAgentWorldModel()
{
	_sensors = NULL;
	_robotLED_status = false; // default: LED is off.
	_robotLED_green = _robotLED_blue = 0; // default value (grey)
	_robotLED_red = 255;
}

RobotAgentWorldModel::~RobotAgentWorldModel()
{
	if ( _sensors != NULL )
		delete[] _sensors;
}

double RobotAgentWorldModel::getSensorDistanceValue( int i )
{
	return _sensors[i][5];
}

double RobotAgentWorldModel::getSensorTypeValue( int i )
{
	return _sensors[i][6];
}

double RobotAgentWorldModel::getSensorMaximumDistanceValue( int i )
{
	return gSensorRange; // BEWARE: sensor[i][3] is distance btw the end point of the sensor and the **center of the agent**
}

void RobotAgentWorldModel::setRobotNeighborhoodCounter( int __value )
{
	_robotNeighborhoodCounter = __value;
}

void RobotAgentWorldModel::incRobotNeighborhoodCounter( ) // increment
{
	_robotNeighborhoodCounter++;
}

int RobotAgentWorldModel::getRobotNeighborhoodCounter()
{
	return _robotNeighborhoodCounter;
}

double RobotAgentWorldModel::getXReal()
{
	return _xReal;
}

double RobotAgentWorldModel::getYReal()
{
	return _yReal;
}

bool RobotAgentWorldModel::getRobotLED_status()
{
	return _robotLED_status;
}

void RobotAgentWorldModel::setRobotLED_status(bool __status)
{
	_robotLED_status = __status;
}

int RobotAgentWorldModel::getRobotLED_redValue()
{
	return _robotLED_red;
}

int RobotAgentWorldModel::getRobotLED_greenValue()
{
	return _robotLED_green;
}

int RobotAgentWorldModel::getRobotLED_blueValue()
{
	return _robotLED_blue;
}

void RobotAgentWorldModel::setRobotLED_colorValues(int __r, int __g, int __b)
{
	_robotLED_red = __r;
	_robotLED_green = __g;
	_robotLED_blue = __b;
}
