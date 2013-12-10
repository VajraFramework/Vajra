#import "AppDelegate.h"

#import "Vajra/Engine/Core/Engine.h"
#import "Vajra/Engine/Profiler/Profiler.h"
#import "Vajra/Framework/DeviceUtils/DeviceStatistics/DeviceStatistics.h"
#import "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#import <fstream>

void printDebugLog() {
	// Ensure that the log file exists in the file structure.
	std::string logPath = FRAMEWORK->GetFileSystemUtils()->GetDeviceLoggingResourcesPath();
	std::string logFilename = logPath + GetOperatingSystem() + ".log";
	NSFileManager* fileMan = [NSFileManager defaultManager];
	NSString* logFilePathNSString = [NSString stringWithCString:logPath.c_str() encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:logFilePathNSString isDirectory:nullptr]) {
		[fileMan createDirectoryAtPath:logFilePathNSString withIntermediateDirectories:NO attributes:nil error:nil];
	}
	NSString* logFilenameNSString = [NSString stringWithCString:(logFilename.c_str()) encoding:NSASCIIStringEncoding];
	if (![fileMan fileExistsAtPath:logFilenameNSString]) {
		[fileMan createFileAtPath:logFilenameNSString contents:nil attributes:nil];
	}
	
	std::ofstream logFile;
	logFile.open(logFilename, std::ios_base::out | std::ios_base::app);
	if (!logFile.fail()) {
		FRAMEWORK->GetLogger()->dbglog("Writing log data to logging file %s", logFilename.c_str());
		ENGINE->GetProfiler()->PrintAllExperimentData(logFile);
		logFile.close();
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Could not open logging file %s", logFilename.c_str());
		ENGINE->GetProfiler()->PrintAllExperimentData();
	}
}

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	printDebugLog();
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
