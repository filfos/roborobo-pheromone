/*
 *  RobotAgent.cpp
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Config/GlobalConfigurationLoader.h"

#include "Agents/RobotAgent.h"
#include "WorldModels/EvolvingRobotAgentWorldModel.h"
#include "BehaviorControlArchitectures/EnergyPerceptronControlArchitecture.h"


RobotAgent::RobotAgent( World *__world )
{
	_wm = gConfigurationLoader->make_RobotAgentWorldModel(); // TODO: externalize object referenced to create the new instance

	_wm->_world = __world;

	_wm->_agentId = gAgentCounter;
	gAgentCounter++;
	
	//Process agent specification (ie. IR/US/laser sensors)
	
	// create dynamic array
	// parse image and add (sensor.x/y, orientation) or (sensorStart.x/y, sensorEnd.x/y)	
	
	_wm->_sensorCount = 0;


	//count sensors
	for ( int x = 0 ; x != gAgentWidth ; x++ ) // image is analysed a first time to count the nuber of sensors (faster than dynamically re-allocating array size for every new sensor)
		for ( int y = 0 ; y != gAgentHeight ; y++ )
		{
			Uint32 pixel = getPixel32( gAgentSpecsImage , x , y);
			if ( pixel != SDL_MapRGB( gAgentSpecsImage->format, 0xFF, 0xFF, 0xFF ) ) 
				_wm->_sensorCount++;
		}

	_wm->_sensors = new double[_wm->_sensorCount][7]; // see header for details.
	for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
		_wm->_sensors[i][0] = -1;
	//int sensorIt = 0;
	
	//register sensors
	for ( int x = 0 ; x != gAgentWidth ; x++ )
		for ( int y = 0 ; y != gAgentHeight ; y++ )
		{
			Uint32 pixel = getPixel32( gAgentSpecsImage , x , y);
			if ( pixel != SDL_MapRGB( gAgentSpecsImage->format, 0xFF, 0xFF, 0xFF ) ) 
			{
				// sensor found, register sensor.
				
				Uint8 r, g, b;
				SDL_GetRGB(pixel,gAgentSpecsImage->format,&r,&g,&b); 

				if ( _wm->_sensors[r][0] != -1 )
				{
					std::cout << "[ERROR] robot sensor id already in use -- check agent specification image." << std::endl;	
					exit(-1);
				}

				if ( r >= _wm->_sensorCount )
				{
					std::cout << "[ERROR] robot sensor id is not permitted (must be defined btw 0 and " << (_wm->_sensorCount-1) << ", got: " << r<< ") -- check agent specification image." << std::endl;
					exit(-1);
				}

				
				_wm->_sensors[r][0] = r; // no. sensor
				
				// sensor origini point location wrt. agent center 
				_wm->_sensors[r][1] = sqrt ( (x-gAgentWidth/2) * (x-gAgentWidth/2) + (y-gAgentHeight/2) * (y-gAgentHeight/2) ); // length
				double angleCosinus = ( (x-(gAgentWidth/2)) / _wm->_sensors[r][1] );
				double angleSinus   =  ( (y-(gAgentHeight/2)) / _wm->_sensors[r][1] );
				if (  angleSinus >= 0 )
					_wm->_sensors[r][2] = acos ( angleCosinus ) + M_PI/2 ; // angle (in radian)
				else
					_wm->_sensors[r][2] = -acos ( angleCosinus ) + M_PI/2 + M_PI*2 ; // angle (in radian)

				// sensor target point location wrt. agent center -- sensor target angle is (green+blue) component values
				double angle = g+b-90;   // note: '-90deg' is due to image definition convention (in image, 0° means front of agent, which is upward -- while 0° in simulation means facing right) 				
				double xTarget = ( x - gAgentWidth/2 ) + cos ( angle * M_PI / 180) * gSensorRange;
				double yTarget = ( y - gAgentHeight/2 ) + sin ( angle * M_PI / 180) * gSensorRange;
				_wm->_sensors[r][3] = sqrt ( xTarget*xTarget + yTarget*yTarget); // length (**from agent center**)
				angleCosinus = xTarget / _wm->_sensors[r][3]; 
				angleSinus   = yTarget / _wm->_sensors[r][3];
				if ( angleSinus >= 0 )
					_wm->_sensors[r][4] = acos ( angleCosinus ) + M_PI/2; // angle (in radian) wrt. agent center
				else
					_wm->_sensors[r][4] = -acos ( angleCosinus ) + M_PI/2 + M_PI*2;
				r++;
			}
		}
	
	_agentObserver = gConfigurationLoader->make_AgentObserver(_wm);		// TODO: externalize
	_behavior = gConfigurationLoader->make_BehaviorControlArchitecture(_wm);		// TODO: externalize

	reset();	

}


RobotAgent::~RobotAgent()
{
	delete _behavior;
	delete _agentObserver;
}

void RobotAgent::reset()
{
    //Initialize general
	_iterations = 0;
	_wm->_isAlive = true;


/**/
	int x = 0 , y = 0;

	std::stringstream out;
	out << _wm->_agentId;

	std::string str_Xcoord = "";
	str_Xcoord += "agent[";
	str_Xcoord += out.str();
	str_Xcoord += "].x";

	std::string str_Ycoord = "";
	str_Ycoord = "agent[";
	str_Ycoord += out.str();
	str_Ycoord += "].y";

	if ( gProperties.hasProperty( str_Xcoord ) == true && gProperties.hasProperty( str_Xcoord ) == true )
	{
		convertFromString<int>(x, gProperties.getProperty( str_Xcoord ), std::dec);
		convertFromString<int>(y, gProperties.getProperty( str_Ycoord ), std::dec);
	}
	else
	{	
		if ( gVerbose) 
			{
				std::cout << "[warning] Agent #" << _wm->_agentId << " X and/or Y initial coordinates are missing. Random pick among possible location." << std::endl;
				// note: infinite calls may occur if no location possible. 
			}

		int callCounter = 0;
		int maxAllowedCalls = 100;
			
		bool valid;

		do {
			valid = true;
		
			// pick random coordinate
			
			x = (int)(ranf() * (double)gAreaWidth);
			y = (int)(ranf() * (double)gAreaHeight);
			
			// check for agents superposition - ie. if picked position is valid vs. already located agents.
			for ( int i = 0 ; i != _wm->_agentId ; i++ )
			{
				if ( ( abs((double)x - gWorld->getAgent(i)->_wm->_xReal) <= gAgentWidth+1 ) && ( abs((double)y - gWorld->getAgent(i)->_wm->_yReal) <= gAgentHeight+1 ) ) // uses square boxes as location approximation
				{
					valid = false;
					//std::cout << "failed: agent #" << _wm->_agentId << "is too close to agent #" << i << std::endl;
					break; // terminate for statement.
				}
			}

			if ( valid == false )
			{
				continue; // no need to perform next check...
			}
			
			// check if position is valid in environment
			
			for ( int i = x - gAgentWidth/2 ; i <= x + gAgentWidth/2 ; i++ )
			{
				for ( int j = y - gAgentHeight/2 ; j <= y + gAgentHeight/2 ; j++ ) // && valid == true
				{
					// get pixel values
					Uint8 r, g, b;
					Uint32 pixel = getPixel32( gEnvironmentImage, i , j);
					SDL_GetRGB(pixel,gEnvironmentImage->format,&r,&g,&b); 
					
					int color = ((r<<16)+(g<<8)+b);
					
					// check if empty
					if ( color != ((255<<16)+(255<<8)+255) ) // r=robot, g=obstacle/object, b=unused
					{
						//std::cout << "faux: " << i << "," << j << " - " << ((r<<16)+(g<<8)+b) << std::endl; //<< " - " << (int)r << " " << (int)g << " " << (int)b << std::endl;
						valid = false; 
					}
					//else std::cout << "ok!: "  << j << "," << j << " - " << ((r<<16)+(g<<8)+b) << std::endl; //<< " - " << (int)r << " " << (int)g << " " << (int)b << std::endl;
				}
			}

			callCounter++;
				
		} while ( valid == false && callCounter < maxAllowedCalls );
			
		if ( callCounter == 100 )
		{
			if ( gVerbose ) 
				std::cerr << "[CRITICAL] Random initialization of initial position for agent #" << _wm->_agentId << " after trying " << maxAllowedCalls << " random picks (all failed). There may be too few (none?) possible locations (you may try to manually set initial positions).";
			exit (-1);
		}
		else
		{
			if ( gVerbose ) 
				std::cout << "[info] Random initialization of initial position for agent #" << _wm->_agentId << " succeeded after trying " << callCounter << " random pick(s).";
		}
	}

	setCoordReal(x,y);

	//_wm->_xReal = x;
	//_wm->_yReal = y;
