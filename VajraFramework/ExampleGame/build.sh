#!/bin/bash

export PATH=$PATH:/usr/bin/

printUsage() {
	echo -e "\nCWD"; pwd;
	echo -e "\nERROR. Usage: ./build.sh <platform> [args]";
	echo -e "\nAvailable Platforms:";
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

RUN=0

MAKEFILE_ARGS=""
if [ $# -eq 2 ]
then
	if [ $2 == "run" ]
	then
		RUN=1
	else
		MAKEFILE_ARGS=$2
	fi
fi

################################################################################

CWD=`pwd`
EXAMPLE_GAME_BASE_PATH="./";

echo -e "\nPreparing Vajra libs and headers:";
cd $EXAMPLE_GAME_BASE_PATH;
./prepare.sh $1
cd $CWD;

if [ "$1" == 'linux' ]
then
	echo -e "\nPLATFORM: linux\n";

	MAKE="make"
	$MAKE -f DesktopProject/makefile_linux $MAKEFILE_ARGS
	
elif [ "$1" == "windows" ]
then
	echo -e "\nPLATFORM: windows\n";

	MAKE="./../Tools/Windows/GnuWin32/bin/make.exe"
	$MAKE -f DesktopProject/makefile_windows $MAKEFILE_ARGS

else
	printUsage
	exit
fi

if [ $RUN -eq 1 ]
then
	echo "\nRUNNING EXECUTABLE";
	cd ./DesktopProject/Build/bin/
	./executable.exe
fi

################################################################################

