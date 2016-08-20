//
//  scrollBar.cpp
//  NIMP_Prototipe
//
//  Created by Nicolas Peri on 11/20/15.
//
//

#include "scrollBar.h"
#include "ofMain.h"
#include "EventHandler.h"
#include "ConsoleLog.h"

scrollBar::scrollBar(){
}

scrollBar::scrollBar(class ofxComposer* _composer, ofxMultiTouchPad* _pad, ofEasyCam* cam, int eventPriority){
    this->composer  = _composer;
    this->pad       = _pad;
    this->cam       = cam;
    
    //  Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &scrollBar::mouseMoved, eventPriority);
    ofAddListener(ofEvents().mousePressed, this, &scrollBar::mousePressed, eventPriority);
    ofAddListener(ofEvents().mouseReleased, this, &scrollBar::mouseReleased, eventPriority);
    ofAddListener(ofEvents().keyPressed, this, &scrollBar::keyPressed, eventPriority);
    ofAddListener(ofEvents().windowResized, this, &scrollBar::windowResized, eventPriority);
    ofAddListener(ofEvents().mouseDragged, this, &scrollBar::mouseDragged, eventPriority);
    
    //    enableScroll = true;
    scale = 1.f;
    updating = false;
    
    windowRatio = 1;
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    
    applyInertia = false;
    drag = 0.9f;
    minScrollDifference = 0.1e-5f;
    clicScale = 1.f;
    
    this->eventPriority = eventPriority;
    
    showMaxZoomReachedMessage = false;
    enableTrackpad = true;
}

scrollBar::~scrollBar(){
    ofRemoveListener(ofEvents().mouseMoved, this, &scrollBar::mouseMoved, eventPriority);
    ofRemoveListener(ofEvents().mousePressed, this, &scrollBar::mousePressed, eventPriority);
    ofRemoveListener(ofEvents().mouseReleased, this, &scrollBar::mouseReleased, eventPriority);
    ofRemoveListener(ofEvents().keyPressed, this, &scrollBar::keyPressed, eventPriority);
    ofRemoveListener(ofEvents().windowResized, this, &scrollBar::windowResized, eventPriority);
    ofRemoveListener(ofEvents().mouseDragged, this, &scrollBar::mouseDragged, eventPriority);
}


