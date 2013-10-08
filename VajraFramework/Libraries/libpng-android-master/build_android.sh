#!/bin/bash

echo -e "\nCleaning Library\n"
rm -rf ./obj
rm -rf ./built

echo -e "\nBuilding Library\n"
ndk-build NDK_PROJECT_PATH=./ $@

mkdir -p ./built
echo -e "\nCopying Built Static Libraries to ./built/"
find . -name "*.a" | xargs -I {} mv {} ./built/.
echo -e "\nStop\n"

NUM_LIBRARIES_BUILT=`ls ./built/ | wc -w`
if [ $NUM_LIBRARIES_BUILT -gt 0 ]
then
	echo -e "\nSUCCESSFUL"
	echo -e "Number of Static Libraries built: $NUM_LIBRARIES_BUILT"
	ls ./built/
	echo -e ""
else
	echo -e "\nFAILED\nNO STATIC LIBRARIES WERE BUILT\n"
fi
	

