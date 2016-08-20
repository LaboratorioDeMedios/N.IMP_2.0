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
	
    IkedaLayer(string name_ = "New Ikeda Layer", int id_ = -1, bool isCanny_ = true, bool isThreshold_ = true, bool isColumns_ = true, bool isInvert_ = true);
    ~IkedaLayer();
	
    void setup();
    void updateParameter(Param* inputParam);
    
    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
    ofxToggle isCanny;
    ofxToggle isThreshold;
    ofxToggle isColumns;
    ofxToggle isInvert;
    
    ofxIntSlider    pNColumns;
    ofxFloatSlider  pCannyX;
    ofxFloatSlider  pCannyY;
    ofxIntSlider    pThreshold;
    
private:
    
    void update();
    
    void _showHelp();
    
    ofImage imgAux;
	
};

#endif
