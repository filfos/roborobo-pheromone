#if defined PRJ_BASICPROJECT_MEDEA || !defined MODULAR

#include "Config/BasicProjectMedeaConfigurationLoader.h"

#include "BasicProject-mEDEA/include/BasicProjectMedeaWorldObserver.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentWorldModel.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentObserver.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaNNControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

BasicProjectMedeaConfigurationLoader::BasicProjectMedeaConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

BasicProjectMedeaConfigurationLoader::~BasicProjectMedeaConfigurationLoader()
{
	//nothing to do
}

WorldObserver* BasicProjectMedeaConfigurationLoader::make_WorldObserver(World* wm)
{
	return new BasicProjectMedeaWorldObserver(wm);
}

RobotAgentWorldModel* BasicProjectMedeaConfigurationLoader::make_RobotAgentWorldModel()
{
	return new BasicProjectMedeaAgentWorldModel();
}

AgentObserver* BasicProjectMedeaConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new BasicProjectMedeaAgentObserver(wm);
}

BehaviorControlArchitecture* BasicProjectMedeaConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new BasicProjectMedeaNNControlArchitecture(wm);
}

RobotAgentWorldInterface* BasicProjectMedeaConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface; // pointer to single instance
}

#endif
