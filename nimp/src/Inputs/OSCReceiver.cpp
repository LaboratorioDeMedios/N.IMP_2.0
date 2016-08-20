//
//  OSCReceiver.c
//  nimp
//
//  Created by Mili Garicoits on 3/8/16.
//
//

#include "OSCReceiver.h"

OSCReceiver::OSCReceiver(string name_, int id_) : InputSource(name_, "OSC Receiver", id_){
    
    isOSC = true;
    disabledEdit = false;
    oscReceiverImg.loadImage("assets/osc_receiver.png");
    
    oldAddress  = "/";
    port        = 6666;
    oldPort     = 6666;
    min         = 0;
    max         = 127;

    address.set("Address", "/");
    oscPortNumber.set("Port", "6666");
    oscMinNumber.set("Min Value", "0");
    oscMaxNumber.set("Max Value", "127");
    gui.add(oscPort.setup("Port", oscPortNumber, 100, 20));
    gui.add(oscAddress.setup("Address", address, 100, 20));
    gui.add(oscMin.setup("Min Value", oscMinNumber, 100, 20));
    gui.add(oscMax.setup("Max Value", oscMaxNumber, 100, 20));
    gui.add(editOSC.set("Edit OSC Inputs", false));
    
    paramsGroup.setName("Params");
    gui.add(paramsGroup);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');
}

OSCReceiver::~OSCReceiver(){
    oscPort.removeListener(this, &OSCReceiver::editPort);
    oscAddress.removeListener(this, &OSCReceiver::editAddress);
    oscMax.removeListener(this, &OSCReceiver::editMax);
    oscMin.removeListener(this, &OSCReceiver::editMin);
    editOSC.removeListener(this, &OSCReceiver::editInputs);
    ofRemoveListener(ofEvents().mouseReleased, this, &OSCReceiver::_mouseReleased, PATCH_EVENT_PRIORITY);
}

//------------------------------------------------------------------
void OSCReceiver::setup() {
    
    if (oldAddress == "/") {
        oldAddress = oldAddress + ofToString(nId);
        address = oldAddress;
        oscAddress.setup("Address", address, 100, 20);
        gui.setWidthElements(INSPECTOR_WIDTH);
    }
    
    oscPort.addListener(this, &OSCReceiver::editPort);
    oscAddress.addListener(this, &OSCReceiver::editAddress);
    oscMax.addListener(this, &OSCReceiver::editMax);
    oscMin.addListener(this, &OSCReceiver::editMin);
    editOSC.addListener(this, &OSCReceiver::editInputs);
    ofAddListener(ofEvents().mouseReleased, this, &OSCReceiver::_mouseReleased, PATCH_EVENT_PRIORITY);

}

//------------------------------------------------------------------
void OSCReceiver::update() {
    
}

//------------------------------------------------------------------
void OSCReceiver::customDraw(){
    
    ofxPatch::customDraw();
}

//------------------------------------------------------------------
ofTexture* OSCReceiver::getTexture(){
    return &oscReceiverImg.getTextureReference();
}

//------------------------------------------------------------------
void OSCReceiver::editPort(string& p){
    port = ofToInt(p.substr(5));
}

//------------------------------------------------------------------
void OSCReceiver::editAddress(string& a){
    address = a.substr(9);
}

//------------------------------------------------------------------
void OSCReceiver::editMin(string& a){
    
    min = ofToInt(a.substr(11));
    
    OSCEvent ev;
    ev.nodeId = nId;
    ev.port = port;
    ev.address = oldAddress;
    ev.max = max;
    ev.min = min;
    ofNotifyEvent(editOSCMinMaxValues, ev);
}

//------------------------------------------------------------------
void OSCReceiver::editMax(string& a){
    
    max = ofToInt(a.substr(11));
    
    OSCEvent ev;
    ev.nodeId = nId;
    ev.port = port;
    ev.address = oldAddress;
    ev.max = max;
    ev.min = min;
    ofNotifyEvent(editOSCMinMaxValues, ev);
}

//------------------------------------------------------------------
void OSCReceiver::editInputs(bool& b){
    
    if (!disabledEdit) {
        OSCEvent ev;
        ev.nodeId = nId;
        ev.port = port ;
        ev.address = oldAddress;
        ofNotifyEvent(editOSCInputsActive, ev);
    }
    else {
        editOSC = false;
    }
}

//------------------------------------------------------------------
void OSCReceiver::setPort(int port_){
    
    port = port_;
}

//------------------------------------------------------------------
void OSCReceiver::addParameter(int nodeId_, string paramName_){
    
    gui.remove("Params");
    
    ofParameter<string> param = paramName_;
    paramsGroup.add(param);
    
    OSCParam newParam;
    newParam.nodeId = nodeId_;
    newParam.paramName = paramName_;
    paramsLabels.push_back(newParam);
    
    gui.add(paramsGroup);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void OSCReceiver::removeParameter(int nodeId_, string paramName_){
    
    gui.remove("Params");
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        if (paramsLabels[i].nodeId == nodeId_ && (paramsLabels[i].paramName == paramName_ ||
            (paramsLabels[i].paramName.compare(0, 5, "Blend")==0 && paramName_.compare(0, 5, "Blend")==0))) {
            paramsLabels.erase(paramsLabels.begin() + i);
        }
    }
    
    paramsGroup.clear();
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        ofParameter<string> p = paramsLabels[i].paramName;
        paramsGroup.add(p);
    }
    
    gui.add(paramsGroup);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void OSCReceiver::clearParameters() {
    
    paramsGroup.clear();
    paramsLabels.clear();
}

