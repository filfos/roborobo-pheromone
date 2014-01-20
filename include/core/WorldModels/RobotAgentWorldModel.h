/*
 *  RobotAgentWorldModel.h
 *  roborobo-online
 *
 *  Filip Fossum 18.10.2013
 *
 */


#ifndef ROBOTAGENTWORLDMODEL_H
#define ROBOTAGENTWORLDMODEL_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "WorldModels/WorldModel.h"

class World;

class RobotAgentWorldModel : public WorldModel
{	
	private: 
		int _robotNeighborhoodCounter; // count the number of other robots within a given radius.
	
	protected:
		// ... TODO: move all data to protected, write accessing method, update code.

		// LED-on-robot: a light displayed at the center of the robot (in display mode). 
		// while it may be used for communication, the LED was initialy used to 'tag' the robot for user-purpose.
		// default value is false.
		bool _robotLED_status; // agent LED light (on top of it, used for vizualization only) -- default is false ("off")
		int _robotLED_red;
		int _robotLED_green;
		int _robotLED_blue;

	public:
	
		World *_world;
	
		int _agentId;
	
		bool _isAlive;  // robot behavior is executed *only* if robot is alived. (default is: alive)
		
		// absolute localization value in the simulated world -- GPS-like (ie. internal data for displaying, not robot-centered)
		double _xReal;
		double _yReal;
		double _agentAbsoluteOrientation;
		double _agentAbsoluteLinearSpeed;
		
		// targeted desired values (impact Absolute Orientation/Linear Speed but bounded by ...MaxDeltaValue(s))
		double _desiredRotationalVelocity;
		double _desiredTranslationalValue;
		
		// delta values that where *actually* executed (ie. may differ if crash occurs)
		double _actualRotationalVelocity;
		double _actualTranslationalValue; // value is equal to _agentAbsoluteLinearSpeed

		// max authorized delta values
		double _maxRotationalDeltaValue;		// default: gMaxRotationalSpeed (ie. instantenous)
		double _maxTranslationalDeltaValue;		// default: gMaxTranslationalSpeed (ie. instantenous)
		
		// description and values of sensors
		double (*_sensors)[7]; // id sensor(R),sensor_origin_norm,sensor_origin_angle,sensor_target_distance_from_agent_center(!),sensor_target_angle, "current value", object Id.
		int _sensorCount;
		int _floorSensor; // result is the **red** value from gBackgroundImage at the center of the agent
		
		int _age;
		int _generation;
	
		double _fitnessValue; // optional
				
	//public:
		// * Initializes the variables
		RobotAgentWorldModel();
		virtual ~RobotAgentWorldModel();
		
		// * sensor methods 
		double getSensorDistanceValue( int i );		// return sensor range (equal to maximum range if no obstacle)
		double getSensorTypeValue( int i );	// return type of obstacle hit by sensor ray (if any)
		double getSensorMaximumDistanceValue( int i );	// return sensor maximum range
	
		// * other methods
		World* getWorld(); // return pointer to the current World
	
		// * accessing methods
		void setRobotNeighborhoodCounter( int __value );
		void incRobotNeighborhoodCounter( ); // increment
		int getRobotNeighborhoodCounter();
		
		double getXReal(); // real absolute X coordinate 
		double getYReal(); // real absolute Y coordinate

		bool getRobotLED_status();
		void setRobotLED_status(bool __status);
		int getRobotLED_redValue();
		int getRobotLED_greenValue();
		int getRobotLED_blueValue();
		void setRobotLED_colorValues(int __r, int __g, int __b);
		
};


#endif


