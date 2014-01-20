/*
 * ReplayConfigurationLoader.h
 *
 * 	Jean-Marc on 20/04/2010
 *
 */

#ifndef REPLAYCONFIGURATIONLOADER_H
#define REPLAYCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class ReplayConfigurationLoader : public ConfigurationLoader
{
	private :

	public:
		ReplayConfigurationLoader();
		~ReplayConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
