//
//  AudioAnalizer.cpp
//  nimp
//
//  Created by Mili Garicoits on 2/25/16.
//
//

#include "AudioAnalizer.h"
#include "EventHandler.h"

AudioAnalizer::AudioAnalizer(string name_, int id_) : InputSource(name_, "Audio Analizer", id_){
    
    isAudio             = true;
    isAudioAnalizer     = true;
    drawAudioAnalizer   = true;
    width               = 1700;
    height              = 850;
    
    gui.add(saturation.set("Sound Saturation",20,10,50));
    gui.add(threshold.set("Threshold",0.02,0.0001,0.4));
    threshold.addListener(this, &AudioAnalizer::editTreshold);
    saturation.addListener(this, &AudioAnalizer::editSaturation);
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');
    
    filterBank.setSoundLevelSaturation(saturation);
    filterBank.setThreshold(threshold);
    filterBank.setup(BUFFER_SIZE, 1, 128, 2, 1.0, 44100, 1.0);
    filterBank.setColor(ofColor::orange);
    
    resetSize();
}

AudioAnalizer::~AudioAnalizer(){
    threshold.removeListener(this, &AudioAnalizer::editTreshold);
    saturation.removeListener(this, &AudioAnalizer::editSaturation);
}

//------------------------------------------------------------------
void AudioAnalizer::setup() {

}

//------------------------------------------------------------------
void AudioAnalizer::update() {
    
}

//------------------------------------------------------------------
void AudioAnalizer::analyze(float * iBuffer) {
    
    if(drawAudioAnalizer && (bEditMode || bVisible)) {
        filterBank.analyze(iBuffer);
    }
}

//------------------------------------------------------------------
void AudioAnalizer::customDraw(){

    if(drawAudioAnalizer && (bEditMode || bVisible)) {
        ofPushStyle();
        ofxPatch::customDraw();
        if (EventHandler::getInstance()->getEncapsulatedIdDraw() == encapsulatedId) {
            ofPushMatrix();
            
            ofVec3f scale = ((ofCamera*)this->getParent())->getScale();
            ofVec3f cam_pos = ((ofCamera*)this->getParent())->getPosition();
            
            ofTranslate((textureCorners[0].x - cam_pos.x)/scale.x, (textureCorners[0].y  - cam_pos.y)/scale.y, scale.z);
            filterBank.draw((textureCorners[1].x - textureCorners[0].x)/scale.x,(textureCorners[2].y - textureCorners[0].y)/scale.y);
            
            ofPopMatrix();
            
        }
        ofPopStyle();
    }
}

//------------------------------------------------------------------
void AudioAnalizer::_mouseDragged(ofMouseEventArgs &e){
    
    ofxPatch::_mouseDragged(e);
    
}

//------------------------------------------------------------------
void AudioAnalizer::editTreshold(float& t) {
    
    filterBank.setThreshold(t);
}

//------------------------------------------------------------------
void AudioAnalizer::editSaturation(int& s) {
    
    filterBank.setSoundLevelSaturation(s);
    filterBank.setThreshold(threshold);
}

//------------------------------------------------------------------
void AudioAnalizer::_showHelp(){
    string name = "Node type name: Audio Analizer";
    string description = "Description: Details of the environment sound decompressed through the FFT (fast fourier transform) algorith";
    string use = "Use: Details of the environment sound decompressed through the FFT (fast fourier transform) algorith";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool AudioAnalizer::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    nId = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    threshold = XML.getAttribute("NODE", "threshold", 0.01, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    bVisible            = XML.getValue("visible", true);
    drawAudioAnalizer   = XML.getValue("active", true);
    
    // Load the texture coorners
    //
    if (XML.pushTag("texture")){
        for(int i = 0; i < 4; i++){
            if (XML.pushTag("point",i)){
                textureCorners[i].set(XML.getValue("x", 0.0),XML.getValue("y", 0.0));
                XML.popTag();
            }
        }
        XML.popTag();
    }
    
    bUpdateMask = true;
    bUpdateCoord = true;
    
    XML.popTag();
    
    return nId != -1;
}

//------------------------------------------------------------------
bool AudioAnalizer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "AUDIO_ANALIZER", lastPlace);
    XML.setAttribute("NODE", "threshold", threshold, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
    
        XML.addTag("visible");
        XML.setValue("visible", bVisible);
        XML.addTag("active");
        XML.setValue("active", drawAudioAnalizer);
        
        // Texture Corners
        //
        XML.addTag("texture");
        saved = XML.pushTag("texture");
        if (saved){
            for(int i = 0; i < 4; i++){
                
                XML.addTag("point");
                
                XML.setValue("point:x",textureCorners[i].x, i);
                XML.setValue("point:y",textureCorners[i].y, i);
            }
            XML.popTag();// Pop "texture"
        }
        
        XML.popTag(); // Pop "NODE"
    }

    return saved;
    
}

