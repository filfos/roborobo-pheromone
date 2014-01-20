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

#define NNTYPE_MLP			1
#define NNTYPE_PERCEPTRON	2
#define NNTYPE_ELMAN		3
// WARNING : depending on setup, PUNN/HONN may need 2 hidden layers to work well
#define NNTYPE_PUNN			4
#define NNTYPE_ESN			5

#define NNTYPE				NNTYPE_MLP


#include "BasicProject-mEDEA/include/BasicProjectMedeaNNControlArchitecture.h"
#include "World/World.h"
#include "Utilities/Misc.h"
#include <math.h>
#include <string>

#if NNTYPE == NNTYPE_MLP
#include <neuralnetworks/MLP.h>
#elif NNTYPE == NNTYPE_PERCEPTRON
#include <neuralnetworks/Perceptron.h>
#elif NNTYPE == NNTYPE_ELMAN
#include <neuralnetworks/Elman.h>
#elif NNTYPE == NNTYPE_PUNN
#include <neuralnetworks/PUNN.h>
#elif NNTYPE == NNTYPE_ESN
#include <neuralnetworks/ESN.h>
#endif


using namespace Neural;


BasicProjectMedeaNNControlArchitecture::BasicProjectMedeaNNControlArchitecture( RobotAgentWorldModel *wm )
{
	_wm = (BasicProjectMedeaAgentWorldModel*)wm;
	//_wm->_genome.resize(18);
	_iteration = 0;

	//_initialEnergy = 2*BasicProjectMedeaSharedData::gEvaluationTime*0.1;  // original CEC
	//_initialEnergy = 2*BasicProjectMedeaSharedData::gEvaluationTime;  // original CEC, adapte avec nv schema
	//_initialEnergy = BasicProjectMedeaSharedData::gEvaluationTime*0.1 / 2; // half the maximum lifetime.  // used to be: 2*BasicProjectMedeaSharedData::gEvaluationTime*0.1;
	//_initialEnergy = BasicProjectMedeaSharedData::gEvaluationTime*2; // MUST BE AT LEAST BasicProjectMedeaSharedData::gEvaluationTime otw a dead agent will never be revived  // / 2.0; // half the maximum lifetime + nv schema (entier)

	_wm->setEnergyLevel(BasicProjectMedeaSharedData::gEnergyInit);
	//_deltaEnergy = 10.0; // CEC
	_wm->setDeltaEnergy(0.0); // Medea

	_wm->setAngleToClosestEnergyPoint(0);
	_wm->setDistanceToClosestEnergyPoint(0);

	createNN();

	_wm->setActiveStatus(true);

	if ( gVerbose )
	{
		std::cout << "robot #" << _wm->_agentId << " NN \n" ;
	}
}


void BasicProjectMedeaNNControlArchitecture::createNN()
{
#if NNTYPE == NNTYPE_MLP
	nn = new MLP(_parameters, _wm->_nbInputs, _wm->_nbOutputs, *(_wm->_nbNeuronsPerHiddenLayer));
#elif NNTYPE == NNTYPE_PERCEPTRON
	nn = new Perceptron(_parameters, _wm->_nbInputs, _wm->_nbOutputs);
#elif NNTYPE == NNTYPE_ELMAN
	nn = new Elman(_parameters, _wm->_nbInputs, _wm->_nbOutputs, *(_wm->_nbNeuronsPerHiddenLayer));
#elif NNTYPE == NNTYPE_PUNN
	std::vector<unsigned int> nbProductNeuronsPerLayer;
	nn = new PUNN(_parameters, _wm->_nbInputs, _wm->_nbOutputs, PUNN_TOPOLOGY_ALTERNATE,
			*(_wm->_nbNeuronsPerHiddenLayer),
			nbProductNeuronsPerLayer,
			PUNN_PRODUCT_NEURON_NORMALIZATION_ABS,
			false);
#elif NNTYPE == NNTYPE_ESN
	nn = new ESN(_parameters, _wm->_nbInputs, _wm->_nbOutputs,
			100, 0.2, 0.88, 0.5, 0.5, 0.5, 0.5, 0.5,
			false,	// allowInputToOutputDirectConnections
			false,	// allowOutputSelfRecurrentConnections
			true,	// allowInputToReservoirConnections
			false 	// allowOutputToReservoirConnections
			);
#endif
}

