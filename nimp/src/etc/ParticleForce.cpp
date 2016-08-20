/*
 *  ParticleForce.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleForce.h"

ParticleForce::ParticleForce(string name, int width, int height){
    parameters.setName(name);
    parameters.add(isAttracting.set("attracts", false));
    parameters.add(pos.set("position",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    parameters.add(radius.set("radius",10,0,100));
    parameters.add(scale.set("scale",2,1,20));
    
    
}
//------------------------------------------------------------------