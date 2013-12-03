//
//  iOSInputSender.mm
//  Created by Alex R. Hogue on 12/3/13.
//

#ifdef PLATFORM_IOS
#import "Vajra/Engine/Input/Platforms/iOSInputSender.h"
#import "Vajra/Engine/Core/Engine.h"
#import "Vajra/Engine/Input/Input.h"
#import <UIKit/UIKit.h>
@interface iOSInputSender ()

@end

@implementation iOSInputSender

- (id) initWithFrame:(CGRect)frame
{
	if (self = [super initWithFrame:frame])
    {
        self.userInteractionEnabled = YES;
		self.multipleTouchEnabled = YES;
	}
	return self;
}

- (void) touchesBegan:(NSSet *) touches withEvent:(UIEvent *) event
{
    for (UITouch *touch in touches)
	{
		CGPoint pt = [touch locationInView:self];
        ENGINE->GetInput()->AddTouch(0, pt.x, pt.y);
    }
}

- (void) touchesMoved:(NSSet *) touches withEvent:(UIEvent *) event
{
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{

}

- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{

}

- (void) updateTouches:(NSSet*)touches second:(TouchPhase)phase
{
    for(UITouch *touch in touches)
    {
		CGPoint pt = [touch locationInView:self];
        ENGINE->GetInput()->UpdateTouch(0, pt.x, pt.y, phase);
    }
}
@end
#endif