/*
 *  PerceptronBehaviorControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 20/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "BehaviorControlArchitectures/PerceptronBehaviorControlArchitecture.h"

#include "Utilities/Misc.h"

PerceptronBehaviorControlArchitecture::PerceptronBehaviorControlArchitecture( RobotAgentWorldModel *__wm )
{
	_wm = (EvolvingRobotAgentWorldModel*)__wm;
	//_wm->_genome.resize(18); 
}

PerceptronBehaviorControlArchitecture::~PerceptronBehaviorControlArchitecture()
{
	// nothing to do.
}

void PerceptronBehaviorControlArchitecture::reset()
{
	_parameters.clear();
	_parameters = _wm->_genome;
}


// perform one controller update
// set motor value using motor format.
void PerceptronBehaviorControlArchitecture::step()
{

	if ( _wm->getNewGenomeStatus() ) // check for new NN parameters
	{
		reset();
		_wm->setNewGenomeStatus(false);
	}

	if ( _wm->_age < 0 )
	{
		// ** security control (prior to a new behavior, get out of crash situation) -- random noise to avoid "STALL" status
		_wm->_desiredTranslationalValue = ( ranf()*2.-1. ) * gMaxTranslationalSpeed ;
		_wm->_desiredRotationalVelocity =( ranf()*2.-1. ) * gMaxRotationalSpeed ;
		return;
	}

	_wm->_desiredTranslationalValue = 0;
	_wm->_desiredRotationalVelocity = 0;

	// input to output
	for ( int i = 0 ; i != _wm->_sensorCount ; i++ )
	{
		_wm->_desiredTranslationalValue += _parameters[i] * _wm->getSensorDistanceValue(i);
		_wm->_desiredRotationalVelocity += _parameters[i+8+1] * _wm->getSensorDistanceValue(i);
	}
	
	// bias to output
	_wm->_desiredTranslationalValue += _parameters[8] * 1.;
	_wm->_desiredRotationalVelocity +=  _parameters[17] * 1.;
	
	_wm->_desiredTranslationalValue = tanh(_wm->_desiredTranslationalValue);
	_wm->_desiredRotationalVelocity = tanh(_wm->_desiredRotationalVelocity);
	
	// normalize to motor interval values
	_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
	_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;

}


//TODO:
/*
void PerceptronBehaviorControlArchitecture::setWeights (double* __params)
{
	_params = new double[_sensorCount*2];
	
	if ( __params == NULL ) // random weights
	{
		for ( int i = 0 ; i != _sensorCount*2 ; i++ )
			_params[i] = (double)(rand()%10)-5.;
	}
	else
		for ( int i = 0 ; i != _sensorCount*2 ; i++ )
			_params[i] = __params[i];
}
*/

