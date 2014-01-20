/*
 *  SDL_gfxRoborobo.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utilities/SDL_gfxRoborobo.h"
#include "Utilities/Misc.h"


int gScreenshotIndex = 0; // numbering screenshot (used by saveScreenshot(...))
int gTrajectoryFileIndex = 0; // numbering trajectory images (used by saveTrajectoryImage(...))


void saveImage ( SDL_Surface *image, std::string __prefix, std::string __comment ) // comment is optional
{
    std::string s = "logs/" + __prefix + "_" + gStartTime;

	if ( __comment != "" )
		s += "_" + __comment;
	
	s += ".bmp";
	
	SDL_SaveBMP(image,s.c_str());
}

void saveTrajectoryImage ( std::string __comment )
{
    
    std::string s = convertToString(gTrajectoryFileIndex);
    
    if ( gTrajectoryMonitorMode == 0 )
        s += "_all";
    else
        s += "_single";

    if ( __comment != "" )
		s += "_" + __comment;
	
	saveImage(gTrajectoryMonitorImage,"trajectory",s);
    
    gTrajectoryFileIndex++;
}

void saveScreenshot( std::string __comment) // parameter is optional
{
    saveImage(gScreen,"screenshot",convertToString(gScreenshotIndex)+__comment);

	gScreenshotIndex++;
}

bool initSDL(Uint32 flags) // parameter is optional (default: SDL_HWSURFACE | SDL_DOUBLEBUF)
{
	
	if ( gBatchMode )
	{
		SDL_putenv((char*)"SDL_VIDEODRIVER=dummy"); // batch mode!
		gDisplayMode = false;
	}
	
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;    
    }
	
	// set up screen
	gScreen = SDL_SetVideoMode( gScreenWidth, gScreenHeight, gScreenBPP, flags );

    //If there was an error in setting up the screen
    if( gScreen == NULL )
    {
        return false;    
    }
    
    //Set the window caption
    SDL_WM_SetCaption( "roborobo!", NULL );
    
    //If everything initialized fine
    return true;
}

Uint32 getPixel32( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
	if ( x >= 0 && x < surface->w && y >= 0 && y < surface->h )
	{
		return pixels[ ( y * surface->w ) + x ];
	}
	else 
		return -1;
}

void putPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	if ( x >= 0 && x < surface->w && y >= 0 && y < surface->h )
	{
		//Convert the pixels to 32 bit
		Uint32 *pixels = (Uint32 *)surface->pixels;
    
		//Set the pixel
		pixels[ ( y * (surface->w) ) + x ] = pixel;
	}
}


void putPixel32secure( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
	//	if ( x >= 0 && x < gScreenWidth && y >= 0 && y < gScreenHeight )  // not necessary -- appy_surface can manage this.
	{
		SDL_Surface* source = SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, gScreenBPP, 0, 0, 0, 0);
		Uint32 *pixels = (Uint32 *)source->pixels;
		pixels[0] = pixel;
		apply_surface( x, y, source, surface );
		SDL_FreeSurface(source);
	}
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect* clip ) // last argument is optional (cf. function header)
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void register_surface( int __x, int __y, SDL_Surface* source, SDL_Surface* destination , int __id ) // last argument is optional (cf. function header)
{
	for ( int x = 0 ; x != source->w ; x++ )
		for ( int y = 0 ; y != source->h ; y++ )
		{
			Uint32 pixel = getPixel32(source,x,y);
			if ( pixel != SDL_MapRGB( destination->format, 0xFF, 0xFF, 0xFF ) )
				//putPixel32( destination, __x+x, __y+y, __id );
				putPixel32( destination, __x+x, __y+y,  SDL_MapRGB( destination->format, ((__id & 0xFF0000)>>16), ((__id & 0xFF00)>>8), (__id & 0xFF) ) );
		}
}

void clean_surface( int __x, int __y, SDL_Surface* source, SDL_Surface* destination ) // last argument is optional (cf. function header)
{

	for ( int x = 0 ; x != source->w ; x++ )
		for ( int y = 0 ; y != source->h ; y++ )
		{
			Uint32 pixel = getPixel32(source,x,y);
			if ( pixel != SDL_MapRGB( destination->format, 0xFF, 0xFF, 0xFF ) )
				putPixel32( destination, __x+x, __y+y, SDL_MapRGB( destination->format, 0xFF, 0xFF, 0xFF ) );
		}
}

// used to register a robot into the environment (used since 2012-6-6)
void register_agentMask( int __x, int __y, SDL_Surface* destination , int __id ) // last argument is optional (cf. function header)
{
    for ( int i = 0 ; i != gAgentMaskData.size() ; i++ )
    {
        int x = __x + gAgentMaskData[i][0];
        int y = __y + gAgentMaskData[i][1];
        putPixel32( destination, x, y, SDL_MapRGB( destination->format, 0x22, 0x11, 0xFF ) );
        //putPixel32( destination, x, y,  SDL_MapRGB( destination->format, ((__id & 0xFF0000)>>16), ((__id & 0xFF00)>>8), (__id & 0xFF) ) );
    }
}

// used to unregister a robot from the environment (used since 2012-6-6)
void clean_agentMask( int __x, int __y, SDL_Surface* destination ) // last argument is optional (cf. function header)
{
    //std::cout << "gAgentMaskData.size() => " << gAgentMaskData.size() << std::endl;
    
    for ( int i = 0 ; i != gAgentMaskData.size() ; i++ )
    {
        int x = __x + gAgentMaskData[i][0];
        int y = __y + gAgentMaskData[i][1];
        
        //std::cout << "coordinates: " << x << " , " << y << std::endl;
        
        putPixel32( destination, x, y, SDL_MapRGB( destination->format, 0xFF, 0xFF, 0xFF ) );
    }
}


/* **************** */
/* * SIMULATOR WINDOW * */
/* **************** */



void toggle_fullscreen()
{
	std::cout << "[critical] Fullscreen mode is not implemented." << std::endl;

	/*
	clean_up();

	gFullScreenMode= !gFullScreenMode;
	
	if( initSDL(SDL_HWSURFACE | SDL_DOUBLEBUF | (gFullScreenMode?SDL_FULLSCREEN:0) ) == false )
    {
		std::cerr << "fullscreen error 1 (toggling fullscreen) \n";
		exit(2);
	}
    
    if( gWorld->loadFiles() == false ) // note: if this block is removed, computer may hang. + gWorld is not in the scope
    {
		std::cerr << "fullscreeen error 2 (re-loading files) \n";
		exit(2);
	}
 
	//if (SDL_WM_ToggleFullScreen(gScreen) == 0)
	*/
}


SDL_Surface *load_image( std::string filename ) 
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image
    loadedImage = IMG_Load( filename.c_str() );
    
    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old surface
        SDL_FreeSurface( loadedImage );
        
        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            //SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }
    
	//Return the optimized surface	
	return optimizedImage;
}




