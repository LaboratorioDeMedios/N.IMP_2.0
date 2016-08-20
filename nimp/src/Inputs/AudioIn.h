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
#include "ofxUIMovingGraph.h"
#include "ofxUISuperCanvas.h"
#include "enumerations.h"
#include "AudioInEvent.h"

class AudioIn : public InputSource{
    
public:
    
    AudioIn(ofxUISuperCanvas* &gui_, string type_, string name_= "New Audio In", int id_ = -1);
    ~AudioIn();
    
    void setup();
    void customDraw();
    
    void _mouseDragged(ofMouseEventArgs &e);
    
    ofTexture*          getTexture(){};
    ofxUIMovingGraph*   getWaveForm(){ return waveform; };
    float               getMidiMin(string param_){};
    float               getMidiMax(string param_){};
    AudioInType         getAudioInType(){ return audioInType; };
    int                 getChannel(){ return selectChannel; };
    int                 getBand(){ return selectBand; };
    bool                getEditInputsActive() { return editFFTInputs; };
    
    void                setWaveFormPosition();
    void                setChannel(int c);
    void                setBand(int band_);
    void                resetSize(int _width, int _height);
    void                disableEdit(bool disabled_) { disabledEdit = disabled_; };
    
    void                updateParameter(Param* inputParam);
    
    
    ofParameter<bool>     editFFTInputs;
    ofParameter<int>      selectChannel;
    ofParameter<int>      selectBand;
    ofParameter<float>    saturation;
    ofEvent<AudioInEvent> editAudioIn;
    ofEvent<AudioInEvent> editAudioInSaturation;
    ofEvent<AudioInEvent> editAudioInBand;
    ofEvent<AudioInEvent> editAudioInEnabled;
    
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
private:
    
    void update();
    void editInputs(bool& g);
    void editBand(int& band_);
    void editSaturation(float& s);
    void editEnabled(bool& e);
    void _showHelp();
    
    ofxUIMovingGraph* waveform;
    vector<float> inputBuffer;
    AudioInType audioInType;
    bool disabledEdit;
    
};

#endif /* AudioIn_h */
