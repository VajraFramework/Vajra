#!/bin/bash

if [ $# -lt 1 ]
then
	echo -e "\nUsage: ./prepare.sh <platform>"
	echo -e "    supported platforms: ios";
	echo -e "\n";
	exit
fi

SRCROOT="../Vajra/";
HEADERS_DESTINATION="./include/Vajra";
echo -e "Extracting and copying Headers from $SRCROOT/ to $HEADERS_DESTINATION/";
mkdir -p $HEADERS_DESTINATION
rsync -r --delete --include "*/" --include "*.h" --exclude "*" $SRCROOT/* $HEADERS_DESTINATION/.

LIBRARIES_HEADERS_ROOT="../Libraries/glm";
LIBRARIES_HEADERS_DESTINATION="./include/Libraries/glm";
echo -e "Extracting and copying Headers from $LIBRARIES_HEADERS_ROOT/ to $LIBRARIES_HEADERS_DESTINATION/";
mkdir -p $LIBRARIES_HEADERS_DESTINATION
rsync -r --delete $LIBRARIES_HEADERS_ROOT/* $LIBRARIES_HEADERS_DESTINATION/.

if [ $1 == "ios" ]
then
	LIBRARIES_BASE_PATH="../XCodeProject/VajraStaticLibrary/Built/";
	LIBRARIES_IPHONEOS_PATH=$LIBRARIES_BASE_PATH"iphoneos/";
	LIBRARIES_IPHONESIMULATOR_PATH=$LIBRARIES_BASE_PATH"iphonesimulator/";
	#
	LIBRARIES_DESTINATION_BASE_PATH="./lib/ios/";
	LIBRARIES_DESTINATION_IPHONEOS_PATH=$LIBRARIES_DESTINATION_BASE_PATH"iphoneos/";
	LIBRARIES_DESTINATION_IPHONESIMULATOR_PATH=$LIBRARIES_DESTINATION_BASE_PATH"iphonesimulator/";
	#
	echo -e "Current directory is: "; pwd ;
	echo -e "Copying library files from $LIBRARIES_IPHONEOS_PATH to $LIBRARIES_DESTINATION_IPHONEOS_PATH";
	cp $LIBRARIES_IPHONEOS_PATH/* $LIBRARIES_DESTINATION_IPHONEOS_PATH/.
	echo -e "Copying library files from $LIBRARIES_IPHONESIMULATOR_PATH to $LIBRARIES_DESTINATION_IPHONESIMULATOR_PATH";
	cp $LIBRARIES_IPHONESIMULATOR_PATH/* $LIBRARIES_DESTINATION_IPHONESIMULATOR_PATH/.

elif [ $1 == "android" ]
then
	LIBRARIES_BASE_PATH="../AndroidProject/Built/libs/";
	LIBRARIES_ARMEABI_PATH=$LIBRARIES_BASE_PATH"armeabi";
	LIBRARIES_ARMEABIV7A_PATH=$LIBRARIES_BASE_PATH"armeabi-v7a";
	#
	LIBRARIES_DESTINATION_BASE_PATH="./lib/android/";
	LIBRARIES_DESTINATION_ARMEABI_PATH=$LIBRARIES_DESTINATION_BASE_PATH"armeabi";
	LIBRARIES_DESTINATION_ARMEABIV7A_PATH=$LIBRARIES_DESTINATION_BASE_PATH"armeabi-v7a";
	#
	JAR_FILE_PATH="../AndroidProject/Built/jars/vajraandroidwrapper.jar";
	JAR_FILE_DESTINATION="AndroidProject/workspace/ExampleGame/libs/";
	#
	mkdir -p $LIBRARIES_DESTINATION_ARMEABI_PATH;
	mkdir -p $LIBRARIES_DESTINATION_ARMEABIV7A_PATH;
	echo -e "Current directory is: "; pwd ;
	echo -e "Copying library files from $LIBRARIES_ARMEABI_PATH to $LIBRARIES_DESTINATION_ARMEABI_PATH";
	cp $LIBRARIES_ARMEABI_PATH/*.so $LIBRARIES_DESTINATION_ARMEABI_PATH/.
	echo -e "Copying library files from $LIBRARIES_ARMEABIV7A_PATH to $LIBRARIES_DESTINATION_ARMEABIV7A_PATH";
	cp $LIBRARIES_ARMEABIV7A_PATH/*.so $LIBRARIES_DESTINATION_ARMEABIV7A_PATH/.
	#
	mkdir -p $JAR_FILE_DESTINATION;
	echo -e "Copying jar file from $JAR_FILE_PATH to $JAR_FILE_DESTINATION";
	cp $JAR_FILE_PATH $JAR_FILE_DESTINATION/.
else
	echo -e "\nUnrecognized platform";
fi


echo -e "\n";

