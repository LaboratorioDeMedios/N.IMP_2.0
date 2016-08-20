//
//  AudioInputGenerator.h
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#ifndef _AudioInputGenerator
#define _AudioInputGenerator

#include "ofMain.h"
#include "AudioListenerInput.h"
#include "ParamInputGenerator.h"
#include "Param.h"
#include "fft.h"
#include "ImageOutput.h"

#define BUFFER_SIZE 256
#define NUM_WINDOWS 80

class AudioInputGenerator: public AudioListenerInput {
    
public:
    
    AudioInputGenerator(string name_, int nodeID_);
    ~AudioInputGenerator();
    void processInput();
    void setChannel(int c) { channel = c; };
    void setBand(int b) { band = b; };
    void setSaturation(float s) { saturation = s; };
    void setEnable(bool e) { enable = e; };
    
    bool addNewAudioMap(int band_, int channel_, ImageOutput* node_, vector<string> params_);
    void clearAudioMap();
    void removeNodeFromParams(int nodeID_);
    
    bool loadSettings(ofxXmlSettings &XML, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
private:

    int 	bufferCounter;
    fft		myfft;
    
    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    
    float freq[NUM_WINDOWS][BUFFER_SIZE/2];
    float freq_phase[NUM_WINDOWS][BUFFER_SIZE/2];
    
    int channel;
    int band;
    float saturation;
    bool enable;

};



#endif /* defined(_AudioInputGenerator) */
