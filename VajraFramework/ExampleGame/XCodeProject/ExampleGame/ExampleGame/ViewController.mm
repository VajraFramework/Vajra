#import "ViewController.h"

#import "Vajra/Engine/Core/Engine.h"
#import "Vajra/Engine/Input/Platforms/IOSInputSender.h"
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

- (void)setupGL
{
    
    [EAGLContext setCurrentContext:self.context];
    
    int width = self.view.bounds.size.width;
    int height = self.view.bounds.size.height;
    //TODO [Hack] set these properly when the device moves
    setupGraphics(height, width);
    
    Tesserakonteres::initGameObjectsForScene();
    Tesserakonteres::initUiGameObjects();
    
	TestFuntion();
    
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    renderFrame(0.02f);
}

#pragma mark -  OpenGL ES 2 shader compilation


@end
