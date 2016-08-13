/*
 *  MixSimpleBlend.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MixSimpleBlend
#define _MixSimpleBlend

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"
#include "ofxPSBlend.h"

struct Entry{
    
    ofParameter<bool>   selected;
    int                 nodeId;
    string              label;
    ofxBaseGui*          guiComponent;
};

//simple sample mixtable
class MixSimpleBlend : public MixTable{
	
public:
	
    MixSimpleBlend(string name_ = "New Mix Simple Blend", int id_ = -1);
	
	void setup();
    void updateParameter(Param* inputParam);

    ofTexture*  getTexture();
    
    void setEnable(bool isEnabled_);
	
    ofParameter<int> blendMode;
    ofParameter<float> opacity;
    
    void resetSizeBasedOnInput(ofxPatch* input_);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
private:
    
    void update();
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    
    void _showHelp();
    
    ofxPSBlend psBlend;
    void blendModeChanged(int& i);
    void entryChanged(bool& b);
    
    vector<Entry*> entries;
    
    int selector1, selector2;
};

#endif
