#!/bin/bash

export PATH=$PATH:/usr/bin/

printUsage() {
	echo -e "\nCWD"; pwd;
	echo -e "\nERROR. Usage: ./build.sh <platform> [args]";
	echo -e "\nAvailable Platforms:";
	echo -e "\tandroid";
	echo -e "\tlinux";
	echo -e "\twindows";
	echo -e "\nOptional args:";
	echo -e "\tclean";
	echo -e "\trun\t(supported only on platforms: linux, windows)";
}

if [ $# -lt 1  -o  $# -gt 2 ]
then
	printUsage
	exit
fi

MAKEFILE_ARGS=""
if [ $# -eq 2 ]
	then
	MAKEFILE_ARGS=$2
fi

if [ "$1" == 'linux' ]
	then
	echo -e "\nPLATFORM: linux\n";

	MAKE="make"
	$MAKE -f DesktopProject/makefile_desktop $MAKEFILE_ARGS
	
elif [ "$1" == "windows" ]
	then
	echo -e "\nPLATFORM: windows\n";

	MAKE="./Tools/Windows/GnuWin32/bin/make.exe"
	$MAKE -f DesktopProject/makefile_windows $MAKEFILE_ARGS

elif [ "$1" == "android" ]
	then
	echo -e "\nPLATFORM: android\n";

	if [ "$MAKEFILE_ARGS" != "clean" ]
		then
		MAKEFILE_ARGS=""
	fi

	cd ./AndroidProject/workspace/VajraAndroidWrapper/
	./build.sh $MAKEFILE_ARGS

else
	printUsage
	exit
fi


exit