/**/

    //Initialize coordinate and displacement
	//_wm->_xReal=gAgentXStart;
	//_wm->_yReal=gAgentYStart;
	_xDelta = 0;
	_yDelta = 0;
	
	//Initialize internal variables	    
	_xDeltaReal=0;//gAgentXStart;
	_yDeltaReal=0;//gAgentYStart;
		
	std::string str_orient = "";
	str_orient = "agent[";
	str_orient += out.str();
	str_orient += "].orientation";
	if ( gProperties.hasProperty( str_orient ) )
		convertFromString<double>(_wm->_agentAbsoluteOrientation, gProperties.getProperty( str_orient ), std::dec);
	else
	{
		if ( gVerbose) 
		{
			std::cout << "[warning] Initial orientation  for agent #" << _wm->_agentId << " not found. Random pick." << std::endl;
		}
		_wm->_agentAbsoluteOrientation = ranf() * 360. - 180.;
	}
	
	_wm->_agentAbsoluteLinearSpeed = 0;

	_wm->_desiredRotationalVelocity = 0;
	_wm->_desiredTranslationalValue = 0;
	
	_wm->_maxRotationalDeltaValue = gMaxRotationalSpeed; // ie. change will be instantenous
	_wm->_maxTranslationalDeltaValue = gMaxTranslationalDeltaValue; //gMaxTranslationalSpeed ; // idem.
	
	_wm->_actualTranslationalValue = 0;
	_wm->_actualRotationalVelocity = 0;

	for ( int i = 0 ; i != _wm->_sensorCount ; i++ ) // initialize sensor values to max range, no contact
	{
		_wm->_sensors[i][5] = gSensorRange; // range: max
		_wm->_sensors[i][6] = 0; // type:  none
	}
	_wm->_floorSensor = 0; // floor sensor value (taken from gZoneImage)
	
	// Initialize agent observer and Behavior Control Architecture


  
	_agentObserver->reset();
	_behavior->reset();

}

