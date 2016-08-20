//
//  DTMidiMap.h
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#ifndef __ofApp__DTMidiMap__
#define __ofApp__DTMidiMap__

#include "ofMain.h"

class DTMidiMap {
    
public:
    DTMidiMap();
    
    int     control;
    int     nodeId;
    string  paramId;
    int     inputMinValue;
    int     inputMaxValue;
    float     paramMinValue;
    float     paramMaxValue;
};



#endif /* defined(__ofApp__DTMidiMap__) */
