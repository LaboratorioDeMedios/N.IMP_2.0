/*
 *  VisualLayer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VisualLayer
#define _VisualLayer

#include "ofMain.h"

#include "ImageOutput.h"
#include "InputSource.h"
#include "Param.h"
#include <map>

#include "ofxGui.h"

//basic class for layers
//gui does not yet work
class VisualLayer : public ImageOutput{
	
  public:
	
    VisualLayer(string name_);
	
	void setup();
	virtual void update() = 0;
	virtual void draw(int x,int y, float scale = 1.0) = 0;
    
    void setInputSource(ImageOutput* input_);
    void setEnable(bool isEnabled_);
    void inputEvent(map<string, Param*>* params);
    
    virtual void inputUpdated(ofImage & img_) = 0;
    
    
    
protected:
    bool isEnabled;
    bool firstInit;
    ImageOutput* input;
    
    
    
    //route mouse events
};

#endif
