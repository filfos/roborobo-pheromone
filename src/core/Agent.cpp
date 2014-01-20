/*
 *  agent.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Agents/Agent.h"


Agent::Agent()
{
	// dont forget to call your own init() method
}

/*
Agent::Agent(SDL_Surface *__mask) 	// NOT IMPLEMENTED
{
	init();
	
	//Initialize the collision box
	
	//Create the necessary SDL_Rects
    box.resize( __mask->h; );
    
    //Initialize the collision boxes' width and height
	for ( int y = 0 ; y != __mask->h ; y++ )
		for ( int x = 0 ; x != __mask->h ; x++ )
		{
			if (  getPixel32(gAgentMask,i,j) != SDL_MapRGB( gForegroundImage->format, 0xFF, 0xFF, 0xFF ) )
			{
				//(...) 	// NOT IMPLEMENTED
			}
		}

    //box[ 0 ].w = 6;
    //box[ 0 ].h = 1;
        
    //Move the collision boxes to their proper spot
    shift_boxes();
}*/

/*
void Agent::shift_boxes()  	// NOT IMPLEMENTED (should be revised if collision box is implemented
{
    //The row offset
    int r = 0;
    
    //Go through the dot's collision boxes
    for( int set = 0; set < box.size(); set++ )
    {
        //Center the collison box
        box[ set ].x = x + ( gAgentWidth - box[ set ].w ) / 2;
        
        //Set the collision box at its row offset
        box[ set ].y = y + r;
        
        //Move the row offset down the height of the collision box
        r += box[ set ].h;    
    }
}
*/



 /**
    * Display agent on screen. Add information caption if needed.
	* (render mode only) 
    */
void Agent::show() // display on screen -- method may be overriden
{    
    //Show the dot
    apply_surface( _x - gCamera.x, _y - gCamera.y, gAgentDisplayImage, gScreen );
	
	Uint32 pixel = getPixel32( gZoneImage , _x , _y);	
	if ( pixel != SDL_MapRGB( gZoneImage->format, 0xFF, 0xFF, 0xFF ) ) // check if there's a "story" to display
	{
		// extract story index (if any)
		Uint8 r, g, b;
		SDL_GetRGB(pixel,gZoneImage->format,&r,&g,&b);
		int storyId = b; // assume the blue component holds the story index.
		
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



 /**
    * update the agent position in the environment. Apply simple physics (ie. obstacle collision detection and consequences).
    */
void Agent::move() // the interface btw agent and world -- in more complex envt, this should be handled by the "world".
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

	int x_old = _x; // backup old position in case of collision
	int y_old = _y;
	
    _x += _xDelta; //Move the dot left or right
    _y += _yDelta; //Move the dot up or down


	// * collision with (image) border of the environment - position at border, then bounce

	//Move the collision boxes
	//shift_boxes(); // NOT IMPLEMENTED
    
    //If the dot went too far to the left or right
    if ( ( _x < 0 ) || ( _x + gAgentWidth > gAreaWidth ) )
	{
		_xDelta=-_xDelta/2; // bounce
        _x -= _xDelta; // move back
		_x= _x < 0 ? 0 : gAreaWidth-gAgentWidth;
	}
    
	//Move the collision boxes
	//shift_boxes(); // NOT IMPLEMENTED
	
    //If the dot went too far up or down
    if ( ( _y < 0 ) || ( _y + gAgentHeight > gAreaHeight ) )
    {
        _yDelta=-_yDelta/2; // bounce
        _y -= _yDelta; // move back
		_y = _y < 0 ? 0 : gAreaHeight-gAgentHeight;
    }	
	
	// * Object/environment collision check 

	bool collision = false;

	// COLLISION DETECTION ROUTINE, Version used before 2012-06-06 (may be deleted)
    /*
	for ( int i = 0 ; i != gAgentWidth ; i++ )
		for ( int j = 0 ; j != gAgentHeight ; j++ )
		{
			if ( getPixel32( gAgentMaskImage , i , j) != SDL_MapRGB( gForegroundImage->format, 0xFF, 0xFF, 0xFF ) ) // opt: bounding box instead of pixel-to-pixel test.
			{
			Uint32 pixel = getPixel32( gForegroundImage , _x+i , _y+j);
			if (  pixel != SDL_MapRGB( gForegroundImage->format, 0xFF, 0xFF, 0xFF ) )
			{
				collision = true;
				break;
			}
			}
		}	
    */

	// COLLISION DETECTION ROUTINE, Version used since 2012-06-06
    for ( int i = 0 ; i != gAgentMaskData.size() ; i++ )
    {
        Uint32 pixel = getPixel32( gForegroundImage , _x + gAgentMaskData[i][0] , _y + gAgentMaskData[i][1] );        
        
        if ( pixel != SDL_MapRGB( gForegroundImage->format, 0xFF, 0xFF, 0xFF ) )
        {
			collision = true;
			break;
        }
    }
        
	
	if ( collision ) // bounce (TODO: position at collision point)
	{	
		_x=x_old;
		_y=y_old;
		_xDelta = -_xDelta/2;
		_yDelta = -_yDelta/2;
	}
}


/* position camera */
void Agent::set_camera()
{
    //Center the camera over the dot
    gCamera.x = ( _x + gAgentWidth / 2 ) - gScreenWidth / 2;
    gCamera.y = ( _y + gAgentHeight / 2 ) - gScreenHeight / 2;
    
    //Keep the camera in bounds.
    if( gCamera.x < 0 )        gCamera.x = 0;    
    if( gCamera.y < 0 )        gCamera.y = 0;    
    if( gCamera.x > gAreaWidth  - gCamera.w )        gCamera.x = gAreaWidth - gCamera.w;    
    if( gCamera.y > gAreaHeight - gCamera.h )        gCamera.y = gAreaHeight - gCamera.h;    

	gCameraParallax.x =  gCamera.x / gParallaxFactor;
	gCameraParallax.y =  gCamera.y / gParallaxFactor;
}

