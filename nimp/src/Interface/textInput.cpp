//
//  textInput.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#include "textInput.h"
#include "textInputEvent.h"
#include "ofxComposer.h"
#include "ofxUISuperCanvas.h"

textInput::textInput(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUITextInput()
{
    init(_name, _textstring, w, h, x, y, _size);

    nodes.push_back("INPUTS");
    nodes.push_back("audio in - left channel");
    nodes.push_back("audio in - right channel");
    nodes.push_back("camera");
    nodes.push_back("image or video");
    nodes.push_back("midi device");
    nodes.push_back("osc receiver");
    nodes.push_back("particle generator");
    nodes.push_back("syphon client");
    nodes.push_back("LAYERS");
    nodes.push_back("glitch 1");
    nodes.push_back("glitch 2");
    nodes.push_back("ikeda");
    nodes.push_back("image processor");
    nodes.push_back("shader");
    nodes.push_back("MIXERS");
    nodes.push_back("mix mask");
    nodes.push_back("mix simple blend");
    nodes.push_back("multi channel switch");
    nodes.push_back("OUTPUTS");
    nodes.push_back("syphon output");
    
    midiList = NULL;
    toggleSelected = 0;
    
}

//------------------------------------------------------------------
textInput::~textInput() {
    
    ofRemoveListener(((ofxUISuperCanvas*) this->getCanvasParent())->newGUIEvent, this, &textInput::guiEvent);
}


/* ================================================ */
/*                      EVENTS                      */
/* ================================================ */

void textInput::keyPressed(int key) {
    
    ofxUITextInput::keyPressed(key);
    if(clicked)
    {
        if ((this->midiList == NULL || !this->midiList->isVisible()) && not this->dropdownList->isOpen()) {
            this->dropdownList->open();
        }
        
        dropdownList->clearSelected();

        if (key == OF_KEY_UP) {
            
            // midi list
            if (this->midiList != NULL && this->midiList->isVisible()) {
                toggleSelected <= 0
                ? toggleSelected = this->midiList->getToggles().size() - 1
                : toggleSelected--;
                
                this->midiList->activateToggle(this->midiList->getToggles()[toggleSelected]->getName());
                this->setTextString(this->midiList->getToggles()[toggleSelected]->getName());
            }
            
            // nodes list
            else {
                if (this->dropdownList->getVisibleToggles().size() == 0) {
                    setVisibleToggles();
                }
                
                if (this->dropdownList->getVisibleToggles().size() > 4) {
                    toggleSelected--;
                    
                    if(toggleSelected < 0) {
                        toggleSelected = this->dropdownList->getToggles().size() - 1;
                    }
                    string toggleSelectedName = this->dropdownList->getToggles()[toggleSelected]->getName();
                    
                    while (!this->dropdownList->getToggles()[toggleSelected]->isVisible() ||
                           toggleSelectedName == "INPUTS" || toggleSelectedName == "LAYERS" || toggleSelectedName == "MIXERS" || toggleSelectedName == "OUTPUTS") {
                        
                        toggleSelected <= 0
                        ? toggleSelected = this->dropdownList->getToggles().size() - 1
                        : toggleSelected--;
                        
                        toggleSelectedName = this->dropdownList->getToggles()[toggleSelected]->getName();
                    }
                    
                    this->dropdownList->activateToggle(this->dropdownList->getToggles()[toggleSelected]->getName());
                    this->setTextString(this->dropdownList->getToggles()[toggleSelected]->getName());
                }
            }
        }
        else if (key == OF_KEY_DOWN){
            
            // midi list
            if (this->midiList != NULL && this->midiList->isVisible()) {
                toggleSelected = (toggleSelected+1) % this->midiList->getToggles().size();
                
                this->midiList->activateToggle(this->midiList->getToggles()[toggleSelected]->getName());
                this->setTextString(this->midiList->getToggles()[toggleSelected]->getName());
            }
            
            // nodes list
            else {
                if (this->dropdownList->getVisibleToggles().size() == 0) {
                    setVisibleToggles();
                }
                
                if (this->dropdownList->getVisibleToggles().size() > 4) {
                    toggleSelected = (toggleSelected+1) % this->dropdownList->getToggles().size();
                    string toggleSelectedName = this->dropdownList->getToggles()[toggleSelected]->getName();
                    
                    while (!this->dropdownList->getToggles()[toggleSelected]->isVisible() ||
                           toggleSelectedName == "INPUTS" || toggleSelectedName == "LAYERS" || toggleSelectedName == "MIXERS" || toggleSelectedName == "OUTPUTS") {
                        
                        toggleSelected = (toggleSelected+1) % this->dropdownList->getToggles().size();
                        toggleSelectedName = this->dropdownList->getToggles()[toggleSelected]->getName();;
                    }
                    
                    this->dropdownList->activateToggle(this->dropdownList->getToggles()[toggleSelected]->getName());
                    this->setTextString(this->dropdownList->getToggles()[toggleSelected]->getName());
                }
            }
        }
        else if (key == OF_KEY_RETURN){
            
            // midi list
            if (this->midiList != NULL && this->midiList->isVisible()) {
                
                this->midiList->activateToggle(this->midiList->getToggles()[toggleSelected]->getName());
                
                ofxUIEventArgs event_;
                event_.widget = this->midiList;
                this->guiMidiEvent(event_);
            }
            
            // nodes list
            else {
                this->dropdownList->activateToggle(this->dropdownList->getToggles()[toggleSelected]->getName());
                
                ofxUIEventArgs event_;
                event_.widget = this->dropdownList;
                this->guiEvent(event_);
                
                if (this->dropdownList->getToggles()[toggleSelected]->getName() == "midi device") {
                    this->dropdownList->setVisible(false);
                    this->midiList->setVisible(true);
                }
            }
            
            return;
        }
        else if (key == OF_KEY_BACKSPACE) {
            
            // midi list
            if ((this->midiList != NULL && this->midiList->isVisible() && this->getTextString() == "No devices availabl") ||
                (this->getTextString() == "midi devic")){
                this->midiList->clearSelected();
                this->dropdownList->setVisible(true);
                this->midiList->setVisible(false);
            }
            else setVisibleToggles();
        }
        else {
            setVisibleToggles();
        }
        
        this->setFocus(true);
    }
}

//------------------------------------------------------------------
bool textInput::mouseDragged(ofMouseEventArgs &e) {
    
    if  (this->hit) {
        ofxUITextInput::mouseDragged(e);
        
        if ((this->draggable) and (this->hit)) {
            this->dropdownList->getRect()->setX(e.x - hitPoint.x);
            this->dropdownList->getRect()->setY(e.y - hitPoint.y);
            
            if (midiList) {
                this->midiList->getRect()->setX(e.x - hitPoint.x);
                this->midiList->getRect()->setY(e.y - hitPoint.y);
            }
        }
    }
    return this->hit;
}

//------------------------------------------------------------------
bool textInput::mouseReleased(ofMouseEventArgs &e) {
    
    if (hit) {
        
        if (isFocused()) {
            
            if (getTextString() != "midi device") {
                this->dropdownList->setVisible(true);
            }
            else if (midiList != NULL){
                this->midiList->setVisible(true);
            }
        }
        else {
            dropdownList->clearSelected();
            if (getTextString() != "midi device") {
                this->dropdownList->setVisible(false);
            }
            else if (midiList != NULL) {
                
                this->midiList->setVisible(false);
                midiList->clearSelected();
            }
        }
    }
    return ofxUITextInput::mouseReleased(e);
}

//------------------------------------------------------------------
void textInput::guiEvent(ofxUIEventArgs &event_){
    
    if(event_.widget == this->dropdownList && this->dropdownList->getSelected().size()) {
        
        string selectedName = this->dropdownList->getSelected()[0]->getName();
        
        if (selectedName == "INPUTS" || selectedName == "LAYERS" || selectedName == "MIXERS" || selectedName == "OUTPUTS"){
            return;
        }
        
        this->dropdownList->setVisible(false);
        this->setTextString(selectedName);
        
        textInputEvent e;
        e.point.set(this->getRect()->getX(), this->getRect()->getY());
        e.widget = this;
        e.type = this->dropdownList->getSelected()[0]->getName();
        
        ofFileDialogResult openFileResult;
        
        if (e.type == "midi device") {
            
            vector<string>& midiPortList = midiIn->getPortList();
            if (midiPortList.size() == 0) {
                midiPortList.push_back("No devices available");
            }
            
            if(midiList == NULL) {
                midiList = new ofxUIDropDownList("", midiPortList, 250, this->getRect()->x, this->getRect()->y);
                this->getCanvasParent()->addWidget(midiList);
                midiList->open();
                midiList->setAutoClose(true);
                this->addEmbeddedWidget(midiList);
                
                ofAddListener(((ofxUISuperCanvas*) midiList->getCanvasParent())->newGUIEvent,this,&textInput::guiMidiEvent);
            }
            else {
                midiList->clearToggles();
                midiList->addToggles(midiPortList);
            }
            midiList->setVisible(true);
            for(auto n : midiList->getToggles()) {
                n->setColorBack(ofxUIColor (70,70,70,250));
                n->setColorOutline(ofxUIColor (50,50,50,250));
                n->setColorFillHighlight(ofxUIColor (30,30,30,250));
                n->setDrawOutline(true);
            }
            
        }
        else if ((e.type == "image or video")){

            openFileResult = ofSystemLoadDialog("Select an image (.jpg, .jpeg, .png, .bmp or .gif) or video (.mov, .mpg, .mp4 or .m4v)");

            if (openFileResult.bSuccess){

                ofFile file (openFileResult.getPath());

                if (file.exists()){

                    string fileExtension = ofToUpper(file.getExtension());

                    //We only want images
                    if (fileExtension == "JPG"  ||
                        fileExtension == "PNG"  ||
                        fileExtension == "JPEG" ||
                        fileExtension == "GIF"  ||
                        fileExtension == "BMP"  ||
                        fileExtension == "MOV"  ||
                        fileExtension == "MPG"  ||
                        fileExtension == "MP4"  ||
                        fileExtension == "M4V" ) {
                        e.path = openFileResult.getPath();
                        e.name = file.getFileName();
                    }
                    else {
                        ConsoleLog::getInstance()->pushError("Select a valid image or video file (.jpg, .jpeg, .gif, .bmp, .mov, .mpg, .mp4 or .m4v)");
                        return;
                    }
                }
                file.close();
                ofNotifyEvent(createNode, e , this);
            }
            else {
                dropdownList->clearSelected();
                return;
            }
        }
        else if(e.type == "shader"){
            openFileResult = ofSystemLoadDialog("Select a shader (.fs)");
            
            if (openFileResult.bSuccess){
                
                ofFile file (openFileResult.getPath());
                
                if (file.exists()){
                    
                    string fileExtension = ofToUpper(file.getExtension());
                    
                    //We only want shaders
                    if (fileExtension == "fs" || fileExtension == "FS") {
                        e.path = openFileResult.getPath();
                        e.name = file.getFileName();
                    }
                    else {
                        ConsoleLog::getInstance()->pushError("Select a valid shader file (.fs)");
                        return;
                    }
                }
                file.close();
                ofNotifyEvent(createNode, e , this);
            }
            else {
                dropdownList->clearSelected();
                return;
            }
        }
        else {
            ofNotifyEvent(createNode, e , this);
        }
    }
}

//------------------------------------------------------------------
void textInput::guiMidiEvent(ofxUIEventArgs &e){
    
    if(e.widget == this->midiList && this->midiList->getSelected().size()) {
        
        if (this->midiList->getSelected()[0]->getName() == "No devices available") {
            midiList->clearSelected();
            return;
        }
        else {
            this->setTextString(this->midiList->getSelected()[0]->getName());
            
            textInputEvent e;
            e.point.set(this->getRect()->getX(), this->getRect()->getY());
            e.widget = this;
            e.type = this->midiList->getSelected()[0]->getName();

            ofNotifyEvent(createNode, e , this);
        }
    }
}

/* ================================================ */
/*       GETTERS | SETTERS | OTHER FUNCTIONS        */
/* ================================================ */

void textInput::setDropdownList(ofxUIDropDownList* dl) {
    
    dl->open();
    dl->setVisible(false);
    dl->setAutoClose(false);
    
    dl->addToggles(nodes);
    dl->setToggleVisibility(false);
    
    for(auto n : dl->getToggles()) {
        
        if (n->getName() == "INPUTS" || n->getName() == "LAYERS" || n->getName() == "MIXERS" || n->getName() == "OUTPUTS") {
            n->setColorBack(ofxUIColor (50,50,50,250));
            n->setColorOutline(ofxUIColor (30,30,30,250));
            n->setColorOutlineHighlight(ofxUIColor (30,30,30,250));
            n->setColorFill(ofxUIColor (50,50,50,250));
            n->setColorFillHighlight(ofxUIColor (50,50,50,250));
            ((ofxUIWidgetWithLabel*)n->getLabelWidget())->setColorFill(ofxUIColor (200,200,200,250));
            ((ofxUIWidgetWithLabel*)n->getLabelWidget())->setColorFillHighlight(ofxUIColor (200,200,200,250));
        }
        else {
            n->setColorBack(ofxUIColor (70,70,70,250));
            n->setColorOutline(ofxUIColor (50,50,50,250));
            n->setColorOutlineHighlight(ofxUIColor (120,120,120,250));
            n->setColorFill(ofxUIColor (85,85,85,250));
            n->setColorFillHighlight(ofxUIColor (30,30,30,250));
            n->setDrawOutline(true);
        }
        n->setLabelAlignment(OFX_UI_TEXT_ALIGN_LEFT);
    }


    this->addEmbeddedWidget(dl);
    this->dropdownList = dl;

    ofAddListener(((ofxUISuperCanvas*) dl->getCanvasParent())->newGUIEvent,this,&textInput::guiEvent);
    
    
}

//------------------------------------------------------------------
ofxUIDropDownList* textInput::getDropdownList() {
    return this->dropdownList;
}

//------------------------------------------------------------------
void textInput::setVisibleToggles() {
    
    string input = ofToLower(this->getTextString());
    for(auto n : this->dropdownList->getToggles()) {
        if (n->getName().find(input) != -1 || n->getName() == "INPUTS" || n->getName() == "LAYERS" || n->getName() == "MIXERS" || n->getName() == "OUTPUTS") {
            n->setVisible(true);
        }
        else n->setVisible(false);
    }
}
