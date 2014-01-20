/*
 *  InspectorAgent.cpp
 *  roborobo
 *
 *  Created by Nicolas on 17/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Agents/InspectorAgent.h"


InspectorAgent::InspectorAgent()
{
	reset();
}


void InspectorAgent::reset()
{
    //Initialize the offsets
	setCoord(gInspectorAgentXStart,gInspectorAgentYStart);
    
    //Initialize the velocity
    _xDelta = 0;
    _yDelta = 0;
}


 /**
    * update the agent control using user keyboard commands. ie. remote control (cursor keys + space bar for basic obstacle avoidance).
	* control assumes "god" view (ie. up is north, etc.)
    */
void InspectorAgent::stepBehavior( ) // the controller
{
	// do nothing.
}

 /**
    * update the agent control using user keyboard commands. ie. remote control (cursor keys + space bar for basic obstacle avoidance).
	* control assumes "god" view (ie. up is north, etc.)
    */
void InspectorAgent::stepBehavior( Uint8* __keyboardStates) // the controller
{

	// key pressed
	gKeyUp = __keyboardStates[ SDLK_UP ];
	gKeyDown = __keyboardStates[ SDLK_DOWN ];
	gKeyLeft = __keyboardStates[ SDLK_LEFT ];
	gKeyRight = __keyboardStates[ SDLK_RIGHT ];

	// update speed
	if ( gKeyUp )
		_yDelta -= gMoveStepHeight; 
	if ( gKeyDown )
		_yDelta += gMoveStepHeight;
	if ( gKeyLeft )
		_xDelta -= gMoveStepWidth;
	if ( gKeyRight )
		_xDelta += gMoveStepWidth;

	// decrease speed
	// natural friction
	if ( !gKeyUp && !gKeyDown && _yDelta != 0 )
		_yDelta = ( _yDelta > 0 ) ? _yDelta - gMoveStepHeight : _yDelta + gMoveStepHeight;
	if ( !gKeyLeft && !gKeyRight && _xDelta != 0 )
		_xDelta = ( _xDelta > 0 ) ? _xDelta - gMoveStepWidth : _xDelta + gMoveStepWidth;
	/**/
	// reverse trust!
	if ( gKeyUp && _yDelta > 0 ) 
		_yDelta = _yDelta - gMoveStepHeight;
	if ( gKeyDown && _yDelta < 0 ) 
		_yDelta = _yDelta + gMoveStepHeight;
	if ( gKeyLeft && _xDelta > 0 ) 
		_xDelta = _xDelta - gMoveStepWidth;
	if ( gKeyRight && _xDelta < 0 ) 
		_xDelta = _xDelta + gMoveStepWidth;		
	/**/
}

void InspectorAgent::move()
{
	// max speed limit (opt: compress with above)
	/**/
	if ( _xDelta > gMaxSpeedOnXaxis )
		_xDelta = gMaxSpeedOnXaxis;
	else
		if ( _xDelta < -gMaxSpeedOnXaxis )
			_xDelta = -gMaxSpeedOnXaxis;
	if ( _yDelta > gMaxSpeedOnYaxis )
		_yDelta = gMaxSpeedOnYaxis;
	else
		if ( _yDelta < -gMaxSpeedOnYaxis )
			_yDelta = -gMaxSpeedOnYaxis;
	/**/


	// update position

    _x += _xDelta; //Move the dot left or right
    _y += _yDelta; //Move the dot up or down

	// * collision with (image) border of the environment - position at border, then bounce

    //If the dot went too far to the left or right
    if ( ( _x < 0 ) || ( _x >= gAreaWidth ) )
	{
		_xDelta=-_xDelta/2; // bounce
        _x -= _xDelta; // move back
		_x= _x < 0 ? 0 : gAreaWidth-1;
	}
    
    //If the dot went too far up or down
    if ( ( _y < 0 ) || ( _y >= gAreaHeight ) )
    {
        _yDelta=-_yDelta/2; // bounce
        _y -= _yDelta; // move back
		_y = _y < 0 ? 0 : gAreaHeight-1;
    }	
	
}


void InspectorAgent::show() // display on screen
{    
    //Show the dot
	int r,g,b;

	r = rand() % 255 ;
	g = rand() % 255 ;
	b = rand() % 255 ;

	int x = _x - gCamera.x;
	int y = _y - gCamera.y;
	
	
	int dx = 4;
	int dy = 4;

	for ( int i = -2 ; i < +3 ; i++ )
		putPixel32secure( gScreen, x + i , y , SDL_MapRGB( gScreen->format, r, g, b ) );
	for ( int j = -2 ; j < +3 ; j++ )
		putPixel32secure( gScreen, x , y + j, SDL_MapRGB( gScreen->format, r, g, b ) );

	for ( int i = x - dx ; i != x + dx + 1 ; i++ )
	{
		putPixel32secure( gScreen, i , y - dy , SDL_MapRGB( gScreen->format, r, g, b ) );
		putPixel32secure( gScreen, i, y + dy , SDL_MapRGB( gScreen->format, r, g, b ) );
	}

	for ( int j = y - dy ; j != y + dy + 1 ; j++ )
	{
		putPixel32secure( gScreen, x - dx, j , SDL_MapRGB( gScreen->format, r, g, b ) );
		putPixel32secure( gScreen, x + dx, j , SDL_MapRGB( gScreen->format, r, g, b ) );
	}


/*
	for ( int xTmp = -2 ; xTmp < +3 ; xTmp++ )
		putPixel32( gScreen, _x - gCamera.x + xTmp , _y - gCamera.y , SDL_MapRGB( gScreen->format, r, g, b ) );
	for ( int yTmp = -2 ; yTmp < +3 ; yTmp++ )
		putPixel32( gScreen, _x - gCamera.x , _y - gCamera.y + yTmp, SDL_MapRGB( gScreen->format, r, g, b ) );
*/
	
	// alternative: "apply_surface( _x - gCamera.x, _y - gCamera.y, gAgentMask, gScreen );"
}



