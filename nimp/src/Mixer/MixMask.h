/*
 *  MixMask.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MixMask
#define _MixMask

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"

class MixMask : public MixTable{
	
public:
	
	MixMask(string name_ = "New Mix Mask", int id_ = -1);
	
	void setup();
    void updateParameter(Param* inputParam);

    ofTexture*  getTexture();
    
    void setEnable(bool isEnabled_);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag, int nodesCount = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
private:
    
    void update();
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    
    void _showHelp();
    
    ofShader maskShader;
    void drawShader();
    
    ofxSlider<float> spin;
	
};

#endif
