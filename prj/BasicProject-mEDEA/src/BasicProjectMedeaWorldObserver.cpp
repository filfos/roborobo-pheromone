/**
 * @file
 * @author Nicolas Bredeche <nicolas.bredeche@gmail.com>
 * @author Jean-Marc Montanier <montanier.jeanmarc@gmail.com>
 * @author Leo Cazenille <leo.cazenille@gmail.com>
 * TODO : Add formal description
 * History :
 *  - imported from project mEDEA-twosuns on 10/05/2011
 *
 *
 */


#include "Observers/AgentObserver.h"
#include "Observers/WorldObserver.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaWorldObserver.h"
#include "World/World.h"


BasicProjectMedeaWorldObserver::BasicProjectMedeaWorldObserver( World* world ) : WorldObserver( world )
{

	_world = world;


	// ==== loading project-specific properties

	gProperties.checkAndGetPropertyValue("gSwarmOnlineObsUsed",&BasicProjectMedeaSharedData::gSwarmOnlineObsUsed,true);
	gProperties.checkAndGetPropertyValue("gDynamicSigma",&BasicProjectMedeaSharedData::gDynamicSigma,true);
	gProperties.checkAndGetPropertyValue("gSigmaMin",&BasicProjectMedeaSharedData::gSigmaMin,true);
	gProperties.checkAndGetPropertyValue("gProbAdd",&BasicProjectMedeaSharedData::gProbAdd,true);
	gProperties.checkAndGetPropertyValue("gProbSub",&BasicProjectMedeaSharedData::gProbSub,true);
	gProperties.checkAndGetPropertyValue("gDynaStep",&BasicProjectMedeaSharedData::gDynaStep,true);
	gProperties.checkAndGetPropertyValue("gSigmaRef",&BasicProjectMedeaSharedData::gSigmaRef,true);
	gProperties.checkAndGetPropertyValue("gSigmaMax",&BasicProjectMedeaSharedData::gSigmaMax,true);
	gProperties.checkAndGetPropertyValue("gProbRef",&BasicProjectMedeaSharedData::gProbRef,true);
	gProperties.checkAndGetPropertyValue("gProbMax",&BasicProjectMedeaSharedData::gProbMax,true);
	gProperties.checkAndGetPropertyValue("gEvaluationTime",&BasicProjectMedeaSharedData::gEvaluationTime,true);
	gProperties.checkAndGetPropertyValue("gDriftEvaluationRate",&BasicProjectMedeaSharedData::gDriftEvaluationRate,true);
	gProperties.checkAndGetPropertyValue("gInitLock",&BasicProjectMedeaSharedData::gInitLock,true);
	gProperties.checkAndGetPropertyValue("gDriftLock",&BasicProjectMedeaSharedData::gDriftLock,true);
	gProperties.checkAndGetPropertyValue("gMaxKeyRange",&BasicProjectMedeaSharedData::gMaxKeyRange,true);
	gProperties.checkAndGetPropertyValue("gDeltaKey",&BasicProjectMedeaSharedData::gDeltaKey,true);
	gProperties.checkAndGetPropertyValue("gSynchronization",&BasicProjectMedeaSharedData::gSynchronization,true);

	gProperties.checkAndGetPropertyValue("gExperimentNumber",&BasicProjectMedeaSharedData::gExperimentNumber,true);
	gProperties.checkAndGetPropertyValue("gExperiment1_genStart",&BasicProjectMedeaSharedData::gExperiment1_genStart,true);
	gProperties.checkAndGetPropertyValue("gExperiment2_genStart",&BasicProjectMedeaSharedData::gExperiment2_genStart,true);

	gProperties.checkAndGetPropertyValue("g_xStart_EnergyZone",&BasicProjectMedeaSharedData::g_xStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yStart_EnergyZone",&BasicProjectMedeaSharedData::g_yStart_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_xEnd_EnergyZone",&BasicProjectMedeaSharedData::g_xEnd_EnergyZone,true);
	gProperties.checkAndGetPropertyValue("g_yEnd_EnergyZone",&BasicProjectMedeaSharedData::g_yEnd_EnergyZone,true);

	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxHarvestValue",&BasicProjectMedeaSharedData::gZoneEnergy_maxHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_minHarvestValue",&BasicProjectMedeaSharedData::gZoneEnergy_minHarvestValue,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_maxFullCapacity",&BasicProjectMedeaSharedData::gZoneEnergy_maxFullCapacity,true);
	gProperties.checkAndGetPropertyValue("gZoneEnergy_saturateCapacityLevel",&BasicProjectMedeaSharedData::gZoneEnergy_saturateCapacityLevel,true);

	gProperties.checkAndGetPropertyValue("gEnergyMax",&BasicProjectMedeaSharedData::gEnergyMax,true);
	gProperties.checkAndGetPropertyValue("gEnergyInit",&BasicProjectMedeaSharedData::gEnergyInit,true);
	gProperties.checkAndGetPropertyValue("gEnergyRevive",&BasicProjectMedeaSharedData::gEnergyRevive,true);

	gProperties.checkAndGetPropertyValue("gMaxPenalizationRate",&BasicProjectMedeaSharedData::gMaxPenalizationRate,true);
	if ( BasicProjectMedeaSharedData::gMaxPenalizationRate < 0 || BasicProjectMedeaSharedData::gMaxPenalizationRate > 1 )
	{
		std::cerr << "gMaxPenalizationRate should be defined btw O and 1" << std::endl;
		exit(-1);
	}

	gProperties.checkAndGetPropertyValue("gDynamicRespawn",&BasicProjectMedeaSharedData::gDynamicRespawn,true); // forced, in this setup.


	// ====

	if ( !gRadioNetwork)
	{
		std::cout << "Error : gRadioNetwork == false. The swarm online observer need the radio network" << std::endl;
		exit(1);
	}

	if ( !BasicProjectMedeaSharedData::gSwarmOnlineObsUsed)
	{
		std::cout << "Error : gSwarmOnlineObsUsed == false. The swarm online observer need some variables. Define this option to true or use another observer" << std::endl;
		exit(1);
	}

	if ( !gEnergyMode )
	{
		std::cout << "Error : gEnergyMode should be true to use SwarmOnlineObserver" << std::endl;
		exit(1);
	}

	// * iteration and generation counters

	_lifeIterationCount = -1;
	_generationCount = -1;

	std::cout << std::endl << "#### Experiment no.0 starts now. ####" << std::endl;
}

