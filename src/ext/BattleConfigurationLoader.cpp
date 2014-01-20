#if defined PRJ_MEDEA || !defined MODULAR

#include "Config/BattleConfigurationLoader.h"

#include "mEDEA/include/BattleWorldObserver.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"
#include "mEDEA/include/BattleAgentObserver.h"
#include "mEDEA/include/MedeaPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

BattleConfigurationLoader::BattleConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

BattleConfigurationLoader::~BattleConfigurationLoader()
{
	//nothing to do
}


WorldObserver* BattleConfigurationLoader::make_WorldObserver(World* wm)
{
	return new BattleWorldObserver(wm);
}

RobotAgentWorldModel* BattleConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAgentWorldModel();
}

AgentObserver* BattleConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new BattleAgentObserver(wm);
}

BehaviorControlArchitecture* BattleConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* BattleConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
