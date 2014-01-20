#if defined PRJ_MEDEA_SPECIALIZE || !defined MODULAR

#include "Config/MedeaSpecializeConfigurationLoader.h"

#include "mEDEA-specialize/include/MedeaSpecializeWorldObserver.h"
#include "mEDEA-specialize/include/MedeaSpecializeAgentWorldModel.h"
#include "mEDEA-specialize/include/MedeaSpecializeAgentObserver.h"
#include "mEDEA-specialize/include/MedeaSpecializePerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaSpecializeConfigurationLoader::MedeaSpecializeConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaSpecializeConfigurationLoader::~MedeaSpecializeConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaSpecializeConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaSpecializeWorldObserver(wm);
}

RobotAgentWorldModel* MedeaSpecializeConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaSpecializeAgentWorldModel();
}

AgentObserver* MedeaSpecializeConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaSpecializeAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaSpecializeConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaSpecializePerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaSpecializeConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