void RobotAgent::callObserver()
{
	_agentObserver->step();
}

void RobotAgent::stepBehavior()
{
	// update status
	_iterations++;
	
	if ( _wm->_isAlive ) // note: reviving a robot is performed in stepGenetics()
	{
		// compute commands
		_behavior->step();
	}

	displayInfo();
}


void RobotAgent::stepBehavior(Uint8* __keyboardStates)
{
	_iterations++;

	// compute commands
	behaviorUpdate_remoteController(__keyboardStates);

	displayInfo();
}

void RobotAgent::displayInfo()
{
	if ( gVerbose && gDisplayMode <= 1)
		if ( gInspectAgent && gAgentIndexFocus == _wm->_agentId )
		{	
			std::cout << "Robot #" << _wm->_agentId << " : ( " ;
			for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
			{
				std::cout << _wm->getSensorDistanceValue(i) << "/" <<  _wm->getSensorMaximumDistanceValue(i) << " ";
			}
			std::cout << " ) => ( " <<  _wm->_desiredTranslationalValue << " ; " << _wm->_desiredRotationalVelocity << " )." << std::endl;
		}
}

void RobotAgent::behaviorUpdate_remoteController(Uint8* __keyboardStates) // user controlled robot
{
	if ( gAgentIndexFocus != _wm->_agentId )
		return;

	// key pressed
	gKeyUp = __keyboardStates[ SDLK_UP ];
	gKeyDown = __keyboardStates[ SDLK_DOWN ];
	gKeyLeft = __keyboardStates[ SDLK_LEFT ];
	gKeyRight = __keyboardStates[ SDLK_RIGHT ];

	// update speed

	if ( gKeyUp )
		_wm->_desiredTranslationalValue += 1; // pixels
	if ( gKeyDown )
		_wm->_desiredTranslationalValue += -1;

	if ( _wm->_desiredTranslationalValue > gMaxTranslationalSpeed )
		 _wm->_desiredTranslationalValue = gMaxTranslationalSpeed;
	else
		if ( _wm->_desiredTranslationalValue < -gMaxTranslationalSpeed )
			_wm->_desiredTranslationalValue = -gMaxTranslationalSpeed;
		 
	if ( gKeyLeft )
		_wm->_desiredRotationalVelocity = -10; // degrees
	if ( gKeyRight )
		_wm->_desiredRotationalVelocity = 10;
		
	if ( !gKeyUp && !gKeyDown )
		_wm->_desiredTranslationalValue = 0;
		
	if ( !gKeyRight && !gKeyLeft )
	{
		if ( _wm->_desiredRotationalVelocity > 0 ) 
			_wm->_desiredRotationalVelocity--;
		else
			if ( _wm->_desiredRotationalVelocity < 0 ) 
				_wm->_desiredRotationalVelocity++;
	}

	if ( __keyboardStates[SDLK_o] )
	{
		_wm->_desiredTranslationalValue = 0;
		_wm->_desiredRotationalVelocity = 0;
	}
}

