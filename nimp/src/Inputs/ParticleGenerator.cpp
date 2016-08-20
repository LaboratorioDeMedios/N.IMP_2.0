/*
 *  ParticleGenerator.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name, int id_) : InputSource(name, "Particle Generator", id_){
    
    width = 1080;
    height = 720;
    
    ofFbo::Settings s;
    s.width             = width;
    s.height            = height;
    s.internalformat    = GL_RGBA;
    s.useDepth			= false;
    fbo.allocate(s);
    
    img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    img.setUseTexture(true);
    
    gui.add(bAddParticles.setup("add Particles"));
    gui.add(bRemoveParticles.setup("remove Particles"));
    gui.add(isClearBg.set("Clear BG",true));
    gui.add(alphaParticles.set("Particles Alpha", 255, 0, 255));
    gui.add(autoGenParticle.set("autogen particle",false));
    gui.add(autoGenAmount.set("autogen amount", 0, 0, 5));
    gui.add(fadeOut.set("fade out",0.4,0.0,1.0));
    
    pScale.setName("Size");
    pScale.add(unityScale.set("Same Size",false));
    pScale.add(minRadius.set("min Size", 4, 1, 50));
    pScale.add(maxRadius.set("max Size", 10, 1, 50));
    gui.add(pScale);
    
    pLifetime.setName("Lifetime");
    pLifetime.add(minLifetime.set("min Lifetime", 0, 0, 60000));
    pLifetime.add(maxLifetime.set("max Lifetime", 0, 0, 60000));
    gui.add(pLifetime);
    
    pForce.setName("Force");
    pForce.add(force_attracting.set("Attracting", false));
    pForce.add(force_radius.set("Radius", 100, 0, 200));
    pForce.add(force_scale.set("Scale", 1, 0, 20));
    pForce.add(force_x.set("Force X", 0, 0, width));
    pForce.add(force_y.set("Force Y", 0, 0, height));
    gui.add(pForce);
    
    force_x.addListener(this, &ParticleGenerator::forceXChanged);
    force_y.addListener(this, &ParticleGenerator::forceYChanged);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    
    baseGui = gui.find("Clear BG");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Particles Alpha");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("autogen particle");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("autogen amount");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("fade out");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Same Size");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("min Size");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("max Size");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("min Lifetime");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("max Lifetime");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Force X");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Force Y");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Attracting");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Radius");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Scale");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
}

//------------------------------------------------------------------
ParticleGenerator::~ParticleGenerator(){
    ofxBaseGui* baseGui;
    baseGui = gui.find("Clear BG");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Particles Alpha");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("autogen particle");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("autogen amount");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("fade out");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Same Size");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("min Size");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("max Size");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("min Lifetime");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("max Lifetime");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Force X");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Force Y");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Attracting");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Radius");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Scale");
    ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    delete particle;
}

//------------------------------------------------------------------
void ParticleGenerator::setup() {

    particle = new ofxParticleSystem(200,width,height,minRadius,(unityScale) ? minRadius : maxRadius,minLifetime,maxLifetime,fadeOut);
}

//------------------------------------------------------------------
void ParticleGenerator::update() {
    
    if (autoGenParticle && (lastGen + 20 < ofGetElapsedTimeMillis())) {
        lastGen = ofGetElapsedTimeMillis();
        particle->addParticles(autoGenAmount,minRadius,maxRadius,minLifetime,maxLifetime,fadeOut);
    }
    
    if (bAddParticles) {
        particle->addParticles(10,minRadius,maxRadius,minLifetime,maxLifetime,fadeOut);
    }
    if (bRemoveParticles) {
        particle->removeParticles(10);
    }
    
    for(int i=0; i<force.size();++i){
        if (force[i].isAttracting) {
            particle->addAttractionForce(force[i].pos->x, force[i].pos->y, force[i].radius, force[i].scale);
        }else{
            particle->addRepulsionForce(force[i].pos->x, force[i].pos->y, force[i].radius, force[i].scale);
        }
    }
    
    particle->update();
    
    fbo.begin();
    ofEnableAlphaBlending();
    if (isClearBg) {
        ofClear(0,0,0,255);
    }
    ofSetColor(255, 255, 255,alphaParticles);
    particle->draw();
    ofDisableAlphaBlending();
    fbo.end();
    
    force.clear();
}

//------------------------------------------------------------------
void ParticleGenerator::forceXChanged(float &x) {
    
    ParticleForce f;
    f.radius = force_radius;
    f.isAttracting = force_attracting;
    f.scale = force_scale;
    f.pos.set(ofVec2f(x,force_y));
    addForce(f);
}

//------------------------------------------------------------------
void ParticleGenerator::forceYChanged(float &y) {
    
    ParticleForce f;
    f.radius = force_radius;
    f.isAttracting = force_attracting;
    f.scale = force_scale;
    f.pos.set(ofVec2f(force_x,y));
    addForce(f);
}

//------------------------------------------------------------------
void ParticleGenerator::addForce(ParticleForce f){
    force.push_back(f);
}

//------------------------------------------------------------------
void ParticleGenerator::updateParameter(Param* inputParam){
    if(inputParam->name.compare("Force X")==0){
        ParticleForce f;
        f.radius = force_radius;
        f.isAttracting = force_attracting;
        f.scale = force_scale;
        f.pos.set(ofVec2f(inputParam->floatVal,force_y));
        addForce(f);
        force_x = inputParam->floatVal;
    }
    else if (inputParam->name.compare("Force Y") == 0 && force.size() > 0){
        force_y = inputParam->floatVal;
        ofVec2f v = force[force.size()-1].pos;
        v.y = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Attracting")==0){
        force_attracting = inputParam->intVal;
    }
    else if(inputParam->name.compare("Radius")==0){
        force_radius = inputParam->intVal;
    }
    else if(inputParam->name.compare("Scale")==0){
        force_scale = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Clear BG")==0){
        isClearBg = inputParam->intVal;
    }
    else if(inputParam->name.compare("Particles Alpha")==0){
        alphaParticles = inputParam->intVal;
    }
    else if(inputParam->name.compare("autogen particle")==0){
        autoGenParticle = inputParam->intVal;
    }
    else if(inputParam->name.compare("autogen amount")==0){
        autoGenAmount = inputParam->intVal;
    }
    else if(inputParam->name.compare("fade out")==0){
        fadeOut = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Same Size")==0){
        unityScale = inputParam->intVal;
    }
    else if(inputParam->name.compare("min Size")==0){
        minRadius = inputParam->intVal;
    }
    else if(inputParam->name.compare("max Size")==0){
        maxRadius = inputParam->intVal;
    }
    else if(inputParam->name.compare("min Lifetime")==0){
        minLifetime = inputParam->intVal;
    }
    else if(inputParam->name.compare("max Lifetime")==0){
        maxLifetime = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float ParticleGenerator::getMidiMin(string param_){
    
    if(param_.compare("Force X")==0){
        return 0;
    }else if (param_.compare("Force Y")==0){
        return 0;
    }else if(param_.compare("Attracting")==0){
        return 0;
    }else if(param_.compare("Radius")==0){
        return 0;
    }else if(param_.compare("Scale")==0){
        return 1;
    }else if(param_.compare("Clear BG")==0){
        return 0;
    }else if(param_.compare("Particles Alpha")==0){
        return 0;
    }else if(param_.compare("autogen particle")==0){
        return 0;
    }else if(param_.compare("autogen amount")==0){
        return 0;
    }else if(param_.compare("fade out")==0){
        return 0.0;
    }else if(param_.compare("Same Size")==0){
        return 0;
    }else if(param_.compare("min Size")==0){
        return 1;
    }else if(param_.compare("max Size")==0){
        return 1;
    }else if(param_.compare("min Lifetime")==0){
        return 0;
    }else if(param_.compare("max Lifetime")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float ParticleGenerator::getMidiMax(string param_){
    
    if(param_.compare("Force X")==0){
        return width;
    }else if (param_.compare("Force Y")==0){
        return height;
    }else if(param_.compare("Attracting")==0){
        return 1;
    }else if(param_.compare("Radius")==0){
        return 200;
    }else if(param_.compare("Scale")==0){
        return 20;
    }else if(param_.compare("Clear BG")==0){
        return 1;
    }else if(param_.compare("Particles Alpha")==0){
        return 255;
    }else if(param_.compare("autogen particle")==0){
        return 1;
    }else if(param_.compare("autogen amount")==0){
        return 5;
    }else if(param_.compare("fade out")==0){
        return 1.0;
    }else if(param_.compare("Same Size")==0){
        return 1;
    }else if(param_.compare("min Size")==0){
        return 50;
    }else if(param_.compare("max Size")==0){
        return 50;
    }else if(param_.compare("min Lifetime")==0){
        return 60000;
    }else if(param_.compare("max Lifetime")==0){
        return 60000;
    }
}

//------------------------------------------------------------------
ofTexture* ParticleGenerator::getTexture(){
    return &fbo.getTextureReference();
}

//------------------------------------------------------------------
void ParticleGenerator::_showHelp(){
    string name = "Node type name: Particle Generator";
    string description = "Description: Generates particles randomly";
    string use = "Use: Generates partilces randomly";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool ParticleGenerator::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool saved = true;
    
    isClearBg       = ofToBool(XML.getAttribute("NODE", "isClearBg","true", nTag_));
    alphaParticles  = ofToFloat(XML.getAttribute("NODE", "alphaParticles","0.4", nTag_));
    autoGenParticle = ofToBool(XML.getAttribute("NODE", "autoGenParticle","false", nTag_));
    autoGenAmount   = ofToFloat(XML.getAttribute("NODE", "autoGenAmount","0.0", nTag_));
    unityScale      = ofToBool(XML.getAttribute("NODE", "unityScale","false", nTag_));
    minRadius       = ofToFloat(XML.getAttribute("NODE", "minRadius","4", nTag_));
    maxRadius       = ofToFloat(XML.getAttribute("NODE", "maxRadius","10", nTag_));
    minLifetime     = ofToFloat(XML.getAttribute("NODE", "minLifetime","0", nTag_));
    maxLifetime     = ofToFloat(XML.getAttribute("NODE", "maxLifetime","0", nTag_));
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    
    saved = XML.pushTag("NODE", nTag_);
    if (saved) {
        type            = XML.getValue("type","none");
        bVisible        = XML.getValue("visible", true);
        
        saved = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
        
        XML.popTag();
    }
    
    return saved;
}

//------------------------------------------------------------------
bool ParticleGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "PARTICLE", lastPlace);
    
    XML.addAttribute("NODE", "isClearBg", isClearBg, lastPlace);
    XML.addAttribute("NODE", "alphaParticles", alphaParticles, lastPlace);
    XML.addAttribute("NODE", "autoGenParticle", autoGenParticle, lastPlace);
    XML.addAttribute("NODE", "autoGenAmount", autoGenAmount, lastPlace);
    XML.addAttribute("NODE", "unityScale", unityScale, lastPlace);
    XML.addAttribute("NODE", "minRadius", minRadius, lastPlace);
    XML.addAttribute("NODE", "maxRadius", maxRadius, lastPlace);
    XML.addAttribute("NODE", "minLifetime", minLifetime, lastPlace);
    XML.addAttribute("NODE", "maxLifetime", maxLifetime, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }

    return saved;
    
}

//------------------------------------------------------------------
bool ParticleGenerator::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "PARTICLE", lastPlace);
    
    XML.addAttribute("NODE", "isClearBg", isClearBg, lastPlace);
    XML.addAttribute("NODE", "alphaParticles", alphaParticles, lastPlace);
    XML.addAttribute("NODE", "autoGenParticle", autoGenParticle, lastPlace);
    XML.addAttribute("NODE", "autoGenAmount", autoGenAmount, lastPlace);
    XML.addAttribute("NODE", "unityScale", unityScale, lastPlace);
    XML.addAttribute("NODE", "minRadius", minRadius, lastPlace);
    XML.addAttribute("NODE", "maxRadius", maxRadius, lastPlace);
    XML.addAttribute("NODE", "minLifetime", minLifetime, lastPlace);
    XML.addAttribute("NODE", "maxLifetime", maxLifetime, lastPlace);

    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();
    }
    
    return saved;
    
}
