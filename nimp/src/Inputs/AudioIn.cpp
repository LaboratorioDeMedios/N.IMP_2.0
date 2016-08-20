//
//  AudioIn.cpp
//  nimp
//
//  Created by Mili Garicoits on 2/13/16.
//
//

#include "AudioIn.h"
#include "EventHandler.h"

AudioIn::AudioIn(ofxUISuperCanvas* &gui_, string type_, string name_, int id_) : InputSource(name_, type_, id_){
    
    isAudio      = true;
    width        = 500;
    height       = 250;
    selectBand   = 1;
    isEnabled    = true;
    disabledEdit = false;
    
    waveform = new ofxUIMovingGraph(0, 0, 150, 75, inputBuffer, BUFFER_SIZE, -10, 20, "FFT");
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(saturation.set("Sound Level",50,0,100));
    saturation.addListener(this, &AudioIn::editSaturation);
    gui.add(selectBand.set("Band",1,1,16));
    
    if (type_ == "Audio In - Left Channel") {
        audioInType = LEFT;
        gui.add(editFFTInputs.set("Edit Left FFT Inputs", false));
    }
    else {
        audioInType = RIGHT;
        gui.add(editFFTInputs.set("Edit Right FFT Inputs", false));
    }
    
    isEnabled.addListener(this, &AudioIn::editEnabled);
    selectBand.addListener(this, &AudioIn::editBand);
    editFFTInputs.addListener(this, &AudioIn::editInputs);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');
}

AudioIn::~AudioIn(){
    isEnabled.removeListener(this, &AudioIn::editEnabled);
    selectBand.removeListener(this, &AudioIn::editBand);
    editFFTInputs.removeListener(this, &AudioIn::editInputs);
}

