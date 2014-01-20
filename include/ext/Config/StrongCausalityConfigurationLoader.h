/*
 * jmConfigurationLoader.h
 *
 * Created by jm on 20/12/10
 *
 */
// Comments from the previous config.h
// Date	       : 2009 04 03
// Description : experiments "strong causality"
// Comments    : ...
// Last Tested : 2009 11 23 

#ifndef STRONGCAUSALITYCONFIGURATIONLOADER_H
#define STRONGCAUSALITYCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class StrongCausalityConfigurationLoader : public ConfigurationLoader
{
	public:
		StrongCausalityConfigurationLoader();
		~StrongCausalityConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
