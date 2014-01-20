/*
 *  Misc.cpp
 *  roborobo
 *
 *  Created by Nicolas on 24/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Utilities/Misc.h"

// useful functions. 
// see header file for description and credits/sources.



double getGaussianRand(double m, double s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	double x1, x2, w, y1;
	static double y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = ((double)rand()/(1.0+(double)RAND_MAX)); //??? check and delete
		
			x1 = 2.0 * ranf() - 1.0;
			x2 = 2.0 * ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( ( w >= 1.0 ) || ( w == 0.0 ) );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}


double getEuclidianDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(getSquaredEuclidianDistance(x1, y1, x2, y2));
}

double getSquaredEuclidianDistance(double x1, double y1, double x2, double y2)
{
	return ( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
}

double getEuclidianDistance(Point2d p1, Point2d p2)
{
	return sqrt( pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}


double computeModulo( double a , double b )
{
	int result = static_cast<int>( a / b );
	return a - static_cast<double>( result ) * b;
}


std::string getCurrentTimeAsReadableString()
{
	// --- get current time information
	
	time_t now = time(0);
	char timestamp[19] = "";
	//strftime (timestamp, 19,"%Y%m%d-%Hh%Mm%Ss", localtime(&now));
	strftime (timestamp, 19,"%Y%m%d-%Hh%Mm%Ss", localtime(&now));
	std::string s = timestamp;
	
	// --- get milliseconds resolution (note: could be used to replace code block above - left for tutorial)
	
	struct timeval now2;
    int mtime;
    gettimeofday(&now2, NULL);
	
	mtime = now2.tv_usec;
	if ( mtime < 100000 )
	{
		s+="0";
		if ( mtime < 10000 )
			s+="0";
		if ( mtime < 1000 )
			s+="0";
		if ( mtime < 100 )
			s+="0";
		if ( mtime < 10 )
			s+="0";
	}	
	s += convertToString(mtime) + "us"; // microseconds

	return s;

}

std::string convertToString( int __value )			
{
	std::string s;
	std::stringstream sOutTmp;
	sOutTmp << __value;
	s = sOutTmp.str();
	
	return s;
}


double getAngleToTarget( Point2d refPt , double orientation, Point2d targetPt )
{
	double angleToClosestEnergyPoint = atan2(targetPt.y-refPt.y,targetPt.x-refPt.x);
	if ( angleToClosestEnergyPoint < 0 )
		angleToClosestEnergyPoint = angleToClosestEnergyPoint + 2*M_PI;
	angleToClosestEnergyPoint = angleToClosestEnergyPoint * 180.0 / M_PI;
	angleToClosestEnergyPoint = angleToClosestEnergyPoint - orientation;
	if ( angleToClosestEnergyPoint >= 180 )
		angleToClosestEnergyPoint = angleToClosestEnergyPoint - 360;

	return angleToClosestEnergyPoint;
}