//------------------------------------------------------------------
void scrollBar::setup(){
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */
    
    margin = SCROLL_BAR_WIDTH; // Distance between the edge of the screen and the panel frame
    scrollBarWidth = SCROLL_BAR_WIDTH;
    
    BEGIN_X = RIGHT_MENU_WIDTH;
    BEGIN_Y = MENU_HEIGHT + MENU_TOP_PADDING;
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, BEGIN_Y, scrollBarWidth, 0);
    gripRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, BEGIN_Y, scrollBarWidth, 0);
    
    
    windowHeight = ofGetHeight()*windowRatio;
    hScrollBarRectangle = ofRectangle(BEGIN_X, windowHeight - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(BEGIN_X, windowHeight - scrollBarWidth, 0, scrollBarWidth);
    
    composer->setDraggingGrip(false); // true when the user is moving the grip
    mouseOverGrip = false; // true when the mouse is over the grip
    
    mouseOverHGrip = false;
    composer->setDraggingHGrip(false);
    
    updateScrollBar(ofVec3f(0,0,0));
    updateHScrollBar(ofVec3f(0,0,0));
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void scrollBar::update(){
    ofVec2f diffVec = ofVec2f(0,0);
    encapsulatedIdToDraw = EventHandler::getInstance()->getEncapsulatedIdDraw();
    
    if(enableTrackpad){
        if(ofGetMouseY() < windowHeight) {
            //** touchpad scroll **//
            std::vector<MTouch> mTouches = pad->getTouches();
            if(mTouches.size() == 2) {
                if (!touchpad_scroll) {
                    touchpad_scroll = true;
                    touchpad_scroll_x = ((mTouches[0].x + mTouches[1].x))*100 / 2;
                    touchpad_scroll_y = ((mTouches[0].y + mTouches[1].y))*100 / 2;
                    
                    prevDist = ofDist(mTouches[0].x, mTouches[0].y, mTouches[1].x, mTouches[1].y);
                    prev0 = ofPoint(mTouches[0].x, mTouches[0].y);
                    prev1 = ofPoint(mTouches[1].x, mTouches[1].y);
                }
                else {
                    
                    if(!updating){
                        post0 = ofPoint(mTouches[0].x, mTouches[0].y);
                        post1 = ofPoint(mTouches[1].x, mTouches[1].y);
                        zooming = isZooming();
                    }
                    
                    updating = true;
                    
                    if(zooming){
                        newDist = ofDist(mTouches[0].x, mTouches[0].y, mTouches[1].x, mTouches[1].y);
                        diffDist = (newDist - prevDist)*120;
                        float scaleAux = scale - diffDist*SCALE_SENSITIVITY;
                        if(scaleAux < MAX_SCALE && scaleAux > MIN_SCALE){
                            scale = cam->getScale().x;
                            clicPoint = ofVec2f(ofGetMouseX(), ofGetMouseY());
                            clicPoint /= scale;
                            clicScale = scale;
                            clicTranslation = cam->getPosition();
                            
                            scale -= diffDist*SCALE_SENSITIVITY;
                            cam->setScale(scale);
                            cam->setPosition(clicTranslation - clicPoint*(scale - clicScale));
                        } else {
                            showMaxZoomReachedMessage = true;
                        }
                        prevDist = newDist;
                    }else{
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
                        if(isHScrollBarVisible){
                            
                            float new_x = ((mTouches[0].x + mTouches[1].x)*100) / 2;
                            float diff_x = (touchpad_scroll_x - new_x)*1.2;
                            
                            if (-4 < diff_x && diff_x < 4) diff_x = 0;
                            
                            diffVec.x = diff_x;
                            
                            touchpad_scroll_x = new_x;
                            float dx = new_x - touchpad_scroll_x;
                            hGripRectangle.x -= diffVec.x;
                            prevDiff.x = diffVec.x;
                            
                        }
                    }
                    
                    prev0 = post0;
                    prev1 = post1;
                }
            }
            else {
                touchpad_scroll = false;
                updating = false;
                if(mTouches.size() == 0) {
                    applyInertia = true;
                }else{
                    applyInertia = false;
                    diffDist = 0;
                    prevDiff.x = 0;
                    prevDiff.y = 0;
                }
                if(showMaxZoomReachedMessage){
                    ConsoleLog::getInstance()->pushWarning("Max zoom in/out reached");
                    showMaxZoomReachedMessage = false;
                }
            }
        }
        
        if(applyInertia){
            if(zooming){
                diffDist *= drag;
                scale = cam->getScale().x;
                float scaleAux = scale - diffDist*SCALE_SENSITIVITY;
                if(scaleAux < MAX_SCALE && scaleAux > MIN_SCALE){
                    clicPoint = ofVec2f(ofGetMouseX(), ofGetMouseY());
                    clicPoint /= scale;
                    clicScale = scale;
                    clicTranslation = cam->getPosition();
                    
                    scale -= diffDist*SCALE_SENSITIVITY;
                    cam->setScale(scale);
                    cam->setPosition(clicTranslation - clicPoint*(scale - clicScale));
                }
                
                if(ABS(diffDist) <= minScrollDifference){
                    applyInertia = false;
                }
                
            } else{
                prevDiff.x *= drag;
                prevDiff.y *= drag;
                diffVec.x = prevDiff.x;
                diffVec.y = prevDiff.y;
                gripRectangle.y -= diffVec.y;
                hGripRectangle.x -= diffVec.x;
                
                if(ABS(prevDiff.x) <= minScrollDifference || ABS(prevDiff.y) <= minScrollDifference){
                    applyInertia = false;
                }
            }
        }
    }
    

    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);

}

//------------------------------------------------------------------
void scrollBar::draw(){
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    ofPushStyle();
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(40);
        ofRect(scrollBarRectangle);
        if (composer->isDraggingGrip() || mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(gripRectangle);
    }
    
    if (isHScrollBarVisible) {
        ofSetColor(40);
        ofRect(hScrollBarRectangle);
        if (composer->isDraggingHGrip() || mouseOverHGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(hGripRectangle);
    }
    ofPopStyle();
    ofPopMatrix();
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                     EVENTS                       */
/* ================================================ */

void scrollBar::mouseDragged(ofMouseEventArgs &e){
    mousePositionX = e.x;
    mousePositionY = e.y;
    
    ofVec3f mouse = ofVec3f(e.x, e.y,0);
    ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
    
    ofVec3f diffVec = ofVec3f(0,0,0);
    
    if (isScrollBarVisible && composer->isDraggingGrip()) {
        diffVec.y = mouseLast.y - mouse.y;
        
        // Move the grip according to the mouse displacement
        int dy = e.y - mousePreviousY;
        mousePreviousY = e.y;
        gripRectangle.y += dy;
        
    }
    if(isHScrollBarVisible && composer->isDraggingHGrip()){
        diffVec.x = mouseLast.x - mouse.x;
        
        // Move the grip according to the mouse displacement
        int dx = e.x - mousePreviousX;
        mousePreviousX = e.x;
        hGripRectangle.x += dx;
    }

    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);

}

//------------------------------------------------------------------
void scrollBar::mouseReleased(ofMouseEventArgs &e){
    mousePositionX = e.x;
    mousePositionY = e.y;
    
    composer->setDraggingGrip(false);
    composer->setDraggingHGrip(false);
    
    enableTrackpad = true;
}

//------------------------------------------------------------------
void scrollBar::mousePressed(ofMouseEventArgs &e){
    mousePositionX = e.x;
    mousePositionY = e.y;
    
    // Check if the click occur on the grip
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        if (r.inside(e.x, e.y)) {
            composer->deactivateAllPatches();
            composer->setDraggingGrip(true);
            mousePreviousY = e.y;
        }
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        if (r.inside(e.x, e.y)) {
            composer->deactivateAllPatches();
            composer->setDraggingHGrip(true);
            mousePreviousX = e.x;
        }
    }
    
    enableTrackpad = false;
}

