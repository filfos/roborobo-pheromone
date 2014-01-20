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




#ifndef BASICPROJECTMEDEAWORLDOBSERVER_H
#define BASICPROJECTMEDEAWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/Observer.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaSharedData.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaAgentWorldModel.h"
#include "BasicProject-mEDEA/include/BasicProjectMedeaSharedData.h"


//class World;

class BasicProjectMedeaWorldObserver : public WorldObserver
{
	private:
		void updateExperimentalSettings(); // check and update (only if required) experimental setting.
		void updateEnvironmentResources();
		void updateAllAgentsEnergyLevel();

	protected:
		int _generationCount;
		int _lifeIterationCount;

	public:
		BasicProjectMedeaWorldObserver(World *world);
		~BasicProjectMedeaWorldObserver();

		void reset();
		void step();

		int getLifeIterationCount();

};

#endif
