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


#ifndef BASICPROJECTMEDEAEVOLVINGROBOTAGENTWORLDMODEL_H
#define BASICPROJECTMEDEAEVOLVINGROBOTAGENTWORLDMODEL_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include <vector>


//class World;

class BasicProjectMedeaAgentWorldModel : public EvolvingRobotAgentWorldModel
{
	protected:

		double _energyPointDirectionAngleValue; // angle to closest energy point -- normalized btw -1 and +1
		double _energyPointDistanceValue ; // distance to the closest energy point -- normalized btw 0  and 1

		bool _isActive; // agent stand still if not.

		double _energyLevel;
		double _deltaEnergy;

		double _angleToClosestEnergyPoint;
		double _distanceToClosestEnergyPoint;



	public:
		//Initializes the variables
		BasicProjectMedeaAgentWorldModel();
		~BasicProjectMedeaAgentWorldModel();

		double getEnergyPointDirectionAngleValue();
		void setEnergyPointDistanceValue(double value);

		double getEnergyPointDistanceValue();
		void setEnergyPointDirectionAngleValue(double value);

		double getEnergyLevel();
		void setEnergyLevel(double inValue);

		double getDeltaEnergy();
		void setDeltaEnergy(double inValue);

		bool getActiveStatus();
		void setActiveStatus(bool isActive);

		void setAngleToClosestEnergyPoint(double value);
		double getAngleToClosestEnergyPoint();

		void setDistanceToClosestEnergyPoint(double value);
		double getDistanceToClosestEnergyPoint();


		// evolutionary engine

		std::map<int, std::vector<double> > _genomesList;
		std::map<int, float > _sigmaList;
		std::vector<double> _currentGenome;
		float _currentSigma;

		void resetActiveGenome();


		// ANN
		double _minValue;
		double _maxValue;
		unsigned int _nbInputs;
		unsigned int _nbOutputs;
		unsigned int _nbHiddenLayers;
		std::vector<unsigned int>* _nbNeuronsPerHiddenLayer;
};


#endif

