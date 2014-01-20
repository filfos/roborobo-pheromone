#!/bin/bash

usage()
{
	echo -e "\033[1mSYNOPSIS\033[0m"
	echo -e "	\033[1m./createProjectFrom\033[0m [OPTION] BASE_PROJECT NEW_PROJECT"
	echo -e ""
	echo -e "\033[1mDESCRIPTION\033[0m"
	echo -e "	Create NEW_PROJECT by copying the files of BASE_PROJECT"
	echo -e ""
	echo -e "	BASE_PROJECT is the name of the project from where all the source code will be copied"
	echo -e ""
	echo -e "	NEW_PROJECT is the name of the project that will be created"
	echo -e ""
	echo -e "	\033[1m-b, --baseDirectory\033[0m=DIRECTORY_PATH"
	echo -e "		use DIRECTORY_PATH instead of BASE_PROJECT as the directory name where BASE_PROJECT is stored"
	echo -e ""
	echo -e "	\033[1m-n, --newDirectory\033[0m=DIRECTORY_PATH"
	echo -e "		use DIRECTORY_PATH instead of NEW_PROJECT as the directory name where NEW_PROJECT will be stored"
	echo -e ""
	echo -e "	\033[1m-c, --baseConfigurationLoader\033[0m=CONFIGURATION_LOADER_NAME"
	echo -e "		use CONFIGURATION_LOADER_NAME.cpp and CONFIGURATION_LOADER_NAME.h files for the ConfigurationLoader of the base project"
	echo -e ""
	echo -e "	\033[1m-C, --newConfigurationLoader\033[0m=CONFIGURATION_LOADER_NAME"
	echo -e "		use CONFIGURATION_LOADER_NAME.cpp and CONFIGURATION_LOADER_NAME.h files for the ConfigurationLoader of the new project"
	echo -e ""
	echo -e "\033[1mAFTER\033[0m"
	echo -e "	Then you will have to do the following actions : "
	echo -e "	# Edit the src/core/ConfigurationLoader.cpp."
	echo -e "		. You will have to use a new keyword to use the newProject ConfigurationLoader."
	echo -e "		. While editing this file, don't forget to include the .h file !"
	echo -e "	# Write a dedicated config file"
	echo -e "	# Add your project in the svn"
}

