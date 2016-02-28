//
//  AudioIn.cpp
//  nimp
//
//  Created by Mili Garicoits on 2/13/16.
//
//

#include "AudioIn.h"

AudioIn::AudioIn(ofxUISuperCanvas* &gui_, float* &inputBuffer_, string type_, string name_, int id_) : InputSource(name_, type_, id_){
    
    inputBuffer = inputBuffer_;
    isAudio     = true;
    width       = 500;
    height      = 250;
    
    waveform = new ofxUIWaveform(0, 0, 200, 100, inputBuffer_, BUFFER_SIZE, -1.5, 1.5, "FFT");
    
//    gui.add(selectChannel.set("Channel",0,0,1));
    
    if (type_ == "Audio In - Left Channel") {
        audioInType = LEFT;
        gui.add(editFFTInputs.set("Edit Left FFT Inputs", false));
    }
    else {
        audioInType = RIGHT;
        gui.add(editFFTInputs.set("Edit Right FFT Inputs", false));
    }
    
//    selectChannel.addListener(this, &AudioIn::editChannel);
    editFFTInputs.addListener(this, &AudioIn::editInputs);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');    
}

//------------------------------------------------------------------
void AudioIn::setup() {
    waveform->ofNode::setParent(*this->getParent());
    waveform->setNoDraw(true);
    waveform->setDraggable(true);
//    waveform->setColorBack(ofxUIColor(0, 0, 0, 210));
    
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::update() {
    

}

//------------------------------------------------------------------
void AudioIn::customDraw(){
    
    if ( bEditMode || bVisible ) {
        
        ofxPatch::customDraw();
        
        waveform->drawBack();
        waveform->drawFill();
    }
    
}

//------------------------------------------------------------------
void AudioIn::_mouseDragged(ofMouseEventArgs &e){
    
    ofxPatch::_mouseDragged(e);
    
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::editInputs(bool& g){
    
    ofNotifyEvent(editAudioIn, g);
}

//------------------------------------------------------------------
//void AudioIn::editChannel(int& c){
//    
//    AudioInEvent e;
//    e.nodeId = nId;
//    e.channel = c;
//    ofNotifyEvent(editAudioInChannel, e);
//}

//------------------------------------------------------------------
void AudioIn::setChannel(int c){
    
    selectChannel = c;
   
//    AudioInEvent e;
//    e.nodeId = nId;
//    e.channel = c;
//    ofNotifyEvent(editAudioInChannel, e);
}

//------------------------------------------------------------------
void AudioIn::setWaveFormPosition() {
    
    ofxUIRectangle* r = waveform->getRect();
    
    r->setX(textureCorners[0].x);
    r->setY(textureCorners[0].y);
    
    r->setWidth(textureCorners[1].x - textureCorners[0].x);
    r->setHeight(textureCorners[2].y - textureCorners[0].y);
    
    waveform->setScale(r->getHeight()*.5);
    waveform->setInc(r->getWidth()/(BUFFER_SIZE-1.0));
}

//------------------------------------------------------------------
void AudioIn::resetSize(int _width, int _height) {
    
    ofxPatch::resetSize(_width, _height);
    setWaveFormPosition();
}

//------------------------------------------------------------------
bool AudioIn::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = false;
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    selectChannel   = XML.getAttribute("NODE", "channel", 1, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type            = XML.getValue("type","none");
    bVisible        = XML.getValue("visible", true);
    filePath        = XML.getValue("path", "none" );
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool AudioIn::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.addAttribute("NODE", "channel", selectChannel, i);
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
            XML.addAttribute("NODE", "channel", selectChannel, lastPlace);
            audioInType == LEFT ? XML.addAttribute("NODE", "type", "LEFT_AUDIO_IN", lastPlace) : XML.addAttribute("NODE", "type", "RIGHT_AUDIO_IN", lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}

//------------------------------------------------------------------
//bool AudioIn::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
//    
//    bool saved = false;
//    int lastPlace = XML.addTag("NODE");
//    
//    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
//    XML.addAttribute("NODE", "name", name, lastPlace);
//    typeName == "Audio In - Left" ? XML.addAttribute("NODE", "type", "LEFT_FFT", lastPlace) : XML.addAttribute("NODE", "type", "RIGHT_FFT", lastPlace);
//    
//    if (XML.pushTag("NODE", lastPlace)){
//        
//        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
//        
//        XML.popTag();
//    }
//    
//    return saved;
//}
