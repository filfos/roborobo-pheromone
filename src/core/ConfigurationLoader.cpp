#include "Config/ConfigurationLoader.h"
#include <string.h>


//-------------------------
//Include under your loaders
#include "Config/StrongCausalityConfigurationLoader.h"
#include "Config/OnePlusOneOnlineConfigurationLoader.h"
#include "Config/BasicConfigurationLoader.h"
#include "Config/MedeaConfigurationLoader.h"
#include "Config/BattleConfigurationLoader.h"
#include "Config/ReplayConfigurationLoader.h"
#include "Config/MedeaTwoSunsConfigurationLoader.h"
#include "Config/MedeaSpecializeConfigurationLoader.h"
#include "Config/MedeaAltruismConfigurationLoader.h"
#include "Config/MedeaAltruismReplayConfigurationLoader.h"
//#include "Config/SpikeAntsConfigurationLoader.h"
#include "Config/MedeaAltUtilityConfigurationLoader.h"
#include "Config/MedeaAltruismReplayPartialEvoConfigurationLoader.h"
#include "Config/MedeaOptimConfigurationLoader.h"
#include "Config/MedeaAltruismReEvoConfigurationLoader.h"
#include "Config/BasicProjectMedeaConfigurationLoader.h"
#include "Config/MedeaCommConfigurationLoader.h"
#include "Config/MedeaSpConfigurationLoader.h"
#include "Config/MedeaHGTConfigurationLoader.h"
#include "Config/MedeaMovingSpConfigurationLoader.h"
#include "Config/BasicPheromoneConfigurationLoader.h"

ConfigurationLoader::ConfigurationLoader()
{
	//nothing to do
}

ConfigurationLoader::~ConfigurationLoader()
{
	//nothing to do
}

ConfigurationLoader* ConfigurationLoader::make_ConfigurationLoader (std::string configurationLoaderObjectName)
{
	if (0)
	{
		// >>> Never meant to be reached
	}
#if defined PRJ_ONEPLUSONE || !defined MODULAR
	else if (configurationLoaderObjectName == "StrongCausalityConfigurationLoader" )
	{
		return new StrongCausalityConfigurationLoader();
	}
#endif
#if defined PRJ_ONEPLUSONE || !defined MODULAR
	else if (configurationLoaderObjectName == "OnePlusOneOnlineConfigurationLoader" )
	{
		return new OnePlusOneOnlineConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaConfigurationLoader" ) // PPSN
	{
		return new MedeaConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA || !defined MODULAR
	else if (configurationLoaderObjectName == "BattleConfigurationLoader" )
	{
		return new BattleConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA || !defined MODULAR
	else if (configurationLoaderObjectName == "ReplayConfigurationLoader" )
	{
		return new ReplayConfigurationLoader();
	}
#endif
#if defined PRJ_BASICPROJECT || !defined MODULAR
else if (configurationLoaderObjectName == "BasicProjectConfigurationLoader" )
{
	return new BasicConfigurationLoader();
}
#endif
#if defined PRJ_MEDEA_TWOSUNS || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaTwoSunsConfigurationLoader" ) // MCMDS
	{
		return new MedeaTwoSunsConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_SPECIALIZE || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaSpecializeConfigurationLoader" )
	{
		return new MedeaSpecializeConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaAltruismConfigurationLoader" )
	{
		return new MedeaAltruismConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaAltruismReplayConfigurationLoader" )
	{
		return new MedeaAltruismReplayConfigurationLoader();
	}
#endif
#if defined PRJ_SPIKEANTS || !defined MODULAR
//	else if (configurationLoaderObjectName == "SpikeAntsConfigurationLoader" )
//	{
//		return new SpikeAntsConfigurationLoader();
//	}
#endif
#if defined PRJ_MEDEA_ALTRUISM_UTILITY || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaAltUtilityConfigurationLoader" )
	{
		return new MedeaAltUtilityConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaAltruismReplayPartialEvoConfigurationLoader" )
	{
		return new MedeaAltruismReplayPartialEvoConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_OPTIM || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaOptimConfigurationLoader" )
	{
		return new MedeaOptimConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_ALTRUISM || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaAltruismReEvoConfigurationLoader" )
	{
		return new MedeaAltruismReEvoConfigurationLoader();
	}
#endif
#if defined PRJ_BASICPROJECT_MEDEA || !defined MODULAR
	else if (configurationLoaderObjectName == "BasicProjectMedeaConfigurationLoader" )
	{
		return new BasicProjectMedeaConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_SP || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaSpConfigurationLoader" )
	{
		return new MedeaSpConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_MOVINGSP || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaMovingSpConfigurationLoader" )
	{
		return new MedeaMovingSpConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_COMM || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaCommConfigurationLoader" )
	{
		return new MedeaCommConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_HGT || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaHGTConfigurationLoader" )
	{
		return new MedeaHGTConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_ECOLOGY || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaEcologyConfigurationLoader" )
	{
		return new MedeaEcologyConfigurationLoader();
	}
#endif
#if defined PRJ_MEDEA_PHEROMONES || !defined MODULAR
	else if (configurationLoaderObjectName == "MedeaPheromonesConfigurationLoader" )
	{
		return new MedeaPheromonesConfigurationLoader();
	}
#endif
#if defined PRJ_BASICPHEROMONE || !defined MODULAR
	else if (configurationLoaderObjectName == "BasicPheromoneConfigurationLoader" )
	{
		return new BasicPheromoneConfigurationLoader();
	}
#endif
	else
	{
		return NULL;
	}

}
