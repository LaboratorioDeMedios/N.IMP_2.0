/*
 *  OscInputGenerator.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 15/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OscInputGenerator
#define _OscInputGenerator

#include "ofMain.h"
#include "ParamInputGenerator.h"
#include "ofxOsc.h"
#include "DTOscMap.h"
#include "ImageOutput.h"
#include <stdlib.h>

class DTOscPortMap {
    
public:
    int port;
    ofxOscMessage msg;
};

class OscInputGenerator : public ParamInputGenerator{
	
  public:
    OscInputGenerator(string name_);
    ~OscInputGenerator();
	
    void processInput();
    void setPort(int port_);
    int  getPort() { return port; };
    void setAddress(string oldAddress_, string address_);
    void setMin(string address_, int min);
    void setMax(string address_, int max);
    
    std::map<string,DTOscMap* >* oscMap;
    void removeNodeFromParams(int nodeID_);
    bool addNewOSCMap(int min_, int max_, string address_, ImageOutput* node_, vector<string> params_);
    bool addNewOSCMap(int min_, int max_, string address_, ImageOutput* node_, string param_);
    void removeOSCMap(string address_, ImageOutput* node_, string param_);
    void clearOSCMap();
    
    void oscReceiverIn() { numberOSCReceiver++; };
    void oscReceiverLeft() { numberOSCReceiver--; };
    int  getNumberOSCReceiver() { return numberOSCReceiver; };
    
    bool loadSettings(ofxXmlSettings &XML, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    DTOscMap* getOSCMapForAddress(string address_);
    void addOSCMapForAddress(string address_, DTOscMap* oscmap);
    vector<string> getAllAddress();
    
private:
    int port, numberOSCReceiver;
    ofxOscReceiver receiver;
	
};

#endif
