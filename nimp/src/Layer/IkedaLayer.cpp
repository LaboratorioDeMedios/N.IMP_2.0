 /*
 *  IkedaLayer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "IkedaLayer.h"

using namespace cv;
using namespace ofxCv;


//------------------------------------------------------------------
IkedaLayer::IkedaLayer(string name_, int id_, bool isCanny_,bool isThreshold_, bool isColumns_, bool isInvert_):VisualLayer(name_, "Ikeda", id_) {
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(isCanny.setup("Canny", isCanny_));
    gui.add(pCannyX.setup("CannyX", 12,0,255));
    gui.add(pCannyY.setup("CannyY", 12,0,255));
    gui.add(isColumns.setup("Columns", isColumns_));
    gui.add(pNColumns.setup("n Columns", 8,1,14));
    gui.add(isThreshold.setup("Threshold", isThreshold_));
    gui.add(pThreshold.setup("Amount", 12,0,255));
    gui.add(isInvert.setup("Invert", isInvert_));
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Canny");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("CannyX");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("CannyY");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Columns");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("n Columns");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Threshold");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Amount");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Invert");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    drawNoInputs = true;
}

IkedaLayer::~IkedaLayer(){
    ofxBaseGui* baseGui;
    baseGui = gui.find("Canny");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Enabled");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("CannyX");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("CannyY");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Columns");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("n Columns");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Threshold");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Amount");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Invert");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
}

//------------------------------------------------------------------
void IkedaLayer::setup() {

    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        width  = input[0]->getWidth();
        height = input[0]->getHeight();
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        imgAux.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        img.setUseTexture(true);
        imgAux.setUseTexture(true);
        
    }
}

//------------------------------------------------------------------
void IkedaLayer::update(){
    
    if(input.size()) {
        
        input[0]->getTextureReference().readToPixels(buff);
        
        //process pipeline
        //canny edge detection
        if(isEnabled){
            
            imgAux.setFromPixels(buff);
            
            //to gray image
            copyGray(imgAux, img);
//            convertColor(imgAux, img, CV_RGBA2GRAY);
            
            if (isCanny) {
                // canny edge detection
                Canny(img, img, pCannyX, pCannyY, 3);
            }

            //reduce columns
            if (isColumns) {
                int n = img.getWidth() / (pNColumns);
                int totalPixels = img.getWidth()*img.getHeight();
                unsigned char* pixels = img.getPixels();
                
                for (int i = 0; i < totalPixels; i+=n){
                    int val = 0;
                    for (int j=0; j<n; j++) {
                        val += pixels[j+i];
                    }
                    val /= n;
                    for (int j=0; j<n; j++) {
                        pixels[j+i] = val;
                    }
                }
            }
            
            //threshold
            if (isThreshold) {
                threshold(img, pThreshold);
            }
            
            //invert
            if (isInvert) {
                invert(img);
            }
            img.setImageType(OF_IMAGE_COLOR_ALPHA);
        }
        else {
            img.setFromPixels(buff);
        }
        img.update();
    }
}

//------------------------------------------------------------------
void IkedaLayer::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("Enabled")==0){
        this->isEnabled = inputParam->intVal;
    }
    else if(inputParam->name.compare("Canny")==0){
        this->isCanny = inputParam->intVal;
    }
    else if(inputParam->name.compare("Threshold")==0){
        this->isThreshold = inputParam->intVal;
    }
    else if(inputParam->name.compare("Columns")==0){
        this->isColumns = inputParam->intVal;
    }
    else if(inputParam->name.compare("Invert")==0){
        this->isInvert = inputParam->intVal;
    }
    else if(inputParam->name.compare("CannyX")==0){
        this->pCannyX = inputParam->intVal;
    }
    else if(inputParam->name.compare("CannyY")==0){
        this->pCannyY = inputParam->intVal;
    }
    else if(inputParam->name.compare("n Columns")==0){
        this->pNColumns = inputParam->intVal;
    }
    else if(inputParam->name.compare("Amount")==0){
        this->pThreshold = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float IkedaLayer::getMidiMin(string param_){
    
    if(param_.compare("Enabled")==0){
        return 0;
    }else if(param_.compare("Canny")==0){
        return 0;
    }else if(param_.compare("Threshold")==0){
        return 0;
    }else if(param_.compare("Columns")==0){
        return 0;
    }else if(param_.compare("Invert")==0){
        return 0;
    }else if(param_.compare("CannyX")==0){
        return 0;
    }else if(param_.compare("CannyY")==0){
        return 0;
    }else if(param_.compare("n Columns")==0){
        return 1;
    }else if(param_.compare("Amount")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float IkedaLayer::getMidiMax(string param_){
    
    if(param_.compare("Enabled")==0){
        return 1;
    }else if(param_.compare("Canny")==0){
        return 1;
    }else if(param_.compare("Threshold")==0){
        return 1;
    }else if(param_.compare("Columns")==0){
        return 1;
    }else if(param_.compare("Invert")==0){
        return 1;
    }else if(param_.compare("CannyX")==0){
        return 255;
    }else if(param_.compare("CannyY")==0){
        return 255;
    }else if(param_.compare("n Columns")==0){
        return 14;
    }else if(param_.compare("Amount")==0){
        return 255;
    }
}

//------------------------------------------------------------------
ofTexture* IkedaLayer::getTexture(){
    
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &img.getTextureReference();
}

//------------------------------------------------------------------
void IkedaLayer::_showHelp(){
    string name = "Node type name: Ikeda";
    string description = "Description: Apply different grayscale and canny transformations to the current input";
    string use = "Use: Apply different grayscale and canny transformations to the current input";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}


//------------------------------------------------------------------
bool IkedaLayer::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    isCanny     = ofToBool(XML.getAttribute("NODE","isCanny","true",nTag_));
    isThreshold = ofToBool(XML.getAttribute("NODE","isThreshold","true",nTag_));
    isColumns   = ofToBool(XML.getAttribute("NODE","isColumns","true",nTag_));
    isInvert    = ofToBool(XML.getAttribute("NODE","isInvert","true",nTag_));
    pNColumns   = ofToInt(XML.getAttribute("NODE","pNColumns","4",nTag_));
    pCannyX     = ofToInt(XML.getAttribute("NODE","pCannyX","12",nTag_));
    pCannyY     = ofToInt(XML.getAttribute("NODE","pCannyY","12",nTag_));
    pThreshold  = ofToInt(XML.getAttribute("NODE","pThreshold","12",nTag_));
    
    nId         = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    isEnabled   = XML.getAttribute("NODE", "enabled", true, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool IkedaLayer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "IKEDA", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    if (input.size()) XML.addAttribute("NODE", "inputSource", input[0]->getId(), lastPlace);
    
    XML.addAttribute("NODE", "isCanny", isCanny, lastPlace);
    XML.addAttribute("NODE", "isThreshold", isThreshold, lastPlace);
    XML.addAttribute("NODE", "isColumns", isColumns, lastPlace);
    XML.addAttribute("NODE", "isInvert", isInvert, lastPlace);
    XML.addAttribute("NODE", "pNColumns", pNColumns, lastPlace);
    XML.addAttribute("NODE", "pCannyX", pCannyX, lastPlace);
    XML.addAttribute("NODE", "pCannyY", pCannyY, lastPlace);
    XML.addAttribute("NODE", "pThreshold", pThreshold, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
}

//------------------------------------------------------------------
bool IkedaLayer::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {

    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "IKEDA", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    if (input.size() && newIdsMap[input[0]->getId()])
        XML.addAttribute("NODE", "inputSource", newIdsMap[input[0]->getId()], lastPlace);
    
    XML.addAttribute("NODE", "isCanny", isCanny, lastPlace);
    XML.addAttribute("NODE", "isThreshold", isThreshold, lastPlace);
    XML.addAttribute("NODE", "isColumns", isColumns, lastPlace);
    XML.addAttribute("NODE", "isInvert", isInvert, lastPlace);
    XML.addAttribute("NODE", "pNColumns", pNColumns, lastPlace);
    XML.addAttribute("NODE", "pCannyX", pCannyX, lastPlace);
    XML.addAttribute("NODE", "pCannyY", pCannyY, lastPlace);
    XML.addAttribute("NODE", "pThreshold", pThreshold, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();
    }
    
    return saved;
}