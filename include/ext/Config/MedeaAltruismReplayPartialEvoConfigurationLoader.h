/*
 * MedeaAltruismReplayPartialEvoConfigurationLoader.h
 *
 * 	Jean-Marc on 25/10/2010
 * 	moved in replay partial evo by Jean-Marc on 08/12/2010
 *
 */

#ifndef MEDEAALTRUISMREPLAYPARTIALEVOCONFIGURATIONLOADER_H
#define MEDEAALTRUISMREPLAYPARTIALEVOCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class MedeaAltruismReplayPartialEvoConfigurationLoader : public ConfigurationLoader
{
	private :

	public:
		MedeaAltruismReplayPartialEvoConfigurationLoader();
		~MedeaAltruismReplayPartialEvoConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
