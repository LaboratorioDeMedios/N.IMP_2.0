/*
 *  VisualLayer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "VisualLayer.h"


VisualLayer::VisualLayer(string name_, string typeName_, int id_) : ImageOutput(name_, typeName_, id_){
    isEnabled = true;
    firstInit = true;
    nodeType = VISUAL_LAYER;
    
    if(typeName_ != "Shader"){
        addInputDot();
    }
}

//------------------------------------------------------------------
void VisualLayer::inputEvent(map<string, Param*>* params){
    //some mapping
}

//------------------------------------------------------------------
void VisualLayer::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    
    if (input.size() > 0)
        input[0]->setEnable(isEnabled);
}