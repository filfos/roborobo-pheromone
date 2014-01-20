#if defined PRJ_MEDEA_SP || !defined MODULAR

#include "Config/MedeaSpConfigurationLoader.h"

#include "mEDEA-sp/include/MedeaSpWorldObserver.h"
#include "mEDEA-sp/include/MedeaSpAgentWorldModel.h"
#include "mEDEA-sp/include/MedeaSpAgentObserver.h"
#include "mEDEA-sp/include/MedeaSpPerceptronControlArchitecture.h"
#include "mEDEA-sp/include/MedeaSpEnergyPoint.h"

#include "World/RobotAgentWorldInterface.h"

MedeaSpConfigurationLoader::MedeaSpConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

MedeaSpConfigurationLoader::~MedeaSpConfigurationLoader()
{
	//nothing to do
}


WorldObserver* MedeaSpConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaSpWorldObserver(wm);
}

RobotAgentWorldModel* MedeaSpConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaSpAgentWorldModel();
}

AgentObserver* MedeaSpConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaSpAgentObserver(wm);
}


BehaviorControlArchitecture * MedeaSpConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaSpPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaSpConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
