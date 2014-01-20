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

#ifndef BASICPROJECTMEDEAAGENTOBSERVER_H
#define BASICPROJECTMEDEAAGENTOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/AgentObserver.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentWorldModel.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaSharedData.h"


class BasicProjectMedeaAgentObserver : public AgentObserver
{
	protected:
		BasicProjectMedeaAgentWorldModel *_wm;

		void pickRandomGenome();
		void mutateWithBouncingBounds(float sigma);

	public:
		BasicProjectMedeaAgentObserver(RobotAgentWorldModel *wm);
		~BasicProjectMedeaAgentObserver();

		void reset();
		void step();

		void writeGenome(std::vector<double> genome, int senderId, float sigma);

};

#endif

