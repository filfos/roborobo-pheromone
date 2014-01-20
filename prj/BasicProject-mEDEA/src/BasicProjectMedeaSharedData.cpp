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


#include "BasicProject-mEDEA/include/BasicProjectMedeaSharedData.h"



// ----- ----- ----- ----- -----



bool BasicProjectMedeaSharedData::gSwarmOnlineObsUsed = false; // define if the SwarmOnlineObserver is used. If it's the case, the following parameter have to be defined and gEnergyMode should be true
bool BasicProjectMedeaSharedData::gDynamicSigma = false;
double BasicProjectMedeaSharedData::gSigmaMin = 0.0;
double BasicProjectMedeaSharedData::gProbSub = 0.0;
double BasicProjectMedeaSharedData::gProbAdd = 0.0;
double BasicProjectMedeaSharedData::gDynaStep = 0.0;
double BasicProjectMedeaSharedData::gSigmaRef = 0.0; // reference value of sigma
double BasicProjectMedeaSharedData::gSigmaMax = 0.0; // maximal value of sigma
double BasicProjectMedeaSharedData::gProbRef = 0.0; // probability of transmitting the current genome mutated with sigma ref
double BasicProjectMedeaSharedData::gProbMax = 0.0; // probability of transmitting the current genome mutatued withe sigma ref
int BasicProjectMedeaSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

//int BasicProjectMedeaSharedData::gMaxEvaluation = 0; // Roughly how many controllers will be evaluated on a robot. Since there is some restart procedure because of the energy, it might happen that more evaluation take place.

double BasicProjectMedeaSharedData::gDriftEvaluationRate = 0.0;
double BasicProjectMedeaSharedData::gInitLock = 0.0;
double BasicProjectMedeaSharedData::gDriftLock = 0.0;
double BasicProjectMedeaSharedData::gMaxKeyRange = 0.0;
double BasicProjectMedeaSharedData::gDeltaKey = 0.0;
bool BasicProjectMedeaSharedData::gSynchronization = true;
// VALUE DEFINED IN CONSTRUCTOR (below)

int BasicProjectMedeaSharedData::gExperimentNumber = 0;
int BasicProjectMedeaSharedData::gExperiment1_genStart = 10;
int BasicProjectMedeaSharedData::gExperiment2_genStart = 20;

int BasicProjectMedeaSharedData::g_xStart_EnergyZone = 0;
int BasicProjectMedeaSharedData::g_xEnd_EnergyZone = 0;
int BasicProjectMedeaSharedData::g_yStart_EnergyZone = 0;
int BasicProjectMedeaSharedData::g_yEnd_EnergyZone = 0;

double BasicProjectMedeaSharedData::gZoneEnergy_harvestValue = 0; // set in the code, depends on the following params
double BasicProjectMedeaSharedData::gZoneEnergy_maxHarvestValue;
double BasicProjectMedeaSharedData::gZoneEnergy_minHarvestValue;
int BasicProjectMedeaSharedData::gZoneEnergy_maxFullCapacity;
int BasicProjectMedeaSharedData::gZoneEnergy_saturateCapacityLevel;

double BasicProjectMedeaSharedData::gEnergyMax;
double BasicProjectMedeaSharedData::gEnergyRevive;
double BasicProjectMedeaSharedData::gEnergyInit;

bool BasicProjectMedeaSharedData::gDynamicRespawn;


double BasicProjectMedeaSharedData::gMaxPenalizationRate;

// ----- ----- ----- ----- ----- 

bool BasicProjectMedeaSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

// ----- ----- ----- ----- ----- 
