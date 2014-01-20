
/*
 *  RobotAgentWorldInterfaceDangerWorld.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include <string.h>

#include "World/RobotAgentWorldInterfaceDangerWorld.h"

#include "World/World.h"


/*
 check parent class C++ file for detailed comments
 */
void RobotAgentWorldInterfaceDangerWorld::prepareInternalDynamics ()
{
	// nothing to do.
}


/*
 check parent class C++ file for detailed comments
 upgrade parent class with environment constraint over movement (slow danger zone)
 */
void RobotAgentWorldInterfaceDangerWorld::applyInternalDynamics( RobotAgentWorldModel *__wm )
{
	std::cout << "in internal dynamics" << std::endl ;
	if ( EvolvingRobotAgentWorldModelDangerWorld::_gLoadedProperties  == false ) // first call: load properties (if any)
	{
		gProperties.checkAndGetPropertyValue("DangerZone_InfluenceRadius", &EvolvingRobotAgentWorldModelDangerWorld::_gInfluenceRadius, true);
		gProperties.checkAndGetPropertyValue("DangerZone_RobotDensityThreshold", &EvolvingRobotAgentWorldModelDangerWorld::_gRobotDensityThreshold, true);
		gProperties.checkAndGetPropertyValue("DangerZone_MaximumVelocityPenalizationFactor", &EvolvingRobotAgentWorldModelDangerWorld::_gMaximumVelocityPenalizationFactor, true);
		EvolvingRobotAgentWorldModelDangerWorld::_gLoadedProperties  = true;
	}

	// update internal data
	
	if ( __wm->_agentId == 0 )
	{
		// re-initialize neighborhood density counter for all agents
		__wm->setRobotNeighborhoodCounter(0);
		for ( int i = 1 ; i < gAgentCounter ; i++ )
			gWorld->getAgent(i)->getWorldModel()->setRobotNeighborhoodCounter(0);
		
		// update neighborhood counter for all robots
		for ( int i = 0 ; i < gAgentCounter-1 ; i++ )
			for ( int j = i+1 ; j < gAgentCounter ; j++ )
			{
				double x1 = gWorld->getAgent(i)->getWorldModel()->getXReal();
				double y1 = gWorld->getAgent(i)->getWorldModel()->getYReal();
				double x2 = gWorld->getAgent(j)->getWorldModel()->getXReal();
				double y2 = gWorld->getAgent(j)->getWorldModel()->getYReal();

				if ( getSquaredEuclidianDistance( x1, y1, x2, y2 ) <= EvolvingRobotAgentWorldModelDangerWorld::_gInfluenceRadius*EvolvingRobotAgentWorldModelDangerWorld::_gInfluenceRadius ) 
				{
					gWorld->getAgent(i)->getWorldModel()->incRobotNeighborhoodCounter();
					gWorld->getAgent(j)->getWorldModel()->incRobotNeighborhoodCounter();
					//if ( verbose ) std::cout << "!" ;
				}
			}
	}
	
	if ( __wm->_floorSensor == 100 ) // robot is in danger zone.
	{
		double density = 
			(double)std::min( __wm->getRobotNeighborhoodCounter(), EvolvingRobotAgentWorldModelDangerWorld::_gRobotDensityThreshold) / (double)EvolvingRobotAgentWorldModelDangerWorld::_gRobotDensityThreshold ;
		double penalizationFactor = 
			EvolvingRobotAgentWorldModelDangerWorld::_gMaximumVelocityPenalizationFactor + ( 1 - EvolvingRobotAgentWorldModelDangerWorld::_gMaximumVelocityPenalizationFactor ) * density;
		__wm->_actualTranslationalValue *= penalizationFactor;
		__wm->_actualRotationalVelocity *= penalizationFactor;
	}
	
	__wm->_agentAbsoluteLinearSpeed = __wm->_actualTranslationalValue;
	__wm->_agentAbsoluteOrientation += __wm->_actualRotationalVelocity;
	std::cout << "Interface(" << __wm->_agentId << ") : " << __wm->_agentAbsoluteOrientation << std::endl ;
	
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
			__wm->_agentAbsoluteOrientation = __wm->_agentAbsoluteOrientation-360.0;
		}
	}

}
