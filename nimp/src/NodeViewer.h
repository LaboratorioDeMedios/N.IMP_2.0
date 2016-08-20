/*
 *  NodeViewer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NodeViewer
#define _NodeViewer

#include "ofMain.h"
#include "NodeElement.h"
#include "ofxComposer.h"
#include "OSCReceiver.h"
#include "AudioIn.h"


class NodeViewer : public ofxComposer {

public:
	
	NodeViewer(string name_="default");
    ~NodeViewer();
	
	void setup();
    void update();
    void draw();
    
    void addElement(NodeElement* elem_);
    void addElement(NodeElement* elem_, ofPoint position_);
    
    void setName(string name_){ name = name_; };
    void setupGuiPositions();
    void setMidiLearnActive(bool active_);
    void setEditLeftAudioInActive(bool active_, int node_);
    void setEditRightAudioInActive(bool active_, int node_);
    void setEditOSCActive(bool active_, int node_);
    
    void createConnections(vector<NodeElement*> _elements);
    void createConnections();
    void closePatch(int _nID);
    void deleteEverything();
    
    string getName();
    map<int, vector <string> > getAttributesClicked();
	
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    
    vector<NodeElement*> elements;
    string name;
};

#endif
