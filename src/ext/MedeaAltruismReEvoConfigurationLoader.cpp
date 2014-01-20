#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR

#include "Config/MedeaAltruismReEvoConfigurationLoader.h"

#include "mEDEA-altruism/include/MedeaAltruismReEvoWorldObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismAgentWorldModel.h"
#include "mEDEA-altruism/include/MedeaAltruismReEvoAgentObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaAltruismReEvoConfigurationLoader::MedeaAltruismReEvoConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaAltruismReEvoConfigurationLoader::~MedeaAltruismReEvoConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaAltruismReEvoConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaAltruismReEvoWorldObserver(wm);
}

RobotAgentWorldModel* MedeaAltruismReEvoConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAltruismAgentWorldModel();
}

AgentObserver* MedeaAltruismReEvoConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismReEvoAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaAltruismReEvoConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaAltruismReEvoConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
