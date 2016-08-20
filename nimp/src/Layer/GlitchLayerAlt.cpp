//
//  GlitchLayerAlt.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayerAlt.h"

// DEPRECATED - Processing is too slow.

//------------------------------------------------------------------
GlitchLayerAlt::GlitchLayerAlt(string name_, int id_):VisualLayer(name_, "Glitch Layer Alt", id_){;
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(dq.setup("DQ", 20,0,255));
    gui.add(qn.setup("QN", 40,0,255));
    gui.add(dht.setup("DHT", 80,0,255));
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("DQ");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("QN");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("DHT");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);

    drawNoInputs = true;
}

GlitchLayerAlt::~GlitchLayerAlt(){
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("DQ");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("QN");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("DHT");
    if (baseGui) ofRemoveListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
}

//------------------------------------------------------------------
void GlitchLayerAlt::setup() {

    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        glitcher.setup(dq, qn, dht);
        
        width  = input[0]->getWidth();
        height = input[0]->getHeight();
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        img.setUseTexture(true);
    }
}

//------------------------------------------------------------------
void GlitchLayerAlt::update(){
    //process pipeline
    
    if(input.size()) {
        
        input[0]->getTextureReference().readToPixels(buff);
        
        if(isEnabled){
           
            //setting glitcher values according to GUI
            glitcher.setDataGlitchness(dq);
            glitcher.setQNGlitchness(qn);
            glitcher.setDHTGlitchness(dht);
            
            glitcher.setPixels(buff);
            glitcher.glitch();
            img.setFromPixels(glitcher.getImage().getPixelsRef());
        }
        else {
            img.setFromPixels(buff);
        }
        
        img.update();
    }
}

//------------------------------------------------------------------
void GlitchLayerAlt::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("Enabled")==0){
        this->isEnabled = inputParam->intVal;
    }
    else if(inputParam->name.compare("DQ")==0){
        this->dq = inputParam->intVal;
    }
    else if(inputParam->name.compare("QN")==0){
        this->qn = inputParam->intVal;
    }
    else if(inputParam->name.compare("DHT")==0){
        this->dht = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float GlitchLayerAlt::getMidiMin(string param_){
    
    if(param_.compare("Enabled")==0){
        return 0;
    }else if(param_.compare("DQ")==0){
        return 0;
    }else if(param_.compare("QN")==0){
        return 0;
    }else if(param_.compare("DHT")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float GlitchLayerAlt::getMidiMax(string param_){
    
    if(param_.compare("Enabled")==0){
        return 1;
    }else if(param_.compare("DQ")==0){
        return 255;
    }else if(param_.compare("QN")==0){
        return 255;
    }else if(param_.compare("DHT")==0){
        return 255;
    }
}

//------------------------------------------------------------------
ofTexture* GlitchLayerAlt::getTexture(){
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &img.getTextureReference();
}

//------------------------------------------------------------------
void GlitchLayerAlt::_showHelp(){
    string name = "Node type name: Glitch 2";
    string description = "Description: Apply different transformations to the current input";
    string use = "Use: Apply different transformations to the current input";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool GlitchLayerAlt::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    isEnabled = XML.getAttribute("NODE", "enabled", true, nTag_);
    dq        = ofToInt(XML.getAttribute("NODE","dq","20",nTag_));
    qn        = ofToInt(XML.getAttribute("NODE","qn","40",nTag_));
    dht       = ofToInt(XML.getAttribute("NODE","dht","80",nTag_));
    
    nId = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    
    XML.pushTag("NODE", nTag_);
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool GlitchLayerAlt::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "GLITCH_2", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    if (input.size()) XML.addAttribute("NODE", "inputSource", input[0]->getId(), lastPlace);
    
    XML.addAttribute("NODE","dq", dq, lastPlace);
    XML.addAttribute("NODE","qn", qn, lastPlace);
    XML.addAttribute("NODE","dht", dht, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool GlitchLayerAlt::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "GLITCH_2", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    if (input.size() && newIdsMap[input[0]->getId()])
        XML.addAttribute("NODE", "inputSource", newIdsMap[input[0]->getId()], lastPlace);
    
    XML.addAttribute("NODE","dq", dq, lastPlace);
    XML.addAttribute("NODE","qn", qn, lastPlace);
    XML.addAttribute("NODE","dht", dht, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();
    }
    
    return saved;
    
}

