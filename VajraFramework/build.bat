set cwd=%~dp0%
echo "Current Working Directory"
echo %cwd%

:: Fix the cwd for cygwin

set driveLetter=%cwd:~0,1%
set pathInDrive=%cwd:~2%
set pathInDrive=%pathInDrive:\=/%
set pathInDrive=%pathInDrive: =\ %

set cwdPathForCygwin=/cygdrive/%driveLetter%%pathInDrive%
echo %cwdPathForCygwin%

bash.exe --login -c 'cd %cwdPathForCygwin% ; dos2unix.exe build.sh; ./build.sh %*'