//------------------------------------------------------------------
void OSCReceiver::removeNodeParams(int nodeId_){
    
    gui.remove("Params");
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        if (paramsLabels[i].nodeId == nodeId_) {
            paramsLabels.erase(paramsLabels.begin() + i);
            i--;
        }
    }
    
    paramsGroup.clear();
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        ofParameter<string> p = paramsLabels[i].paramName;
        paramsGroup.add(p);
    }
    
    gui.add(paramsGroup);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void OSCReceiver::_showHelp(){

    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage("Node type name: OSC Receiver");
    ConsoleLog::getInstance()->pushMessage("Description: Listens to incoming OSC messages in the configured port and address.", false);
    ConsoleLog::getInstance()->pushMessage("Use: Setup in the Inspector the port and address of the OSC message you want to start listening to,", false);
    ConsoleLog::getInstance()->pushMessage(" \t \t and also the minimun and maxumin values the incoming messages can take.", false);
    ConsoleLog::getInstance()->pushMessage("To control nodes attributes with this OSC message receiver node follow this steps:", false);
    ConsoleLog::getInstance()->pushMessage(" \t 1. Open the Inspector and click on the 'Edit OSC Inputs' checkbox.", false);
    ConsoleLog::getInstance()->pushMessage(" \t 2. Select the desired attributes in another node's Inspector to be controlled by messages received by this node.", false);
    ConsoleLog::getInstance()->pushMessage(" \t 3. Click again on the 'Edit OSC Inputs' checkbox to save the configuration.", false);
}

//------------------------------------------------------------------
bool OSCReceiver::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    nId     = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    port    = XML.getAttribute("NODE", "port", 6666, nTag_);
    min     = XML.getAttribute("NODE", "min", 0, nTag_);
    max     = XML.getAttribute("NODE", "max", 127, nTag_);
    oldAddress = XML.getAttribute("NODE", "address", "/", nTag_);
    
    address = oldAddress;
    oscPortNumber = ofToString(port);
    oscMinNumber = ofToString(min);
    oscMaxNumber = ofToString(max);
    
    oldPort = port;
    oscPort.setup("Port", oscPortNumber, 100, 20);
    oscAddress.setup("Address", address, 100, 20);
    oscMin.setup("Min Value", oscMinNumber, 100, 20);
    oscMax.setup("Max Value", oscMaxNumber, 100, 20);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    loaded = XML.pushTag("NODE", nTag_);
    
    if (loaded) {
        type            = XML.getValue("type","none");
        bVisible        = XML.getValue("visible", true);
        filePath        = XML.getValue("path", "none" );
        
        int numParamTag = XML.getNumTags("PARAM");
        if (numParamTag > 0){
            for (int v = 0; v < numParamTag; v++){
                string name_ = XML.getAttribute("PARAM","name","",v);
                int nodeID_ = XML.getAttribute("PARAM","nodeID",-1,v);
                if (name_ != "" && nodeID_ != -1) {
                    addParameter(nodeID_, name_);
                }
            }
        }

        loaded = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
        
        XML.popTag();
    }
    
    return loaded;
}

//------------------------------------------------------------------
bool OSCReceiver::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "OSC_RECEIVER", lastPlace);
    XML.addAttribute("NODE", "port", port, lastPlace);
    XML.addAttribute("NODE", "address", oldAddress, lastPlace);
    XML.addAttribute("NODE", "min", min, lastPlace);
    XML.addAttribute("NODE", "max", max, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        
        for (int v = 0; v < paramsLabels.size(); v++){
            XML.addTag("PARAM");
            XML.addAttribute("PARAM", "name", paramsLabels[v].paramName, v);
            XML.addAttribute("PARAM", "nodeID", paramsLabels[v].nodeId, v);
        }
        
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        
        XML.popTag(); // NODE
    }

    return saved;
}

//------------------------------------------------------------------
bool OSCReceiver::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    
    if (newIdsMap[nId]) {
        
        int lastPlace = XML.addTag("NODE");
        
        XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
        XML.addAttribute("NODE", "name", name, lastPlace);
        XML.addAttribute("NODE", "type", "OSC_RECEIVER", lastPlace);
        XML.addAttribute("NODE", "port", port, lastPlace);
        XML.addAttribute("NODE", "address", oldAddress, lastPlace);
        XML.addAttribute("NODE", "min", min, lastPlace);
        XML.addAttribute("NODE", "max", max, lastPlace);
        
        saved = XML.pushTag("NODE", lastPlace);
        if (saved){
            
            for (int v = 0; v < paramsLabels.size(); v++){
                
                if (newIdsMap[paramsLabels[v].nodeId]) {
                    XML.addTag("PARAM");
                    XML.addAttribute("PARAM", "name", paramsLabels[v].paramName, v);
                    XML.addAttribute("PARAM", "nodeID", paramsLabels[v].nodeId, v);
                }
            }
            
            saved = ofxPatch::saveSettings(XML, true, lastPlace);
            
            XML.popTag(); // NODE
        }
    }
    
    return saved;
}


//------------------------------------------------------------------
void OSCReceiver::_mouseReleased(ofMouseEventArgs &e){
    if(oldPort != port && !oscPort.isClicked()){
        OSCEvent ev;
        ev.nodeId = nId;
        ev.port = port;
        ev.oldPort = oldPort;
        ev.address = oldAddress;
        ev.oldAddress = oldAddress;
        ev.max = max;
        ev.min = min;
        ofNotifyEvent(editOSCPort, ev);
        oldPort = port;
    }
    
    if(oldAddress != address.get() && !oscAddress.isClicked()){
        OSCEvent ev;
        ev.nodeId = nId;
        ev.port = port;
        ev.oldPort = port;
        ev.address = address;
        ev.oldAddress = oldAddress;
        ev.max = max;
        ev.min = min;
        ofNotifyEvent(editOSCPort, ev);
        oldAddress = address;
    }
}