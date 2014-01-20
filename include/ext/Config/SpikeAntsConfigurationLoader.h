/*
 * SpikeAntsConfigurationLoader.h
 *
 */

#ifndef SPIKEANTSPROJECTCONFIGURATIONLOADER_H
#define	SPIKEANTSPROJECTCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"

class SpikeAntsConfigurationLoader : public ConfigurationLoader
{
	public:
		SpikeAntsConfigurationLoader();
		~SpikeAntsConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};

#endif
