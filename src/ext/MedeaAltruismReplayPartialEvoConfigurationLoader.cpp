#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR

#include "Config/MedeaAltruismReplayPartialEvoConfigurationLoader.h"

#include "mEDEA-altruism/include/MedeaAltruismReplayPartialEvoWorldObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismReplayPartialEvoAgentWorldModel.h"
#include "mEDEA-altruism/include/MedeaAltruismReplayPartialEvoAgentObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaAltruismReplayPartialEvoConfigurationLoader::MedeaAltruismReplayPartialEvoConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaAltruismReplayPartialEvoConfigurationLoader::~MedeaAltruismReplayPartialEvoConfigurationLoader()
{
	//nothing to do
}


WorldObserver* MedeaAltruismReplayPartialEvoConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaAltruismReplayPartialEvoWorldObserver(wm);
}

RobotAgentWorldModel* MedeaAltruismReplayPartialEvoConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAltruismReplayPartialEvoAgentWorldModel();
}

AgentObserver* MedeaAltruismReplayPartialEvoConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismReplayPartialEvoAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaAltruismReplayPartialEvoConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaAltruismReplayPartialEvoConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
