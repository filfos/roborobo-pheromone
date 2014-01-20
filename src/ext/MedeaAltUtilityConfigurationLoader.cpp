#if defined PRJ_MEDEA_ALTRUISM_UTILITY || !defined MODULAR

#include "Config/MedeaAltUtilityConfigurationLoader.h"

#include "mEDEA-altruism-utility/include/MedeaAltUtilityWorldObserver.h"
#include "mEDEA-altruism-utility/include/MedeaAltUtilityAgentWorldModel.h"
#include "mEDEA-altruism-utility/include/MedeaAltUtilityAgentObserver.h"
#include "mEDEA-altruism-utility/include/MedeaAltUtilityPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaAltUtilityConfigurationLoader::MedeaAltUtilityConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaAltUtilityConfigurationLoader::~MedeaAltUtilityConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaAltUtilityConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaAltUtilityWorldObserver(wm);
}

RobotAgentWorldModel* MedeaAltUtilityConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaAltUtilityAgentWorldModel();
}

AgentObserver* MedeaAltUtilityConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaAltUtilityAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaAltUtilityConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaAltUtilityPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaAltUtilityConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
