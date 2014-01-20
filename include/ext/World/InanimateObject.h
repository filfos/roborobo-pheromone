/*
 *  InanimateObject.h
 *  roborobo
 *
 *  Created by Nicolas on 29/4/2011.
 *
 */

#ifndef INANIMATEOBJECT_H
#define INANIMATEOBJECT_H

#include "RoboroboMain/common.h"
#include "Utilities/Misc.h"

/**
 InanimateObject can be added by the user through the World->addObject(...) method. Note that EnergyPoints (derived from InanimateObject) can be automaticaly created through the config files. Other objects must be added manually.
 */

class InanimateObject
{
	private :
		int _id;
		static int _nextId;

	
	protected : 
	
		// coordinates
		Point2d _position;
		Sint16 _xCenterPixel;
		Sint16 _yCenterPixel;

		bool _visible; // display option (default: true)

	public :

		InanimateObject();
		InanimateObject(int id);
		~InanimateObject();

		void setPosition(Point2d position);
		Point2d getPosition();

		virtual void step();

		virtual void display();
		virtual void hide();
	
		int getId();	
};

#endif
