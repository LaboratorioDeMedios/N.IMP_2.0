//
//  scrollBar.h
//  NIMP_Prototipe
//
//  Created by Nicolas Peri on 11/20/15.
//
//

#ifndef __NIMP_Prototipe__scrollBar__
#define __NIMP_Prototipe__scrollBar__

#include <stdio.h>
#include "ofMain.h"
#include "ofxComposer.h"
#include "ofxMultiTouchPad.h"

class scrollBar {
    
public:
    
    scrollBar();
    scrollBar(ofxComposer* composer, ofxMultiTouchPad* pad, int eventPriority = OF_EVENT_ORDER_AFTER_APP);
    //    ~scrollBar();
    
    
    //*** LOOPS ***//
    //
    void setup();
    void update();
    void draw();
    
    
    //*** EVENTS ***//
    //
    void    mouseMoved(ofMouseEventArgs &e);
    void    keyPressed(ofKeyEventArgs &e);
    void    mousePressed(ofMouseEventArgs &e);
    void    mouseReleased(ofMouseEventArgs &e);
    void    windowResized(ofResizeEventArgs &e);
    void    mouseDragged(ofMouseEventArgs &e);
    
    void updateScrollBar(ofVec3f diffVec);
    void updateHScrollBar(ofVec3f diffVec);
    
private:
    /* Display parameters for the panel */
    float margin;
    float scrollBarWidth;
    
    float panelWidth;
    float panelHeight;
    
    
    //*** VERTICAL SCROLLBAR ***//
    //
    bool isScrollBarVisible;
    bool mouseOverGrip;
    ofRectangle scrollBarRectangle;
    ofRectangle gripRectangle;
    int mousePreviousY;
    
    
    //*** HORIZONTAL SCROLLBAR ***//
    //
    bool isHScrollBarVisible;
    bool mouseOverHGrip;
    ofRectangle hScrollBarRectangle;
    ofRectangle hGripRectangle;
    int mousePreviousX;
    
   
    //*** TOUCHPAD ***//
    //
    ofxMultiTouchPad* pad;
    std::vector<ofPoint> touches;
    bool touchpad_scroll;
    float touchpad_scroll_x;
    float touchpad_scroll_y;
    
    // CONSTANTS
    int BEGIN_X;
    int BEGIN_Y;
    
    ofxComposer* composer;
    
    ofVec4f getUntransformedCoords(int x, int y);
};


#endif /* defined(__NIMP_Prototipe__scrollBar__) */