//
//  AudioAnalizer.h
//  nimp
//
//  Created by Mili Garicoits on 2/25/16.
//
//

#ifndef AudioAnalizer_h
#define AudioAnalizer_h

#include <stdio.h>
#include "InputSource.h"
#include "ofxFilterbank.h"
#include "ofxUISuperCanvas.h"
#include "enumerations.h"

class AudioAnalizer : public InputSource{
    
public:
    
    AudioAnalizer(string name_= "Audio Analizer", int id_ = -1);
    ~AudioAnalizer();
    
    void setup();
    void customDraw();
    
    void _mouseDragged(ofMouseEventArgs &e);
    
    ofTexture*  getTexture(){};
    float       getMidiMin(string param_){};
    float       getMidiMax(string param_){};
    void        updateParameter(Param* inputParam){};
    
    void analyze(float * iBuffer);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap){};
    
private:
    
    void update();
    void editTreshold(float& t);
    void editSaturation(int& s);
    void _showHelp();

    ofxFilterbank filterBank;
    ofParameter<float> threshold;
    ofParameter<int> saturation;
    
};
#endif /* AudioAnalizer_h */
