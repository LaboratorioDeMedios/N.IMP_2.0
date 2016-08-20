//
//  textInput.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#ifndef __NIMP_Prototipe__textInput__
#define __NIMP_Prototipe__textInput__

#include <stdio.h>
#include "ofxUITextInput.h"
#include "ofxUIDropDownList.h"
#include "ofxUIEventArgs.h"
#include "textInputEvent.h"
#include "ofxMidiIn.h"

class textInput : public ofxUITextInput
{
    
public:
    
    textInput(string _name, string _textstring, float w, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL);
    ~textInput();
    
    void keyPressed(int key);
    bool mouseDragged(ofMouseEventArgs &e);
    bool mouseReleased(ofMouseEventArgs &e);
    void guiEvent(ofxUIEventArgs &e);
    void guiMidiEvent(ofxUIEventArgs &e);
    
    void setMidiIn(ofxMidiIn* midiIn_){ midiIn = midiIn_; };
    void setDropdownList(ofxUIDropDownList* dl);
    ofxUIDropDownList* getDropdownList();
    
    ofEvent<textInputEvent> createNode;

private:
    
    ofxUIDropDownList *dropdownList;
    ofxUIDropDownList *midiList;
    vector<string> nodes;
    ofxMidiIn* midiIn;
    int toggleSelected;
    
    void setVisibleToggles();

};

#endif /* defined(__NIMP_Prototipe__textInput__) */
