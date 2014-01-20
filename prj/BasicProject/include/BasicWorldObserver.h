/*
 *  BasicWorldObserver.h
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */



#ifndef BASICWORLDOBSERVER_H
#define BASICWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"

class World;

class BasicWorldObserver : public WorldObserver
{
	protected:
		
	public:
		BasicWorldObserver( World *__world );
		~BasicWorldObserver();
				
		void reset();
		void step();
		
};

#endif

