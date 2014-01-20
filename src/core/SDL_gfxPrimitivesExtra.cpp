/*
 *  SDL_gfxPrimitivesExtra.cpp
 *  roborobo
 *
 *  Created by Nicolas on 05/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utilities/SDL_gfxPrimitivesExtra.h"



/**
	trace ray from (x1,y1) to (x2,y2) whatever the obstacles.
*/
void traceRayRGBA(SDL_Surface * image, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	//lineRGBA(image, x1, y1, x2, y2, 255 , 255 , 255 , 255); // nearly equivalent (small algorithmic differences)

	Uint32 pixelColor =SDL_MapRGB( image->format, r, g, b );
	
	if ( abs(x1-x2) > abs (y1-y2) )
	{
		int it;
		
		double dy = double(y1-y2) / double(x1-x2);

		if ( (x1-x2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dy=-dy;
		}
		
		double yReal = y1;
		
		for ( int x = x1 ; x != x2 ; x = x + it )
		{
			putPixel32secure( image, x, (int)(yReal+0.5) , pixelColor );
			yReal += dy;
		}
	}
	else
	{
		int it;
		
		double dx = double(x1-x2) / double(y1-y2);

		if ( (y1-y2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dx=-dx;
		}
		
		double xReal = x1;
		
		for ( int y = y1 ; y != y2 ; y = y + it )
		{
			putPixel32secure( image, (int)(xReal+0.5), y , pixelColor );
			xReal += dx;
		}
	}

}

/**
	cast ray from (x1,y1) to (x2,y2). Stops whenever ray encounters something. (x2,y2) are update with point of contact
	__maxValue is the maximum distance possible -- ie. if no collision during ray casting (makes it possible to return an exact value without the cost of distance (with sqrt) computation)
*/
int castSensorRay(SDL_Surface * image, double x1, double y1, double *x2pt, double *y2pt, int __maxValue )
{	
	double x2 = *x2pt;
	double y2 = *y2pt;
	
	bool isCollision = false; // check collision btw sensor ray and object.
	
	if ( abs(x1-x2) > abs (y1-y2) )
	{
		int it;
		
		double dy = (y1-y2) / (x1-x2);

		if ( (x1-x2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dy=-dy;
		}
		
		double yReal = y1;
		
		for ( int x = (int)(x1+0.5) ; x != (int)(x2+0.5) ; x = x + it )
		{
			if ( getPixel32 ( image, x, (int)(yReal+0.5) ) != SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF ) )
			{
				*x2pt = (double)x;
				*y2pt = yReal;
				isCollision = true;
				break;
			}
			yReal += dy;
		}
	}
	else
	{
		int it;
		
		double dx = (x1-x2) / (y1-y2);

		if ( (y1-y2) < 0 )
		{
			it=1;
		}
		else
		{
			it=-1;
			dx=-dx;
		}
		
		double xReal = x1;
		
		for ( int y = (int)(y1+0.5) ; y != (int)(y2+0.5) ; y = y + it )
		{
			if ( getPixel32 ( image, (int)(xReal+0.5), y ) != SDL_MapRGB( image->format, 0xFF, 0xFF, 0xFF ) )
			{
				*x2pt = xReal;
				*y2pt = (double)y;
				isCollision = true;
				break;
			}
			xReal += dx;
		}
	}

	if ( isCollision == false && __maxValue != -1 )
		return __maxValue;
	else
		return sqrt ( ( x1 - *x2pt ) * ( x1 - *x2pt ) + ( y1 - *y2pt ) * ( y1 - *y2pt ) );
    // should be equal to gSensorRange; // no hit
}



