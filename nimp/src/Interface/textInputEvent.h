//
//  textInputEvent.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/10/15.
//
//

#ifndef NIMP_Prototipe_textInputEvent_h
#define NIMP_Prototipe_textInputEvent_h

#include "ofxUIWidget.h"

class textInputEvent : ofEventArgs {
    
public:
    string type;
    ofPoint point;
    string path;
    string name;
    ofxUIWidget* widget;
};

#endif
