/*
 * MedeaCommConfigurationLoader.h
 *
 *  imported from Jean-Marc on 20/12/09
 *  current dev: Nicolas on 15/9/2010
 *  moved in altruism by Jean-Marc on 17/09/2010
 *
 */

#ifndef MEDEACOMMCONFIGURATIONLOADER_H
#define MEDEACOMMCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaCommConfigurationLoader : public ConfigurationLoader
{
	private:

	public:
		MedeaCommConfigurationLoader();
		~MedeaCommConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
