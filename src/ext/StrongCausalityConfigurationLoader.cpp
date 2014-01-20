#if defined PRJ_ONEPLUSONE || !defined MODULAR

#include "Config/StrongCausalityConfigurationLoader.h"

#include "Observers/WorldObserver.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include "OnePlusOne-Online/include/StrongCausalityExperimentAgentObserver.h"
#include "BehaviorControlArchitectures/PerceptronBehaviorControlArchitecture.h"
#include "World/RobotAgentWorldInterface.h"

StrongCausalityConfigurationLoader::StrongCausalityConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();
}

StrongCausalityConfigurationLoader::~StrongCausalityConfigurationLoader()
{
	//nothing to do
}


WorldObserver* StrongCausalityConfigurationLoader::make_WorldObserver(World* wm)
{
	return new WorldObserver(wm);
}

RobotAgentWorldModel* StrongCausalityConfigurationLoader::make_RobotAgentWorldModel()
{
	return new EvolvingRobotAgentWorldModel();
}

AgentObserver* StrongCausalityConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new StrongCausalityExperimentAgentObserver(wm);
}

BehaviorControlArchitecture* StrongCausalityConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new PerceptronBehaviorControlArchitecture(wm);
}

RobotAgentWorldInterface* StrongCausalityConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
