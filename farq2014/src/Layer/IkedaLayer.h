/*
 *  IkedaLayer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _IkedaLayer
#define _IkedaLayer

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxCv.h"

//sample layer
class IkedaLayer : public VisualLayer{
	
  public:
	
    IkedaLayer(string name_ = "visualLayer"):VisualLayer(name_){};
	
	void setup(bool isCanny_ = true ,bool isThreshold_ = true, bool isColumns_ = true, bool isInvert_ = true);
	void update();
	void draw();
    
    void inputUpdated(ofImage & img_);
    
    ofxToggle isCanny;
    ofxToggle isThreshold;
    ofxToggle isColumns;
    ofxToggle isInvert;
    
    ofxIntSlider                  pNColumns;
    ofxFloatSlider                pCannyX;
    ofxFloatSlider                pCannyY;
    ofxIntSlider                  pThreshold;
	
};

#endif