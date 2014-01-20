#include <Utilities/ExtendedProperties.h>
//#include <zsu/Properties.h>
#include "Utilities/Misc.h"
#include <cstdlib> // new C++ way to include the exit instruction

using namespace zsu;

ExtendedProperties::ExtendedProperties() : Properties()
{
	// nothing to do.
}

bool ExtendedProperties::checkAndGetPropertyValue( std::string __propertyName, int* __propertyValue, bool __exitProgramOnError = false)
{
	if ( hasProperty(__propertyName) )
		convertFromString<int>(*__propertyValue, getProperty(__propertyName), std::dec);
	else
	{
		std::cerr << "[MISSING] " << __propertyName << " value is missing.\n";
		if ( __exitProgramOnError == true ) 
			exit(-1);
		return false;
	}
	return true;
}


bool ExtendedProperties::checkAndGetPropertyValue( std::string __propertyName, double* __propertyValue, bool __exitProgramOnError = false)
{
	if ( hasProperty(__propertyName) )
		convertFromString<double>(*__propertyValue, getProperty(__propertyName), std::dec);
	else
	{
		std::cerr << "[MISSING] " << __propertyName << " value is missing.\n";
		if ( __exitProgramOnError == true ) 
			exit(-1);
		return false;
	}
	return true;
}


bool ExtendedProperties::checkAndGetPropertyValue( std::string __propertyName, bool* __propertyValue, bool __exitProgramOnError = false)
{
	std::string s = getProperty(__propertyName);
	if ( s == "true" || s == "True" || s == "TRUE" )
		*__propertyValue = true;
	else
		if ( s == "false" || s == "False" || s == "FALSE" )
			*__propertyValue = false;
		else
		{
			std::cerr << "[CORRUPT] " << __propertyName << " should be boolean (\"true\" or \"false\").\n";
			if ( __exitProgramOnError == true ) 
				exit(-1);
			return false;
		}
	return true;
}


bool ExtendedProperties::checkAndGetPropertyValue( std::string __propertyName, std::string* __propertyValue, bool __exitProgramOnError = false)
{
	if ( hasProperty(__propertyName) )
		*__propertyValue = getProperty(__propertyName);
	else
	{
		std::cerr << "[MISSING] " << __propertyName << " string value is missing.\n";
		if ( __exitProgramOnError == true ) 
			exit (-1);
		return false;
	}
	return true;
}

