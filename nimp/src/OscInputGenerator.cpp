/*
 *  OscInputGenerator.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 15/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscInputGenerator.h"


OscInputGenerator::OscInputGenerator(string name_):ParamInputGenerator(name_, true){
    oscMap              = new std::map<string,DTOscMap* >();
    type                = OSC;
    numberOSCReceiver   = 1;
}

//------------------------------------------------------------------
OscInputGenerator::~OscInputGenerator(){
    
    stopThread();
    oscMap->clear();
    delete oscMap;
    oscMap = NULL;
}

//------------------------------------------------------------------
void OscInputGenerator::processInput() {
    if(lock()){
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            std::map<string,DTOscMap* >::iterator it;
            
            it = oscMap->find(m.getAddress());
            
            if(it!=oscMap->end()){
                for (int j = 0; j < m.getNumArgs() && j < it->second->paramId.size(); ++j) {
                    
                    Param* p        = new Param();
                    p->inputMax     = it->second->inputMaxValue[j];
                    p->inputMin     = it->second->inputMinValue[j];
                    p->imageInputId = it->second->nodeId[j];
                    p->name         = it->second->paramId[j];
                    p->value        = m.getArgAsFloat(j) ? m.getArgAsFloat(j) : m.getArgAsInt32(j);
                    p->floatVal     = ofMap(m.getArgAsFloat(j), it->second->inputMinValue[j], it->second->inputMaxValue[j], it->second->paramMinValue[j], it->second->paramMaxValue[j]);
                    p->intVal       = ofMap(m.getArgAsInt32(j), it->second->inputMinValue[j], it->second->inputMaxValue[j], it->second->paramMinValue[j], it->second->paramMaxValue[j]);
                    storeMessage(p);
                }
            }
        }
        unlock();
    }
}

//------------------------------------------------------------------
void OscInputGenerator::setPort(int port_) {
    
    port = port_;
    if (port > 999 && port < 65535) {
        try {
            receiver.setup(port);
        } catch (int ex) {
            // TODO
        }
    }
    
}

//------------------------------------------------------------------
void OscInputGenerator::setAddress(string oldAddress_, string address_) {
 
    std::map<string,DTOscMap* >::iterator it;
    
    it = oscMap->find(oldAddress_);
    if (it != oscMap->end()) {
        oscMap->insert(std::pair<string,DTOscMap* >(address_,it->second));
        oscMap->erase(it);
    }
}

//------------------------------------------------------------------
void OscInputGenerator::setMin(string address_, int _min) {
    std::map<string,DTOscMap* >::iterator it;
    it = oscMap->find(address_);
    
    if(it != oscMap->end()){
        for(int i = 0; i < it->second->paramId.size(); i++) {
            it->second->inputMinValue[i] = _min;
        }
    }
}

//------------------------------------------------------------------
void OscInputGenerator::setMax(string address_, int _max) {
    std::map<string,DTOscMap* >::iterator it;
    it = oscMap->find(address_);
    
    if(it != oscMap->end()){
        for(int i = 0; i < it->second->paramId.size(); i++) {
            it->second->inputMaxValue[i] = _max;
        }
    }
}

//------------------------------------------------------------------
bool OscInputGenerator::addNewOSCMap(int min_, int max_, string address_, ImageOutput* node_, vector<string> params_) {
    
    std::map<string,DTOscMap* >::iterator it = oscMap->find(address_);
    
    if (it != oscMap->end()) {
        for (int i = 0; i < params_.size(); i++) {
            it->second->paramId.push_back(params_[i]);
            it->second->nodeId.push_back(node_->getId());
            it->second->inputMinValue.push_back(min_);
            it->second->inputMaxValue.push_back(max_);
            it->second->paramMinValue.push_back(node_->getMidiMin(params_[i]));
            it->second->paramMaxValue.push_back(node_->getMidiMax(params_[i]));
        }
    }
    else {
        DTOscMap* dtM = new DTOscMap();
        
        for (int i = 0; i < params_.size(); i++) {
            dtM->paramId.push_back(params_[i]);
            dtM->nodeId.push_back(node_->getId());
            dtM->inputMinValue.push_back(min_);
            dtM->inputMaxValue.push_back(max_);
            dtM->paramMinValue.push_back(node_->getMidiMin(params_[i]));
            dtM->paramMaxValue.push_back(node_->getMidiMax(params_[i]));
        }
        oscMap->insert(std::pair<string,DTOscMap* >(address_,dtM));
    }
}

//------------------------------------------------------------------
bool OscInputGenerator::addNewOSCMap(int min_, int max_, string address_, ImageOutput* node_, string param_) {
    
    std::map<string,DTOscMap* >::iterator it = oscMap->find(address_);
    
    if (it != oscMap->end()) {
        it->second->paramId.push_back(param_);
        it->second->nodeId.push_back(node_->getId());
        it->second->inputMinValue.push_back(min_);
        it->second->inputMaxValue.push_back(max_);
        it->second->paramMinValue.push_back(node_->getMidiMin(param_));
        it->second->paramMaxValue.push_back(node_->getMidiMax(param_));
    }
    else {
        DTOscMap* dtM = new DTOscMap();
        
        dtM->paramId.push_back(param_);
        dtM->nodeId.push_back(node_->getId());
        dtM->inputMinValue.push_back(min_);
        dtM->inputMaxValue.push_back(max_);
        dtM->paramMinValue.push_back(node_->getMidiMin(param_));
        dtM->paramMaxValue.push_back(node_->getMidiMax(param_));
        
        oscMap->insert(std::pair<string,DTOscMap* >(address_,dtM));
    }
}

//------------------------------------------------------------------
void OscInputGenerator::removeOSCMap(string address_, ImageOutput* node_, string param_) {
    
    std::map<string,DTOscMap* >::iterator it = oscMap->find(address_);
    
    if (it != oscMap->end()) {
        
        int j = 0;
        while (j < it->second->paramId.size()) {
            if (it->second->nodeId[j] == node_->getId() && (it->second->paramId[j] == param_ ||
               (it->second->paramId[j].compare(0, 5, "Blend")==0 && param_.compare(0, 5, "Blend")==0))) {
                
                it->second->paramId.erase(it->second->paramId.begin() + j);
                it->second->nodeId.erase(it->second->nodeId.begin() + j);
                it->second->inputMinValue.erase(it->second->inputMinValue.begin() + j);
                it->second->inputMaxValue.erase(it->second->inputMaxValue.begin() + j);
                it->second->paramMinValue.erase(it->second->paramMinValue.begin() + j);
                it->second->paramMaxValue.erase(it->second->paramMaxValue.begin() + j);
                
                j = it->second->paramId.size();
            }
            
            j++;
        }
    }
}

//------------------------------------------------------------------
void OscInputGenerator::removeNodeFromParams(int nodeID_) {
    
    for (std::map<string,DTOscMap* >::iterator it = oscMap->begin(); it != oscMap->end(); it++) {
        for (int j = 0; j < it->second->nodeId.size(); j++) {
            if (it->second->nodeId[j] == nodeID_) {
                it->second->paramId.erase(it->second->paramId.begin() + j);
                it->second->nodeId.erase(it->second->nodeId.begin() + j);
                it->second->inputMinValue.erase(it->second->inputMinValue.begin() + j);
                it->second->inputMaxValue.erase(it->second->inputMaxValue.begin() + j);
                it->second->paramMinValue.erase(it->second->paramMinValue.begin() + j);
                it->second->paramMaxValue.erase(it->second->paramMaxValue.begin() + j);
                j--;
            }
        }
    }
}

//------------------------------------------------------------------
void OscInputGenerator::clearOSCMap() {
    oscMap->clear();
}

//------------------------------------------------------------------
DTOscMap* OscInputGenerator::getOSCMapForAddress(string address_) {
    DTOscMap* ret = NULL;
    
    std::map<string,DTOscMap* >::iterator it;
    it = oscMap->find(address_);
    
    if(it != oscMap->end()){
        ret = it->second;
        oscMap->erase(it);
    }
    
    // decrease 1 the counter of OSC Receivers mapped to this OSC Input Generator
    oscReceiverLeft();
    
    return ret;
}

//------------------------------------------------------------------
vector<string> OscInputGenerator::getAllAddress(){
    vector<string> keys;
    for(std::map<string,DTOscMap* >::iterator it = oscMap->begin(); it != oscMap->end(); ++it) {
        keys.push_back(it->first);
    }
    return keys;
}

//------------------------------------------------------------------
void OscInputGenerator::addOSCMapForAddress(string address_, DTOscMap* oscmap){
    std::map<string,DTOscMap* >::iterator it;
    it = oscMap->find(address_);
    
    if(it != oscMap->end()){
        for(int i=0; i < oscmap->paramId.size(); i++) {
            it->second->paramId.push_back(oscmap->paramId[i]);
            it->second->nodeId.push_back(oscmap->nodeId[i]);
            it->second->inputMinValue.push_back(oscmap->inputMinValue[i]);
            it->second->inputMaxValue.push_back(oscmap->inputMaxValue[i]);
            it->second->paramMinValue.push_back(oscmap->paramMinValue[i]);
            it->second->paramMaxValue.push_back(oscmap->paramMaxValue[i]);
        }
    } else {
        oscMap->insert(std::pair<string,DTOscMap* >(address_,oscmap));
    }
    
    // increase 1 the counter of OSC Receivers mapped to this OSC Input Generator
    oscReceiverIn();
}

//------------------------------------------------------------------
bool OscInputGenerator::loadSettings(ofxXmlSettings &XML, int nodesCount_) {
    
    bool result = true;
    
    //get osc input device
    //
    port                = ofToInt(XML.getAttribute("OSC_SETTINGS","port","6666"));
    numberOSCReceiver   = ofToInt(XML.getAttribute("OSC_SETTINGS", "numberOSCReceiver", "1"));
    receiver.setup(port);
    
    //get osc mapping data
    //
    XML.pushTag("OSC_SETTINGS");
    int getNumAddressTag = XML.getNumTags("ADDRESS");
    
    for (int j = 0; j < getNumAddressTag; ++j) {
        
        string address = XML.getAttribute("ADDRESS","path","/",j);
        XML.pushTag("ADDRESS", j);
        int getNumOSCMapTag = XML.getNumTags("OSC_MAP");
        DTOscMap* dtM = new DTOscMap();
        
        for(int i = 0; i < getNumOSCMapTag; i++){
            
            int     nodeId = XML.getAttribute("OSC_MAP","nodeId",0,i) + nodesCount_;
            string  param = XML.getAttribute("OSC_MAP","param","",i);
            float   inputMinValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMinValue","0",i));
            float   inputMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMaxValue","127",i));
            float   paramMinValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMinValue","0",i));
            float   paramMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMaxValue","127",i));
            
            dtM->paramId.push_back(param);
            dtM->nodeId.push_back(nodeId);
            dtM->inputMinValue.push_back(inputMinValue);
            dtM->inputMaxValue.push_back(inputMaxValue);
            dtM->paramMinValue.push_back(paramMinValue);
            dtM->paramMaxValue.push_back(paramMaxValue);
            
            oscMap->insert(std::pair<string,DTOscMap* >(address,dtM));
        }
        XML.popTag();
    }
    XML.popTag();
    
    return result;
}


//------------------------------------------------------------------
bool OscInputGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
    
    // Insert a new INPUT_GEN tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("INPUT_GEN");
    
    XML.addAttribute("INPUT_GEN", "type", "OSC", lastPlace);
    
    saved = XML.pushTag("INPUT_GEN", lastPlace);
    if (saved) {
        
        lastPlace = XML.addTag("OSC_SETTINGS");
        XML.addAttribute("OSC_SETTINGS", "port", port, lastPlace);
        XML.addAttribute("OSC_SETTINGS", "numberOSCReceiver", numberOSCReceiver, lastPlace);
        saved = XML.pushTag("OSC_SETTINGS", lastPlace);
        if (saved) {
            
            for (std::map<string,DTOscMap* >::iterator it = oscMap->begin(); it != oscMap->end(); it++) {
                lastPlace = XML.addTag("ADDRESS");
                XML.setAttribute("ADDRESS","path", it->first, lastPlace);
                XML.pushTag("ADDRESS", lastPlace);
                for (int j = 0; j < it->second->nodeId.size(); j++) {
                    
                    XML.addTag("OSC_MAP");
                    XML.addAttribute("OSC_MAP","nodeId", it->second->nodeId[j], j);
                    XML.addAttribute("OSC_MAP","param", it->second->paramId[j], j);
                    XML.addAttribute("OSC_MAP","inputMinValue", it->second->inputMinValue[j], j);
                    XML.addAttribute("OSC_MAP","inputMaxValue", it->second->inputMaxValue[j], j);
                    XML.addAttribute("OSC_MAP","paramMinValue", it->second->paramMinValue[j], j);
                    XML.addAttribute("OSC_MAP","paramMaxValue", it->second->paramMaxValue[j], j);
                }
                XML.popTag(); // ADDRESS
            }
            
            XML.popTag(); // FFT_SETTINGS
        }
        XML.popTag(); // INPUT_GEN
    }
    
    return saved;
}

//------------------------------------------------------------------
bool OscInputGenerator::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("INPUT_GEN");

    XML.addAttribute("INPUT_GEN", "type", "OSC", lastPlace);
    
    saved = XML.pushTag("INPUT_GEN", lastPlace);
    if (saved) {
        
        lastPlace = XML.addTag("OSC_SETTINGS");
        XML.addAttribute("OSC_SETTINGS", "port", port, lastPlace);
        XML.addAttribute("OSC_SETTINGS", "numberOSCReceiver", numberOSCReceiver, lastPlace);
        saved = XML.pushTag("OSC_SETTINGS", lastPlace);
        if (saved) {
            
            for (std::map<string,DTOscMap* >::iterator it = oscMap->begin(); it != oscMap->end(); it++) {
                lastPlace = XML.addTag("ADDRESS");
                XML.setAttribute("ADDRESS","path", it->first, lastPlace);
                XML.pushTag("ADDRESS", lastPlace);
                for (int j = 0; j < it->second->nodeId.size(); j++) {
                    
                    if (newIdsMap[it->second->nodeId[j]]) {
                        XML.addTag("OSC_MAP");
                        XML.addAttribute("OSC_MAP","nodeId", it->second->nodeId[j], j);
                        XML.addAttribute("OSC_MAP","param", it->second->paramId[j], j);
                        XML.addAttribute("OSC_MAP","inputMinValue", it->second->inputMinValue[j], j);
                        XML.addAttribute("OSC_MAP","inputMaxValue", it->second->inputMaxValue[j], j);
                        XML.addAttribute("OSC_MAP","paramMinValue", it->second->paramMinValue[j], j);
                        XML.addAttribute("OSC_MAP","paramMaxValue", it->second->paramMaxValue[j], j);
                    }
                }
                XML.popTag(); // ADDRESS
            }
            
            XML.popTag(); // FFT_SETTINGS
        }
        XML.popTag(); // INPUT_GEN
    }
    
    return saved;
}


