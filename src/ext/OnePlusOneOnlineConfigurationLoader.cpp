#if defined PRJ_ONEPLUSONE || !defined MODULAR

#include "Config/OnePlusOneOnlineConfigurationLoader.h"

#include "Observers/WorldObserver.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include "OnePlusOne-Online/include/OnePlusOneOnlineGeneticsEngineAgentObserver.h"
#include "BehaviorControlArchitectures/PerceptronBehaviorControlArchitecture.h"
#include "World/RobotAgentWorldInterface.h"

OnePlusOneOnlineConfigurationLoader::OnePlusOneOnlineConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
}

OnePlusOneOnlineConfigurationLoader::~OnePlusOneOnlineConfigurationLoader()
{
	//nothing to do
}


WorldObserver* OnePlusOneOnlineConfigurationLoader::make_WorldObserver(World* wm)
{
	return new WorldObserver(wm);
}

RobotAgentWorldModel* OnePlusOneOnlineConfigurationLoader::make_RobotAgentWorldModel()
{
	return new EvolvingRobotAgentWorldModel();
}

AgentObserver* OnePlusOneOnlineConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new OnePlusOneOnlineGeneticsEngineAgentObserver(wm);
}

BehaviorControlArchitecture* OnePlusOneOnlineConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new PerceptronBehaviorControlArchitecture(wm);
}

RobotAgentWorldInterface* OnePlusOneOnlineConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
