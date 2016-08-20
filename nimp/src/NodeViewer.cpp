/*
 *  NodeViewer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeViewer.h"

NodeViewer::NodeViewer(string name_) : ofxComposer() {
    name = name_;
}

//------------------------------------------------------------------
NodeViewer::~NodeViewer() {

}

//------------------------------------------------------------------
void NodeViewer::setup() {
	
}

//------------------------------------------------------------------
void NodeViewer::update() {
    
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->getImageOutput()->update();
    }
    
    ofxComposer::update();
}

//------------------------------------------------------------------
void NodeViewer::draw() {
    
    ofxComposer::customDraw();
}

//------------------------------------------------------------------
void NodeViewer::setMidiLearnActive(bool active_){
    
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->getImageOutput()->setMidiLearnActive(active_);
        
        if (elements[i]->getImageOutput()->getTypeName() == "OSC Receiver")
            ((OSCReceiver*)elements[i]->getImageOutput())->disableEdit(active_);
        else  if (elements[i]->getImageOutput()->getTypeName() == "Audio In - Left Channel" || elements[i]->getImageOutput()->getTypeName() == "Audio In - Right Channel")
            ((AudioIn*)elements[i]->getImageOutput())->disableEdit(active_);
    }
    
    this->midiLearnActive = active_;
}

//------------------------------------------------------------------
void NodeViewer::setEditLeftAudioInActive(bool active_, int node_){
    
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->getImageOutput()->setEditLeftAudioInActive(active_, node_);
        
        if (node_ != elements[i]->getImageOutput()->getId()) {
            if (elements[i]->getImageOutput()->getTypeName() == "OSC Receiver")
                ((OSCReceiver*)elements[i]->getImageOutput())->disableEdit(active_);
            else  if (elements[i]->getImageOutput()->getTypeName() == "Audio In - Left Channel" || elements[i]->getImageOutput()->getTypeName() == "Audio In - Right Channel")
                ((AudioIn*)elements[i]->getImageOutput())->disableEdit(active_);
        }
    }
    
    this->editAudioInActive = active_;
}

//------------------------------------------------------------------
void NodeViewer::setEditRightAudioInActive(bool active_, int node_){
    
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->getImageOutput()->setEditRightAudioInActive(active_, node_);
        
        if (node_ != elements[i]->getImageOutput()->getId()) {
            if (elements[i]->getImageOutput()->getTypeName() == "OSC Receiver")
                ((OSCReceiver*)elements[i]->getImageOutput())->disableEdit(active_);
            else  if (elements[i]->getImageOutput()->getTypeName() == "Audio In - Left Channel" || elements[i]->getImageOutput()->getTypeName() == "Audio In - Right Channel")
                ((AudioIn*)elements[i]->getImageOutput())->disableEdit(active_);
        }
    }
    
    this->editAudioInActive = active_;
}

//------------------------------------------------------------------
void NodeViewer::setEditOSCActive(bool active_, int node_){
    
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->getImageOutput()->setEditOSCActive(active_, node_);
        
        if (node_ != elements[i]->getImageOutput()->getId()) {
            if (elements[i]->getImageOutput()->getTypeName() == "OSC Receiver")
                ((OSCReceiver*)elements[i]->getImageOutput())->disableEdit(active_);
            else  if (elements[i]->getImageOutput()->getTypeName() == "Audio In - Left Channel" || elements[i]->getImageOutput()->getTypeName() == "Audio In - Right Channel")
                ((AudioIn*)elements[i]->getImageOutput())->disableEdit(active_);
        }
    }
    
    this->editOSCActive = active_;
}

//------------------------------------------------------------------
void NodeViewer::addElement(NodeElement* elem_){
    addElement(elem_, ofPoint(ofGetMouseX(), ofGetMouseY()));
}

//------------------------------------------------------------------
void NodeViewer::addElement(NodeElement* elem_, ofPoint position_){
    elem_->getImageOutput()->setNodeViewerIBelong(this);
    elements.push_back(elem_);
    addPatch(elem_->getImageOutput(), position_);
}

//------------------------------------------------------------------
void NodeViewer::createConnections(){
    
    //connections.clear();
    vector<string> elemNames;
    
    vector<ImageOutput*> inputs;
    
    for (int i=0; i<elements.size(); ++i) {
        inputs = elements[i]->getImageOutput()->getInputs();
        
        for (int j = 0; j < inputs.size(); j++) {
            connect(inputs[j]->getId(), elements[i]->getImageOutput()->getId(), 0, false);
        }
    }
}

//------------------------------------------------------------------
void NodeViewer::createConnections(vector<NodeElement*> _elements){
    vector<string> elemNames;
    
    vector<ImageOutput*> inputs;
    
    for (int i = 0; i < _elements.size(); ++i) {
        inputs = _elements[i]->getImageOutput()->getInputs();
        
        for (int j = 0; j < inputs.size(); j++) {
            connect(inputs[j]->getId(), _elements[i]->getImageOutput()->getId(), 0, false);
        }
    }
}

//------------------------------------------------------------------
void NodeViewer::closePatch( int _nID ){
    
    bool deleted = false;
    
    if (_nID > 0) {
        int i = 0;
        while (i < elements.size()) {
            if (elements[i]->getImageOutput()->getId() == _nID) {
                
                patches.erase(_nID);
                delete elements[i];
                elements.erase(elements.begin() + i);
                i = elements.size();
                
                //Delete links and input Dependences
                //
                for(map<int,ofxPatch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
                    for (int j = it->second->outPut.size()-1; j >= 0 ; j--){
                        if ( it->second->outPut[j].toId == _nID){
                            it->second->outPut.erase( it->second->outPut.begin() + j );
                        }
                    }
                    ((ImageOutput*)it->second)->removeInput(_nID);
                }
            }
            i++;
        }
    }
}

//------------------------------------------------------------------
string NodeViewer::getName(){
    return name;
}

//------------------------------------------------------------------
map<int, vector <string> > NodeViewer::getAttributesClicked() {
    
    map<int, vector <string> > result;
    vector<string> partialResult;
    for (int i = 0; i < elements.size(); ++i) {
        partialResult = elements[i]->getImageOutput()->getAttributesClicked();
        if (partialResult.size() > 0) {
            result[elements[i]->getImageOutput()->getId()] = partialResult;
        }
    }
    return result;
}

//------------------------------------------------------------------
void NodeViewer::setupGuiPositions(){
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->setupGuiPositions();
    }
}

//------------------------------------------------------------------
void NodeViewer::deleteEverything() {

    for (int i=0; i<elements.size(); ++i) {
        delete elements[i];
        elements[i] = NULL;
    }
    elements.clear();
    patches.clear();
}

//------------------------------------------------------------------
bool NodeViewer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    bool found = false;
    int totalNodes = 0;
    int totalViews = 0;
    
    totalViews = XML.getNumTags("NODE_VIEW");
    
    for (int i = 0; i <= totalViews; i++){
        
        if ( XML.getAttribute("NODE_VIEW", "name", "", i) == name){
            
            saved = XML.pushTag("NODE_VIEW");
            
            if (saved){
                totalNodes = XML.getNumTags("NODE");
                
                for (int e = 0; e < elements.size(); e++) {
                    
                    found = false;
                    
                    for (int n = 0; n <= totalNodes; n++) {
                        if (XML.getAttribute("NODE", "id", -1, n) == elements[e]->getImageOutput()->getId()) {
                            XML.setAttribute("NODE", "name", elements[e]->getImageOutput()->getName(), n);
                            found = true;
                        }
                    }
                    
                    if (!found) {
                        int lastPlace = XML.addTag("NODE");
                        
                        XML.addAttribute("NODE", "id", elements[e]->getImageOutput()->getId(), lastPlace);
                        XML.addAttribute("NODE", "name", elements[e]->getImageOutput()->getName(), lastPlace);
                    }
                }
            }
            
            break;
        }
        else if (i >= totalNodes-1) {
            
            int lastPlace = XML.addTag("NODE_VIEW");
            
            XML.addAttribute("NODE_VIEW", "name", name, lastPlace);
            saved = XML.pushTag("NODE_VIEW");
            
            if (saved){
                for (int e = 0; e < elements.size(); e++) {
                    
                    lastPlace = XML.addTag("NODE");
                    XML.addAttribute("NODE", "id", elements[e]->getImageOutput()->getId(), lastPlace);
                    XML.addAttribute("NODE", "name", elements[e]->getImageOutput()->getName(), lastPlace);
                }
            }
        }
        
        XML.popTag();
    }
    
    return saved;
}