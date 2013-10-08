#!/bin/bash

if [ $# -ne 1 ]
then
	echo -e "\nUsage: ./build.sh <appname>\n";
	exit;
fi


APP_NAME="$1.app"

SIMULATOR_APPLICATION_PATH=~/Library/Application\ Support/iPhone\ Simulator/7.0/Applications;

APP_DIRECTORY=$SIMULATOR_APPLICATION_PATH/7AEB1DF6-36AE-41E6-AEE1-27881F70EC20;

xcodebuild -configuration Debug CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO -arch i386 -sdk iphonesimulator7.0 -alltargets build;

xcodebuild -configuration Debug CODE_SIGN_IDENTITY="" CODE_SIGNING_REQUIRED=NO -arch i386 -sdk iphonesimulator7.0 -alltargets install DSTROOT="$APP_DIRECTORY";

# This actually would have have put the .app folder into $APP_DIRECTORY/Applications/$APP_NAME
# Lets fix that:

echo -e "\nPath: $APP_DIRECTORY/$APP_NAME\n";

echo -e "\nRemoving old\n";
rm -r "$APP_DIRECTORY/$APP_NAME";

echo -e "\nCopying new\n";
cp -R "$APP_DIRECTORY/Applications/$APP_NAME" "$APP_DIRECTORY/$APP_NAME";

echo -e "\nRemoving temp\n";
rm -r "$APP_DIRECTORY/Applications";

echo -e "\nDONE\n";
