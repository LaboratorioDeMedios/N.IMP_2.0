/*
 *  NodeElement.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NodeElement
#define _NodeElement

#include "ofMain.h"
#include "ImageOutput.h"


class NodeElement {
	
  public:
	
	NodeElement(ImageOutput* imo_, int x_=50, int y_=50, int guiX_=50, int guiY_=50, int guiWidth_=200, float scale_=0.4);
    ~NodeElement();
    
	void draw();
    void drawGui();
    void setupGuiPositions();
    ImageOutput* getImageOutput();
    
    //return connection points
    ofVec2f getPosIn();
    ofVec2f getPosOut();
    
    
private:
    ImageOutput* imageOutput;
    int x,y,guiX,guiY;
    int guiWidth;
    float scale;
	
};

#endif
