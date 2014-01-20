#if defined PRJ_SPIKEANTS || !defined MODULAR

#include "Config/SpikeAntsConfigurationLoader.h"

#include "SpikeAnts/include/SpikeAntsWorldObserver.h"
#include "SpikeAnts/include/SpikeAntsAgentWorldModel.h"
#include "SpikeAnts/include/SpikeAntsAgentObserver.h"
#include "SpikeAnts/include/SpikeAntsControlArchitecture.h"

#include "World/RobotAgentWorldInterface.h"

SpikeAntsConfigurationLoader::SpikeAntsConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

SpikeAntsConfigurationLoader::~SpikeAntsConfigurationLoader()
{
	//nothing to do
}


WorldObserver* SpikeAntsConfigurationLoader::make_WorldObserver(World* wm)
{
	return new SpikeAntsWorldObserver(wm);
}

RobotAgentWorldModel* SpikeAntsConfigurationLoader::make_RobotAgentWorldModel()
{
	return new SpikeAntsAgentWorldModel();
}

AgentObserver* SpikeAntsConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new SpikeAntsAgentObserver(wm);
}

BehaviorControlArchitecture* SpikeAntsConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new SpikeAntsControlArchitecture(wm);
}

RobotAgentWorldInterface* SpikeAntsConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
