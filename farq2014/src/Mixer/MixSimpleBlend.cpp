/*
 *  MixSimpleBlend.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixSimpleBlend.h"
#include "ImageAndVideoInputList.h"

MixSimpleBlend::MixSimpleBlend(string name_, int id_):MixTable(name_, "Mix Simple Blend", id_){
    
    maxInputs = 2;

    gui.add(isEnabled.setup("Enabled", isEnabled, 100, 20));
    
    gui.add(blendMode.set("BlendNormal", 0, 0, 24));
    gui.add(opacity.set("opacity", 0, 0, 255));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("BlendNormal");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("opacity");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    isEnabled = false;
    selector1 = 0;
    selector2 = 0;
    
    blendMode.addListener(this, &MixSimpleBlend::blendModeChanged);
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void MixSimpleBlend::setup() {
    
    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        drawNoInputs  = false;
        
        width  = input[selector1]->getTexture()->getWidth();
        height = input[selector1]->getTexture()->getHeight();

        fbo.allocate(width, height);
        psBlend.setup(width, height);
    }
}

//------------------------------------------------------------------
void MixSimpleBlend::update(){
    
    if(input.size()) {
        ofPushStyle();
        if (isEnabled) {
            fbo.begin();
            ofClear(255,255,255, 0);
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
            if (blendMode == 0) {
                
                if (opacity < 255) {
                    ofSetColor(255, 255, 255);
                    input[selector1]->getTextureReference().draw(0, 0, width, height);
                }
                if (opacity > 0) {
                    ofSetColor(255, 255, 255,opacity);
                    input[selector2]->getTextureReference().draw(0, 0, width, height);
                }
            }
            else{
                psBlend.begin();
                ofSetColor(255, 255, 255,opacity);
                input[selector2]->getTextureReference().draw(0, 0, width, height);
                psBlend.end();
                psBlend.draw(input[selector1]->getTextureReference(), blendMode);
            }
            glDisable(GL_BLEND);
            glPopAttrib();
            fbo.end();
        }
        else {
            fbo.begin();
            ofSetColor(255);
            input[selector1]->getTextureReference().draw(0, 0, width, height);
            fbo.end();
        }
        ofPopStyle();
    }
}

//------------------------------------------------------------------
void MixSimpleBlend::blendModeChanged(int& i){
    blendMode.setName(psBlend.getBlendMode(i));
}

//------------------------------------------------------------------
void MixSimpleBlend::entryChanged(bool& b){
    
    if (entries.size() == 1) {
        if (entries[0]->selected) {
            selector1 = 0;
            selector2 = 0;
            drawNoInputs = false;
        }
        else {
            drawNoInputs = true;
        }
    }
    else {
        if (entries[0]->selected && entries[1]->selected) {
            selector1 = 0;
            selector2 = 1;
            drawNoInputs = false;
        }
        else if (entries[0]->selected && !entries[1]->selected) {
            selector1 = 0;
            selector2 = 0;
            drawNoInputs = false;
        }
        else if (!entries[0]->selected && entries[1]->selected) {
            selector1 = 1;
            selector2 = 1;
            drawNoInputs = false;
        }
        else {
            drawNoInputs = true;
        }
    }
    drawNoInputs ? resetSizeToNoInputs() : resetSizeBasedOnInput(input[selector1]);
    getNodeViewerIBelong()->updateConnectionsSize(this);
}

//------------------------------------------------------------------
void MixSimpleBlend::inputAdded(ImageOutput* in_){
    Entry* e = new Entry();
    e->nodeId = in_->getId();
    e->label = in_->getName();
    if (entries.size() == 0) {
        selector1 == 0 ? e->selected.set(in_->getName(),true) : e->selected.set(in_->getName(),false);
        
    } else {
        selector2 == 1 ? e->selected.set(in_->getName(),true) : e->selected.set(in_->getName(),false);
    }
    
    e->guiComponent = gui.add(e->selected);
    e->selected.addListener(this, &MixSimpleBlend::entryChanged);
    entries.push_back(e);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void MixSimpleBlend::inputRemoved(int id_){
    
    selector1 = 0;
    selector2 = 0;
    
    for (int i = 0; i < entries.size(); i++) {
        if(entries[i]->nodeId == id_){
            gui.remove(entries[i]->guiComponent);
            entries.erase(entries.begin() + i);
        }
    }
    if (entries.size() != 0) {
        entries[0]->selected = true;
        resetSizeBasedOnInput(input[selector1]);
        getNodeViewerIBelong()->updateConnectionsSize(this);
    }
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void MixSimpleBlend::updateParameter(Param* inputParam){
    if(inputParam->name.compare("Enabled")==0){
        this->isEnabled = inputParam->intVal;
    }
    else if(inputParam->name.compare("opacity")==0){
        opacity.set(inputParam->intVal);
    }
    else if(inputParam->name.compare(0, 5, "Blend")==0){
        this->blendMode = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float MixSimpleBlend::getMidiMin(string param_){
    
    if(param_.compare("Enabled")==0){
        return 0;
    }else if(param_.compare("opacity")==0){
        return 0;
    }else if(param_.compare(0, 5, "Blend")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float MixSimpleBlend::getMidiMax(string param_){
    
    if(param_.compare("Enabled")==0){
        return 1;
    }else if(param_.compare("opacity")==0){
        return 255;
    }else if(param_.compare(0, 5, "Blend")==0){
        return 24;
    }
}

//------------------------------------------------------------------
ofTexture* MixSimpleBlend::getTexture(){
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &fbo.getTextureReference();
}

//------------------------------------------------------------------
void MixSimpleBlend::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    if (input.size() > 0) input[0]->setEnable(isEnabled);
    if (input.size() > 1) input[1]->setEnable(isEnabled);
}

//------------------------------------------------------------------
void MixSimpleBlend::resetSizeBasedOnInput(ofxPatch* input_){
    
//    if (input_ == input[selector2]) {
//        ofxPatch::resetSizeBasedOnInput(input_);
//    }
    ofxPatch::resetSizeBasedOnInput(input[selector1]);
}

//------------------------------------------------------------------
void MixSimpleBlend::_showHelp(){
    string name = "Node type name: Mix Simple Blend";
    string description = "Description: Blends two different images";
    string use = "Use: Blends two different images. This node accepts several inputs but only blends the first two selected";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool MixSimpleBlend::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    
    selector1   = ofToInt(XML.getAttribute("NODE","selector1","0", nTag_));
    selector2   = ofToInt(XML.getAttribute("NODE","selector2","0", nTag_));
    blendMode   = ofToInt(XML.getAttribute("NODE","blendmode","0", nTag_));
    opacity     = ofToFloat(XML.getAttribute("NODE","opacity","0", nTag_));
    
    nId         = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    isEnabled   = XML.getAttribute("NODE", "enabled", true, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    for(int j=0; j<numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j) + nodesCount_;
        addInputIdentifier(inputId);
    }
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool MixSimpleBlend::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SIMPLE_BLEND", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    
    XML.addAttribute("NODE", "selector1", selector1, lastPlace);
    XML.addAttribute("NODE", "selector2", selector2, lastPlace);
    XML.addAttribute("NODE", "blendmode", blendMode, lastPlace);
    XML.addAttribute("NODE", "opacity", opacity, lastPlace);
    
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
bool MixSimpleBlend::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SIMPLE_BLEND", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    
    XML.addAttribute("NODE", "blendmode", blendMode, lastPlace);
    XML.addAttribute("NODE", "opacity", opacity, lastPlace);
    
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
        if (saved) {
            XML.popTag();
            
            if (selector1 >= count)
                XML.addAttribute("NODE", "selector1", 0, lastPlace);
            else
                XML.addAttribute("NODE", "selector1", selector1, lastPlace);
            if (selector2 >= count)
                XML.addAttribute("NODE", "selector2", 0, lastPlace);
            else
                XML.addAttribute("NODE", "selector2", selector2, lastPlace);
        }
    }
    
    return saved;
}