/*
	Compute actual Translational value and Rotational velocity that can be applied given the agent/robot dynamics.
	By default, this means: apply min/max values bounds, apply threshold on maximum delta values wrt. current values.
	By extension, this means: also apply some specific environment impact on the agent/robot dynamics (e.g. slow down due to sticky terrain, etc.). Details:
	1. convert desired translation/rotation to possible translation/rotation values
	2. apply environment contraints (call external method)
	3. translate into internal delta position update (ie. 2d x/y delta values)
*/
void RobotAgent::applyDynamics() 
{
	
	_lastAgentAbsoluteOrientation = _wm->_agentAbsoluteOrientation; // in case of collision, original orientation should be restored.
	
	// * translation
	// assume controller gives desired speed (the following models a feedback system (fr.: systeme asservi))
	
	_wm->_actualTranslationalValue = _wm->_agentAbsoluteLinearSpeed;
	
	if ( _wm->_desiredTranslationalValue != _wm->_agentAbsoluteLinearSpeed )
	{
		double diff = fabs( _wm->_desiredTranslationalValue - _wm->_agentAbsoluteLinearSpeed ); // Note: using abs(...) would truncate to integer. Use fabs instead. (minor bug solved as of 2010 06 18)

		if ( _wm->_desiredTranslationalValue > _wm->_agentAbsoluteLinearSpeed )
		{
			if ( diff > _wm->_maxTranslationalDeltaValue )
				_wm->_actualTranslationalValue += _wm->_maxTranslationalDeltaValue;
			else
				_wm->_actualTranslationalValue += diff;
		}
		else // strictly below
		{
			if ( diff > _wm->_maxTranslationalDeltaValue )
				_wm->_actualTranslationalValue -= _wm->_maxTranslationalDeltaValue;
			else
				_wm->_actualTranslationalValue -= diff;
		}

		// upper and lower bound for translation speed
		if ( _wm->_actualTranslationalValue > gMaxTranslationalSpeed )
			_wm->_actualTranslationalValue = gMaxTranslationalSpeed;
		else
			if ( _wm->_actualTranslationalValue < -gMaxTranslationalSpeed )
				_wm->_actualTranslationalValue = -gMaxTranslationalSpeed;
	}
	
	// * rotation
	// assume controller gives angular velocity.

	if ( _wm->_desiredRotationalVelocity > 0 )
	{
		if ( _wm->_desiredRotationalVelocity <= _wm->_maxRotationalDeltaValue )
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		else
			_wm->_actualRotationalVelocity = _wm->_maxRotationalDeltaValue;
	}
	else
		if ( _wm->_desiredRotationalVelocity < 0 )
		{
			if ( _wm->_desiredRotationalVelocity >= -_wm->_maxRotationalDeltaValue )
				_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
			else
				_wm->_actualRotationalVelocity = -_wm->_maxRotationalDeltaValue;
		}

	// * compute true displacement (actual (thought to be true) and true (really effected) rotation and translation, 

	// update both actual trans/rot values and internal data (see method definition for detailed comments)
	// after this call, wm->_agentAbsoluteLinearSpeed AND _wm->_agentAbsoluteOrientation are updated (internal data)
	gConfigurationLoader->make_RobotAgentWorldInterface()->applyInternalDynamics(_wm); //applyInternalDynamics is a static function 


	// note: the next locomotion-related method called will be the "move" method. It handles collision.
	// In case of collision, actual translation/rotation variables will be updated accordingly
	// Keep in mind that roborobo philosophy assumes pixel-based level of details for collision.
}




 /**
    * update the agent position in the environment. Apply simple physics (ie. obstacle collision detection and consequences).
	* if collision, translation and rotation speed are set to zero.
    * note: __recursiveIt is currently unused (maybe used for debug purpose, eg. checking for infinite loop.)
    */
