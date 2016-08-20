/*
 *  InputCamera.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputCamera.h"


InputCamera::InputCamera(string name, int id_) : InputSource(name, "Camera", id_){
    
    drawNoInputs = true;
    videoGrabber = new ofVideoGrabber();
    videoGrabber->setUseTexture(false);
    bool loaded = videoGrabber->initGrabber(width, height);
    
    if (loaded){
        width   = videoGrabber->getWidth();
        height  = videoGrabber->getHeight();
        drawNoInputs = false;
    }
    
    cameraIndex = 0;
    cameraName = videoGrabber->listDevices()[cameraIndex].deviceName;
    
    gui.add(cameraSelectedName.set("", cameraName));
    gui.add(nextCamera.setup(">> next camera"));
    gui.add(prevCamera.setup("<< previous camera"));
    nextCamera.addListener(this, &InputCamera::changeToNextCamera);
    prevCamera.addListener(this, &InputCamera::changeToPrevCamera);
    
    resolutionLabels.push_back("320x240");
    resolutionLabels.push_back("480x360");
    resolutionLabels.push_back("640x480");
    resolutionLabels.push_back("960x540");
    resolutionLabels.push_back("1024x576");			
    resolutionLabels.push_back("1280x720");
    resolutionLabels.push_back("1366x768");
    resolutionLabels.push_back("1920x1080");
    resolutionLabels.push_back("2560x1440");

    
    gui.add(resolutionName.set("", "Resolution: 640x480"));
    gui.add(higherResolution.setup(">> higher resolution"));
    gui.add(lowerResolution.setup("<< lower resolution"));
    higherResolution.addListener(this, &InputCamera::changeToHigherResolution);
    lowerResolution.addListener(this, &InputCamera::changeToLowerResolution);
    
    previous_resolution = 2;
    
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
InputCamera::~InputCamera() {
    nextCamera.removeListener(this, &InputCamera::changeToNextCamera);
    prevCamera.removeListener(this, &InputCamera::changeToPrevCamera);
    higherResolution.removeListener(this, &InputCamera::changeToHigherResolution);
    lowerResolution.removeListener(this, &InputCamera::changeToLowerResolution);
}

//------------------------------------------------------------------
void InputCamera::setup() {

}

//------------------------------------------------------------------
void InputCamera::update() {
    
    if (videoGrabber->isInitialized()) {
        videoGrabber->update();
    }
}

//------------------------------------------------------------------
ofTexture* InputCamera::getTexture(){
    
    if (drawNoInputs || !videoGrabber->isInitialized()) {
        return &noInputsImg.getTextureReference();
    }
    else {
        return &videoGrabber->getTextureReference();
    }
}

//------------------------------------------------------------------
void InputCamera::changeToNextCamera() {

    if(videoGrabber->listDevices().size() == 1) {
        if (drawNoInputs){
            cameraIndex = 0;
            initCamera();
        }
        else return;
    };
    
    if(videoGrabber->listDevices().size() > 0){
        
        drawNoInputs = false;
        cameraIndex++;
        if(cameraIndex > videoGrabber->listDevices().size() - 1)
            cameraIndex = 0;
        
        initCamera();
    }
    else {
        drawNoInputs = true;
        resetSizeToNoInputs();
        
        cameraName = "No Camera Available";
        cameraSelectedName.set("", cameraName);
    }
}

//------------------------------------------------------------------
void InputCamera::changeToPrevCamera() {

    if(videoGrabber->listDevices().size() == 1) {
        if (drawNoInputs){
            cameraIndex = 0;
            initCamera();
        }
        else return;
    };
    
    if(videoGrabber->listDevices().size() > 0){
        
        drawNoInputs = false;
        cameraIndex--;
        if(cameraIndex < 0)
            cameraIndex = videoGrabber->listDevices().size() - 1;
        
        initCamera();
    }
    else {
        drawNoInputs = true;
        resetSizeToNoInputs();
        
        cameraName = "No Cameras Available";
        cameraSelectedName.set("", cameraName);
    }
}

//------------------------------------------------------------------
void InputCamera::initCamera() {
    
    cameraName = videoGrabber->listDevices()[cameraIndex].deviceName;
    cameraSelectedName.set("", cameraName);
    
    videoGrabber->close();
    videoGrabber->setDeviceID(cameraIndex);
    videoGrabber->initGrabber(width, height);
}

//------------------------------------------------------------------
void InputCamera::changeToHigherResolution() {
    
    if (previous_resolution < resolutionLabels.size()-1) {
        previous_resolution++;
        editResolution(resolutionLabels[previous_resolution]);
        
//        if (resolutionLabels[previous_resolution] == "960x540
            getNodeViewerIBelong()->updateConnectionsSize(this);
    }
}

//------------------------------------------------------------------
void InputCamera::changeToLowerResolution() {
    
    if (previous_resolution > 0) {
        previous_resolution--;
        editResolution(resolutionLabels[previous_resolution]);
        
//        if (resolutionLabels[previous_resolution] == "640x480")
            getNodeViewerIBelong()->updateConnectionsSize(this);
    }
}

//------------------------------------------------------------------
void InputCamera::editResolution(string resolution_) {
    
    resolutionName.set("", "Resolution: " + resolution_);
    
    if (resolution_ == "320x240") {
        width = 320;
        height = 240;
    }
    else if (resolution_ == "480x360") {
        width = 480;
        height = 360;
    }
    else if (resolution_ == "640x480") {
        width = 640;
        height = 480;
    }
    else if (resolution_ == "960x540") {
        width = 960;
        height = 540;
    }
    else if (resolution_ == "1024x576") {
        width = 1024;
        height = 576;
    }
    else if (resolution_ == "1280x720") {
        width = 1280;
        height = 720;
    }
    else if (resolution_ == "1366x768") {
        width = 1366;
        height = 768;
    }
    else if (resolution_ == "1920x1080") {
        width = 1920;
        height = 1080;
    }
    else if (resolution_ == "2560x1440") {
        width = 2560;
        height = 1440;
    }
    
    resetSize(width, height);
    videoGrabber->close();
    videoGrabber->setDeviceID(cameraIndex);
    videoGrabber->initGrabber(width, height);
    
}

//------------------------------------------------------------------
void InputCamera::_showHelp(){
    string name = "Node type name: Camera";
    string description = "Description: Input node that captures the image from a camera.";
    string use = "Use: Through the Inspector you can switch between all cameras conected to the computer and available resolutions.";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool InputCamera::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    nId                 = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    cameraName          = XML.getAttribute("NODE", "cameraName","No Camera Available", nTag_);
    previous_resolution = XML.getAttribute("NODE", "resolution", 2, nTag_);
    width               = XML.getAttribute("NODE", "width", 640, nTag_);
    height              = XML.getAttribute("NODE", "height", 480, nTag_);
    resolutionName.set("", resolutionLabels[previous_resolution]);
    
    vector<ofVideoDevice> devices = videoGrabber->listDevices();
    bool found = false;
    int  i = 0;
    
    while (!found && i < devices.size()) {
        if(devices[i].deviceName == cameraName) {
            cameraIndex = i;
            
            videoGrabber->close();
            videoGrabber->setDeviceID(cameraIndex);
            videoGrabber->initGrabber(width, height);
            found = true;
        }
        else i++;
    }
    
    if (!found) {
        videoGrabber->close();
        drawNoInputs = true;
        ConsoleLog::getInstance()->pushMessage("Camera " + cameraName + " is not connected.");
    }
    
    XML.pushTag("NODE", nTag_);
    
    type            = XML.getValue("type","none");
    bVisible        = XML.getValue("visible", true);
    filePath        = XML.getValue("path", "none" );

    loaded = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool InputCamera::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");

    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "CAM", lastPlace);
    XML.addAttribute("NODE", "cameraName", cameraName, lastPlace);
    XML.addAttribute("NODE", "resolution", previous_resolution, lastPlace);
    XML.addAttribute("NODE", "width", width, lastPlace);
    XML.addAttribute("NODE", "height", height, lastPlace);

    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool InputCamera::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "CAM", lastPlace);
    XML.addAttribute("NODE", "cameraName", cameraName, lastPlace);
    XML.addAttribute("NODE", "resolution", previous_resolution, lastPlace);
    XML.addAttribute("NODE", "width", width, lastPlace);
    XML.addAttribute("NODE", "height", height, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();  // NODE
    }
    
    return saved;
}
