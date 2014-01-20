/*
 * LogManager.cpp
 *
 *  Created on: 15 juin 2009
 *      Author: delarbop
 */

#include "Utilities/LogManager.h"


std::string LogManager::PATH_D="./logs/";


LogManager::LogManager(const std::string &c_name) {

	path =  PATH_D+c_name ;

	file =new  std::fstream( path.c_str()  , std::ios::out | std::ios::trunc );
	//file =new  std::fstream( path.c_str()  , std::ios::out  );

	if(! file)
	{
		std::cerr << " unable to create " << path << std::endl;
	}
}

LogManager::~LogManager() {

	file->close();
	delete file;
}

void LogManager::write(const std::string & line)
{
	if(file)
	{
		*file <<  line << std::endl;
	}
}

