/*
 * MedeaEcologyConfigurationLoader.h
 *
 *  current dev: Nicolas on 16/6/2012
 *
 */

#ifndef MEDEAECOLOGYCONFIGURATIONLOADER_H
#define MEDEAECOLOGYCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaEcologyConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MedeaEcologyConfigurationLoader();
		~MedeaEcologyConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
