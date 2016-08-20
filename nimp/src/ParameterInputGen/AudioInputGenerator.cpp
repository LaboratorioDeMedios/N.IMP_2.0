//
//  AudioInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "AudioInputGenerator.h"


AudioInputGenerator::AudioInputGenerator(string name_, int nodeID_):AudioListenerInput(name_, nodeID_){
    
    audioMap = new vector<DTAudioMap*>();
    hasNewData = false;
    isDataProcessed = false;
    bufferCounter = 0;
    saturation = 20;
    enable = true;
}

//------------------------------------------------------------------
AudioInputGenerator::~AudioInputGenerator(){

    for(int i = 0; i < audioMap->size(); i++){
        delete audioMap->at(i);
    }
    audioMap->clear();
    delete audioMap;
    audioMap = NULL;
}

//------------------------------------------------------------------
void AudioInputGenerator::processInput(){
    
    if(lock()){
        if(hasNewData && enable){
            //if I have new data, I begin processing
            if(!isDataProcessed){
                
                static int index=0;
                
                float avg_power = 0.0f;
                
                if(index < 80)
                    index += 1;
                else
                    index = 0;
                
                /* do the FFT	*/
                if (channel == 0) {
                    myfft.powerSpectrum(0, (int)BUFFER_SIZE/2, left, BUFFER_SIZE, &magnitude[0], &phase[0], &power[0], &avg_power);
                }
                else {
                    myfft.powerSpectrum(0, (int)BUFFER_SIZE/2, right, BUFFER_SIZE, &magnitude[0], &phase[0], &power[0], &avg_power);
                }

                
                Param* p = new Param();
                
                if(magnitude[band] > saturation){
                    // we limit the max output to 50 ... this is arbitrary
                    magnitude[band] = saturation;
                }
                else if(magnitude[band] < 00.01){
                    // we limit the min output to 0
                    magnitude[band] = 0;
                }
                
                p->inputMax     = saturation;
                p->inputMin     = 0;
                p->imageInputId = nodeID;
                p->name         = "AudioBandWaveForm";
                p->value        = magnitude[band];
                p->intVal       = magnitude[band];
                p->floatVal     = magnitude[band];
                
                storeMessage(p);
                
                for(int i=0; i<audioMap->size(); i++){
                    Param* p = new Param();
                    
                    p->inputMax     = saturation;
                    p->inputMin     = 0;
                    p->imageInputId = audioMap->at(i)->nodeId;
                    p->name         = audioMap->at(i)->paramId;
                    p->value        = magnitude[band];
                    p->intVal       = ofMap(magnitude[band], 0, saturation, audioMap->at(i)->paramMinValue, audioMap->at(i)->paramMaxValue);
                    p->floatVal     = ofMap(magnitude[band], 0, saturation, audioMap->at(i)->paramMinValue, audioMap->at(i)->paramMaxValue);
                    
                    storeMessage(p);
                }
                
                //delete everything
                disposeData();
            }
        }
        unlock();
    }
}

//------------------------------------------------------------------
bool AudioInputGenerator::addNewAudioMap(int band_, int channel_, ImageOutput* node_, vector<string> params_) {
    for (int i = 0; i < params_.size(); i++) {
        
        DTAudioMap* dtM = new DTAudioMap();
        
        dtM->band           = band_;
        dtM->nodeId         = node_->getId();
        dtM->paramId        = params_[i];
        dtM->inputMinValue  = 0;
        dtM->inputMaxValue  = saturation;
        dtM->paramMinValue  = node_->getMidiMin(dtM->paramId);
        dtM->paramMaxValue  = node_->getMidiMax(dtM->paramId);
        
        audioMap->push_back(dtM);
    }
}

//------------------------------------------------------------------
void AudioInputGenerator::removeNodeFromParams(int nodeID_) {
    
    for (int i = 0; i < audioMap->size(); i++) {
        if (audioMap->at(i)->nodeId == nodeID_) {
            audioMap->erase(audioMap->begin() + i);
            i--;
        }
    }
}

//------------------------------------------------------------------
void AudioInputGenerator::clearAudioMap() {
    audioMap->clear();
}