if [ $# -lt 2 ]
then
	usage
else
	
	baseDirectory=""
	newDirectory=""
	baseName=""
	newName=""

	baseConfigurationLoader=""
	newConfigurationLoader=""

	while [ "$1" != "" ]; do
			case $1 in
					-b | --baseDirectory )  shift
																	baseDirectory=$1
																	;;
					-n | --newDirectory )		shift    
																	newDirectory=$1
																	;;
					-c | --baseConfigurationLoader) shift
																	baseConfigurationLoader=$1
																	;;
					-C | --newConfigurationLoader) shift
																	newConfigurationLoader=$1
																	;;
					-h | --help )           usage
																	exit
																	;;
					*) break
			esac
			shift
	done

	baseName=$1
	newName=$2
	baseName=${baseName%\/} #Remove / at the end of the name
	newName=${newName%\/} #Remove / at the end of the name
	upperBaseName=`echo $baseName | tr '[:lower:]' '[:upper:]';`
	upperNewName=`echo $newName | tr '[:lower:]' '[:upper:]';`

	if [ "$baseDirectory" = "" ]
	then
		baseDirectory=$baseName
	fi
	if [ "$newDirectory" = "" ]
	then
		newDirectory=$2
	fi
	baseDirectory=${baseDirectory%\/} #Remove / at the end of the name
	newDirectory=${newDirectory%\/} #Remove / at the end of the name

	upperBaseDirectory=`echo $baseDirectory | tr '[:lower:]' '[:upper:]';`
	upperNewDirectory=`echo $newDirectory | tr '[:lower:]' '[:upper:]';`

	if [ "$baseConfigurationLoader" = "" ]
	then
		baseConfigurationLoader=${baseName}ConfigurationLoader
	fi
	if [ "$newConfigurationLoader" = "" ]
	then
		newConfigurationLoader=${newName}ConfigurationLoader
	fi
	upperBaseConfigurationLoader=`echo $baseConfigurationLoader | tr '[:lower:]' '[:upper:]' | sed 's/\-//g';`
	upperNewConfigurationLoader=`echo $newConfigurationLoader | tr '[:lower:]' '[:upper:]' | sed 's/\-//g';`

	modularBaseName=PRJ_`echo $upperBaseName | sed 's/\-/\_/g' `
	modularNewName=PRJ_`echo $upperNewName | sed 's/\-/\_/g' `


	#copy structure of the previous project
	cp -r $baseDirectory $newDirectory
	rm -rf $newDirectory/.svn
	rm -rf $newDirectory/*/.svn
	rm -rf $newDirectory/*/*/.svn
	#find . -name .svn -exec rm -fr {} \;

	#rename the files
	cd $newDirectory/src
	#rename s/"${baseName}(.*)"\\.cpp/"${newName}\$1".cpp/g *
	for i in *.cpp
	do
		newFileName="${newName}${i#$baseName}"
		mv $i $newFileName
	done
	cd ../include
	#rename s/"${baseName}(.*)"\\.h/"${newName}\$1".h/g *
	for i in *.h
	do
		newFileName="${newName}${i#$baseName}"
		mv $i $newFileName
	done
	cd ..

	#modify the files
	perl -pi -e s/"#include \"${baseDirectory}(.*)\""/"#include \"${newDirectory}\$1\""/ src/*.cpp
	perl -pi -e s/$baseName/$newName/g src/*.cpp
	perl -pi -e s/"#include \"${baseDirectory}(.*)\""/"#include \"${newDirectory}\$1\""/ include/*.h
	perl -pi -e s/$upperBaseName/$upperNewName/g include/*.h
	perl -pi -e s/$baseName/$newName/g include/*.h
	cd ../../

	#copy/modify ConfigurationLoader.cpp
	cp "src/ext/${baseConfigurationLoader}.cpp" "src/ext/${newConfigurationLoader}.cpp"
	perl -pi -e s/"#include \"${baseDirectory}(.*)\""/"#include \"${newDirectory}\$1\""/ "src/ext/${newConfigurationLoader}.cpp"
	perl -pi -e s/$baseName/$newName/g "src/ext/${newConfigurationLoader}.cpp"
	perl -pi -e s/$baseConfigurationLoader/$newConfigurationLoader/g "src/ext/${newConfigurationLoader}.cpp"
	perl -pi -e s/$modularBaseName/$modularNewName/g "src/ext/${newConfigurationLoader}.cpp"

	#copy/modify ConfigurationLoader.h
	cp "include/ext/Config/${baseConfigurationLoader}.h" "include/ext/Config/${newConfigurationLoader}.h"
	perl -pi -e s/"#include \"${baseDirectory}(.*)\""/"#include \"${newDirectory}\$1\""/ "include/ext/Config/${newConfigurationLoader}.h"
	perl -pi -e s/$baseName/$newName/g "include/ext/Config/${newConfigurationLoader}.h"
	perl -pi -e s/$upperBaseName/$upperNewName/g "include/ext/Config/${newConfigurationLoader}.h"
	perl -pi -e s/$baseConfigurationLoader/$newConfigurationLoader/g "include/ext/Config/${newConfigurationLoader}.h"
	perl -pi -e s/$modularBaseName/$modularNewName/g "include/ext/Config/${newConfigurationLoader}.h"
	perl -pi -e s/$upperBaseConfigurationLoader/$upperNewConfigurationLoader/g "include/ext/Config/${newConfigurationLoader}.h"

	#tell a human what to do next
	echo "You have to do the following actions : "
	echo "# Edit the src/core/ConfigurationLoader.cpp."
	echo "	. Use a new keyword to use $newName ConfigurationLoader."
	echo "	. While editing this file, don't forget to include the .h file !"
	echo "# Write a dedicated config file"
	echo "# Add your project in the svn"
fi

