/*
 * MedeaPheromonesConfigurationLoader.h
 *
 *  current dev: Nicolas on 16/6/2012
 *
 */

#ifndef MEDEAPHEROMONESCONFIGURATIONLOADER_H
#define MEDEAPHEROMONESCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaPheromonesConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MedeaPheromonesConfigurationLoader();
		~MedeaPheromonesConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
