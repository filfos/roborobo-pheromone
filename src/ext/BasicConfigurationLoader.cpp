#if defined PRJ_BASICPROJECT || !defined MODULAR

#include "Config/BasicConfigurationLoader.h"

#include "BasicProject/include/BasicWorldObserver.h"
#include "BasicProject/include/BasicAgentWorldModel.h"
#include "BasicProject/include/BasicAgentObserver.h"
#include "BasicProject/include/BasicControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

BasicConfigurationLoader::BasicConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

BasicConfigurationLoader::~BasicConfigurationLoader()
{
	//nothing to do
}


WorldObserver* BasicConfigurationLoader::make_WorldObserver(World* wm)
{
	return new BasicWorldObserver(wm);
}

RobotAgentWorldModel* BasicConfigurationLoader::make_RobotAgentWorldModel()
{
	return new BasicAgentWorldModel();
}

AgentObserver* BasicConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new BasicAgentObserver(wm);
}

BehaviorControlArchitecture* BasicConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new BasicControlArchitecture(wm);
}

RobotAgentWorldInterface* BasicConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
