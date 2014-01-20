#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR

#include "Config/MedeaAltruismReplayConfigurationLoader.h"

#include "mEDEA-altruism/include/MedeaAltruismReplayWorldObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismAgentWorldModel.h"
#include "mEDEA-altruism/include/MedeaAltruismReplayAgentObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaAltruismReplayConfigurationLoader::MedeaAltruismReplayConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaAltruismReplayConfigurationLoader::~MedeaAltruismReplayConfigurationLoader()
{
	//nothing to do
}


WorldObserver* MedeaAltruismReplayConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaAltruismReplayWorldObserver(wm);
}

RobotAgentWorldModel* MedeaAltruismReplayConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAltruismAgentWorldModel();
}

AgentObserver* MedeaAltruismReplayConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismReplayAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaAltruismReplayConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaAltruismReplayConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
