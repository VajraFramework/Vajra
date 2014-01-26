#!/bin/bash

printUsage() {
	echo -e "\nUsage: ./prepare.sh <platform> [<architecture>]"
	echo -e "    supported platforms: ios, android, linux, windows";
	echo -e "    architectures (required for ios only): arm, i386";
	echo -e "\n";
}

if [ $# -lt 1 ]
then
	printUsage
	exit
fi

SRCROOT="../Vajra/";
HEADERS_DESTINATION="./include/Vajra";
echo -e "Extracting and copying Headers from $SRCROOT/ to $HEADERS_DESTINATION/";
mkdir -p $HEADERS_DESTINATION
rsync -r --delete --update --include "*/" --include "*.h" --exclude "*" $SRCROOT/* $HEADERS_DESTINATION/.

LIBRARIES_HEADERS_ROOT="../Libraries/glm";
LIBRARIES_HEADERS_DESTINATION="./include/Libraries/glm";
echo -e "Extracting and copying Headers from $LIBRARIES_HEADERS_ROOT/ to $LIBRARIES_HEADERS_DESTINATION/";
mkdir -p $LIBRARIES_HEADERS_DESTINATION
rsync -r --delete --update $LIBRARIES_HEADERS_ROOT/* $LIBRARIES_HEADERS_DESTINATION/.

LIBRARIES_HEADERS_ROOT="../Libraries/openal/headers";
LIBRARIES_HEADERS_DESTINATION="./include/Libraries/openal/headers";
echo -e "Extracting and copying Headers from $LIBRARIES_HEADERS_ROOT/ to $LIBRARIES_HEADERS_DESTINATION/";
mkdir -p $LIBRARIES_HEADERS_DESTINATION
rsync -r --delete --update $LIBRARIES_HEADERS_ROOT/* $LIBRARIES_HEADERS_DESTINATION/.

if [ $1 == "ios" ]
then
	if [ $# -lt 2 -o $# -gt 2 ]
	then
		printUsage
		exit
	fi

	echo -e "\nCurrent architecture: $2";
	SIMULATOR=0
	if [ $2 == "i386" ]
	then
		SIMULATOR=1
	fi

	if [ $SIMULATOR -eq 1 ]
	then
		LIBRARIES_BASE_PATH="../XCodeProject/VajraStaticLibrary/Built/iphonesimulator";
	else
		LIBRARIES_BASE_PATH="../XCodeProject/VajraStaticLibrary/Built/iphoneos";
	fi
	#
	LIBRARIES_DESTINATION_BASE_PATH="./lib/ios/";
	LIBRARIES_DESTINATION_IPHONEOS_PATH=$LIBRARIES_DESTINATION_BASE_PATH"iphoneos/";
	LIBRARIES_DESTINATION_IPHONESIMULATOR_PATH=$LIBRARIES_DESTINATION_BASE_PATH"iphonesimulator/";
	#
	echo -e "Current directory is: "; pwd ;
	echo -e "Copying library files from $LIBRARIES_BASE_PATH to $LIBRARIES_DESTINATION_IPHONEOS_PATH";
	cp $LIBRARIES_BASE_PATH/* $LIBRARIES_DESTINATION_IPHONEOS_PATH/.
	echo -e "Copying library files from $LIBRARIES_BASE_PATH to $LIBRARIES_DESTINATION_IPHONESIMULATOR_PATH";
	cp $LIBRARIES_BASE_PATH/* $LIBRARIES_DESTINATION_IPHONESIMULATOR_PATH/.

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
	RESOURCES_PATH="./Resources/";
	ASSETS_PATH="./AndroidProject/workspace/ExampleGame/assets/Resources/";
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
	#
	mkdir -p $ASSETS_PATH
	echo -e "Copying resource files from $RESOURCES_PATH to $ASSETS_PATH";
	rsync -r --delete $RESOURCES_PATH $ASSETS_PATH/.

elif [ $1 == "linux" ]
then
	LIBRARIES_BASE_PATH="../DesktopProject/Built/bin/";
	LIBRARIES_DESTINATION_BASE_PATH="./lib/desktop/";
	mkdir -p $LIBRARIES_DESTINATION_BASE_PATH;
	echo -e "Copying library files from $LIBRARIES_BASE_PATH to $LIBRARIES_DESTINATION_BASE_PATH";
	cp $LIBRARIES_BASE_PATH/libVajra.a $LIBRARIES_DESTINATION_BASE_PATH/.

elif [ $1 == "windows" ]
then
	LIBRARIES_BASE_PATH="../DesktopProject/Built/bin/";
	ADDITIONAL_LIBRARIES="../Libraries/openal/built/OpenAL32.lib"
	LIBRARIES_DESTINATION_BASE_PATH="./lib/desktop/";
	mkdir -p $LIBRARIES_DESTINATION_BASE_PATH;
	echo -e "Copying library files from $LIBRARIES_BASE_PATH to $LIBRARIES_DESTINATION_BASE_PATH";
	cp $LIBRARIES_BASE_PATH/libVajra.a $LIBRARIES_DESTINATION_BASE_PATH/.
	cp $ADDITIONAL_LIBRARIES $LIBRARIES_DESTINATION_BASE_PATH/.

else
	echo -e "\nUnrecognized platform: $1";
fi

echo -e "\n";

