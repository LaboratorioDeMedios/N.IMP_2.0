/*
 *  ImageOutput.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageOutput
#define _ImageOutput

#include "ofMain.h"
#include "ofxGui.h"
#include "Param.h"

// abstract class that covers all image output classes
// includes an image and some events
/*            
                imageOutput
                    |
&        -----------&--------------
        |           |           |
   InputSource  VisualLayer MixTable
 
*/
class ImageOutput {
	
  public:
	
    ImageOutput(string name_,int maxInputs_ = 1, int width_ = 1280, int height_ = 720);
    
    string getName();
    ofImage* getImage();
    ofTexture* getTexture();
    
    void addInput(ImageOutput* layer_);
    void addInputIdentifier(string inputId_);
    bool findAndAssignInputs(map<string,ImageOutput*> &nodeCollection);
    vector<ImageOutput*> getInputs();
    vector<string> getInputNames();
    
    virtual void draw(int x,int y, float scale = 1.0) = 0;
    void drawGui();
    void drawGui(int x, int y);
    void setGui(int x,int y, int width = 240);
    
    void resetProcessedFlag();
    
    virtual void updateParameter(Param* inputParam)=0;
    
protected:
    
    virtual void update() = 0;
    virtual void inputAdded(ImageOutput* in_){}; //called when input is added
    
    ofxPanel panel;
    ofxGuiGroup gui;
    string name;
    ofImage img;
    ofTexture tex;
    
    vector<ImageOutput*> input;
    vector<string> inputIdentifiers;
    int maxInputs;
    
    //resolution
    int width, heigth;
    bool isProcessed;
	
};

#endif