//------------------------------------------------------------------
bool AudioInputGenerator::loadSettings(ofxXmlSettings &XML, int nodesCount_) {
    
    bool result = true;
    
    enable  = XML.getAttribute("FFT_SETTINGS","enabled",true,0);
    channel = XML.getAttribute("FFT_SETTINGS","channel",0,0);
    band    = XML.getAttribute("FFT_SETTINGS","band",0,0);
    result = XML.pushTag("FFT_SETTINGS");

    if (result) {
        int numAudioMapTag = XML.getNumTags("AUDIO_MAP");
        
        for(int i = 0; i < numAudioMapTag; i++){
            
            DTAudioMap* dtM = new DTAudioMap();

            dtM->band           = band;
            dtM->nodeId         = XML.getAttribute("AUDIO_MAP","nodeId",0,i) + nodesCount_;
            dtM->paramId        = XML.getAttribute("AUDIO_MAP","param","",i);
            dtM->inputMinValue  = ofToFloat(XML.getAttribute("AUDIO_MAP","inputMinValue","0",i));
            dtM->inputMaxValue  = ofToFloat(XML.getAttribute("AUDIO_MAP","inputMaxValue","127",i));
            dtM->paramMinValue  = ofToFloat(XML.getAttribute("AUDIO_MAP","paramMinValue","0",i));
            dtM->paramMaxValue  = ofToFloat(XML.getAttribute("AUDIO_MAP","paramMaxValue","127",i));
            
            audioMap->push_back(dtM);
        }
        
        XML.popTag();
    }
    
    return result;
}

//------------------------------------------------------------------
bool AudioInputGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new INPUT_GEN tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("INPUT_GEN");
    
    XML.addAttribute("INPUT_GEN", "nodeId", nodeID, lastPlace);
    XML.addAttribute("INPUT_GEN", "type", "FFT", lastPlace);
    
    saved = XML.pushTag("INPUT_GEN", lastPlace);
    if (saved) {
        lastPlace = XML.addTag("FFT_SETTINGS");
        XML.addAttribute("FFT_SETTINGS", "enabled", enable, lastPlace);
        XML.addAttribute("FFT_SETTINGS", "channel", channel, lastPlace);
        XML.addAttribute("FFT_SETTINGS", "band", band, lastPlace);
        saved = XML.pushTag("FFT_SETTINGS", lastPlace);
        if (saved) {
            for(int i = 0; i < audioMap->size(); i++){
                
                XML.addTag("AUDIO_MAP");
                XML.addAttribute("AUDIO_MAP","nodeId", audioMap->at(i)->nodeId,i);
                XML.addAttribute("AUDIO_MAP","param", audioMap->at(i)->paramId,i);
                XML.addAttribute("AUDIO_MAP","inputMinValue", audioMap->at(i)->inputMinValue,i);
                XML.addAttribute("AUDIO_MAP","inputMaxValue", audioMap->at(i)->inputMaxValue,i);
                XML.addAttribute("AUDIO_MAP","paramMinValue", audioMap->at(i)->paramMinValue,i);
                XML.addAttribute("AUDIO_MAP","paramMaxValue", audioMap->at(i)->paramMaxValue,i);
                
            }
            
            XML.popTag(); // FFT_SETTINGS
        }
        XML.popTag(); // INPUT_GEN
    }
    
    return saved;
}

//------------------------------------------------------------------
bool AudioInputGenerator::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {

    bool saved = true;
    
    if (newIdsMap[nodeID]) {
        int lastPlace = XML.addTag("INPUT_GEN");
        
        XML.addAttribute("INPUT_GEN", "nodeId", newIdsMap[nodeID], lastPlace);
        XML.addAttribute("INPUT_GEN", "type", "FFT", lastPlace);
        
        saved = XML.pushTag("INPUT_GEN", lastPlace);
        if (saved) {
            lastPlace = XML.addTag("FFT_SETTINGS");
            XML.addAttribute("FFT_SETTINGS", "enabled", enable, lastPlace);
            XML.addAttribute("FFT_SETTINGS", "channel", channel, lastPlace);
            XML.addAttribute("FFT_SETTINGS", "band", band, lastPlace);
            saved = XML.pushTag("FFT_SETTINGS", lastPlace);
            if (saved) {
                for(int i = 0; i < audioMap->size(); i++){
                    
                    if (newIdsMap[audioMap->at(i)->nodeId]){
                        XML.addTag("AUDIO_MAP");
                        XML.addAttribute("AUDIO_MAP","nodeId", newIdsMap[audioMap->at(i)->nodeId],i);
                        XML.addAttribute("AUDIO_MAP","param", audioMap->at(i)->paramId,i);
                        XML.addAttribute("AUDIO_MAP","inputMinValue", audioMap->at(i)->inputMinValue,i);
                        XML.addAttribute("AUDIO_MAP","inputMaxValue", audioMap->at(i)->inputMaxValue,i);
                        XML.addAttribute("AUDIO_MAP","paramMinValue", audioMap->at(i)->paramMinValue,i);
                        XML.addAttribute("AUDIO_MAP","paramMaxValue", audioMap->at(i)->paramMaxValue,i);
                    }
                }
                
                XML.popTag(); // FFT_SETTINGS
            }
            XML.popTag(); // INPUT_GEN
        }
    }
    
    return saved;
}
