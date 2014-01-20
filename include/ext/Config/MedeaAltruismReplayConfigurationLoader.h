/*
 * ReplayConfigurationLoader.h
 *
 * 	Jean-Marc on 25/10/2010
 *
 */

#ifndef MEDEAALTRUISMREPLAYCONFIGURATIONLOADER_H
#define MEDEAALTRUISMREPLAYCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaAltruismReplayConfigurationLoader : public ConfigurationLoader
{
	private :

	public:
		MedeaAltruismReplayConfigurationLoader();
		~MedeaAltruismReplayConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
