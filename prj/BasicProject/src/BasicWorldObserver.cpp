/*
 *  BasicWorldObserver.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 27/05/10.
 *
 */

#include "BasicProject/include/BasicWorldObserver.h"

#include "World/World.h"


BasicWorldObserver::BasicWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

BasicWorldObserver::~BasicWorldObserver()
{
	// nothing to do.
}

void BasicWorldObserver::reset()
{
	// nothing to do.
}

void BasicWorldObserver::step()
{
	// nothing to do.
}


