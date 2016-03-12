//
//  AudioIn.h
//  nimp
//
//  Created by Mili Garicoits on 2/13/16.
//
//

#ifndef AudioIn_h
#define AudioIn_h

#include <stdio.h>
#include "InputSource.h"
#include "ofxUIWaveform.h"
#include "ofxUISuperCanvas.h"
#include "enumerations.h"
#include "AudioInEvent.h"

class AudioIn : public InputSource{
    
public:
    
    AudioIn(ofxUISuperCanvas* &gui_, float* &inputBuffer_, string type_, string name_= "New Audio In", int id_ = -1);
    
    void setup();
    void customDraw();
    
    void _mouseDragged(ofMouseEventArgs &e);
    
    ofImage*        getImage(){};
    ofTexture*      getTexture(){};
    ofxUIWaveform*  getWaveForm(){ return waveform; };
    float           getMidiMin(string param_){};
    float           getMidiMax(string param_){};
    AudioInType     getAudioInType(){ return audioInType; };
    int             getChannel(){ return selectChannel; };
    int             getBand(){ return selectBand; };
    
    void setWaveFormPosition();
    void setChannel(int c);
    void setBand(int band_);
    void resetSize(int _width, int _height);
    
    void updateParameter(Param* inputParam){};
    
    ofParameter<bool>     editFFTInputs;
    ofParameter<int>      selectChannel;
    ofParameter<int>      selectBand;
    ofEvent<AudioInEvent> editAudioIn;
//    ofEvent<AudioInEvent> editAudioInChannel;
    ofEvent<AudioInEvent> editAudioInBand;
    
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap){};
    
private:
    
    void update();
    void editInputs(bool& g);
//    void editChannel(int& c);
    void editBand(int& band_);
    
    ofxUIWaveform* waveform;
    float* inputBuffer;
    AudioInType audioInType;
    
};

#endif /* AudioIn_h */
