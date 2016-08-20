//
//  menuItem.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 11/7/15.
//
//

#include "menuItem.h"

menuItem::menuItem(ofxUISuperCanvas* menu_, string type, string name, string img, bool active, float x, float y) {
    
    this->label   = name;
    this->counter = 0;
    this->toggle  = NULL;
    this->button  = NULL;
    this->menu    = menu_;
    
    if (type == "MultiImageButton") {
        
        button = new ofxUIMultiImageButton(x, y, MENU_ITEM_SIZE, MENU_ITEM_SIZE, active, img, name);
        menu->addWidget(button);
        
        this->setPos(button->getRect()->getX(), button->getRect()->getY());
        this->setSize(button->getRect()->getWidth(), button->getRect()->getHeight());
    }
    else if (type == "MultiImageToggle") {
        
        if (name == "Midi Learn")
            toggle = new ofxUIMultiImageToggle(x, y, MENU_ITEM_SIZE*4, MENU_ITEM_SIZE, active, img, name);
        else toggle = new ofxUIMultiImageToggle(x, y, MENU_ITEM_SIZE, MENU_ITEM_SIZE, active, img, name);

        menu->addWidget(toggle);
        
        this->setPos(toggle->getRect()->getX(), toggle->getRect()->getY());
        this->setSize(toggle->getRect()->getWidth(), toggle->getRect()->getHeight());
    }
    
    enableMouseEvents();
    registerMouseEvents();
}

menuItem::~menuItem(){
    delete menu;
}

//------------------------------------------------------------------
void menuItem::draw() {
        
    if(isMouseOver()) {

        if (counter < 10)
            counter++;
        else {
            
            if (label == "Print Shortcuts (F1)") {
                ofDrawBitmapStringHighlight(label, ofGetMouseX()-140, ofGetMouseY()+30);
            }
            else if (label == "Zoom In" || label == "Zoom Out" || label == "Analizer") {
                ofDrawBitmapStringHighlight(label, ofGetMouseX()+15, ofGetMouseY()-10);
            }
            else {
                ofDrawBitmapStringHighlight(label, ofGetMouseX()+10, ofGetMouseY()+30);
            }
        }
    }
    else {
        counter = 0;
    }
}

//------------------------------------------------------------------
void menuItem::onMouseMove(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mouseMoved(e) : this->button->mouseMoved(e);
}

//------------------------------------------------------------------
void menuItem::onRollOut(ofMouseEventArgs &e) {
    
    this->toggle != NULL ? this->toggle->mouseMoved(e) : this->button->mouseMoved(e);
}

//------------------------------------------------------------------
void menuItem::onPress(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mousePressed(e) : this->button->mousePressed(e);
}

//------------------------------------------------------------------
void menuItem::onRelease(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mouseReleased(e) : this->button->mouseReleased(e);
}

//------------------------------------------------------------------
void menuItem::onReleaseOutside(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mouseReleased(e) : this->button->mouseReleased(e);
}



