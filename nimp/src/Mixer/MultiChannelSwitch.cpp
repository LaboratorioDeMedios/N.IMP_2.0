/*
 *  ChannelMixer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 10/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MultiChannelSwitch.h"

MultiChannelSwitch::MultiChannelSwitch(string name_, int id_):MixTable(name_, "Multi Channel Switch", id_){
    maxInputs = 16;
    selChannel = 0;
    
    gui.add(drawInputGui.set("Show Input Inspector", false));
    drawInputGui.addListener(this, &MultiChannelSwitch::cGui);
    
    gui.add(disableOtherChannels.set("Disable other channels", false));
    disableOtherChannels.addListener(this, &MultiChannelSwitch::cDisableChannels);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Show Input Inspector");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Disable other channels");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);

    selChannel.addListener(this, &MultiChannelSwitch::cselChannel);
    
    drawNoInputs = true;
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::inputAdded(ImageOutput* in_){
    
    Channel* c = new Channel();
    c->nodeId = in_->getId();
    c->label = in_->getName();
    if (input.size()>1) {
        c->selected.set(in_->getName(),false);
    } else {
        c->selected.set(in_->getName(),true);
        in_->setDrawInspector(drawInputGui);
    }
    
    selChannel.setMax(input.size()-1);
    channels.push_back(c);
    
    c->guiComponent = gui.add(c->selected);
    c->selected.addListener(this, &MultiChannelSwitch::cLabel);

    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find(c->label);
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    //hack
    lastClicked = ofGetElapsedTimeMillis();
}

//------------------------------------------------------------------
void MultiChannelSwitch::inputRemoved(int id_){
    
    if (selChannel == input.size() && input.size() != 0){
        selChannel -= 1;
        channels[selChannel]->selected = true;
    }
    for (int i = 0; i < channels.size(); i++) {
        if(channels[i]->nodeId == id_){
            gui.remove(channels[i]->guiComponent);
            channels.erase(channels.begin() + i);
        }
    }
    
    if (input.size() == 0) {
        selChannel.setMax(0);
        drawNoInputs = true;
    } else {
        selChannel.setMax(input.size());
        channels[selChannel]->selected = true;
    }

    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void MultiChannelSwitch::setup() {

    if(!input.size()) {
        drawNoInputs  = true;
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::update() {
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("Show Input Inspector")==0){
        this->drawInputGui = inputParam->intVal;
    }
    else if(inputParam->name.compare("Disable other channels")==0){
        this->disableOtherChannels = inputParam->intVal;
    }
    else if (inputParam->intVal > 0){
        for (int i = 0; i < channels.size(); i++) {
            if (inputParam->name == channels[i]->label && !channels[i]->selected.get()) {
                channels[i]->selected.set(true);
            }
        }
    }
}

//------------------------------------------------------------------
float MultiChannelSwitch::getMidiMin(string param_){
    
    if(param_.compare("Show Input Inspector")==0){
        return 0;
    }
    else if(param_.compare("Disable other channels")==0){
        return 0;
    }
    else {
        return 0;
    }
}

//------------------------------------------------------------------
float MultiChannelSwitch::getMidiMax(string param_){
    
    if(param_.compare("Show Input Inspector")==0){
        return 1;
    }
    else if(param_.compare("Disable other channels")==0){
        return 1;
    }
    else {
        return 1;
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::cselChannel(int& s){
    
    if (input.size()) {
        resetSizeBasedOnInput(input[selChannel]);
        getNodeViewerIBelong()->updateConnectionsSize(this);
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::cGui(bool& g){
    
    if (input.size() > 0)
        input[selChannel]->setDrawInspector(g);
}

//------------------------------------------------------------------
void MultiChannelSwitch::cDisableChannels(bool& c){
    
    if (input.size() > 0) {
        for (int i = 0; i < input.size(); i++) {
            if (i != selChannel) {
                input[i]->setEnable(!c);
            }
        }
        
        if (input[selChannel])
            input[selChannel]->setEnable(true);
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::cLabel(bool& b){
    if (ofGetElapsedTimeMillis()-lastClicked > 200) {
        //hacked radio button
        lastClicked = ofGetElapsedTimeMillis();
        if (b) {
            for (int i = 0; i < channels.size(); ++i) {
                channels[i]->selected = false;
            }
            b = true;
            drawNoInputs = false;
        }
        else {
            drawNoInputs = true;
            resetSizeToNoInputs();
        }
        for (int i = 0; i < channels.size(); ++i) {
            if (channels[i]->selected == true) {
                if (disableOtherChannels) {
                    input[selChannel]->setEnable(false);
                }
                selChannel = i;
                input[selChannel]->setEnable(true);
                input[selChannel]->setDrawInspector(drawInputGui);
            }
        }
    }
}

//------------------------------------------------------------------
ofTexture* MultiChannelSwitch::getTexture(){
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &input[selChannel]->getTextureReference();
}

//------------------------------------------------------------------
void MultiChannelSwitch::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    input[selChannel]->setEnable(isEnabled);
}

//------------------------------------------------------------------
void MultiChannelSwitch::resetSizeBasedOnInput(ofxPatch* input_){
    
    if (input_ == input[selChannel]) {
        ofxPatch::resetSizeBasedOnInput(input_);
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::_showHelp(){
    string name = "Node type name: Multi Channel Switch";
    string description = "Description: Forwards through the pipeline only one input";
    string use = "Use: Accepts several inputs but only forwards through the pipeline the selected one";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool MultiChannelSwitch::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {

    nId                  = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    drawInputGui         = ofToBool(XML.getAttribute("NODE","drawInputGui","false",nTag_));
    disableOtherChannels = ofToBool(XML.getAttribute("NODE", "disableOtherChannels","false",nTag_));
    selChannel           = ofToInt(XML.getAttribute("NODE","selChannel","0",nTag_));
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    std::map<string,ImageOutput*>::iterator it;
    
    for(int j=0; j < numINPUTTag; j++){
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
bool MultiChannelSwitch::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "MULTI_CHANNEL", lastPlace);
    
    XML.addAttribute("NODE", "selChannel", selChannel, lastPlace);
    XML.addAttribute("NODE", "drawInputGui", drawInputGui, lastPlace);
    XML.addAttribute("NODE", "disableOtherChannels", disableOtherChannels, lastPlace);
    
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
bool MultiChannelSwitch::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "MULTI_CHANNEL", lastPlace);
    
    XML.addAttribute("NODE", "selChannel", selChannel, lastPlace);
    XML.addAttribute("NODE", "drawInputGui", drawInputGui, lastPlace);
    XML.addAttribute("NODE", "disableOtherChannels", disableOtherChannels, lastPlace);
    
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
