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
	echo -e "\trun\t(supported only on platforms: android, linux, windows)";
	echo -e "\tdeploy\t(supported only on platform: android)";
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
	$MAKE -f Makefiles/makefile_desktop $MAKEFILE_ARGS
	
elif [ "$1" == "windows" ]
	then
	echo -e "\nPLATFORM: windows\n";
	MAKE="./Tools/Windows/GnuWin32/bin/make.exe"
	$MAKE -f Makefiles/makefile_windows $MAKEFILE_ARGS

elif [ "$1" == "android" ]
	then
	echo -e "\nPLATFORM: android\n";

	RUN=0
	DEPLOY=0

	if [ "$MAKEFILE_ARGS" == "run" ]
		then
		RUN=1
	fi
	if [ "$MAKEFILE_ARGS" == "deploy" ]
		then
		DEPLOY=1
	fi

	if [ "$MAKEFILE_ARGS" != "clean" ]
		then
		MAKEFILE_ARGS=""
	fi

	cd ./Android/workspace/GL2JNIActivity/.
	ndk-build NDK_DEBUG=1 $MAKEFILE_ARGS

	if [ $RUN -eq 1 -o $DEPLOY -eq 1 ]
		then
		cd ./Android/workspace/GL2JNIActivity/
		echo -e "\nBUILDING APK\n"
		android.bat update project -p . -t 1
		ant debug
		adb shell pm uninstall com.android.gl2jni
		echo -e "\nDEPLOYING TO DEVICE\n"
		adb install -r ./bin/GL2JNIActivity-debug.apk
		if [ $RUN -eq 1 ]
			then
			echo -e "\nRUNNING...\n"
			adb shell am start -n com.android.gl2jni/com.android.gl2jni.GL2JNIActivity
		fi
	fi

else
	printUsage
	exit
fi


exit
