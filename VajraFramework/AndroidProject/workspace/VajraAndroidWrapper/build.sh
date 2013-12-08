#!/bin/bash

export PATH=$PATH:/usr/bin/

printUsage() {
	echo -e "\nCWD"; pwd;
	echo -e "\nERROR. Usage: ./build.sh [args]";
	echo -e "\nOptional args:";
	echo -e "\tclean";
}

if [ $# -gt 1 ]
	then
	printUsage
	exit
fi

if [ $# -eq 1 ]
then
	if [ $1 == "clean" ]
	then
		ndk-build clean
		android update lib-project -p . -t 1
		ant clean
		rm -rf ../../Built
		exit
	fi
fi

# Build Vajra as a shared library for android
echo -e "\nBuilding Vajra as a shared library for android"
ndk-build NDK_DEBUG=1

# Build the VajraAndroidWrapper jar
echo -e "\nBuilding VajraAndroidWrapper jar"
android update lib-project -p . -t 1
ant debug
# Rename the generated jar. For some reason the ant build doesn't name the jar correctly.
mv bin/classes.jar bin/vajraandroidwrapper.jar

# Copy the built products to the Built/ folder
echo -e "\nCopying the built products to the Built/ folder"
mkdir -p ../../Built/jars ../../Built/libs/armeabi ../../Built/libs/armeabi-v7a
cp bin/vajraandroidwrapper.jar ../../Built/jars/.
cp libs/armeabi/libVajra.so ../../Built/libs/armeabi/.
cp libs/armeabi-v7a/libVajra.so ../../Built/libs/armeabi-v7a/.

echo -e "\nDONE\n"

