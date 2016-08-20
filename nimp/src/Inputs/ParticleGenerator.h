/*
 *  ParticleGenerator.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ParticleGenerator
#define _ParticleGenerator

#include "ofMain.h"
#include "InputSource.h"

#include "ofxParticleSystem.h"
#include "ParticleForce.h"
#include "ofxGui.h"


class ParticleGenerator : public InputSource{
	
  public:
	
	ParticleGenerator(string name = "New Particle Generator", int id_ = -1);
    ~ParticleGenerator();
	
	void setup();
    void updateParameter(Param* inputParam);
    
    ofTexture*  getTexture();
    
    void addForce(ParticleForce f);
    
    ofParameter<bool>   isClearBg;
    ofParameter<float>  alphaParticles;
    
    ofParameter<bool> autoGenParticle;
    ofParameter<float> autoGenAmount;
    
    ofParameterGroup pScale;
    ofParameter<bool> unityScale;
    ofParameter<float> minRadius, maxRadius;
    
    ofParameterGroup pLifetime;
    ofParameter<float> minLifetime,maxLifetime;
    
    ofParameter<float> fadeOut;
    
    ofParameterGroup pForce;
    ofParameter<float> force_x, force_y, force_scale;
    ofParameter<int> force_radius;
    ofParameter<bool> force_attracting;
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
private:
    
    void update();
    void _showHelp();
    void forceXChanged(float &x);
    void forceYChanged(float &y);
    
    ofxParticleSystem* particle;
    int nForces = 3;
    vector<ParticleForce> force;
    
    ofxButton bAddParticles,bRemoveParticles;
    
    long int lastGen = 0;
    
    bool forceAttracting;
    int forceRadius;
    float forceScale;
};



#endif
