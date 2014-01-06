package com.vajra.androidwrapper;

import android.util.Log;
import android.view.MotionEvent;

public class InputManager {

	public static void reportTouchEvent(MotionEvent event) {
		int pointerIdx = event.getActionIndex();
		int pointerId = event.getPointerId(pointerIdx);
		float x = event.getX(pointerIdx);
		float y = event.getY(pointerIdx);
		
		int touchEventType = kTOUCH_EVENT_TYPE_INVALID;
		switch (event.getActionMasked()) {
		case MotionEvent.ACTION_DOWN:
			touchEventType = kTOUCH_EVENT_TYPE_DOWN; break;
		case MotionEvent.ACTION_POINTER_DOWN:
			touchEventType = kTOUCH_EVENT_TYPE_DOWN; break;
		case MotionEvent.ACTION_UP:
			touchEventType = kTOUCH_EVENT_TYPE_UP; break;
		case MotionEvent.ACTION_POINTER_UP:
			touchEventType = kTOUCH_EVENT_TYPE_UP; break;
		case MotionEvent.ACTION_MOVE:
			touchEventType = kTOUCH_EVENT_TYPE_MOVE; break;
		case MotionEvent.ACTION_CANCEL:
			touchEventType = kTOUCH_EVENT_TYPE_CANCEL; break;
		}
		
		if (touchEventType != kTOUCH_EVENT_TYPE_INVALID) {
			VajraAndroidWrapper.touchEventOccurred(pointerId, x, y, touchEventType);
		}
	}

	// Note: these must stay in sync with their counterpart definitions in AndroidInputSender.h
	private static final int kTOUCH_EVENT_TYPE_INVALID = -1;
	private static final int kTOUCH_EVENT_TYPE_DOWN    =  0;
	private static final int kTOUCH_EVENT_TYPE_UP      =  1;
	private static final int kTOUCH_EVENT_TYPE_MOVE    =  2;
	private static final int kTOUCH_EVENT_TYPE_CANCEL  =  3;
}
