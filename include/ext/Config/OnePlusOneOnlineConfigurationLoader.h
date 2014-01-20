/*
 * jmConfigurationLoader.h
 *
 * Created by jm on 20/12/10
 *
 */
// Date	       : 2009 04 02
// Description : (1+1)-online, perceptron
// Comments    : reproduction of the experiments done in the cortex
// Last Tested : 2009 11 23 

#ifndef ONEPLUSONECONFIGURATIONLOADER_H
#define ONEPLUSONECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class OnePlusOneOnlineConfigurationLoader : public ConfigurationLoader
{
	public:
		OnePlusOneOnlineConfigurationLoader();
		~OnePlusOneOnlineConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
