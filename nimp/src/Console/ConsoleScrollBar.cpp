//
//  ConsoleScrollBar.cpp
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#include "ConsoleScrollBar.h"
#include "enumerations.h"
#include "ConsoleLog.h"
#include "EventHandler.h"

ConsoleScrollBar::ConsoleScrollBar(){
    
}

//------------------------------------------------------------------
ConsoleScrollBar::ConsoleScrollBar(ofxMultiTouchPad* _pad, int eventPriority){
    this->pad = _pad;
    
    //  Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &ConsoleScrollBar::mouseMoved, eventPriority);
    ofAddListener(ofEvents().mousePressed, this, &ConsoleScrollBar::mousePressed, eventPriority);
    ofAddListener(ofEvents().windowResized, this, &ConsoleScrollBar::windowResized, eventPriority);
    ofAddListener(ofEvents().mouseDragged, this, &ConsoleScrollBar::mouseDragged, eventPriority);
}

ConsoleScrollBar::~ConsoleScrollBar(){
    ofRemoveListener(ofEvents().mouseMoved, this, &ConsoleScrollBar::mouseMoved);
    ofRemoveListener(ofEvents().mousePressed, this, &ConsoleScrollBar::mousePressed);
    ofRemoveListener(ofEvents().windowResized, this, &ConsoleScrollBar::windowResized);
    ofRemoveListener(ofEvents().mouseDragged, this, &ConsoleScrollBar::mouseDragged);
}

