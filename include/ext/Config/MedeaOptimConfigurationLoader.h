/*
 * MedeaOptimConfigurationLoader.h
 *
 *  current dev: Nicolas on 18/1/2011
 *
 */

#ifndef MEDEAOPTIMCONFIGURATIONLOADER_H
#define MEDEAOPTIMCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaOptimConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MedeaOptimConfigurationLoader();
		~MedeaOptimConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
