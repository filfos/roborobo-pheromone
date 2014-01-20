#if defined PRJ_MEDEA_OPTIM || !defined MODULAR

#include "Config/MedeaOptimConfigurationLoader.h"

#include "mEDEA-optim/include/MedeaOptimWorldObserver.h"
#include "mEDEA-optim/include/MedeaOptimAgentWorldModel.h"
#include "mEDEA-optim/include/MedeaOptimAgentObserver.h"
#include "mEDEA-optim/include/MedeaOptimPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaOptimConfigurationLoader::MedeaOptimConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaOptimConfigurationLoader::~MedeaOptimConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaOptimConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaOptimWorldObserver(wm);
}

RobotAgentWorldModel* MedeaOptimConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaOptimAgentWorldModel();
}

AgentObserver* MedeaOptimConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaOptimAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaOptimConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaOptimPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaOptimConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
