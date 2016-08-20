//
//  AudioListener.h
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#ifndef __ofApp__AudioListener__
#define __ofApp__AudioListener__

#include "ofMain.h"
#include "ParamInputGenerator.h"
#include "DTAudioMap.h"

// this class implements the behavior of a audio listener.
// it runs on its own thread, and receives audio data fro the main thread.
// the main thread copies data to the thread, and lets the thread use and dispose it.

// this class is used to control the data exchange behavior between audio specific paramGenerators as FFT, etc.

class AudioListenerInput: public ParamInputGenerator {
    
public:
    
    AudioListenerInput(string name_, int nodeID_);
    ~AudioListenerInput();
    void processInput();
    bool fillNewData(float* left_, float* right_, int bufferLen_);
    void disposeData();
    
    int getNodeID(){ return nodeID; };
    vector<DTAudioMap*>* getAudioMap(){ return audioMap; };
    virtual void removeNodeFromParams(int nodeID_){};

protected:
    
    void setDataProcessed();
    bool hasNewData;
    bool isDataProcessed;
    int bufferLen;
    float* left;
    float* right;
    int nodeID;
    
    //for storing audio feature mapping
    std::vector<DTAudioMap*>* audioMap;
    
};


#endif /* defined(__ofApp__AudioListener__) */
