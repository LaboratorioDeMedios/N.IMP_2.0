/*
 *  VideoPlayerMac.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "VideoPlayerMac.h"

VideoPlayerMac::VideoPlayerMac(string name, int id_) : InputSource(name, "Video Player", id_){
    //there always has to exist at least one player.
    currentPlayer=0;
    
    play.addListener(this, &VideoPlayerMac::playVideo);
    stop.addListener(this, &VideoPlayerMac::stopVideo);
    
    gui.add(play.setup("Play"));
    gui.add(stop.setup("Stop"));
    gui.setWidthElements(INSPECTOR_WIDTH);

    //drawTexture = true;
}

//------------------------------------------------------------------
void VideoPlayerMac::setup() {
    
    if (players[currentPlayer]) {
        
        width  = players[currentPlayer]->getWidth();
        height = players[currentPlayer]->getHeight();
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        img.setUseTexture(true);
    }
    else {
        drawNoInputs = true;
    }
}

//------------------------------------------------------------------
void VideoPlayerMac::update() {
    if (players[currentPlayer]->isLoaded()) {
        
        players[currentPlayer]->update();
        if (players[currentPlayer]->isFrameNew()){
            img.setFromPixels(players[currentPlayer]->getPixels(), players[currentPlayer]->getWidth(), players[currentPlayer]->getHeight(), OF_IMAGE_COLOR_ALPHA);
            //img.resize(width, height);
            //tex = img.getTextureReference();
        }
    }
}

//------------------------------------------------------------------
void VideoPlayerMac::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
float VideoPlayerMac::getMidiMin(string param_){
    
}

//------------------------------------------------------------------
float VideoPlayerMac::getMidiMax(string param_){
    
}

//------------------------------------------------------------------
ofImage* VideoPlayerMac::getImage(){
    return &img;
}

//------------------------------------------------------------------
ofTexture* VideoPlayerMac::getTexture(){
    return &img.getTextureReference();
}

//------------------------------------------------------------------
void VideoPlayerMac::loadVideo(string path) {
    
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    ofQTKitPlayer* vP = new ofQTKitPlayer();
    vP->setPixelFormat(OF_PIXELS_RGBA);
    vP->loadMovie(path, decodeMode);
    //ofVideoPlayer* vP = new ofVideoPlayer();
    //vP->loadMovie(path);
    players.push_back(vP);
    playerPaths.push_back(path);
}

//------------------------------------------------------------------
void VideoPlayerMac::nextVideo(){
    //stopping current video
    players[currentPlayer]->stop();
    //incrementing to next video index
    currentPlayer += 1;
    if (currentPlayer > players.size()){
        currentPlayer = 0 ;
    }
    //playing current video
    players[currentPlayer]->play();
}

//------------------------------------------------------------------
void VideoPlayerMac::prevVideo(){
    //stopping current video
    players[currentPlayer]->stop();
    //decrementing to next video index
    currentPlayer -= 1;
    if (currentPlayer < 0){
        currentPlayer = players.size()-1 ;
    }
    //playing current video
    players[currentPlayer]->play();
}

//------------------------------------------------------------------
void VideoPlayerMac::playVideo(){
    players[currentPlayer]->play();
}

//------------------------------------------------------------------
void VideoPlayerMac::stopVideo(){
    players[currentPlayer]->stop();
}

//------------------------------------------------------------------
void VideoPlayerMac::setSpeed(float speed){
    players[currentPlayer]->setSpeed(speed);
}

//------------------------------------------------------------------
float VideoPlayerMac::getSpeed(){
    return players[currentPlayer]->getSpeed();
}

//------------------------------------------------------------------
bool VideoPlayerMac::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    path    = XML.getAttribute("NODE", "path","none", nTag_);
    nId     = XML.getAttribute("NODE", "id", -1, nTag_ + nodesCount_);
    
    XML.pushTag("NODE",nTag_);
    
    int numVideoTag = XML.getNumTags("VIDEO");
    
    if(numVideoTag>0){
        for (int v=0; v<numVideoTag; v++){
            string path = XML.getAttribute("VIDEO","path","default",v);
            loadVideo(path);
        }
    }
    else{
        loaded = false;
    }
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool VideoPlayerMac::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.setAttribute("NODE", "path", path, i);
            
            XML.pushTag("NODE", i);
            
            int numVideoTag = XML.getNumTags("VIDEO");
            
            for (int v = 0; v < playerPaths.size(); v++){
                
                if (v >= numVideoTag) {
                    XML.addTag("VIDEO");
                }
                
                XML.setAttribute("VIDEO", "path", playerPaths[v], v);
            }
            
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
            XML.addAttribute("NODE", "type", "VIDEO", lastPlace);
            XML.addAttribute("NODE", "path", path, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                for (int v = 0; v < playerPaths.size(); v++){
                    XML.addTag("VIDEO");
                    XML.addAttribute("VIDEO", "path", playerPaths[v], v);
                }
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool VideoPlayerMac::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {

    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "VIDEO", lastPlace);
    XML.addAttribute("NODE", "path", path, lastPlace);
    
    if (XML.pushTag("NODE", lastPlace)){
        
        for (int v = 0; v < playerPaths.size(); v++){
            XML.addTag("VIDEO");
            XML.addAttribute("VIDEO", "path", playerPaths[v], v);
        }
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();
    }
    
    return saved;
}