/*
 *  MixMask.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixMask.h"

MixMask::MixMask(string name_, int id_):MixTable(name_, "Mix Mask", id_){
    
    gui.add(isEnabled.setup("Enabled", isEnabled, 100, 20));
    
    gui.add(spin.setup("spin", 90, 0, 360));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("spin");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    maxInputs = 2;
    maskShader.load("Shaders/composite");
    maskShader.begin();
    maskShader.end();
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void MixMask::setup() {
    
    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        width  = input[0]->getWidth();
        height = input[0]->getHeight();
        
        fbo.allocate(width, height);
    }
}

//------------------------------------------------------------------
void MixMask::update(){
    
    if(input.size()) {
        ofPushStyle();
        if (isEnabled) {
            fbo.begin();
            ofClear(255,255,255, 0);
            input[0]->getTextureReference().draw(0, 0, width, height);
            ofEnableAlphaBlending();
            ofPushMatrix();
            ofTranslate(width/2, height/2);
            ofPushMatrix();
            ofRotate(spin);
            ofPushMatrix();
            ofTranslate(-width/2, -height/2);
            drawShader();
            ofPopMatrix();
            ofPopMatrix();
            ofPopMatrix();
            ofDisableBlendMode();
            fbo.end();
        }
        else {
            fbo.begin();
            ofSetColor(255);
            input[0]->getTextureReference().draw(0, 0, width, height);
            fbo.end();
        }
        ofPopStyle();
    }
}

//------------------------------------------------------------------
void MixMask::drawShader(){
    maskShader.begin();
    maskShader.setUniformTexture("Tex0", input[0]->getTextureReference(), 0);
    if(input.size() > 1){
        maskShader.setUniformTexture("Tex1", input[1]->getTextureReference(), 1);
    } else {
        maskShader.setUniformTexture("Tex1", input[0]->getTextureReference(), 1);
    }
    
    //our shader uses two textures, the top layer and the alpha
    //we can load two textures into a shader using the multi texture coordinate extensions
    glActiveTexture(GL_TEXTURE0_ARB);
    input[0]->getTexture()->bind();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    if(input.size() > 1) {
        input[1]->getTexture()->bind();
    } else {
        input[0]->getTexture()->bind();
    }
    
    //draw a quad the size of the frame
    glBegin(GL_QUADS);
    
    //move the mask around with the mouse by modifying the texture coordinates
    float maskOffset = 0;//15 - mouseY;
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskOffset);
    glVertex2f( 0, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, width, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, width, maskOffset);
    glVertex2f( width, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, width, height);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, width, height + maskOffset);
    glVertex2f( width,height);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, height);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, height + maskOffset);
    glVertex2f( 0, height );
    
    glEnd();
    
    //deactive and clean up
    glActiveTexture(GL_TEXTURE1_ARB);
    input[0]->getTexture()->unbind();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    if(input.size() > 1) {
        input[1]->getTexture()->unbind();
    } else {
        input[0]->getTexture()->unbind();
    }
    
    maskShader.end();
}

//------------------------------------------------------------------
void MixMask::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("spin")==0){
        spin = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float MixMask::getMidiMin(string param_){
    
    if(param_.compare("spin")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float MixMask::getMidiMax(string param_){
    
    if(param_.compare("spin")==0){
        return 360;
    }
}

//------------------------------------------------------------------
ofTexture* MixMask::getTexture(){
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &fbo.getTextureReference();
}

//------------------------------------------------------------------
void MixMask::inputAdded(ImageOutput* in_){

}

//------------------------------------------------------------------
void MixMask::inputRemoved(int id_){
    
}

//------------------------------------------------------------------
void MixMask::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    
    if (input.size() > 0) input[0]->setEnable(isEnabled);
    if (input.size() > 1) input[1]->setEnable(isEnabled);
}

//------------------------------------------------------------------
void MixMask::_showHelp(){
    string name = "Node type name: Mix Mask";
    string description = "Description: Mix two different images";
    string use = "Use: Mix two different images, letting rotate one. This node accepts two inputs maximum";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool MixMask::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    nId       = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    spin      = XML.getAttribute("NODE", "spin", 90, nTag_);
    isEnabled = XML.getAttribute("NODE", "enabled", true, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    std::map<string,ImageOutput*>::iterator it;
    
    for(int j=0; j<numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j) + nodesCount_;
        addInputIdentifier(inputId);
    }
    
    type     = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool MixMask::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "MASK", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    
    XML.addAttribute("NODE", "spin", spin, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        for (int iS = 0; iS < input.size(); iS++){
            XML.addTag("INPUT_SOURCE");
            XML.addAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
        }
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
}

//------------------------------------------------------------------
bool MixMask::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "MASK", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    
    XML.addAttribute("NODE", "spin", spin, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        int count = 0;
        for (int iS = 0; iS < input.size(); iS++){
            
            if (newIdsMap[input[iS]->getId()]) {
                XML.addTag("INPUT_SOURCE");
                XML.addAttribute("INPUT_SOURCE", "nodeId", newIdsMap[input[iS]->getId()], count);
                count++;
            }
        }
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();
    }
    
    return saved;
}