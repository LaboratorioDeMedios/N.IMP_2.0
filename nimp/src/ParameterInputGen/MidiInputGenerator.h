//
//  MidiInputGenerator.h
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#ifndef _MidiInputGenerator
#define _MidiInputGenerator

#include "ParamInputGenerator.h"
#include "Param.h"
#include "DTMidiMap.h"
#include "ofxMidi.h"
#include "ImageOutput.h"

class MidiInputGenerator: public ParamInputGenerator, public ofxMidiListener  {
    
public:
    
    MidiInputGenerator(string name_, string midiDeviceName);
    ~MidiInputGenerator();
    void processInput();
    void setMidiIn(ofxMidiIn* midiIn_);
    void newMidiMessage(ofxMidiMessage& eventArgs);
    bool addNewMidiMap(int control_, ImageOutput* node_, vector<string> params_);
    void setMidiLearnActive(bool active_) { midiLearnActive = active_; };
    string getMidiDeviceName(){ return midiDeviceName; };
    
    //for storing midi control ids and map them to Node id and parameter.
    map<int,vector<DTMidiMap*> > midiControlMaps;
    void removeNodeFromParams(int nodeID_);
    
    bool loadSettings(ofxXmlSettings &XML, map<int, ImageOutput*> nodes_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    
    string midiDeviceName;
    ofxMidiIn* midiIn;
    ofxMidiMessage midiMessage;
    bool midiLearnActive;
    
};


#endif /* defined(_MidiInputGenerator) */
