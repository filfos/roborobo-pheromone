#if defined PRJ_MEDEA || !defined MODULAR

#include "Config/MedeaConfigurationLoader.h"

#include "mEDEA/include/MedeaWorldObserver.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"
#include "mEDEA/include/MedeaAgentObserver.h"
#include "mEDEA/include/MedeaPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaConfigurationLoader::MedeaConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaConfigurationLoader::~MedeaConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaWorldObserver(wm);
}

RobotAgentWorldModel* MedeaConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAgentWorldModel();
}

AgentObserver* MedeaConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