//------------------------------------------------------------------
void scrollBar::mouseMoved(ofMouseEventArgs &e){
    mousePositionX = e.x;
    mousePositionY = e.y;
    if (isScrollBarVisible) {
        ofRectangle r = gripRectangle;
        mouseOverGrip = r.inside(e.x, e.y);
    } else {
        mouseOverGrip = false;
    }
    
    if (isHScrollBarVisible) {
        ofRectangle r = hGripRectangle;
        mouseOverHGrip = r.inside(e.x, e.y);
    } else {
        mouseOverHGrip = false;
    }
}

//------------------------------------------------------------------
void scrollBar::keyPressed(ofKeyEventArgs &e){
    ofVec3f diffVec = ofVec3f(0, 0, 0);
    if (isScrollBarVisible) {
        if (e.key == OF_KEY_UP ){
            diffVec.y = KEY_SCROLL_SENSITIVITY;
            gripRectangle.y -= KEY_SCROLL_SENSITIVITY;
        } else if (e.key == OF_KEY_DOWN){
            diffVec.y = -KEY_SCROLL_SENSITIVITY;
            gripRectangle.y += KEY_SCROLL_SENSITIVITY;
        }
    }
    if(isHScrollBarVisible){
        if (e.key == OF_KEY_LEFT ){
            diffVec.x = KEY_SCROLL_SENSITIVITY;
            hGripRectangle.x -= KEY_SCROLL_SENSITIVITY;
        } else if (e.key == OF_KEY_RIGHT ){
            diffVec.x = -KEY_SCROLL_SENSITIVITY;
            hGripRectangle.x += KEY_SCROLL_SENSITIVITY;
        }
    }
    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);
}

//------------------------------------------------------------------
void scrollBar::windowResized(ofResizeEventArgs &e){
    windowWidth = e.width;
    windowHeight = e.height*windowRatio;
    this->setup();
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                  OTHER FUNCTIONS                 */
/* ================================================ */

void scrollBar::updateScrollBar(ofVec2f diffVec){
    
    
    if(cam->getScale().x > 1){
        diffVec.y *= cam->getScale().x;
    }
    if(diffVec.y != 0){
        if(!(gripRectangle.y < BEGIN_Y) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            cam->setPosition(cam->getPosition().x, cam->getPosition().y - diffVec.y, cam->getPosition().z);
        }
        
        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < BEGIN_Y) {
            gripRectangle.y = BEGIN_Y;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
        }
    }
    
    // The size of the panel. All the screen except margins
    panelWidth = windowWidth - margin;
    panelHeight = windowHeight - margin - BEGIN_Y;
    
    // Also adjust the grip x coordinate
    gripRectangle.x = scrollBarRectangle.x;
    int unTransformedLowest = (composer->getPatchesLowestCoord(encapsulatedIdToDraw) - cam->getPosition().y)/cam->getScale().y - margin - BEGIN_Y;
    int unTransformedHighest = (composer->getPatchesHighestCoord(encapsulatedIdToDraw) - cam->getPosition().y)/cam->getScale().y - margin;
    int inspectorHighestY = composer->getPatchesHighestYInspectorCoord(encapsulatedIdToDraw);
    
    if(unTransformedHighest < inspectorHighestY && inspectorHighestY > 0){
        unTransformedHighest = inspectorHighestY;
    }
    
    
    // Show scroll bar
    isScrollBarVisible = true;
    // Set its height as tall as screen
    scrollBarRectangle.height = panelHeight;
    
    // These ratios are the proportion of what's underneath and above of what's displaying
    // The ratios values go from 0 to 1
    float gripSizeRatioLow = 1.f;
    float gripSizeRatioHigh = 1.f;
    if ( unTransformedLowest + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)unTransformedLowest);
    }
    if ( unTransformedHighest - SCROLL_TOLERANCE > panelHeight ) {
        gripSizeRatioLow = (float)panelHeight / ( (float)unTransformedHighest );
    }

    // Height of the grip bar is the panel height multiplied by both ratios
    gripRectangle.height = panelHeight * gripSizeRatioLow * gripSizeRatioHigh;
    
    // The 'y' coordinate is in the beginning + what is left above
    gripRectangle.y = BEGIN_Y + (1-gripSizeRatioHigh)*scrollBarRectangle.height;
    
    // If both heights are equal is because everything is displaying and set visibility to false
    // leave a small margin of 2 since they are not exactly equal
    if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
        isScrollBarVisible = false;
    }
    
}

