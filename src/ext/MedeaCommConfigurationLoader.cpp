#if defined PRJ_MEDEA_COMM || !defined MODULAR

#include "Config/MedeaCommConfigurationLoader.h"

#include "mEDEA-comm/include/MedeaCommWorldObserver.h"
#include "mEDEA-comm/include/MedeaCommAgentWorldModel.h"
#include "mEDEA-comm/include/MedeaCommAgentObserver.h"
#include "mEDEA-comm/include/MedeaCommPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaCommConfigurationLoader::MedeaCommConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaCommConfigurationLoader::~MedeaCommConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaCommConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaCommWorldObserver(wm);
}

RobotAgentWorldModel* MedeaCommConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaCommAgentWorldModel();
}

AgentObserver* MedeaCommConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaCommAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaCommConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaCommPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaCommConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
