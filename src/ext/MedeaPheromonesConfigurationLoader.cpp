#if defined PRJ_MEDEA_PHEROMONES || !defined MODULAR

#include "Config/MedeaPheromonesConfigurationLoader.h"

#include "mEDEA-pheromones/include/MedeaPheromonesWorldObserver.h"
#include "mEDEA-pheromones/include/MedeaPheromonesAgentWorldModel.h"
#include "mEDEA-pheromones/include/MedeaPheromonesAgentObserver.h"
#include "mEDEA-pheromones/include/MedeaPheromonesControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaPheromonesConfigurationLoader::MedeaPheromonesConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaPheromonesConfigurationLoader::~MedeaPheromonesConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaPheromonesConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaPheromonesWorldObserver(wm);
}

RobotAgentWorldModel* MedeaPheromonesConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaPheromonesAgentWorldModel();
}

AgentObserver* MedeaPheromonesConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaPheromonesAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaPheromonesConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaPheromonesControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaPheromonesConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
