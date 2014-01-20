#if defined PRJ_BASICPHEROMONE || !defined MODULAR

#include "Config/BasicPheromoneConfigurationLoader.h"

#include "BasicPheromone/include/BasicPheromoneWorldObserver.h"
#include "BasicPheromone/include/BasicPheromoneAgentWorldModel.h"
#include "BasicPheromone/include/BasicPheromoneAgentObserver.h"
#include "BasicPheromone/include/BasicPheromoneControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

BasicPheromoneConfigurationLoader::BasicPheromoneConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

BasicPheromoneConfigurationLoader::~BasicPheromoneConfigurationLoader()
{
	//nothing to do
}


WorldObserver* BasicPheromoneConfigurationLoader::make_WorldObserver(World* wm)
{
	return new BasicPheromoneWorldObserver(wm);
}

RobotAgentWorldModel* BasicPheromoneConfigurationLoader::make_RobotAgentWorldModel()
{
	return new BasicPheromoneAgentWorldModel();
}

AgentObserver* BasicPheromoneConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new BasicPheromoneAgentObserver(wm);
}

BehaviorControlArchitecture* BasicPheromoneConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new BasicPheromoneControlArchitecture(wm);
}

RobotAgentWorldInterface* BasicPheromoneConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