//------------------------------------------------------------------
void AudioIn::setup() {
    waveform->ofNode::setParent(*this->getParent());
    waveform->setNoDraw(true);
    waveform->setDraggable(true);
    
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::update() {
    
}

//------------------------------------------------------------------
void AudioIn::customDraw(){
    
    if ( bEditMode || bVisible ) {
        ofxPatch::customDraw();
        if (EventHandler::getInstance()->getEncapsulatedIdDraw() == encapsulatedId) {
            setWaveFormPosition();
            waveform->drawBack();
            waveform->drawFill();
        }
    }
}

//------------------------------------------------------------------
void AudioIn::updateParameter(Param* inputParam) {
    
    if(inputParam->name.compare("AudioBandWaveForm") == 0){
        waveform->addPoint(inputParam->floatVal);
    }
}

//------------------------------------------------------------------
void AudioIn::_mouseDragged(ofMouseEventArgs &e){
    ofxPatch::_mouseDragged(e);
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::editInputs(bool& b){
    
    if (!disabledEdit) {
        AudioInEvent ev;
        ev.nodeId = nId;
        ev.band = selectBand;
        ev.channel = selectChannel;
        ev.active = b;
        
        ofNotifyEvent(editAudioIn, ev);
    }
    else {
        editFFTInputs = false;
    }
}

//------------------------------------------------------------------
void AudioIn::editEnabled(bool& e){
    
    AudioInEvent ev;
    ev.nodeId = nId;
    ev.enable = e;
    ofNotifyEvent(editAudioInEnabled, ev);
}

//------------------------------------------------------------------
void AudioIn::editBand(int& band_){

    AudioInEvent e;
    e.nodeId = nId;
    e.band = band_;
    e.channel = selectChannel;
    ofNotifyEvent(editAudioInBand, e);
}

//------------------------------------------------------------------
void AudioIn::setChannel(int c){
    
    selectChannel = c;
}

//------------------------------------------------------------------
void AudioIn::setBand(int band_){
    
    selectBand = band_;
}

//------------------------------------------------------------------
void AudioIn::editSaturation(float& s) {
    
    if (s == 0) {
        waveform->setMax(0);
        waveform->setMin(-0);
    }
    else {
        waveform->setMax(1000/s);
        waveform->setMin(500/-s);
    }
    
    AudioInEvent ev;
    ev.nodeId = nId;
    ev.saturation = 1000/s;
    
    ofNotifyEvent(editAudioInSaturation, ev);
}

//------------------------------------------------------------------
void AudioIn::setWaveFormPosition() {
    
    ofVec3f scale = ((ofCamera*)this->getParent())->getScale();
    ofVec3f cam_pos = ((ofCamera*)this->getParent())->getPosition();
    ofxUIRectangle* r = waveform->getRect();
    
    r->setX((textureCorners[0].x - cam_pos.x)/scale.x);
    r->setY((textureCorners[0].y - cam_pos.y)/scale.y);

    r->setWidth((textureCorners[1].x - textureCorners[0].x)/scale.x);
    r->setHeight((textureCorners[2].y - textureCorners[0].y)/scale.y);
    
    waveform->setScale(r->getHeight()*.5);
    waveform->setInc(r->getWidth()/(BUFFER_SIZE-1.0));
}

//------------------------------------------------------------------
void AudioIn::resetSize(int _width, int _height) {
    
    ofxPatch::resetSize(_width, _height);
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::_showHelp(){
    string name = "Node type name: Audio In";
    string description = "Description: Listens to the enviroment sound through the computer's microphone";
    string use = "Use: Configure the threshold and of the sounds you want to start listening to";
    string use2 = "To control another node's parameter with the sound this nodes listens to:";
    string use3 = " \t 1. Open the inspector and click on the 'Edit Left/Right FFT Inputs' checkbox";
    string use4 = " \t 2. Select the desired parameter to be controled by this message";
    string use5 = " \t 3. Click again on the 'Edit Left/Right FFT Inputs' checkbox";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
    ConsoleLog::getInstance()->pushMessage(use2, false);
    ConsoleLog::getInstance()->pushMessage(use3, false);
    ConsoleLog::getInstance()->pushMessage(use4, false);
    ConsoleLog::getInstance()->pushMessage(use5, false);
}

//------------------------------------------------------------------
bool AudioIn::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    isEnabled       = XML.getAttribute("NODE", "enabled", true, nTag_);
    selectChannel   = XML.getAttribute("NODE", "channel", 1, nTag_);
    selectBand      = XML.getAttribute("NODE", "band", 1, nTag_);
    
    loaded = XML.pushTag("NODE", nTag_);
    if (loaded) {
        type            = XML.getValue("type","none");
        bVisible        = XML.getValue("visible", true);
        filePath        = XML.getValue("path", "none" );
        
        loaded = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
        
        XML.popTag();
    }
    
    return loaded;
}

//------------------------------------------------------------------
bool AudioIn::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    XML.addAttribute("NODE", "channel", selectChannel, lastPlace);
    XML.addAttribute("NODE", "band", selectBand, lastPlace);
    audioInType == LEFT ? XML.addAttribute("NODE", "type", "LEFT_AUDIO_IN", lastPlace) : XML.addAttribute("NODE", "type", "RIGHT_AUDIO_IN", lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool AudioIn::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    
    if (newIdsMap[nId]) {
        int lastPlace = XML.addTag("NODE");
        
        XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
        XML.addAttribute("NODE", "name", name, lastPlace);
        XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
        XML.addAttribute("NODE", "channel", selectChannel, lastPlace);
        XML.addAttribute("NODE", "band", selectBand, lastPlace);
        audioInType == LEFT ? XML.addAttribute("NODE", "type", "LEFT_AUDIO_IN", lastPlace) : XML.addAttribute("NODE", "type", "RIGHT_AUDIO_IN", lastPlace);
        
        saved = XML.pushTag("NODE", lastPlace);
        if (saved){
            saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
            XML.popTag(); // NODE
        }
    }
    
    return saved;
}

