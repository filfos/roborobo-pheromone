#if defined PRJ_MEDEA_HGT || !defined MODULAR

#include "Config/MedeaHGTConfigurationLoader.h"

#include "mEDEA-HGT/include/MedeaHGTWorldObserver.h"
#include "mEDEA-HGT/include/MedeaHGTAgentWorldModel.h"
#include "mEDEA-HGT/include/MedeaHGTAgentObserver.h"
#include "mEDEA-HGT/include/MedeaHGTNNControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaHGTConfigurationLoader::MedeaHGTConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaHGTConfigurationLoader::~MedeaHGTConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaHGTConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaHGTWorldObserver(wm);
}

RobotAgentWorldModel* MedeaHGTConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaHGTAgentWorldModel();
}

AgentObserver* MedeaHGTConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaHGTAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaHGTConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaHGTNNControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaHGTConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface; // pointer to single instance
}

#endif