unsigned int BasicProjectMedeaNNControlArchitecture::computeRequiredNumberOfWeights(BasicProjectMedeaAgentWorldModel *wm)
{
	NeuralNetwork *a;

#if NNTYPE == NNTYPE_MLP
	a = new MLP(wm->_nbInputs, wm->_nbOutputs, *(wm->_nbNeuronsPerHiddenLayer));
#elif NNTYPE == NNTYPE_PERCEPTRON
	a = new Perceptron(wm->_nbInputs, wm->_nbOutputs);
#elif NNTYPE == NNTYPE_ELMAN
	a = new Elman(wm->_nbInputs, wm->_nbOutputs, *(wm->_nbNeuronsPerHiddenLayer));
#elif NNTYPE == NNTYPE_PUNN
	std::vector<unsigned int> nbProductNeuronsPerLayer;
	a = new PUNN(wm->_nbInputs, wm->_nbOutputs, PUNN_TOPOLOGY_ALTERNATE,
			*(wm->_nbNeuronsPerHiddenLayer),
			nbProductNeuronsPerLayer,
			PUNN_PRODUCT_NEURON_NORMALIZATION_ABS,
			false);
#elif NNTYPE == NNTYPE_ESN
	a = new ESN(wm->_nbInputs, wm->_nbOutputs,
			100, 0.2, 0.88, 0.5, 0.5, 0.5, 0.5, 0.5,
			false,	// allowInputToOutputDirectConnections
			false,	// allowOutputSelfRecurrentConnections
			true,	// allowInputToReservoirConnections
			false 	// allowOutputToReservoirConnections
			);
#endif
	unsigned int res = a->getRequiredNumberOfWeights();
	delete a;
	return res;
}


BasicProjectMedeaNNControlArchitecture::~BasicProjectMedeaNNControlArchitecture()
{
	delete nn;
	nn = NULL;
}

void BasicProjectMedeaNNControlArchitecture::reset()
{
	_parameters.clear();
	_parameters = _wm->_genome;
}


