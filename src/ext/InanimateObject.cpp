#include "World/InanimateObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "Utilities/SDL_gfxPrimitives.h"

#include "World/World.h"


int InanimateObject::_nextId = 0;

InanimateObject::InanimateObject()
{
	_id = InanimateObject::_nextId; // set unique id.
	InanimateObject::_nextId++; // update id reference.
	
	_visible=true;
}

InanimateObject::InanimateObject(int id)
{
	_id = id;
	_visible=true;
}

InanimateObject::~InanimateObject()
{
	// called if deleted from the list of inanimatedObject
}

int InanimateObject::getId()
{
	return _id;
}

void InanimateObject::step()
{
	if ( _visible && gDisplayMode == 0 )
	{
		display();
	}
}

void InanimateObject::display() // display on screen (called in the step() method if gDisplayMode=0 and _visible=true)
{
	// ...
}

void InanimateObject::hide(){}

void InanimateObject::setPosition (Point2d __position) // note: if your object is registered (ie. physically in the envt), you should be careful to handle registration update while moving it.
{
	_position = __position;
	_xCenterPixel = __position.x;
	_yCenterPixel = __position.y;
}


Point2d InanimateObject::getPosition()
{
	return _position;
}
