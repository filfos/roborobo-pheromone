/*
 *  main.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *  List of Updates:
 *    - 2012-06-05 by Leo
 *
 *  See roborbo.cpp for description
 *
 */

#include "RoboroboMain/main.h"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// For getopt
extern char *optarg;
extern int  optind, opterr;


void displayGeneralInformation()
{
	std::cout << std::endl;
	std::cout << " =-= Roborobo! " << std::endl;
	std::cout << " =-= nicolas.bredeche(at)gmail.com" << std::endl;
	std::cout << " =-= Official version tag    : " << gVersion << std::endl;
	std::cout << " =-= Current build name      : " << gCurrentBuildInfo << std::endl;
	std::cout << " =-= Compilation version tag : " << gCompileDate << " - " << gCompileTime << std::endl;
    
	std::cout << std::endl;
}


// Called when roborobo must quit
void quit(int signal)
{
	closeRoborobo();
	exit(0);
}

/** Print usage (e.g. when user enters bad command line options, or '-h' parameter) */
static void usage(char* argv0)
{
	std::cout << std::endl << "Roborobo! Command line help:" << std::endl << std::endl;
	std::cout << "  usage  : " << argv0 << " [-h] [-v] [-l <propertiesfilename>]" << std::endl << std::endl;
	std::cout << "  options:" << std::endl;
	std::cout << "    h: print this help message." << std::endl;
	std::cout << "    v: print current version number." << std::endl;
	std::cout << "    l: specify location and name of the properties file." << std::endl;
	std::cout << "         next parameter should be the filename (e.g. ./roborobo -l config/MyConfigFile.properties\")" << std::endl;
	std::cout << "         default properties file is \"" << gPropertiesFilename << "\"" << std::endl;
	std::cout << std::endl;
}

/** Print infos about this version of Roborobo */
static void versionInfos()
{
	std::cout << std::endl;
	std::cout << " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
	std::cout << " =-=-=-=-=-=-= Roborobo! =-=-=-=-=-=-=" << std::endl;
	std::cout << " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
	std::cout << std::endl;
	std::cout << "  description     : A lightweight robot/agent swarm simulator written in C++/SDL." << std::endl;
	std::cout << "  author          : nicolas.bredeche(at)gmail.com" << std::endl;
	std::cout << "  started         : december 2008" << std::endl;
	std::cout << "  version tag     : " << gVersion << std::endl;
	std::cout << "  current build   : " << gCurrentBuildInfo << std::endl << std::endl;
	std::cout << "  compilation tag : " << gCompileDate << " - " << gCompileTime << std::endl;
}


/** default demo mode with visualization. If at least one argument, launch demo in batch mode (ie. no display, fast pace). */
int main(int argc, char** argv)
{
	// Install signal handler, to handle Ctrl-C and 'kill' signals
	signal(SIGINT, quit);
	signal(SIGTERM, quit);
    
	// Parse Command line parameters (in argv), using getopt
	static char optstring[] = "vl:";
	opterr=0;
    
		switch(getopt(argc, argv, optstring))
		{
			case 'v':
				versionInfos();
				return -1;
				break;
                
			case 'l':
				displayGeneralInformation();
				gPropertiesFilename = optarg;
				std::cout << "[info] Properties will be loaded from file \"" << gPropertiesFilename << "\"." << std::endl;
				break;
                
			case 'h':
				usage(argv[0]);
				return -1;
				break;
                
			default:
                usage(argv[0]);
                return -1;
				break;
		}
    
	// Get remaining command line parameters (i.e. not used by getopt, so parameters that do not begin by '-')
	for(int i = optind; i < argc; i++)
		gRemainingCommandLineParameters.push_back(argv[i]);
    
    /**/
    char *path = NULL;
    size_t size = 0;
    path = getcwd(path,size);
    std::cout << "[info] Current location : " << path << std::endl;
    /**/
    
	std::cout << std::endl << std::endl;
    
	int returnValue = 0;
	returnValue = launchRoborobo();
    
	return returnValue;
}

