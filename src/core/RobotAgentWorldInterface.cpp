
/*
 *  RobotAgentWorldInterface.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "World/RobotAgentWorldInterface.h"

/*
	This method prepares agents for applying dynamics. It is called by the World->updateWorld method only once per world iteration.
*/
void RobotAgentWorldInterface::prepareInternalDynamics ()
{
	// nothing to do.
}


/*
	This method apply internal world dynamics onto actual effector values (ie. values that comply 
	with controller hardware limitations). In this method, two important steps are performed:
	1. update actual translation/rotation values wrt. to environmental constraints (optional)
		e.g.: add noise to motor execution that can be perceived back,
		interpretation: actual values are also used as proprioceptive sensors
	2. compute internal data for moving the robot (should not be accessed by the robot)
		e.g.: slippery terrain, proprioceptive sensor defect, etc.
		interpretation: all the world dynamics that cannot be grasped through proprioceptive sensors
*/
void RobotAgentWorldInterface::applyInternalDynamics( RobotAgentWorldModel *__wm )
{
	// * update internal data

	__wm->_agentAbsoluteLinearSpeed = __wm->_actualTranslationalValue;
	__wm->_agentAbsoluteOrientation += __wm->_actualRotationalVelocity;
	
	// * recalibrate orientation within ]-180°,+180°]
	
	// deprecated -- do not cover extreme cases.
	//if ( __wm->_agentAbsoluteOrientation < 0 )
	//	__wm->_agentAbsoluteOrientation += 360; // assume cannot go below -360deg in one step only (TODO: explicit check)

	if ( __wm->_agentAbsoluteOrientation <= -180.0 )
	{
		__wm->_agentAbsoluteOrientation = __wm->_agentAbsoluteOrientation + 360.0;
	}
	else
	{
		if ( __wm->_agentAbsoluteOrientation > 180.0 )
		{
			__wm->_agentAbsoluteOrientation = __wm->_agentAbsoluteOrientation - 360.0;
		}
	}

}
