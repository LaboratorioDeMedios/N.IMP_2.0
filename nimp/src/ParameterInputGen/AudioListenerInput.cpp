//
//  AudioListener.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "AudioListenerInput.h"

AudioListenerInput::AudioListenerInput(string name_, int nodeID_):ParamInputGenerator(name_,true){
    left = NULL;
    right = NULL;
    hasNewData = false;
    type = FFT;
    nodeID = nodeID_;
}

AudioListenerInput::~AudioListenerInput() {
    
}

void AudioListenerInput::processInput(){
    
}

bool AudioListenerInput::fillNewData(float* left_, float* right_, int bufferLen_){
    
    // if we dont have unprocessed data in the buffer, then we store it and process it
    // if we store it, the secondary thread has to deal with the memory
    // successfull storage is reported on variable: stored
    
    bool stored = false;
    
    if(lock()){
        
        if(!hasNewData){
            
            bufferLen =bufferLen_;
            left = left_;
            right = right_;
            hasNewData = true;
            stored = true;
            isDataProcessed = false;

        }
        
        unlock();
    }
    
    return stored;
}



void AudioListenerInput::disposeData(){
    
    // used to dispose of memory of stored data.
    
    delete[] left;
    delete[] right;
    left = NULL;
    right = NULL;
    hasNewData = false;
    isDataProcessed = false;
    
}

void AudioListenerInput::setDataProcessed(){
    
    isDataProcessed = true;
    
}

