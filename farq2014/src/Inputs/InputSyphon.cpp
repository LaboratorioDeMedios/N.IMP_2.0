#include "InputSyphon.h"
#include "ConsoleLog.h"
#include "SyphonClientHandler.h"

InputSyphon::InputSyphon(ofxSyphonServerDirectory* serverDir, string name, int id_) : InputSource(name, "Syphon Client", id_){
    dirIdx = 0;
    dir = serverDir;
    client.setup();
    
    drawNoInputs = true;
    serverDown = false;
    fbo.allocate(width, height);
    
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
    serverName = "No server selected";
    
    // inspector setup
    gui.add(serverSelectedName.set("", serverName));
    gui.add(nextServer.setup(">> next server"));
    gui.add(prevServer.setup("<< previous server"));
    gui.setWidthElements(INSPECTOR_WIDTH);
    nextServer.addListener(this, &InputSyphon::changeToNextServer);
    prevServer.addListener(this, &InputSyphon::changeToPrevServer);
    
    serverDownImg.loadImage("assets/syphon_server_down.png");
    
}

InputSyphon::~InputSyphon(){
    SyphonClientHandler::getInstance()->removeClient(this);
}

//------------------------------------------------------------------
void InputSyphon::setup() {
    
}

//------------------------------------------------------------------
void InputSyphon::update() {
    if(!serverDown){
        if(dir->size() > 0){
            if(dir->isValidIndex(dirIdx)){
                fbo.begin();
                ofClear(255,255,255, 0);
                client.draw(0, 0);
                fbo.end();
            }
        } else {
            if (!drawNoInputs) {
                resetSizeToNoInputs();
                serverName = "No server selected";
                serverSelectedName.set("", serverName);
            }
            drawNoInputs = true;
        }
    }
}

//------------------------------------------------------------------
ofImage* InputSyphon::getImage(){
    
    if(serverDown){
        return &serverDownImg;
    }
    
    if (drawNoInputs)
        return &noInputsImg;
    else {
        fbo.readToPixels(buff);
        img.setFromPixels(buff);
        return &img;
    }
}

//------------------------------------------------------------------
ofTexture* InputSyphon::getTexture(){
    
    if(serverDown){
        return &serverDownImg.getTextureReference();
    }
    
    if (drawNoInputs){
        return &noInputsImg.getTextureReference();
    } else {
        return &fbo.getTextureReference();
    }
}

//------------------------------------------------------------------
void InputSyphon::changeToNextServer() {
    serverDown = false;
    if(dir->size() > 0){
        drawNoInputs = false;
        dirIdx++;
        if(dirIdx > dir->size() - 1)
            dirIdx = 0;
        
        client.set(dir->getDescription(dirIdx));
        this->serverName = client.getServerName();
        this->appName = client.getApplicationName();
        
        if(serverName == ""){
            serverName = "null";
        }
        if(appName == ""){
            appName = "null";
        }
        
        serverSelectedName.set("", serverName);
        
        width  = (textureCorners[1].x - textureCorners[0].x)/SCALE_RATIO;
        height = (width*client.getHeight())/client.getWidth();
        
        resetSize();
    }
    else {
        drawNoInputs = true;
        resetSizeToNoInputs();
    }
}

//------------------------------------------------------------------
void InputSyphon::changeToPrevServer() {
    serverDown = false;
    if(dir->size() > 0){
        drawNoInputs = false;
        dirIdx--;
        if(dirIdx < 0)
            dirIdx = dir->size() - 1;
        
        client.set(dir->getDescription(dirIdx));
        this->serverName = client.getServerName();
        this->appName = client.getApplicationName();
        
        if(serverName == ""){
            serverName = "null";
        }
        if(appName == ""){
            appName = "null";
        }
        
        serverSelectedName.set("", serverName);
        
        width  = (textureCorners[1].x - textureCorners[0].x)/SCALE_RATIO;
        height = (width*client.getHeight())/client.getWidth();
        
        resetSize();
    }
    else {
        drawNoInputs = true;
        resetSizeToNoInputs();
    }
}

//------------------------------------------------------------------
void InputSyphon::serverRetired(string serverName, string appName) {
    if(this->serverName == serverName && this->appName == appName){
        serverDown = true;
        resetSizeToNoInputs();
    }
}

//------------------------------------------------------------------
bool InputSyphon::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    bool loaded = false;
    nId        = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    serverName = XML.getAttribute("NODE", "serverName", "", nTag_);
    appName    = XML.getAttribute("NODE", "appName", "", nTag_);
    
    XML.pushTag("NODE", nTag_);

    bool serverFound = false;
    int j = dir->size();
    int i = 0;
    while(i < j && !serverFound){
        client.set(dir->getDescription(i));
        if(client.getServerName() == serverName && client.getApplicationName() == appName){
            serverFound = true;
            dirIdx = i;
            break;
        }
        i++;
    }
    
    serverSelectedName.set("", serverName);
    
    if (!serverFound) {
        serverDown = true;
        ConsoleLog::getInstance()->pushMessage("Server " + serverName + " is down.");
    }
    
    drawNoInputs = false;
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool InputSyphon::saveSettings(ofxXmlSettings &XML) {
    bool saved = false;
    
    // Search for the patch ID to update information
    // If the patch ID doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of nodes of the same type ...
    //
    int totalNodes = XML.getNumTags("NODE");
    
    // ... and search for the right id for loading
    //
    for (int i = 0; i <= totalNodes; i++){
        
        
        // Once it found the right surface that match the id ...
        //
        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
            
            XML.setAttribute("NODE", "name", name, i);
            XML.setAttribute("NODE", "serverName", serverName, i);
            XML.setAttribute("NODE", "appName", appName, i);
            XML.pushTag("NODE", i);
            
            ofxPatch::saveSettings(XML, false, i);
            
            XML.popTag();
            
            break;
        }
        
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            
            XML.addAttribute("NODE", "id", nId, lastPlace);
            XML.addAttribute("NODE", "name", name, lastPlace);
            XML.addAttribute("NODE", "type", "SYPHON_CLIENT", lastPlace);
            XML.addAttribute("NODE", "serverName", serverName, lastPlace);
            XML.addAttribute("NODE", "appName", appName, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }

    return saved;
    
}

//------------------------------------------------------------------
bool InputSyphon::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {

    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SYPHON_CLIENT", lastPlace);
    XML.addAttribute("NODE", "serverName", serverName, lastPlace);
    XML.addAttribute("NODE", "appName", appName, lastPlace);
    
    if (XML.pushTag("NODE", lastPlace)){
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        
        XML.popTag();
    }
    
    return saved;
}

//------------------------------------------------------------------
void InputSyphon::updateParameter(Param* inputParam){ }

//------------------------------------------------------------------
float InputSyphon::getMidiMin(string param_){ }

//------------------------------------------------------------------
float InputSyphon::getMidiMax(string param_){ }