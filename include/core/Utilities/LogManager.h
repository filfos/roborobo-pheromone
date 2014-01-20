/*
 * LogManager.h
 *
 *  Created on: 15 juin 2009
 *      Author: delarbop - slight refactoring on 2010/05/25 (nicolas) -- original class name: Logger
 */

#ifndef LOGMANAGER_H_
#define LOGMANAGER_H_
#include <fstream>
#include <iostream>


class LogManager {

private:

	std::fstream *file;
	std::string path;

public:

	static std::string PATH_D;
	static inline void set_path_d(std::string path){PATH_D = path;}


	LogManager(){}
	LogManager(const std::string &c_name);
	virtual ~LogManager();

	virtual void writeLog(){}

	void write(const std::string & line);

	inline void set_path(std::string name)
	{
		path =  PATH_D+name ;
		file =new  std::fstream( path.c_str()  , std::ios::out | std::ios::trunc );
		if(! file)
		{
			std::cerr << " [warning] Unable to create " << path << std::endl;
		}
	}

};

#endif
