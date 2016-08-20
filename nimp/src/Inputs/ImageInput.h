//
//  ImageInput.h
//  ofApp
//
//  Created by Christian Clark on 12/7/14.
//
//

//not longer used

#ifndef _ImageInput
#define _ImageInput

#include "ofMain.h"
#include "InputSource.h"
#include "ofxImageSequencePlayer.h"
#include "ofEvents.h"

class ImageInput : public InputSource{
    
public:
    
    ImageInput(string name = "image input");
    
    void setup();
    void draw(int x,int y, float scale);
    void loadImage(string path_);
    void updateParameter(Param* inputParam);
    
    
    ofParameter<float> bpm;
    ofParameter<int> bpmMultiplier;
    ofParameter<bool> isPlaying;
    ofParameter<bool> isPalindromLoop;
    ofParameter<bool> isPlayingBackwards;
    ofParameter<bool> isMatchBpmToSequenceLength;
    ofParameter<float> playPosition;
    ofxButton nextFrame;
    ofxButton previousFrame;
    ofxButton setOriginalPlaySpeed;
    
    
private:
    
    void update();
    

    unsigned long long lastFrameSent;
    
    ofxImageSequencePlayer player;
    ofQTKitPlayer videoPlayer;
    bool isImageSequence;
    bool isVideo;
    
    void setOriginalPlaySpeedChanged();
    
    void loopTypeChanged(bool &b);
    void isPlayingBackwardsChanged(bool &b);
    void bpmChanged(float &b);
    void bpmMultiplierChanged(int &b);
    void isMatchBpmToSequenceLengthChanged(bool &b);
    void nextFrameChanged();
    void previousFrameChanged();
    void playPositionChanged(float &pos);
    void calculateFPS();
    
    
    void videoLoopTypeChanged(bool &b);
    void videoBpmChanged(float &b);
    void videoBpmMultiplierChanged(int &b);
    void videoIsMatchBpmToSequenceLengthChanged(bool &b);
    void videoNextFrameChanged();
    void videoPreviousFrameChanged();
    void videoPlayPositionChanged(float &pos);
    void videoCalculateFPS();
    
};


#endif /* defined(_ImageInput) */
