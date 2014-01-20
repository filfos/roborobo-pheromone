#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR

#include "Config/MedeaAltruismConfigurationLoader.h"

#include "mEDEA-altruism/include/MedeaAltruismWorldObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismAgentWorldModel.h"
#include "mEDEA-altruism/include/MedeaAltruismAgentObserver.h"
#include "mEDEA-altruism/include/MedeaAltruismPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaAltruismConfigurationLoader::MedeaAltruismConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaAltruismConfigurationLoader::~MedeaAltruismConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaAltruismConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaAltruismWorldObserver(wm);
}

RobotAgentWorldModel* MedeaAltruismConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAltruismAgentWorldModel();
}

AgentObserver* MedeaAltruismConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaAltruismConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaAltruismPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaAltruismConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
