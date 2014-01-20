#if defined PRJ_MEDEA || !defined MODULAR

#include "Config/ReplayConfigurationLoader.h"

#include "mEDEA/include/ReplayWorldObserver.h"
#include "mEDEA/include/MedeaAgentWorldModel.h"
#include "mEDEA/include/ReplayAgentObserver.h"
#include "mEDEA/include/MedeaPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

ReplayConfigurationLoader::ReplayConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

ReplayConfigurationLoader::~ReplayConfigurationLoader()
{
	//nothing to do
}


WorldObserver* ReplayConfigurationLoader::make_WorldObserver(World* wm)
{
	return new ReplayWorldObserver(wm);
}

RobotAgentWorldModel* ReplayConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAgentWorldModel();
}

AgentObserver* ReplayConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new ReplayAgentObserver(wm);
}

BehaviorControlArchitecture* ReplayConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* ReplayConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
