//
//  GlitchLayer.h
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#ifndef _GlitchLayer
#define _GlitchLayer

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxPostGlitch.h"

class GlitchLayer : public VisualLayer{
    
public:
    
    GlitchLayer(string name_ = "glitchLayer");
    
    void setup();
    void draw(int x,int y, float scale);
    void updateParameter(Param* inputParam);
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    
    ofFbo			myFbo;
    ofTexture		texture;
    ofxPostGlitch	myGlitch;
    
    //glitch parameters
    
    ofxToggle do_CONVERGENCE;
    ofxToggle do_GLOW;
    ofxToggle do_SHAKER;
    ofxToggle do_CUTSLIDER;
    ofxToggle do_TWIST;
    ofxToggle do_OUTLINE;
    ofxToggle do_NOISE;
    ofxToggle do_SLITSCAN;
    ofxToggle do_SWELL;
    ofxToggle do_INVERT;
                                   
    ofxToggle do_CR_HIGHCONTRAST;
    ofxToggle do_CR_BLUERAISE;
    ofxToggle do_CR_REDRAISE;
    ofxToggle do_CR_GREENRAISE;
    ofxToggle do_CR_BLUEINVERT;
    ofxToggle do_CR_REDINVERT;
    ofxToggle do_CR_GREENINVERT;

private:
    
    void update();
    
    void setGlitchParameters();
    
};


#endif /* defined(_GlitchLayer) */
