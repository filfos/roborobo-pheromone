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

#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentWorldModel.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaNNControlArchitecture.h"
#include <neuralnetworks/MLP.h>


using namespace Neural;


BasicProjectMedeaAgentWorldModel::BasicProjectMedeaAgentWorldModel()
{
}

BasicProjectMedeaAgentWorldModel::~BasicProjectMedeaAgentWorldModel()
{
}

double BasicProjectMedeaAgentWorldModel::getEnergyPointDirectionAngleValue()
{
	return _energyPointDirectionAngleValue;
}

double BasicProjectMedeaAgentWorldModel::getEnergyPointDistanceValue()
{
	return _energyPointDistanceValue;
}

void BasicProjectMedeaAgentWorldModel::setEnergyPointDirectionAngleValue( double value )
{
	_energyPointDirectionAngleValue = value;
}

void BasicProjectMedeaAgentWorldModel::setEnergyPointDistanceValue( double value )
{
	_energyPointDistanceValue = value;
}

bool BasicProjectMedeaAgentWorldModel::getActiveStatus()
{
	return _isActive;
}

void BasicProjectMedeaAgentWorldModel::setActiveStatus( bool isActive )
{
	_isActive = isActive;
}

double BasicProjectMedeaAgentWorldModel::getEnergyLevel()
{
	return _energyLevel;
}

void BasicProjectMedeaAgentWorldModel::setEnergyLevel(double inValue)
{
	_energyLevel = inValue;
}

double BasicProjectMedeaAgentWorldModel::getDeltaEnergy()
{
	return _deltaEnergy;
}

void BasicProjectMedeaAgentWorldModel::setDeltaEnergy(double inValue)
{
	_deltaEnergy = inValue;
}

void BasicProjectMedeaAgentWorldModel::setAngleToClosestEnergyPoint( double value )
{
	_angleToClosestEnergyPoint = value;
}

double BasicProjectMedeaAgentWorldModel::getAngleToClosestEnergyPoint()
{
	return _angleToClosestEnergyPoint;
}

void BasicProjectMedeaAgentWorldModel::setDistanceToClosestEnergyPoint( double value )
{
	_distanceToClosestEnergyPoint = value;
}

double BasicProjectMedeaAgentWorldModel::getDistanceToClosestEnergyPoint()
{
	return _distanceToClosestEnergyPoint;
}


void BasicProjectMedeaAgentWorldModel::resetActiveGenome()
{
	_nbInputs = _sensorCount + 4;
	_nbOutputs = 2;
	_nbHiddenLayers = 1;

	_nbNeuronsPerHiddenLayer = new std::vector<unsigned int>(_nbHiddenLayers);
	for(unsigned int i = 0; i < _nbHiddenLayers; i++)
		(*_nbNeuronsPerHiddenLayer)[i] = 5;

	unsigned int const nbGene = BasicProjectMedeaNNControlArchitecture::computeRequiredNumberOfWeights(this);

	std::cout << std::flush ;
	_genome.clear();
	for ( unsigned int i = 0 ; i != nbGene ; i++ )
	{
		_genome.push_back(((rand()%800)/400.0)-1.0);
	}
	_currentGenome = _genome;
	setNewGenomeStatus(true);
	_genomesList.clear();
}

