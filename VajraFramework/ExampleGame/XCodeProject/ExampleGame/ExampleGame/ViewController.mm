#import "ViewController.h"

#import "Vajra/Engine/Core/Engine.h"
#import "Vajra/Engine/Input/Platforms/IOSInputSender.h"
#import "Vajra/Engine/Timer/Timer.h"
#import "Vajra/Placeholder/Renderer/Renderer.h"
#import "Vajra/Placeholder/Tesserakonteres.h"

#import "ExampleGame/Test/TestFile.h"

#import <objc/message.h>


@interface ViewController () {
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
	
	
	objc_msgSend([UIDevice currentDevice], @selector(setOrientation:), UIInterfaceOrientationLandscapeLeft);
    
    
    // TODO [Implement] look into possible optimizations to retain frames, etc, that can be passed in as properties for this function
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.multipleTouchEnabled = YES;
	
	// Set up the Engine input handler
	IOSInputSender *sender = [[IOSInputSender alloc] initWithFrame:self.view.frame];
	[self.view addSubview: sender];
    [self setupGL];
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}

#define DPI_IPHONE       163
#define DPI_IPAD	     132
#define DPI_IPAD_MINI    163
#define DPI_DEFAULT      160

#import <sys/utsname.h>

NSString* machineName()
{
    struct utsname systemInfo;
    uname(&systemInfo);

    return [NSString stringWithCString:systemInfo.machine
                              encoding:NSUTF8StringEncoding];
}

bool isIPadMini() {
	return  strcmp([machineName() UTF8String], "iPad2,5") == 0 ||
			strcmp([machineName() UTF8String], "iPad2,6") == 0 ||
			strcmp([machineName() UTF8String], "iPad2,7") == 0 ||
			strcmp([machineName() UTF8String], "iPad4,4") == 0 ||
			strcmp([machineName() UTF8String], "iPad4,5") == 0;
}

- (void)setupGL
{
    
    [EAGLContext setCurrentContext:self.context];
    
    // find the scale
    float scale = 1;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
		scale = [[UIScreen mainScreen] scale];
    }
	
    // ensure width and height are landscape width and height
    int width = scale * self.view.bounds.size.width;
    int height = scale * self.view.bounds.size.height;
    
    int max = fmax(width, height);
    int min = fmin(width, height);

    // Calculate device DPI
    int dpi;
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		if (isIPadMini()) {
			dpi = DPI_IPAD_MINI * scale;
		} else {
			dpi = DPI_IPAD * scale;
		}
    } else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
		dpi = DPI_IPHONE * scale;
    } else {
		dpi = DPI_DEFAULT * scale;
    }
	
    setupGraphics(max, min, dpi);
    
    Tesserakonteres::initGameObjectsForScene();
    
	TestFuntion();
    
	self.preferredFramesPerSecond = CONST_FPS;
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    renderFrame();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
}

#pragma mark -  OpenGL ES 2 shader compilation


@end
