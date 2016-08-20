/*
 *  InputCamera.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _InputCamera
#define _InputCamera

#include "ofMain.h"
#include "InputSource.h"
#include "ofxGui.h"

// a standard camera input
class InputCamera : public InputSource{
	
  public:

    InputCamera(string name = "New Camera", int id_ = -1);
    ~InputCamera();
	
	void setup();
    
    ofTexture*  getTexture();
    
    void updateParameter(Param* inputParam){};
    
    float getMidiMin(string param_){};
    float getMidiMax(string param_){};
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    void changeToNextCamera();
    void changeToPrevCamera();
    void changeToHigherResolution();
    void changeToLowerResolution();
    void editResolution(string resolution);
    void initCamera();

private:
    
    void update();
    void _showHelp();
    
    ofParameter<string> cameraSelectedName;
    string cameraName;
	
    ofxButton nextCamera;
    ofxButton prevCamera;
    
    int cameraIndex;
    
    ofParameter<string> resolutionName;
    ofxButton higherResolution;
    ofxButton lowerResolution;
    int previous_resolution;
    vector<string> resolutionLabels;
};

#endif
