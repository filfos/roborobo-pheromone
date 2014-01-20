#if defined PRJ_MEDEA_ECOLOGY || !defined MODULAR

#include "Config/MedeaEcologyConfigurationLoader.h"

#include "mEDEA-ecology/include/MedeaEcologyWorldObserver.h"
#include "mEDEA-ecology/include/MedeaEcologyAgentWorldModel.h"
#include "mEDEA-ecology/include/MedeaEcologyAgentObserver.h"
#include "mEDEA-ecology/include/MedeaEcologyControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaEcologyConfigurationLoader::MedeaEcologyConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaEcologyConfigurationLoader::~MedeaEcologyConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaEcologyConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaEcologyWorldObserver(wm);
}

RobotAgentWorldModel* MedeaEcologyConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaEcologyAgentWorldModel();
}

AgentObserver* MedeaEcologyConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaEcologyAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaEcologyConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaEcologyControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaEcologyConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
