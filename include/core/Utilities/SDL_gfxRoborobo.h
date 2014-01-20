/*
 *  SDL_gfxRoborobo.h
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SDL_GFXROBOROBO_H
#define SDL_GFXROBOROBO_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

extern int gScreenshotIndex;
extern int gTrajectoryImageIndex;

bool initSDL(Uint32 flags = 0); // parameter is optional (default: SDL_HWSURFACE | SDL_DOUBLEBUF)

Uint32 getPixel32( SDL_Surface *surface, int x, int y );
void putPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
void putPixel32secure( SDL_Surface *surface, int x, int y, Uint32 pixel ); // slower, but sure.

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect* clip = NULL );

void register_surface( int __x, int __y, SDL_Surface* source, SDL_Surface* destination , int __id );
void clean_surface( int __x, int __y, SDL_Surface* source, SDL_Surface* destination );

void register_agentMask( int __x, int __y, SDL_Surface* destination , int __id ); // use gAgentMaskData
void clean_agentMask( int __x, int __y, SDL_Surface* destination ); // use gAgentMaskData

void toggle_fullscreen();

SDL_Surface *load_image( std::string filename );

void saveScreenshot( std::string __comment = "" );

void saveTrajectoryImage( std::string __comment = "" );

void saveImage ( SDL_Surface image, std::string __prefix, std::string __comment = "" );

#endif // SDL_GFXROBOROBO_H