//------------------------------------------------------------------
void ConsoleScrollBar::setup(float screenRatio_){
    
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */

    margin          = SCROLL_BAR_WIDTH; // Distance between the edge of the screen and the panel frame
    scrollBarWidth  = SCROLL_BAR_WIDTH;
    windowRatio     = screenRatio_;     // position of the console in the screen
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle  = ofRectangle(ofGetWidth() - scrollBarWidth, ofGetHeight()*windowRatio, scrollBarWidth, ofGetHeight()-ofGetHeight()*windowRatio);
    gripRectangle       = ofRectangle(ofGetWidth() - scrollBarWidth, ofGetHeight()*windowRatio, scrollBarWidth, ofGetHeight()-ofGetHeight()*windowRatio);
    
    mouseOverGrip = false; // true when the mouse is over the grip
    
    // The size of the panel. All the console screen except margins
    panelHeight = ofGetHeight() - ofGetHeight()*windowRatio - scrollBarWidth;
    
    applyInertia = false;
    updating = false;
    drag = 0.9f;
    minScrollDifference = 0.1e-5f;
    
    updateScrollBar(ofVec3f(0,0,0));
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void ConsoleScrollBar::update(){
    
    ofVec3f diffVec = ofVec3f(0,0,0);
    if(ofGetMouseY() > scrollBarRectangle.y) {
        //** touchpad scroll **//
        std::vector<MTouch> mTouches = pad->getTouches();
        if(mTouches.size() == 2) {
            if (!touchpad_scroll) {
                touchpad_scroll = true;
                touchpad_scroll_x = ((mTouches[0].x + mTouches[1].x))*100 / 2;
                touchpad_scroll_y = ((mTouches[0].y + mTouches[1].y))*100 / 2;
                prev0 = ofPoint(mTouches[0].x, mTouches[0].y);
                prev1 = ofPoint(mTouches[1].x, mTouches[1].y);
            }
            else {
                if(!updating){
                    post0 = ofPoint(mTouches[0].x, mTouches[0].y);
                    post1 = ofPoint(mTouches[1].x, mTouches[1].y);
                }
                
                updating = true;
                if (isScrollBarVisible) {
                    float new_y = ((mTouches[0].y + mTouches[1].y)*100) / 2;
                    float diff_y = (touchpad_scroll_y - new_y)*1.1;
                    
                    if (-3 < diff_y && diff_y < 3) diff_y = 0;
                    
                    diffVec.y = diff_y;
                    
                    touchpad_scroll_y = new_y;
                    float dy = new_y - touchpad_scroll_y;
                    gripRectangle.y -= diffVec.y;
                    prevDiff.y = diffVec.y;
                }
            }
        }
        else {
            touchpad_scroll = false;
            updating = false;
            if(mTouches.size() == 0) {
                applyInertia = true;
            }else{
                applyInertia = false;
                prevDiff.x = 0;
                prevDiff.y = 0;
            }
        }
        //** **//
    }

    
    if(applyInertia){
        prevDiff.y *= drag;
        diffVec.y = prevDiff.y;
        gripRectangle.y -= diffVec.y;
        
        if(ABS(prevDiff.y) <= minScrollDifference){
            applyInertia = false;
        }
    }
    updateScrollBar(diffVec);
}

//------------------------------------------------------------------
void ConsoleScrollBar::draw(){
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    ofPushStyle();
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(40);
        ofRect(scrollBarRectangle);
        if (mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(gripRectangle);
    }
    ofPopStyle();
    ofPopMatrix();
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                     EVENTS                       */
/* ================================================ */

bool ConsoleScrollBar::mouseDragged(ofMouseEventArgs &e){

    ofVec3f mouse = ofVec3f(e.x, e.y,0);
    ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
    
    ofVec3f diffVec = ofVec3f(0,0,0);
    
    if (isScrollBarVisible && gripRectangle.inside(e.x, e.y)) {
        diffVec.y = mouseLast.y - mouse.y;

        // Move the grip according to the mouse displacement
        int dy = e.y - mousePreviousY;
        mousePreviousY = e.y;
        gripRectangle.y += dy;
        
    }
    updateScrollBar(diffVec);
    return false;
}

//------------------------------------------------------------------
bool ConsoleScrollBar::mouseReleased(ofMouseEventArgs &e){
    
}

//------------------------------------------------------------------
bool ConsoleScrollBar::mousePressed(ofMouseEventArgs &e){
    // Check if the click occur on the grip
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        if (r.inside(e.x, e.y)) {
            mousePreviousY = e.y;
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------
bool ConsoleScrollBar::mouseMoved(ofMouseEventArgs &e){
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        mouseOverGrip = r.inside(e.x, e.y);
    } else {
        mouseOverGrip = false;
    }
    return false;
}

//------------------------------------------------------------------
void ConsoleScrollBar::windowResized(ofResizeEventArgs &e){
    ConsoleLog::getInstance()->restartStartY(windowRatio);
    this->setup(windowRatio);
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                  OTHER FUNCTIONS                 */
/* ================================================ */

void ConsoleScrollBar::updateScrollBar(ofVec3f diffVec){
    if(diffVec.y != 0){
        if(!(gripRectangle.y < scrollBarRectangle.y) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            ConsoleLog::getInstance()->setDiffStartY(diffVec.y);
        }

        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < scrollBarRectangle.y) {
            gripRectangle.y = scrollBarRectangle.y;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.width;
        }
    }

    // ScrollBar height = panelHeight
    scrollBarRectangle.height = panelHeight;

    // Also adjust the grip x coordinate
    gripRectangle.x = scrollBarRectangle.x;
    int highestCoordMessage = ConsoleLog::getInstance()->getLowestCoord();
    int lowestCoordMessage = ConsoleLog::getInstance()->getHighestCoord();

    // show scrollBar
    isScrollBarVisible = true;

    // these ratios are the proportion of what's above and below of what's displaying
    // they go from 0 to 1
    float gripSizeRatioLeft = 1.f;
    float gripSizeRatioRight = 1.f;
    if ( (lowestCoordMessage + SCROLL_TOLERANCE < scrollBarRectangle.y)  && (highestCoordMessage - SCROLL_TOLERANCE > panelHeight) ) {
        gripSizeRatioRight = (float)panelHeight / (panelHeight - (float)lowestCoordMessage);
        gripSizeRatioLeft = (float)panelHeight / ( (float)highestCoordMessage );
    } else if ( lowestCoordMessage + SCROLL_TOLERANCE < scrollBarRectangle.y ){
        gripSizeRatioRight = (float)panelHeight / (panelHeight - (float)lowestCoordMessage);
    } else if ( highestCoordMessage - SCROLL_TOLERANCE > panelHeight ) {
        gripSizeRatioLeft = (float)panelHeight / ( (float)highestCoordMessage );
    }

    // Grip height = panelHeight * ratios
    gripRectangle.height = panelHeight * gripSizeRatioLeft * gripSizeRatioRight;

    // The 'y' position of the grip is calculated through the ratio and the height of the scrollBar
    gripRectangle.y = ofGetHeight()*windowRatio + (1-gripSizeRatioRight)*scrollBarRectangle.height;

    if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
        isScrollBarVisible = false;
    }

}