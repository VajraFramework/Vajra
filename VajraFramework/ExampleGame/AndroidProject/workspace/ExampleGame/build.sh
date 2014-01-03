#!/bin/bash

export PATH=$PATH:/usr/bin/

printUsage() {
	echo -e "\nCWD"; pwd;
	echo -e "\nERROR. Usage: ./build.sh <arg1> [<arg2>]";
	echo -e "\narg1:";
	echo -e "\tclean";
	echo -e "\tnative";
	echo -e "\tjava";
	echo -e "\tall";
	echo -e "\narg2 [optional]:";
	echo -e "\trun";
	echo -e "\tdeploy";
}

if [ $# -gt 2 -o $# -lt 1 ]
then
	printUsage
	exit
fi

RUN=0
DEPLOY=0

BUILD_NATIVE=0
BUILD_JAVA=0

if [ $# -ge 1 ] 
then
	if [ "$1" == "clean" ] 
	then
		rm -rf bin libs gen obj
		exit
	fi

	if [ $1 == "native" ] 
	then
		BUILD_NATIVE=1
	fi
	if [ $1 == "java" ] 
	then
		BUILD_JAVA=1
	fi
	if [ $1 == "all" ] 
	then
		BUILD_NATIVE=1
		BUILD_JAVA=1
	fi

	if [ $# -eq 2 ] 
	then

		if [ "$2" == "run" ] 
		then
			RUN=1
		fi

		if [ "$2" == "deploy" ] 
		then
			DEPLOY=1
		fi
	fi
fi


CWD=`pwd`
EXAMPLE_GAME_BASE_PATH="../../../"

echo -e "\nPreparing Vajra libs and headers:";
cd $EXAMPLE_GAME_BASE_PATH;
./prepare.sh android
cd $CWD;

if [ $BUILD_NATIVE -eq 1 ] 
then
	echo -e "\nBuilding game native code:";
	ndk-build NDK_DEBUG=1
fi

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

if [ $BUILD_JAVA -eq 1 ] 
then
	android.bat update project -p . -t 1
	echo -e "\nBUILDING APK\n"
	ant debug
fi

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

