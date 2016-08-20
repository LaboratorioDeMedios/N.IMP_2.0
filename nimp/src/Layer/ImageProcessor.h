/*
 *  ImageProcessor.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageProcessor
#define _ImageProcessor

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxPostProcessing.h"

class ImageProcessor : public VisualLayer{
	
  public:
	
	ImageProcessor(string name_ = "New Image Processor", int id_ = -1);
    ~ImageProcessor();
	
    void setup();
    void updateParameter(Param* inputParam);

    ofTexture*  getTexture();
    
    ofParameter<bool> isBloom;
    ofParameter<bool> isContrast;
    ofParameter<float> coContrast;
    ofParameter<float> coBrightness;
    ofParameter<float> coMultiple;
    
    ofParameter<bool> isKaleidoscope;
    ofParameter<float> kaSegments;
    
    ofParameter<bool> isNoise;
    ofParameter<float> noFreq;
    ofParameter<float> noAmp;
    ofParameter<float> noSpeed;
    
    ofParameter<bool> isPixelate;
    ofParameter<ofVec2f> piRes;
    ofParameter<bool> piIsUnityScale;
    
    ofParameter<bool> isEdge;
    ofParameter<float> edHue;
    ofParameter<float> edSat;
    
    ofParameter<bool> isRGBShift;
    ofParameter<float> rgAngle;
    ofParameter<float> rgAmount;
    
    ofParameter<bool> isGod;
    
    ofParameter<bool> isZoomBlur;
    ofParameter<float> zbCenterX;
    ofParameter<float> zbCenterY;
    ofParameter<float> zbExposure;
    ofParameter<float> zbDecay;
    ofParameter<float> zbDensity;
    ofParameter<float> zbWeight;
    ofParameter<float> zbClamp;
    
    ofParameter<bool> isBleach;
    ofParameter<float> blOpacity;
    
    ofParameterGroup bloom,contrast,kaleidoscope,noise,pixelate,edge,
                     god,bleach,rgbshift,zoomBlur;
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
private:
    
    void update();
    ofxPostProcessing post;
    
    void _showHelp();
    
    void cIsBloom(bool &b);
    void cIsContrast(bool &b);
    void ccoContrast(float &f);
    void ccoBrightness(float &f);
    void ccoMultiple(float &f);
    
    void cisKaleidoscope(bool &b);
    void ckaSegments(float &f);
    
    void cisNoise(bool &b);
    void cnoFreq(float &f);
    void cnoAmp(float &f);
    void cnoSpeed(float &f);
    
    void cisPixelate(bool &b);
    void cpiIsUnityScale(bool &b);
    void cpiRes(ofVec2f &f);
    
    void cisEdge(bool &b);
    void cedHue(float &f);
    void cedSat(float &f);
    
    void cisBleach(bool &b);
    void cblOpacity(float &b);
    
    void cisGod(bool &b);
    
    void cisRGBShift(bool &b);
    void crgAngle(float &b);
    void crgAmount(float &b);
    
    void cisZoomBlur(bool &b);
    void czbCenterX(float &b);
    void czbCenterY(float &b);
    void czbExposure(float &b);
    void czbDecay(float &b);
    void czbDensity(float &b);
    void czbWeight(float &b);
    void czbClamp(float &b);
};

#endif