// perform one controller update
// set motor value using motor format.
void BasicProjectMedeaNNControlArchitecture::step()
{
	_iteration++;

	if ( _wm->getNewGenomeStatus() ) // check for new NN parameters
	{
		reset();
		_wm->setNewGenomeStatus(false);
	}

/*	if ( _wm->_age < 0 ) // problem: _age is nowhere to be incremented
	{
		// ** security control (prior to a new behavior, get out of crash situation) -- random noise to avoid "STALL" status
		_wm->_desiredTranslationalValue = ( ranf()*2.-1. ) * gMaxTranslationalSpeed ;
		_wm->_desiredRotationalVelocity =( ranf()*2.-1. ) * gMaxRotationalSpeed ;
		currentObserver->setKey( ( ranf()*2.-1. ) * BasicProjectMedeaSharedData::gMaxKeyRange);
		return;
	}
*/
	_wm->_desiredTranslationalValue = 0.0;
	_wm->_desiredRotationalVelocity = 0.0;

	if ( _wm->getActiveStatus() == true )
	{
		double angleToClosestEnergyPoint = 0.0;
		double shortestDistance = 0.0;
		if ( BasicProjectMedeaSharedData::gExperimentNumber == 2)
		{
			//the only energy point is on the east of the map
			angleToClosestEnergyPoint = 90;  // SOUTH!
		}
		else
		{
			//search the current active energy point

			Point2d posRobot(_wm->_xReal,_wm->_yReal);
//			Point2d closestPoint = gEnergyPoints[BasicProjectMedeaSharedData::gActiveSun].getPosition();
//
//			shortestDistance = getEuclidianDistance (posRobot,closestPoint);
//
//			//compute the orientation of the active sun ( in degree between 0 and 360 )
//			angleToClosestEnergyPoint = (atan2(closestPoint.y-posRobot.y,closestPoint.x-posRobot.x)/M_PI)*180.0;
//			angleToClosestEnergyPoint += 360.0 ;
//			angleToClosestEnergyPoint = computeModulo(angleToClosestEnergyPoint,360.0);
//			if ( angleToClosestEnergyPoint > 180 ) // force btw -180 and 180
//				angleToClosestEnergyPoint -= 360.0;
			angleToClosestEnergyPoint = 0.0;
			shortestDistance = 0.0;
		}

		//compute the angle between the actual orientation of the robot and the orientation of the closest energy point ( in degree between -180 and 180 )
		double diffAngleToClosestEnergyPoint = angleToClosestEnergyPoint -  _wm->_agentAbsoluteOrientation ;
		if ( diffAngleToClosestEnergyPoint < -180.0 )
		{
			diffAngleToClosestEnergyPoint += 360.0 ;
		}
		if ( diffAngleToClosestEnergyPoint > 180.0 )
		{
			diffAngleToClosestEnergyPoint -= 360.0 ;
		}

//		if ( gAgentIndexFocus == _wm->_agentId )
		if ( gVerbose && gInspectAgent && gAgentIndexFocus == _wm->_agentId )
		{
			std::cout << "Sun           : " << diffAngleToClosestEnergyPoint << "°, " << shortestDistance << "mm"<< std::endl;
		}

		//cast the diffAngle between -1 and 1
		diffAngleToClosestEnergyPoint = diffAngleToClosestEnergyPoint / 180.0 ;
		_wm->setEnergyPointDirectionAngleValue(diffAngleToClosestEnergyPoint);

		//cast the shortest distance between 0 and 1
		if ( shortestDistance > gSensorRange )
			shortestDistance = 1.0;
		else
			shortestDistance = shortestDistance / (double)gSensorRange;
		_wm->setEnergyPointDistanceValue(shortestDistance);

//		if ( gAgentIndexFocus == _wm->_agentId )
		if ( gVerbose && gInspectAgent && gAgentIndexFocus == _wm->_agentId )
		{
			std::cout << "SunSensorValue: " << _wm->getEnergyPointDirectionAngleValue() << " , " << _wm->getEnergyPointDistanceValue() << std::endl;
		}


		// ---- Build inputs ----
		std::vector<double>* inputs = new std::vector<double>(_wm->_nbInputs);
		int inputToUse = 0;

		// distance sensors
		for(inputToUse = 0; inputToUse < _wm->_sensorCount; inputToUse++)
			(*inputs)[inputToUse] = _wm->getSensorDistanceValue(inputToUse) / _wm->getSensorMaximumDistanceValue(inputToUse);
		// floor sensor
		if(_wm->_floorSensor != 0)
			(*inputs)[inputToUse++] = 1.0;
		else
			(*inputs)[inputToUse++] = 0.0;
		// direction of the closest energy point
		(*inputs)[inputToUse++] = _wm->getEnergyPointDirectionAngleValue();
		// direction of the closest energy point
		(*inputs)[inputToUse++] = _wm->getEnergyPointDistanceValue();
		// energy level
		(*inputs)[inputToUse++] = _wm->getEnergyLevel() / BasicProjectMedeaSharedData::gEnergyMax;

#if NNTYPE == NNTYPE_PUNN
		// Normalize inputs
		for(size_t i = 0; i < inputs->size(); i++) {
			if((*inputs)[i] < 0.0) {
				if((*inputs)[i] < -0.9999) {
					(*inputs)[i] = -0.9999;
				}
			} else if((*inputs)[i] == 0.0) {
				(*inputs)[i] = 0.0001;
			} else {
				if((*inputs)[i] > 0.9999) {
					(*inputs)[i] = 0.9999;
				}
			}
		}
#endif

		// ---- Create NN ----
		nn->setWeigths(_parameters);
		nn->setInputs(*inputs);
		nn->step();
		std::vector<double> outputs = nn->readOut();
		_wm->_desiredTranslationalValue = outputs[0];
		_wm->_desiredRotationalVelocity = outputs[1];

		// normalize to motor interval values
		_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
		_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;

		// penalize if needed (only experiment no.2, in energy zone)
		if ( BasicProjectMedeaSharedData::gExperimentNumber == 2 && ( (int)_wm->_xReal >= BasicProjectMedeaSharedData::g_xStart_EnergyZone && (int)_wm->_yReal >= BasicProjectMedeaSharedData::g_yStart_EnergyZone && (int)_wm->_xReal <= BasicProjectMedeaSharedData::g_xEnd_EnergyZone && (int)_wm->_yReal <= BasicProjectMedeaSharedData::g_yEnd_EnergyZone ) )
		{
			double penalizationFactor = ( 1.0 - BasicProjectMedeaSharedData::gMaxPenalizationRate )  + BasicProjectMedeaSharedData::gMaxPenalizationRate*(BasicProjectMedeaSharedData::gZoneEnergy_harvestValue/BasicProjectMedeaSharedData::gZoneEnergy_maxHarvestValue);  // based on BasicProjectMedeaSharedData::gZoneEnergy_harvestValue/BasicProjectMedeaSharedData::gZoneEnergy_maxHarvestValue
			_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue*penalizationFactor;    //*(1/(1+exp(-0.17*(_deltaEnergy-5))));
			_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity*penalizationFactor;
		}


	}

}

