/*
 * MedeaSpecializeConfigurationLoader.h
 *
 *  Nicolas on 15/9/2010
 *
 */

#ifndef MEDEASPECIALIZECONFIGURATIONLOADER_H
#define MEDEASPECIALIZECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaSpecializeConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MedeaSpecializeConfigurationLoader();
		~MedeaSpecializeConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
