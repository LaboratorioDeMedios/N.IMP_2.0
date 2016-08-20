//
//  menuItem.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 11/7/15.
//
//

#ifndef __NIMP_Prototipe__menuItem__
#define __NIMP_Prototipe__menuItem__

#include <stdio.h>
#include "ofxMSAInteractiveObject.h"
#include "ofxUISuperCanvas.h"
#include "enumerations.h"

class menuItem: public ofxMSAInteractiveObject {
    
public:
    
    menuItem(ofxUISuperCanvas* menu, string type, string name, string img, bool active, float x = 0, float y = 0);
    ~menuItem();
    
    void draw();
    
    void onMouseMove(ofMouseEventArgs &e);
    void onRollOut(ofMouseEventArgs &e);
    void onPress(ofMouseEventArgs &e);
    void onRelease(ofMouseEventArgs &e);
    void onReleaseOutside(ofMouseEventArgs &e);
    
private:
    string label;
    int counter;
    ofxUIMultiImageButton* button;
    ofxUIMultiImageToggle* toggle;
    ofxUISuperCanvas* menu;
};

#endif /* defined(__NIMP_Prototipe__menuItem__) */
