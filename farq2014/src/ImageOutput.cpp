//
//  ImageOutput.cpp
//  ofApp
//
//  Created by Brian Eschrich on 09/12/14.
//
//

#include "ImageOutput.h"
#include "ImageAndVideoInputList.h"

ImageOutput::ImageOutput(string name_, string typeName_, int id_, int maxInputs_, int width_, int height_) : ofxPatch(){
    nId         = id_;
    name        = name_;
    typeName    = typeName_;
    width       = width_;
    height      = height_;

    maxInputs   = maxInputs_;
    panel.setup();
    panel.add(gui.setup(typeName));
    gui.add(patchName.setup("Name", name, 100, 20));
    patchName.addListener(this, &ImageOutput::editName);
    panel.setDraggable(false);
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    if (typeName_ != "OSC Receiver") {
        ofAddListener(gui.addOrRemoveOSCInputGui , this, &ImageOutput::addOrRemoveOSCInput);
    }
    
    title->setTitle( name );
};

ImageOutput::~ImageOutput(){
    patchName.removeListener(this, &ImageOutput::editName);
    if (typeName != "OSC Receiver") {
        ofRemoveListener(gui.addOrRemoveOSCInputGui , this, &ImageOutput::addOrRemoveOSCInput);
    }
}

//------------------------------------------------------------------
string ImageOutput::getName(){
    return name;
}

//------------------------------------------------------------------
void ImageOutput::editName(string& name_) {
    
    name = name_;
    title->setTitle( name_ );
}

//------------------------------------------------------------------
string ImageOutput::getTypeName(){
    return typeName;
}

//------------------------------------------------------------------
ofxComposer* ImageOutput::getNodeViewerIBelong() {
    return viewer;
}

//------------------------------------------------------------------
void ImageOutput::setNodeViewerIBelong(ofxComposer* viewer_) {
    viewer = viewer_;
}

//------------------------------------------------------------------
bool ImageOutput::addInput(ofxPatch* layer_){
    if (input.size()<maxInputs) {
        for(int i=0; i<input.size(); i++) {
            if(input.at(i)->getId() == layer_->getId()){
                ConsoleLog::getInstance()->pushError("Input already exists");
                return false;
            }
        }
        input.push_back((ImageOutput*)layer_);
        
        if (input.size() == 1) {
            resetSizeBasedOnInput(input[0]);
        }
        inputAdded((ImageOutput*)layer_);
    }
}

//------------------------------------------------------------------
void ImageOutput::addInputIdentifier(int inputId_){
    if (inputIdentifiers.size() < maxInputs) {
        inputIdentifiers.push_back(inputId_);
    }
}

//------------------------------------------------------------------
bool ImageOutput::findAndAssignInputs(map<int,ImageOutput*> &nodeCollection){
    std::map<int,ImageOutput*>::iterator it;
    
    ImageOutput* iO = NULL;
    
    bool error = false;

    for(int i = 0 ;i < inputIdentifiers.size(); i++){
        it = nodeCollection.find(inputIdentifiers[i]);
        
        if(it!=nodeCollection.end()){
            iO = it->second;
            addInput(iO);
        }
        else{
            error = true;
            break;
        }
    }
    return error;

}

//------------------------------------------------------------------
void ImageOutput::removeInput(int inputId_){
    
    for(std::vector<ImageOutput*>::iterator it = input.begin(); it != input.end(); it++ ){
        
        if((*it)->getId() == inputId_){
            input.erase(it);
            input[input.size()] = NULL;
            
            if (input.size() == 0) {
                drawNoInputs = true;
                resetSizeToNoInputs();
            }
            else {
                resetSizeBasedOnInput(input[0]);
            }
            
            inputRemoved(inputId_);
            break;
        }
    }
}

//------------------------------------------------------------------
vector<ImageOutput*> ImageOutput::getInputs(){
    return input;
}

//------------------------------------------------------------------
vector<string> ImageOutput::getInputNames(){
    vector<string> vout;
    for (int i=0; i<input.size(); ++i) {
        vout.push_back(input[i]->getName());
    }
    return vout;
}

//------------------------------------------------------------------
bool ImageOutput::aceptsMoreInputs() {
    
    if (input.size() < maxInputs) {
        return true;
    }
    else {
        ConsoleLog::getInstance()->pushError("No more inputs available for this node. Remove any existing connnection and try again.");
        return false;
    }
}


//------------------------------------------------------------------
void ImageOutput::addOrRemoveOSCInput(ofxOSCGuiEvent &e_) {
    e_.nodeId = nId;
    
    ofNotifyEvent(editOSCInputs, e_);
}

//------------------------------------------------------------------
void ImageOutput::drawGui(){
    panel.draw();
}

//------------------------------------------------------------------
void ImageOutput::drawGui(int x, int y){
    panel.setPosition(x, y);
    panel.draw();
}

//------------------------------------------------------------------
void ImageOutput::setGui(int x,int y, int width){
    panel.setPosition(x,y);
    panel.setWidthElements(width);
};

//------------------------------------------------------------------
void ImageOutput::resetProcessedFlag(){
    isProcessed = false;
}

//------------------------------------------------------------------
void ImageOutput::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
}

//------------------------------------------------------------------
vector<string> ImageOutput::getAttributesClicked() {
    
    return panel.getAttributesClicked();
}

//------------------------------------------------------------------
bool ImageOutput::setAttributesForAudioIn(string name_, bool left, int nodeID_) {
    
    ofxBaseGui* control = gui.getControl(name_);
    if (control) {
        left ? control->setSelectedForLeftAudio(true, nodeID_, name_) : control->setSelectedForRightAudio(true, nodeID_, name_);
    }
    else {
        left ? panel.setSelectedForLeftAudio(true, nodeID_, name_) : panel.setSelectedForRightAudio(true, nodeID_, name_);
    }
}

//------------------------------------------------------------------
bool ImageOutput::setAttributesForOSC(string name_, int nodeID_) {
    
    ofxBaseGui* control = gui.getControl(name_);
    if (control) {
        control->setSelectedForOSC(true, nodeID_, name_);
    }
    else {
        panel.setSelectedForOSC(true, nodeID_, name_);
    }
}

//------------------------------------------------------------------
bool ImageOutput::setAttributesForMIDI(string name_, int control_) {
    
    ofxBaseGui* control = gui.getControl(name_);
    if (control) {
        control->setSelectedForMIDI(true, control_, name_);
    }
    else {
        panel.setSelectedForMIDI(true, control_, name_);
    }
}


//------------------------------------------------------------------
bool ImageOutput::setMidiControlActive(int control_) {
    
        panel.setMidiControlActive(control_);
}

//------------------------------------------------------------------
bool ImageOutput::resetMidiControlActive(int control_) {
    
    panel.resetMidiControlActive(control_);
}
