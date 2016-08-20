/*
 *  ChannelMixer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 10/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ChannelSwitch
#define _ChannelSwitch

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"

/*  Simple Channel Switcher. up ro 16 channels
 */

class MultiChannelSwitch : public MixTable{
	
struct Channel{
    
    ofParameter<bool>   selected;
    int                 nodeId;
    string              label;
    ofxBaseGui*         guiComponent;
};

public:
	
	MultiChannelSwitch(string name_ = "New MultiChannel Switch", int id_ = -1);
	
	void setup();
    void updateParameter(Param* inputParam);

    ofTexture*  getTexture();
    
    void setEnable(bool isEnabled_);
    
    ofParameter<int>  selChannel;
    ofParameter<bool> drawInputGui;
    ofParameter<bool> disableOtherChannels;
    
    void resetSizeBasedOnInput(ofxPatch* input_);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
private:
    
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    void update();
    
    void _showHelp();
    
    void cselChannel(int& s);
    void cGui(bool& g);
    void cDisableChannels(bool& c);
    void cLabel(bool& b);
    
    vector<Channel*> channels;
    
    //hack to kill double callback
    long lastClicked;
};

#endif
