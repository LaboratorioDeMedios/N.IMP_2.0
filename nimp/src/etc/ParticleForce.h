/*
 *  ParticleForce.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ParticleForce
#define _ParticleForce

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofMain.h"

class ParticleForce {
	
  public:
	
	ParticleForce(string name = "force", int width = 640, int height = 480);
	
	
    ofParameterGroup parameters;
    ofParameter<int> radius;
    ofParameter<ofVec2f> pos;
    int _id, timeStamp;
    ofParameter<float> scale;
    ofParameter<bool> isAttracting;
};

#endif
