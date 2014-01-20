/*
 *  BasicControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicProject/include/BasicControlArchitecture.h"

BasicControlArchitecture::BasicControlArchitecture( RobotAgentWorldModel *__wm ) : BehaviorControlArchitecture ( __wm )
{
	// nothing to do
}

BasicControlArchitecture::~BasicControlArchitecture()
{
	// nothing to do.
}

void BasicControlArchitecture::reset()
{
	// nothing to do.
}

void BasicControlArchitecture::step()
{
	// a basic obstacle avoidance behavior

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
}

