/*
 *  ImageAndVideoInputList.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageAndVideoInputList
#define _ImageAndVideoInputList

#include "ofMain.h"
#include "InputSource.h"
#include "ofEvents.h"
#include "ofxGui.h"
#include "ImageType.h"
#include "ImageTypeMovie.h"
#include "ImageTypePicture.h"
#include "ImageTypePictureSequence.h"
#include "VideoPool.h"
#include <math.h>



class ImageAndVideoInputList : public InputSource{
	
  public:
	
    ImageAndVideoInputList(string name = "New Image & Video List", int id_ = -1);
    ~ImageAndVideoInputList();
    
    void setup();
    void loadImage(string name_,string path_);
    void updateParameter(Param* inputParam);
    void setEnable(bool isEnabled_);
    
    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
    string videoTime;
    ofParameter<string> videoPlayingTime;
    
    float prevPosition;
    
    ofxButton nextSequence;
    ofxButton prevSequence;
    
    ofParameter<int> currentSequence;
    ofxButton deleteCurrentSequence;
    ofxButton addVideoOrImage;
    
    ofParameterGroup seqSettings;
    ofParameter<float> bpm;
    ofParameter<int> bpmMultiplier;
    ofParameter<bool> isPlaying;
    ofParameter<bool> repeatSequence;
    ofParameter<bool> playInLoop;
    ofParameter<bool> isPalindromLoop;
    ofParameter<bool> isPlayingBackwards;
    ofParameter<bool> isMatchBpmToSequenceLength;
    ofParameter<float> playPosition;
    ofParameter<float> playPos2;
    ofxButton nextFrame;
    ofxButton previousFrame;
    ofParameter<bool> setOriginalPlaySpeed;
    
    bool isCurrentSequenceHap();
	
private:
    vector<ImageType*> inputs;
    
    void update();
    
    void prevSequenceChanged();
    void nextSequenceChanged();
    void sequenceChanged(int &s);
    
    void _showHelp();
    
    void setOriginalPlaySpeedChanged(bool &b);
    void repeatSequenceChanged(bool &b);
    void playInLoopChanged(bool &b);
    void loopTypeChanged(bool &b);
    void isPlayingBackwardsChanged(bool &b);
    void bpmChanged(float &b);
    void bpmMultiplierChanged(int &b);
    void isMatchBpmToSequenceLengthChanged(bool &b);
    void nextFrameChanged();
    void previousFrameChanged();
    void playPositionChanged(float &pos);
    void calculateFPS();
    void isPlayingChanged(bool &b);
    void setEnableChanged(bool &b);
    void deleteSequence();
    void addNewInput();
    string getTime(float t);
    
    int lastSequence;
    bool hasMovie;
    ofBaseVideoPlayer* videoPlayer;
    ofBaseVideoPlayer* hapPlayer;
    ofBaseVideoPlayer* commonPlayer;
    
    string path;
};

#endif