//------------------------------------------------------------------
void scrollBar::updateHScrollBar(ofVec2f diffVec){
    if(cam->getScale().x > 1){
        diffVec.x *= cam->getScale().x;
    }
    if(diffVec.x != 0){
        if(!(hGripRectangle.x < BEGIN_X) && !(hGripRectangle.getRight() > hScrollBarRectangle.getRight())){
            cam->setPosition(cam->getPosition().x - diffVec.x, cam->getPosition().y, cam->getPosition().z);
        }
        
        // Check if the grip is still in the scroll bar
        if (hGripRectangle.x < BEGIN_X) {
            hGripRectangle.x = BEGIN_X;
        }
        if (hGripRectangle.getRight() > hScrollBarRectangle.getRight()) {
            hGripRectangle.x = hScrollBarRectangle.getRight() - hGripRectangle.width;
        }
    }
    
    
    // The size of the panel. All the screen except margins
    panelWidth = windowWidth - margin - BEGIN_X;
    panelHeight = windowHeight - margin;
    
    // Set its width as wide as screen
    hScrollBarRectangle.width = panelWidth;
    
    // Also adjust the grip x coordinate
    hGripRectangle.y = hScrollBarRectangle.y;
    int unTransformedLeft = (composer->getPatchesLeftMostCoord(encapsulatedIdToDraw) - cam->getPosition().x)/cam->getScale().x - margin - BEGIN_X;
    int unTransformedRight = (composer->getPatchesRightMostCoord(encapsulatedIdToDraw) - cam->getPosition().x)/cam->getScale().x - margin;
    int inspectorHighestX = composer->getPatchesHighestXInspectorCoord(encapsulatedIdToDraw);

    if(unTransformedRight < inspectorHighestX && inspectorHighestX > 0){
        unTransformedRight = inspectorHighestX;
    }
    
    // Show scroll bar
    isHScrollBarVisible = true;
    
    // These ratios are the proportion of what's left and right of what's displaying
    // The ratios values go from 0 to 1
    float gripSizeRatioLeft = 1.f;
    float gripSizeRatioRight = 1.f;
    if ( (unTransformedLeft + SCROLL_TOLERANCE < 0)  && (unTransformedRight - SCROLL_TOLERANCE > panelWidth) ) {
        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)unTransformedLeft);
        gripSizeRatioLeft = (float)panelWidth / ( (float)unTransformedRight );
    } else if ( unTransformedLeft + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)unTransformedLeft);
    } else if ( unTransformedRight - SCROLL_TOLERANCE > panelWidth ) {
        gripSizeRatioLeft = (float)panelWidth / ( (float)unTransformedRight );
    }
    
    // Width of the grip bar is the panel width multiplied by both ratios
    hGripRectangle.width = panelWidth * gripSizeRatioLeft * gripSizeRatioRight;
    
    // The 'x' coordinate is in the beginning + what is left and not shown
    hGripRectangle.x = BEGIN_X + (1-gripSizeRatioRight)*hScrollBarRectangle.width;
    
    // If both widths are equal is because everything is displaying and set visibility to false
    // leave a small margin of 2 since they are not exactly equal
    if( (hScrollBarRectangle.width - hGripRectangle.width) < 2 ){
        isHScrollBarVisible = false;
    }
}

//------------------------------------------------------------------
void scrollBar::changeWindowHeight(float windowRatio_) {
    
    windowRatio = windowRatio_;
    windowHeight = ofGetHeight()*windowRatio_;
    
    hScrollBarRectangle = ofRectangle(BEGIN_X, ofGetHeight()*windowRatio - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(BEGIN_X, ofGetHeight()*windowRatio - scrollBarWidth, 0, scrollBarWidth);
}

/* ================================================ */
/* ================================================ */

bool scrollBar::isZooming(){
    if( (prev0.y < post0.y && prev1.y > post1.y) ||
       (prev0.y > post0.y && prev1.y < post1.y) ){
        return true;
    }
    return false;
}
