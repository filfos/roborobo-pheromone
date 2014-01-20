/*
 * BattleConfigurationLoader.h
 *
 * 	Jean-Marc on 14/04/2010
 *
 */

#ifndef BATTLECONFIGURATIONLOADER_H
#define BATTLECONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"


class BattleConfigurationLoader : public ConfigurationLoader
{
	private :

	public:
		BattleConfigurationLoader();
		~BattleConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotAgentWorldModel *make_RobotAgentWorldModel();
		AgentObserver *make_AgentObserver(RobotAgentWorldModel* wm) ;
		BehaviorControlArchitecture *make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) ;
		RobotAgentWorldInterface *make_RobotAgentWorldInterface() ; // interface created once in constructor, returns a pointer
};



#endif
