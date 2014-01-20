/*
 * MedeaConfigurationLoader.h
 *
 *  imported from Jean-Marc on 20/12/09
 *  current dev: Nicolas on 1/4/2009
 *
 */

#ifndef BASICPROJECTMEDEACONFIGURATIONLOADER_H
#define BASICPROJECTMEDEACONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class BasicProjectMedeaConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		BasicProjectMedeaConfigurationLoader();
		~BasicProjectMedeaConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
