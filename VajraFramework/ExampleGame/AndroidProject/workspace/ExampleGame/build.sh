#!/bin/bash

export PATH=$PATH:/usr/bin/

printUsage() {
	echo -e "\nCWD"; pwd;
	echo -e "\nERROR. Usage: ./build.sh [args]";
	echo -e "\nOptional args:";
	echo -e "\tclean";
	echo -e "\trun";
	echo -e "\tdeploy";
}

if [ $# -gt 1 ]
then
	printUsage
	exit
fi

if [ $# == 1 ]
then
	if [ "$1" == "clean" ]
	then
		rm -rf bin libs gen obj
		exit
	fi

	RUN=0
	DEPLOY=0
	if [ "$1" == "run" ]
	then
		RUN=1
	fi

	if [ "$1" == "deploy" ]
	then
		DEPLOY=1
	fi
fi


CWD=`pwd`
EXAMPLE_GAME_BASE_PATH="../../../"

echo -e "\nPreparing Vajra libs and headers:";
cd $EXAMPLE_GAME_BASE_PATH;
./prepare.sh android
cd $CWD;

echo -e "\nBuilding game native code:";
ndk-build NDK_DEBUG=1

# Must copy over the Vajra prebuilt library to the libs folder:
cd $EXAMPLE_GAME_BASE_PATH;
VAJRA_PREBUILT_LIBRARY_BASE_PATH="./lib/android";
VAJRA_PREBUILT_LIBRARY_DESTINATION_BASE_PATH="./AndroidProject/workspace/ExampleGame/libs/"
cp $VAJRA_PREBUILT_LIBRARY_BASE_PATH"/armeabi/libVajra.so" $VAJRA_PREBUILT_LIBRARY_DESTINATION_BASE_PATH"/armeabi/."
cp $VAJRA_PREBUILT_LIBRARY_BASE_PATH"/armeabi-v7a/libVajra.so" $VAJRA_PREBUILT_LIBRARY_DESTINATION_BASE_PATH"/armeabi-v7a/."
if [ ! -f $VAJRA_PREBUILT_LIBRARY_DESTINATION_BASE_PATH"/armeabi/libVajra.so" ]
then
	echo -e "\nERROR: Couldn't copy vajra prebuilt library";
	exit
fi
if [ ! -f $VAJRA_PREBUILT_LIBRARY_DESTINATION_BASE_PATH"/armeabi-v7a/libVajra.so" ]
then
	echo -e "\nERROR: Couldn't copy vajra prebuilt library";
	exit
fi
cd $CWD;

android update project -p . -t 1
echo -e "\nBUILDING APK\n"
ant debug

if [ $# -eq 1 ]
then
	if [ $RUN -eq 1 -o $DEPLOY -eq 1 ]
	then
		adb shell pm uninstall com.vajra.examplegame
		echo -e "\nDEPLOYING TO DEVICE\n"
		adb install -r ./bin/GL2JNIActivity-debug.apk
		if [ $RUN -eq 1 ]
		then
			echo -e "\nRUNNING...\n"
			adb shell am start -n com.vajra.examplegame/com.vajra.examplegame.ExampleGameActivity
		fi
	fi
fi