BasicProjectMedeaWorldObserver::~BasicProjectMedeaWorldObserver()
{
	// nothing to do.
}

void BasicProjectMedeaWorldObserver::reset()
{
	// ...
}

void BasicProjectMedeaWorldObserver::step()
{

	// ***
	// * update iteration and generation counters + switch btw experimental setups if required
	// ***

	_lifeIterationCount++;

	if( _lifeIterationCount >= BasicProjectMedeaSharedData::gEvaluationTime ) // go to next generation.
	{
		// * monitoring: count number of active agents.

		int activeCount = 0;
		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			if ( (dynamic_cast<BasicProjectMedeaAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()))->getActiveStatus() == true )
				activeCount++;
		}

		if ( !gVerbose )
		{
			std::cout << "[" << activeCount << "]";
		}

		gLogFile << "Info(" << gWorld->getIterations() << ") : active count is " << activeCount << std::endl;


		// * monitor and log orientation and distance to center for all agents

		// build heatmap

		int heatmapSize = 100; // arbitrary, but should be enough to get precise view
		int maxDistance = sqrt ( gAreaWidth*gAreaWidth + gAreaHeight*gAreaHeight ) / 2.0 ; // distance max to center.

		int *distanceHeatmap;
		distanceHeatmap = new int[heatmapSize];
		int *orientationHeatmap;
		orientationHeatmap = new int[heatmapSize];

		for (int i = 0 ; i != heatmapSize ; i++ )
		{
			distanceHeatmap[i] = 0;
			orientationHeatmap[i] = 0;
		}

		double xRef = gAreaWidth / 2.0 ;
		double yRef = gAreaHeight / 2.0 ;

		for ( int i = 0 ; i != gAgentCounter ; i++ )
		{
			BasicProjectMedeaAgentWorldModel *wm = (dynamic_cast<BasicProjectMedeaAgentWorldModel*>(gWorld->getAgent(i)->getWorldModel()));
			if ( wm->getActiveStatus() == true ) // only active agents
			{
				// monitor distance

				double dist = getEuclidianDistance( xRef, yRef, wm->_xReal, wm->_yReal );
				int indexDist = (int)dist * heatmapSize / maxDistance; // normalize within heatmap bounds
				distanceHeatmap[indexDist]++;

				// monitor orientationHeatmap

				double orient = acos ( ( wm->_xReal - xRef ) / (double)dist );
				if ( wm->_yReal - yRef > 0 ) // [trick] why ">0" ?  : it should be <0, but as the 2D-display norm for the coordinate system origin is upper-left (and not bottom-left), the sign is inversed.
				{
					orient = -orient;
				}

				int indexOrient = ( orient + M_PI ) * heatmapSize / (2.0 * M_PI);
				orientationHeatmap[heatmapSize-1-((indexOrient+((heatmapSize*3)/4))%heatmapSize)]++; // index is such that list ordering is as follow: [South-West-North-East-South]

				//std::cout << "\n agent[" << wm->_xReal << "," << wm->_yReal << "](" << dist << "," << orient << ") -- " << wm->_xReal << ";" << xRef << ";" << ( ( wm->_xReal - xRef ) / (double)dist ) << ";" << acos ( ( wm->_xReal - xRef ) / (double)dist ); //debug
			}
		}
		//std::cout << "\n"; //debug


		// update log file

		std::string str_agentDistancesToRef ="";
		std::string str_agentOrientationsToRef = "";

		for (int i = 0 ; i != heatmapSize ; i++ )
		{
			str_agentDistancesToRef += convertToString(distanceHeatmap[i]);
			str_agentDistancesToRef += ",";
			str_agentOrientationsToRef += convertToString(orientationHeatmap[i]);
			str_agentOrientationsToRef += ",";
		}

		//gLogFile
		//std::cout << std::endl << std::endl;//debug
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor distance to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentDistancesToRef << std::endl;
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor orientation to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentOrientationsToRef << std::endl;

		for ( int i = 0 ; i != heatmapSize ; i++ )
		{
			gLogFile << "monitorDistance: " << gAgentCounter << "," << (int)sqrt(gMaxRadioDistanceToSquare) << "," << (_generationCount+1) << "," << i << "," << distanceHeatmap[i] << std::endl;
			gLogFile << "monitorOrientation: " << gAgentCounter << "," << (int)sqrt(gMaxRadioDistanceToSquare) << "," << (_generationCount+1) << "," << i << "," << orientationHeatmap[i] << std::endl;
		}
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor distance to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentDistancesToRef << std::endl;
		//gLogFile << "Info(" << gWorld->getIterations() << ") : monitor orientation to ref - " << (_generationCount+1) << "," << gAgentCounter << "," << str_agentOrientationsToRef << std::endl;

		delete [] distanceHeatmap;
		delete [] orientationHeatmap;

		// * update iterations and generations counters

		_lifeIterationCount = 0;
		_generationCount++;

		// * Switch btw experiment setups, if required

		updateExperimentalSettings();

	}

	// * Update environment status wrt. nature and availability of energy resources (update at each iteration)

	updateEnvironmentResources();

	// * update energy level for each agents (ONLY active agents)

	updateAllAgentsEnergyLevel();

	// prepape and take screenshot of ultimate iteration

	if ( gWorld->getIterations() == gMaxIt-2 )
	{
		gDisplayMode = 0;
		gDisplaySensors = true; // prepape for next it.
	}
	else
	{
		if ( gWorld->getIterations() == gMaxIt-1 )
			saveScreenshot("lastIteration");
	}

}



// ***
// * Check and update (ONLY IF REQUIRED) experimental setup.
// ***
void BasicProjectMedeaWorldObserver::updateExperimentalSettings()
{
	// ...
}

// ***
// * Update environment status wrt. nature and availability of energy resources (update at each iteration)
// ***
void BasicProjectMedeaWorldObserver::updateEnvironmentResources()
{
	// ...
}


// *
// * update energy level for each agents (ONLY active agents) (only in experimental setups featuring energy items)
// *
void BasicProjectMedeaWorldObserver::updateAllAgentsEnergyLevel()
{
	// ...
}


int BasicProjectMedeaWorldObserver::getLifeIterationCount()
{
	return _lifeIterationCount;
}

