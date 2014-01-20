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



#ifndef BASICPROJECTMEDEAENERGYNNCONTROLARCHITECTURE_H
#define BASICPROJECTMEDEAENERGYNNCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxRoborobo.h"
#include "BehaviorControlArchitectures/BehaviorControlArchitecture.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentWorldModel.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentObserver.h"
#include <neuralnetworks/NeuralNetwork.h>


using namespace Neural;


class BasicProjectMedeaNNControlArchitecture : public BehaviorControlArchitecture
{
	private:

		int _iteration;
		BasicProjectMedeaAgentWorldModel *_wm;

		std::vector<double> _parameters;
		std::string _nnType;
		int _nbHiddenLayers;
		std::vector<int> _nbHiddenNeuronsPerLayer;
		std::vector<int> _nbBiaisNeuronsPerLayer;
		NeuralNetwork* nn;

		void searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints);

		void createNN();


	public:
		//Initializes the variables
		BasicProjectMedeaNNControlArchitecture(RobotAgentWorldModel *wm);
		~BasicProjectMedeaNNControlArchitecture();

		void reset();
		void step();		// if (_wm._age<0) then move randomly (useful to unstuck robot in evolution mode)

		static unsigned int computeRequiredNumberOfWeights(BasicProjectMedeaAgentWorldModel *wm);
};


#endif