void RobotAgent::move( int __recursiveIt ) // the interface btw agent and world -- in more complex envt, this should be handled by the "world".
{
	// apply world dynamics onto this agent

	// compute real valued delta (convert to x/y delta coordinates)	
	applyDynamics();

	// save position

	double xReal_old = _wm->_xReal; // backup old position in case of collision
	double yReal_old = _wm->_yReal;
	
	
	// update x/y position
		
	_xDeltaReal = _wm->_agentAbsoluteLinearSpeed * cos(_wm->_agentAbsoluteOrientation * M_PI / 180);
	_yDeltaReal = _wm->_agentAbsoluteLinearSpeed * sin(_wm->_agentAbsoluteOrientation * M_PI / 180);

	_wm->_xReal += _xDeltaReal;
	_wm->_yReal += _yDeltaReal;	// TODO: round is for positive values... (ok in this case however as 0,0 is up-left)

	setCoord((int)_wm->_xReal+0.5,(int)_wm->_yReal+0.5);
	
	// * collision with (image) border of the environment - position at border, then bounce

	//Move the collision boxes
	//shift_boxes(); // NOT IMPLEMENTED

	if ( isCollision() ) 
	{
		_wm->_desiredTranslationalValue = 0; // cancel any translation order as agent translation speed is set to zero after collision. (note that rotation is still ok)
		
		if (_wm->_agentAbsoluteLinearSpeed >= 1.0 )
		{
			_wm->_agentAbsoluteLinearSpeed--;
		}
		else
		{
			if ( _wm->_agentAbsoluteLinearSpeed <= -1.0 )
				_wm->_agentAbsoluteLinearSpeed++;
		}
		
		if ( ( _wm->_agentAbsoluteLinearSpeed > -1.0 ) && ( _wm->_agentAbsoluteLinearSpeed < 1.0 ) ) 
		{
			_wm->_agentAbsoluteLinearSpeed = 0; // roborobo assumes pixel-based resolution for solving collision. Ie. ignore sub-pixel translation values. (see _README.TXT for details)
		}
		
		_wm->_xReal=xReal_old;
		_wm->_yReal=yReal_old;
		
		if (_wm->_agentAbsoluteLinearSpeed != 0 ) // if zero: move is canceled
		{
			__recursiveIt++;
			move(__recursiveIt);
		}
		else
		{
			// special case: agent cannot not move at all - restore original coordinate (remember, _x/yReal are global and modified during recursive call).
			_wm->_xReal=xReal_old;
			_wm->_yReal=yReal_old;			
			setCoord((int)_wm->_xReal+0.5,(int)_wm->_yReal+0.5);
		}

		// update world model variables and internal data
		
		_wm->_agentAbsoluteLinearSpeed = 0;
		
		if ( gLocomotionMode == 1 ) // consider obstacle friction or not for rotation?
		{
			_wm->_agentAbsoluteOrientation = _lastAgentAbsoluteOrientation;
			_wm->_actualRotationalVelocity = 0;
		}
		else
			_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
			
		//_wm->_desiredTranslationalValue = _wm->_desiredRotationalVelocity = 0;
		_wm->_actualTranslationalValue = 0;

	}
	else
	{
		// actual rotational and translational values matches desired values
		_wm->_actualRotationalVelocity = _wm->_desiredRotationalVelocity;
		_wm->_actualTranslationalValue = _wm->_agentAbsoluteLinearSpeed; // (!) _wm->_desiredTranslationalValue is different as the "desired" translation speed may not be reached due to physical actuator limitations
	}

	
	/*
	if ( collision ) // bounce (TODO: position at collision point)
	{	
		_x=x_old;
		_y=y_old;
		_xDelta = 0;
		_yDelta = 0;
		_xReal=xReal_old;
		_yReal=yReal_old;
		_xDeltaReal=_yDeltaReal=0;
	}
	*/
	
	// update sensors
	for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
	{
		// Warning: the following is repeated in the show method because coordinates are not stored, but are needed to display the sensor rays.
		double x1 = (_wm->_xReal + _wm->_sensors[i][1] * cos( _wm->_sensors[i][2] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
		double y1 = (_wm->_yReal + _wm->_sensors[i][1] * sin( _wm->_sensors[i][2] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
		double x2 = (_wm->_xReal + _wm->_sensors[i][3] * cos( _wm->_sensors[i][4] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ; 
		double y2 = (_wm->_yReal + _wm->_sensors[i][3] * sin( _wm->_sensors[i][4] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ; 

		// cast sensor ray.
		_wm->_sensors[i][5] = castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, _wm->getSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object. -- not used here.
		Uint8 r, g, b;
		Uint32 pixel = getPixel32( gEnvironmentImage, x2 , y2);
		SDL_GetRGB(pixel,gEnvironmentImage->format,&r,&g,&b); 
		_wm->_sensors[i][6] = (r<<16)+(g<<8)+b; // R=objects, B=agents, can only be one agent OR one object in each location.
	} //(old: (r<<16)+(g<<8)+b;)
	
	Uint8 r, g, b;
	Uint32 pixel = getPixel32( gZoneImage, _wm->_xReal+0.5, _wm->_yReal+0.5);
	SDL_GetRGB(pixel,gZoneImage->format,&r,&g,&b); 
	_wm->_floorSensor = r;
}


bool RobotAgent::isCollision()
{
	// check collision with borders and environment objects.
    if ( 
		( _x < 0 ) || ( _x + gAgentWidth >= gAreaWidth ) ||
		( _y < 0 ) || ( _y + gAgentHeight >= gAreaHeight )
	   )
	{
		// * collision with border 
		return true;
	}
	else
	{
		// * environment objects 
		for ( int i = 0 ; i != gAgentWidth ; i++ )
			for ( int j = 0 ; j != gAgentHeight ; j++ )
			{
				if ( getPixel32( gAgentMaskImage , i , j) != SDL_MapRGB( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF ) ) // opt: bounding box instead of pixel-to-pixel test.
				{
				  // USELESS
				    if ( 
						( _x+i < 0 ) || ( _x + i  >= gAreaWidth ) ||
						( _y+j < 0 ) || ( _y + i  >= gAreaHeight )
						)
					{
						return true;
					}
				
					Uint32 pixel = getPixel32( gEnvironmentImage , _x+i , _y+j);
					if (  pixel != SDL_MapRGB( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF ) )
					{
						return true;
					}
				}
			}
	}	
	
	return false;
}


 /**
    * Display agent on screen. Add information caption if needed.
	* 
	* (render mode only) 
    */
void RobotAgent::show() // display on screen
{    
    //Show the dot
    
	if ( gNiceRendering )
		apply_surface( _x - gCamera.x, _y - gCamera.y, gAgentDisplayImage, gScreen ); // OPTIONAL (agent is already visible/registered through the environment image -- but: may be useful for image capture

	if ( getWorldModel()->getRobotLED_status() == true )
	{
			int dx = 1;
			int dy = 1;
			int xcenter = (int)_wm->_xReal + 0.5;
			int ycenter = (int)_wm->_yReal + 0.5;
			int r = getWorldModel()->getRobotLED_redValue();
			int g = getWorldModel()->getRobotLED_greenValue(); 
			int b = getWorldModel()->getRobotLED_blueValue();
			
			for ( int xTmp = xcenter - dx; xTmp != xcenter + dx + 1 ; xTmp++ )
				for ( int yTmp = ycenter - dy - 1 ; yTmp != ycenter + dy ; yTmp++ )
				{
					putPixel32secure( gScreen, xTmp - gCamera.x, yTmp + dy - gCamera.y , SDL_MapRGB( gScreen->format, r, g, b ) );
				}		
	}


	if ( _wm->_agentId == gAgentIndexFocus && ( gUserCommandMode || gAgentDisplayFocus ) ) // && _iterations%10 < 5) 
	{
			int dx = 10;
			int dy = 10;
			int xcenter = (int)_wm->_xReal + 0.5;
			int ycenter = (int)_wm->_yReal + 0.5;
			int r = 255.0 * ranf();
			int g = 255.0 * ranf();
			int b = 255.0 * ranf();
						
			for ( int xTmp = xcenter - dx ; xTmp != xcenter + dx + 1 ; xTmp++ )
			{
				putPixel32secure( gScreen, xTmp - gCamera.x, ycenter - dy - gCamera.y , SDL_MapRGB( gScreen->format, r, g, b ) );
				putPixel32secure( gScreen, xTmp - gCamera.x, ycenter + dy - gCamera.y , SDL_MapRGB( gScreen->format, r, g, b ) );
			}

			for ( int yTmp = ycenter - dy ; yTmp != ycenter + dy + 1 ; yTmp++ )
			{
				putPixel32secure( gScreen, xcenter - dx - gCamera.x, yTmp - gCamera.y , SDL_MapRGB( gScreen->format, r, g, b ) );
				putPixel32secure( gScreen, xcenter + dx - gCamera.x, yTmp - gCamera.y , SDL_MapRGB( gScreen->format, r, g, b ) );
			}

//			for ( int xTmp = (int)_wm->_xReal - dx ; xTmp != (int)_wm->_xReal + dx + 1 ; xTmp++ )
//				for ( int yTmp = (int)_wm->_yReal - dy ; yTmp != (int)_wm->_yReal + dy + 1 ; yTmp++ )
//						putPixel32secure( gScreen, xTmp - gCamera.x, yTmp - gCamera.y , SDL_MapRGB( gScreen->format, 0xFF, 0x00, ranf() ) );
	}


	if ( gDisplaySensors == true )
	{
		// * show orientation
		
		int xOrientationMarker =  (int)(_wm->_xReal+0.5) + gAgentWidth/2 * cos(_wm->_agentAbsoluteOrientation * M_PI / 180);
		int yOrientationMarker =  (int)(_wm->_yReal+0.5) + gAgentWidth/2 * sin(_wm->_agentAbsoluteOrientation * M_PI / 180);
	
		if ( _wm->_agentId == gAgentIndexFocus && gUserCommandMode ) 
		{
			int g,b;
			g = b = (32*_iterations%256) > 128 ? 0 : 255 ;
			for ( int xTmp = -2 ; xTmp != 3 ; xTmp++ )
				for ( int yTmp = -2 ; yTmp != 3 ; yTmp++ )
					putPixel32secure( gScreen, xOrientationMarker - gCamera.x + xTmp, yOrientationMarker - gCamera.y + yTmp , SDL_MapRGB( gScreen->format, 0x00, b , g ) );
		}
		else
		{
			for ( int xTmp = -2 ; xTmp != 3 ; xTmp++ )
				for ( int yTmp = -2 ; yTmp != 3 ; yTmp++ )
					putPixel32secure( gScreen, xOrientationMarker - gCamera.x + xTmp, yOrientationMarker - gCamera.y + yTmp , SDL_MapRGB( gScreen->format, 0xFF, 0x00, 0x00 ) );
		}
	
		// * show sensors

		for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
		{
			// Warning: the following is a repetition of code already in the move method (sensor ray casting) in order to display it (coordinates are not stored)
			double x1 = (_wm->_xReal + _wm->_sensors[i][1] * cos( _wm->_sensors[i][2] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
			double y1 = (_wm->_yReal + _wm->_sensors[i][1] * sin( _wm->_sensors[i][2] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ;
			double x2 = (_wm->_xReal + _wm->_sensors[i][3] * cos( _wm->_sensors[i][4] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ; 
			double y2 = (_wm->_yReal + _wm->_sensors[i][3] * sin( _wm->_sensors[i][4] + _wm->_agentAbsoluteOrientation * M_PI / 180 ) ) ; 

			// sensor ray casting is also performed in the move method -- this time we dont store data (already done). -- this one is only used to *display* the ray.
			/*_sensors[i][5] = */castSensorRay(gEnvironmentImage, x1, y1, &x2, &y2, _wm->getSensorMaximumDistanceValue(i)); // x2 and y2 are overriden with collision coordinate if ray hits object.

			// display on screen
			if ( _wm->_sensors[i][5] < _wm->getSensorMaximumDistanceValue(i)-1 ) //gSensorRange-1 ) 
				traceRayRGBA(gScreen, int(x1+0.5)-gCamera.x, int(y1+0.5)-gCamera.y, int(x2+0.5)-gCamera.x, int(y2+0.5)-gCamera.y, 255 , 0 , 0 , 255);
			else
				traceRayRGBA(gScreen, int(x1+0.5)-gCamera.x, int(y1+0.5)-gCamera.y, int(x2+0.5)-gCamera.x, int(y2+0.5)-gCamera.y, 0 , 0 , 255 , 255);
		}
	}
	
	
	// caption for storyzones
	
	if ( gDisplayZoneCaption )
	{
		Uint32 pixel = getPixel32( gZoneImage , _x+gAgentWidth/2 , _y+gAgentHeight/2);	
		if ( pixel != SDL_MapRGB( gZoneImage->format, 0x00, 0x00, 0x00 ) ) // check if there's a "story" to display
		{
			// extract story index (if any)
			Uint8 r, g, b;
			SDL_GetRGB(pixel,gZoneImage->format,&r,&g,&b);
			int storyId = r; // assume the red component holds the story index.
		
			if ( storyId >= 0 && storyId < 256 && gZoneCaptionImage[storyId] != NULL && gZoneStatus[storyId] ) // security check: story exists?
			{
				// display story caption

				//set caption position 
				int xCaption=0, yCaption=0;
				if ( _x < gAreaWidth/2 )
					xCaption = _x - gCamera.x + 40;
				else
					xCaption = _x - gCamera.x - gZoneCaptionImage[storyId]->w - 40;
				if ( _y < gAreaHeight/2 )
					yCaption = _y - gCamera.y + 40;
				else
					yCaption = _y - gCamera.y - gZoneCaptionImage[storyId]->h - 40;
			
				//display caption
				apply_surface(xCaption, yCaption, gZoneCaptionImage[storyId], gScreen );
			
				// update story flags (if needed)
				if ( storyId>=100 && storyId<200 && storyId % 10 == 0 ) // key story, btw 100 and 199: activate story flag of the 9 next sub-stories
				{	
					for ( int i = 1 ; i != 10 ; i++ )
						gZoneStatus[storyId+i]=true;
				}
				else
					if ( storyId>=200 && storyId<256 && storyId % 10 != 9) // sub-story, btw 200 and 299: activate the next story flag (if not last)
					{
						gZoneStatus[storyId+1]=true;
					}
				}
		}
	}
}

void RobotAgent::registerAgent()
{
    // [!n] CONVENTION: _agentId+100 to avoid confusion with empty space (0: empty ; 1: static object; 2-99: reserved for other static objects)
    
	if ( gAgentRegistration )
        register_agentMask( _x, _y, gEnvironmentImage, _wm->_agentId+1000 );  
		//register_surface( _x, _y, gAgentMaskImage, gEnvironmentImage, _wm->_agentId+1000 ); // used before 2012-6-6
}

void RobotAgent::unregisterAgent()
{
	if ( gAgentRegistration	)
        clean_agentMask( _x, _y, gEnvironmentImage );
		//clean_surface( _x, _y, gAgentMaskImage, gEnvironmentImage ); // used before 2012-6-6
}	


void RobotAgent::setCoordReal (int __x, int __y) // agent is centered on point
{
	//setCoord(gAgentXStart+__x,gAgentYStart+__y);
	//setCoord(__x,__y);
	_wm->_xReal = __x;
	_wm->_yReal = __y;
}


AgentObserver* RobotAgent::getObserver()
{
	return _agentObserver;
}

BehaviorControlArchitecture* RobotAgent::getBehavior()
{
	return _behavior;
}

RobotAgentWorldModel* RobotAgent::getWorldModel()
{
	return _wm;
}

		
std::vector<int> RobotAgent::getNeighbors()
{ 
  std::vector<int> neighborsList;
  
  if ( gRadioNetwork )
  {  
	int id = _wm->_agentId;
  
	// internal cuisine: for all existing agents, check if in the vincinity. Yes? add to neighbors list.
	for ( int id_target = 0 ; id_target != gAgentCounter ; id_target++)
	{
	  if ( gRadioNetworkArray[id][id_target] > 0 ) // note that the diagonal is always zero (ie. self-connection is not considered). check anyway (avoid an extra if statement).
		neighborsList.push_back(id_target);
	}
  }
  else
	if ( gVerbose )
	  std::cout << "[WARNING] RobotAgent::getNeighbors() is called but radio network is *inactive*! (always return empty list)" << std::endl;

  
  return neighborsList;
}


// ---- ---- ---- ---- ----



