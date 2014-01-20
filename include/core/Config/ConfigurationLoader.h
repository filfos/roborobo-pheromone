/*
 *	ConfigurationLoader.h
 *
 *	Created by Jean-marc on 19/02/10
 *
 */

#ifndef CONFIGURATIONLOADER_H
#define CONFIGURATIONLOADER_H


#include "Observers/WorldObserver.h"
#include "WorldModels/RobotAgentWorldModel.h"
#include "Observers/AgentObserver.h"
#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "World/RobotAgentWorldInterface.h"

#include "RoboroboMain/common.h"
#include "Utilities/ExtendedProperties.h"
#include <string.h>


class ConfigurationLoader
{
	protected:
		ConfigurationLoader();
		~ConfigurationLoader();
		
		RobotAgentWorldInterface* _AgentWorldInterface; // only one AgentWorldInterface instance is created in constructor, then a pointer is returned (ie. same instance for all)

	public:

		static ConfigurationLoader* make_ConfigurationLoader(std::string configurationLoaderObjectName);

		virtual WorldObserver* make_WorldObserver(World* wm) = 0 ;
		virtual RobotAgentWorldModel* make_RobotAgentWorldModel() = 0 ;
		virtual AgentObserver* make_AgentObserver(RobotAgentWorldModel* wm) = 0 ;
		virtual BehaviorControlArchitecture* make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) = 0;
		virtual RobotAgentWorldInterface* make_RobotAgentWorldInterface() = 0; // interface created once in constructor, returns a pointer
};


#endif
