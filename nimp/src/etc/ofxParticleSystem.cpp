/*
 *  ofxParticleSystem.cpp
 *  ofxParticleSystem
 *
 *  Created by nathan_laptop on 7/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxParticleSystem.h"

ofxParticleSystem::ofxParticleSystem(int initialAmount, int width_, int height_,int minSize, int maxSize, int minLifetime, int maxLifetime, float fadeOut) {
    width = width_;
    height = height_;
    bRepel		= true;
    radius		= 100;
    strength	= 0.5f;
    
    addParticles(initialAmount, minSize,maxSize,minLifetime,maxLifetime,fadeOut);
}

void ofxParticleSystem::update() {
    vector<int> toDel;
    for (int i = 0; i < particles.size(); i++){
        if (!particles[i].getAlive()) {
            toDel.push_back(i);
        }
    }

    for (int i=0; i<toDel.size(); ++i) {
        swap(particles[toDel[i]], particles.back());
        particles.pop_back();
    }
    
    for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){
		for (int j = 0; j < particles.size(); j++){
			if (i != j){
				particles[i].addForFlocking(particles[j]);
			}
		}
	}
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].addFlockingForce();
		particles[i].addDampingForce();
		particles[i].bounceOffWalls();
		particles[i].update();
	}
    
}

void ofxParticleSystem::addForce(ofVec2f force) {
    addForce(force.x,force.y);
}

void ofxParticleSystem::resetForce(){
    for (int i = 0; i < particles.size(); i++){
        particles[i].resetForce();
        particles[i].update();
    }
}

void ofxParticleSystem::addForce(float x, float y)
{
    for (int i = 0; i < particles.size(); i++){
        particles[i].addForce(x,y);
        particles[i].update();
    }
}

void ofxParticleSystem::addRepulsionForce(float x, float y, float radius, float scale)
{
    for (int i = 0; i < particles.size(); i++){
        particles[i].addRepulsionForce(x, y, radius, scale);
        particles[i].update();
    }
}

void ofxParticleSystem::addAttractionForce(float x, float y, float radius, float scale)
{
    for (int i = 0; i < particles.size(); i++){
        particles[i].addAttractionForce(x, y, radius, scale);
        particles[i].update();
    }
}

void ofxParticleSystem::draw() {
    ofEnableAlphaBlending();
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
    ofDisableAlphaBlending();
}

void ofxParticleSystem::addParticles( int amount, int minSize, int maxSize, int minLifetime, int maxLifetime, float fadeOut ) {
	for (int i = 0; i < amount; i++) {
        ofxParticle myParticle(width,height,ofRandom(minLifetime, maxLifetime),ofRandom(minSize,maxSize),fadeOut);
        myParticle.setInitialCondition(ofRandom(0,width),ofRandom(0,height),0,0);
        particles.push_back( myParticle );
	}
}

void ofxParticleSystem::removeParticles( int amount ) {
	for (int i = 0; i < amount; i++) {
		if (!particles.empty()) {
			particles.pop_back();
		}
	}
}

int ofxParticleSystem::numParticles() {
	return particles.size();
}
