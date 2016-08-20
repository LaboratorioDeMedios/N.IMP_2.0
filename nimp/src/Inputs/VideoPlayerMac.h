/*
 *  VideoPlayerMac.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VideoPlayerMac
#define _VideoPlayerMac

#include "ofMain.h"
#include "InputSource.h"

class VideoPlayerMac : public InputSource{
	
  public:
	
	VideoPlayerMac(string name = "New Video Player", int id_ = -1);
	
	void setup();

    void loadVideo(string path);
    void nextVideo();
    void prevVideo();
    void playVideo();
    void stopVideo();
    void setSpeed(float speed);
    float getSpeed();
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    void updateParameter(Param* inputParam);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
private:
    void update();
    
    vector<ofQTKitPlayer*> players;
    int currentPlayer;
    
    ofxButton play;
    ofxButton stop;
    ofxFloatSlider speed;
    
    string path;
    vector<string> playerPaths;
};

#endif
