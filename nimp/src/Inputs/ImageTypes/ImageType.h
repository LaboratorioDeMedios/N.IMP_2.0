/*
 *  ImageType.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageType
#define _ImageType

#include "ofMain.h"
#include "enumerations.h"

class ImageType {
	
  public:
	
    ImageType(string name_ , string path_){
        name = name_;
        path = path_;
    };
    virtual ~ImageType() {};
	
    imgType getType(){return mediaType;};
    virtual void update(ofImage& _img, ofFbo& _fbo) = 0;
    
    virtual int getFrameRate() = 0;
    virtual float getPosition() = 0;
    virtual void calculateFPS() = 0;
    virtual void setPosition(float p, ofImage& _img) = 0;
    virtual bool isVideoDone() = 0;
    
    ofParameter<bool> isPlayingBackwards;
    ofParameter<bool> isMatchBpmToSequenceLength;
    ofParameter<float> bpm;
    ofParameter<int> bpmMultiplier;
    
    virtual void activate(ofImage& _img, ofFbo& _fbo) = 0;
    virtual void setLoopState(ofLoopType l) = 0;
    ofParameter<bool> isPlaying;
    
    string getName(){return name;};
    string getPath(){return path;};
    imgExt getFileExtension(){return ext;};
    
    virtual float getHeight() = 0;
    virtual float getWidth() = 0;
    
    virtual float getCurrentSecond() = 0;
    virtual bool isHap() = 0;
    
    protected:
    imgType mediaType;
    string name;
    string path;
    imgExt ext;
    
};

#endif
