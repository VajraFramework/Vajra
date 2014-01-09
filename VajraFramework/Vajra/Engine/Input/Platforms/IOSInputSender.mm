//
//  IOSInputSender.m
//  VajraStaticLibrary
//
//  Created by Alex Hogue on 12/11/13.
//  Copyright (c) 2013 Vajra. All rights reserved.
//

#import "Vajra/Engine/Core/Engine.h"
#import "Vajra/Engine/Input/Input.h"
#import "Vajra/Engine/Input/Platforms/IOSInputSender.h"

@implementation IOSInputSender

- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame: frame];
	// resize the view on rotate
	self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleBottomMargin |
	UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleRightMargin |
	UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
	
	// subscribe to the pinch gesture
	UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinch:)];
	[self addGestureRecognizer: pinch];
	
	return self;
}
- (void) touchesBegan:(NSSet *) touches withEvent:(UIEvent *) event {
	for (UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		ENGINE->GetInput()->AddTouch((long)(id)touch, pt.x, pt.y);
    }
}

- (void) touchesMoved:(NSSet *) touches withEvent:(UIEvent *) event {
    [self updateTouches:touches second:TouchPhase::Moved];
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    [self updateTouches:touches second:TouchPhase::Ended];
}

- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    [self updateTouches:touches second:TouchPhase::Cancelled];
}

- (void) handlePinch: (UIPinchGestureRecognizer *) uigr
{
	ENGINE->GetInput()->UpdatePinch(uigr.scale, uigr.velocity, (GestureState)uigr.state);
}

- (void) updateTouches:(NSSet*)touches second:(TouchPhase)phase {
    for(UITouch *touch in touches) {
		CGPoint pt = [touch locationInView:self];
		ENGINE->GetInput()->UpdateTouch((long)(id)touch, pt.x, pt.y, phase);
    }
}
@end
