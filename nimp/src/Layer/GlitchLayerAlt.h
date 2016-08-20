//
//  GlitchLayerAlt.h
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#ifndef _GlitchLayerAlt
#define _GlitchLayerAlt

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxJpegGlitch.h"

// DEPRECATED - Processing is too slow.

class GlitchLayerAlt : public VisualLayer{
    
public:
    
    GlitchLayerAlt(string name_ = "New Glitch Layer Alt", int id_ = -1);
    ~GlitchLayerAlt();
    
    void setup();
    void updateParameter(Param* inputParam);
    
    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
    //glitch parameters
    ofxIntSlider dq;
    ofxIntSlider qn;
    ofxIntSlider dht;
    
    ofxJpegGlitch glitcher;
    
private:
    
    void update();
    
    void _showHelp();
    
};


#endif /* defined(_GlitchLayerAlt) */
