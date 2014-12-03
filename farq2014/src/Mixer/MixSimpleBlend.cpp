/*
 *  MixSimpleBlend.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixSimpleBlend.h"


//------------------------------------------------------------------
void MixSimpleBlend::setup() {
    
}


//------------------------------------------------------------------
void MixSimpleBlend::update() {
	
	
}


//------------------------------------------------------------------
void MixSimpleBlend::draw() {
    ofSetColor(255, 255, 255);
    fbo.draw(0, 0);
	ofDrawBitmapString(name, 10, 30);
}

void MixSimpleBlend::inputUpdated(ofImage & img){
    fbo.begin();
    ofClear(255,255,255, 0);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    for (int i=0; i<input.size(); ++i) {
        input[i]->getImage()->draw(0, 0);
    }
    fbo.end();
    ofNotifyEvent(textureEvent, fbo.getTextureReference(), this);
}