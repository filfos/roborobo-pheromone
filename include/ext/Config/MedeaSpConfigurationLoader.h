/*
 * MedeaSpConfigurationLoader.h
 *
 * Created by jm on 20/12/10 - last update: nicolas 27/5/2010
 *
 */
// Date	       : 2009 11 19
// Description : default environment, default behavior (wall-avoidance) -- related to the MedeaSpProject project
// Comments    : The basic setup you may want to start with, features a braitenberg obstacle avoidance agent.
// Last Tested : 2010 05 27

#ifndef MEDEASPCONFIGURATIONLOADER_H
#define	MEDEASPCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class MedeaSpConfigurationLoader : public ConfigurationLoader
{
	public:
		MedeaSpConfigurationLoader();
		~MedeaSpConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
