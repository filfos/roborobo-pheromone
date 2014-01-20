#if defined PRJ_MEDEA_MOVINGSP || !defined MODULAR

#include "Config/MedeaMovingSpConfigurationLoader.h"

#include "mEDEA-MovingSp/include/MedeaMovingSpWorldObserver.h"
#include "mEDEA-MovingSp/include/MedeaSpAgentWorldModel.h"
#include "mEDEA-MovingSp/include/MedeaMovingSpAgentWorldModel.h"
#include "mEDEA-MovingSp/include/MedeaMovingSpAgentObserver.h"
#include "mEDEA-MovingSp/include/MedeaSpPerceptronControlArchitecture.h"
#include "mEDEA-MovingSp/include/MedeaMovingSpPerceptronControlArchitecture.h"
#include "mEDEA-MovingSp/include/MedeaSpEnergyPoint.h"


#include "mEDEA-MovingSp/include/MedeaSpWorldObserver.h"
#include "mEDEA-MovingSp/include/MedeaSpAgentObserver.h"

#include "World/RobotAgentWorldInterface.h"

MedeaMovingSpConfigurationLoader::MedeaMovingSpConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterface();

}

MedeaMovingSpConfigurationLoader::~MedeaMovingSpConfigurationLoader()
{
	//nothing to do
}


WorldObserver* MedeaMovingSpConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaSpWorldObserver(wm);
	// Segmentation fault
}

RobotAgentWorldModel* MedeaMovingSpConfigurationLoader::make_RobotAgentWorldModel()
{
  return new MedeaSpAgentWorldModel();
  // Bad_alloc
}

AgentObserver* MedeaMovingSpConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaSpAgentObserver(wm);
}


BehaviorControlArchitecture * MedeaMovingSpConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaSpPerceptronControlArchitecture(wm);
	// Segmentation Fault
}

RobotAgentWorldInterface* MedeaMovingSpConfigurationLoader::make_RobotAgentWorldInterface()
{
	return _AgentWorldInterface;
}

#endif
