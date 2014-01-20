/*
 * MedeaConfigurationLoader.h
 *
 *  imported from Jean-Marc on 20/12/09
 *  current dev: Nicolas on 15/9/2010
 *
 */

#ifndef MEDEACONFIGURATIONLOADER_H
#define MEDEACONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MedeaConfigurationLoader();
		~MedeaConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
