#if defined PRJ_MEDEA_TWOSUNS || !defined MODULAR

#include "Config/MedeaTwoSunsConfigurationLoader.h"

#include "mEDEA-twosuns/include/MedeaTwoSunsWorldObserver.h"
#include "mEDEA-twosuns/include/MedeaTwoSunsAgentWorldModel.h"
#include "mEDEA-twosuns/include/MedeaTwoSunsAgentObserver.h"
#include "mEDEA-twosuns/include/MedeaTwoSunsPerceptronControlArchitecture.h"
#include "World/RobotAgentWorldInterfaceDangerWorld.h"

MedeaTwoSunsConfigurationLoader::MedeaTwoSunsConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
	_AgentWorldInterface = new RobotAgentWorldInterfaceDangerWorld();
}

MedeaTwoSunsConfigurationLoader::~MedeaTwoSunsConfigurationLoader()
{
	//nothing to do
}

WorldObserver* MedeaTwoSunsConfigurationLoader::make_WorldObserver(World* wm)
{
	return new MedeaTwoSunsWorldObserver(wm);
}

RobotAgentWorldModel* MedeaTwoSunsConfigurationLoader::make_RobotAgentWorldModel()
{
	return new MedeaTwoSunsAgentWorldModel();
}

AgentObserver* MedeaTwoSunsConfigurationLoader::make_AgentObserver(RobotAgentWorldModel* wm)
{
	return new MedeaTwoSunsAgentObserver(wm);
}

BehaviorControlArchitecture* MedeaTwoSunsConfigurationLoader::make_BehaviorControlArchitecture(RobotAgentWorldModel* wm)
{
	return new MedeaTwoSunsPerceptronControlArchitecture(wm);
}

RobotAgentWorldInterface* MedeaTwoSunsConfigurationLoader::make_RobotAgentWorldInterface()
{
		return _AgentWorldInterface; // pointer to single instance
}

#endif
