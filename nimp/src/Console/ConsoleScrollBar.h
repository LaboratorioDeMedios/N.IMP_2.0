//
//  ConsoleScrollBar.h
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#ifndef __nimp__ConsoleScrollBar__
#define __nimp__ConsoleScrollBar__

#include <stdio.h>
#include "ofMain.h"
#include "ofxMultiTouchPad.h"

class ConsoleScrollBar {
    
public:
    
    ConsoleScrollBar();
    ConsoleScrollBar(ofxMultiTouchPad* pad, int eventPriority = OF_EVENT_ORDER_AFTER_APP);
    ~ConsoleScrollBar();
    void setup(float screenRatio_);
    
    //*** LOOPS ***//
    //
    void update();
    void draw();
    
    
    //*** EVENTS ***//
    //
    bool    mouseMoved(ofMouseEventArgs &e);
    bool    mousePressed(ofMouseEventArgs &e);
    bool    mouseReleased(ofMouseEventArgs &e);
    void    windowResized(ofResizeEventArgs &e);
    bool    mouseDragged(ofMouseEventArgs &e);
    
    //*** FUNCTIONS ***//
    //
    void updateScrollBar(ofVec3f diffVec);
    void setWindowRatio(float windowRatio_) { windowRatio = windowRatio_; };
    
    
private:
    /* Display parameters for the panel */
    float margin;
    float scrollBarWidth;
    float windowRatio;
    
    float panelHeight;
    
    
    //*** VERTICAL SCROLLBAR ***//
    //
    bool isScrollBarVisible;
    bool mouseOverGrip;
    ofRectangle scrollBarRectangle;
    ofRectangle gripRectangle;
    int mousePreviousY;
    
    
    //*** TOUCHPAD ***//
    //
    ofxMultiTouchPad* pad;
    std::vector<ofPoint> touches;
    bool touchpad_scroll;
    float touchpad_scroll_x;
    float touchpad_scroll_y;

    bool applyInertia, updating;
    ofPoint prev0, prev1, post0, post1;
    ofVec2f prevDiff;
    float drag;
    float minScrollDifference;
    
};


#endif /* defined(__nimp__ConsoleScrollBar__) */
