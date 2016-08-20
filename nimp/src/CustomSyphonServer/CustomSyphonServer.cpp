//
//  CustomSyphonServer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "CustomSyphonServer.h"
#include "ConsoleLog.h"


CustomSyphonServer::CustomSyphonServer(string name_, ImageOutput* feeder_, int id_): ImageOutput(name_, "Syphon Output", id_){
    feeder = feeder_;
    
    addInputDot();

    maxInputs = 1;
    isSyphonServer = true;
    drawNoInputs = (feeder_ == NULL);
    
    previous_index = 0;
    
    if (feeder != NULL) {
        drawing_width = feeder->getWidth();
        drawing_height = feeder->getHeight();
    }
    
    aspectRatioLabels.push_back("1:1");
    aspectRatioLabels.push_back("5:4");
    aspectRatioLabels.push_back("4:3");
    aspectRatioLabels.push_back("1.48:1");
    aspectRatioLabels.push_back("16:10");
    aspectRatioLabels.push_back("16:9");
    aspectRatioLabels.push_back("1.85:1");
    aspectRatioLabels.push_back("2.35:1");
    
    gui.add(aspectRatio.set("Aspect Ratio - 16:9", 5, 0, 7));
    aspectRatio.addListener(this, &CustomSyphonServer::aspectRatioChanged);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------

CustomSyphonServer::~CustomSyphonServer(){

}

//------------------------------------------------------------------
void CustomSyphonServer::setup(){
    
    server.setName(name);
}

//------------------------------------------------------------------
void CustomSyphonServer::update(){
    if(!drawNoInputs){
        ofPushStyle();
        server.publishTexture(&feeder->getTextureReference());
        
        fbo.begin();
        ofSetColor(255);
        feeder->getTextureReference().draw(-padding_x, -padding_y, drawing_width, drawing_height);
        fbo.end();
        ofPopStyle();
    }
    else {
        server.publishTexture(&noInputsImg.getTextureReference());
    }
}

//------------------------------------------------------------------
ofTexture* CustomSyphonServer::getTexture(){
    if (drawNoInputs){
        return &noInputsImg.getTextureReference();
    } else {
        return &fbo.getTextureReference();
    }
}

//------------------------------------------------------------------
void CustomSyphonServer::aspectRatioChanged(int& index_){
    
    if (previous_index != index_) {
        
        previous_index = index_;
        
        aspectRatio.setName("Aspect Ratio - " + aspectRatioLabels[index_]);
        
        height = (textureCorners[2].y - textureCorners[0].y)/SCALE_RATIO;
        
        if (aspectRatioLabels[index_] == "1:1") {
            width = height;
        }
        else if (aspectRatioLabels[index_] == "5:4") {
            width = height + height*0.25;
        }
        else if (aspectRatioLabels[index_] == "4:3") {
            width = height + height*0.33;
        }
        else if (aspectRatioLabels[index_] == "1.48:1") {
            width = height + height*0.48;
        }
        else if (aspectRatioLabels[index_] == "16:10") {
            width = height + height*0.60;
        }
        else if (aspectRatioLabels[index_] == "16:9") {
            width = height + height*0.78;
        }
        else if (aspectRatioLabels[index_] == "1.85:1") {
            width = height + height*0.85;
        }
        else if (aspectRatioLabels[index_] == "2.35:1") {
            width = height*2 + height*0.35;
        }
        
        resetSize(width, height);
        fbo.allocate(width, height);
        
        if (feeder != NULL) {
            
            drawing_width = feeder->getBox().getWidth();
            drawing_height = feeder->getBox().getHeight();
            
            // adapting height and width
            //
            if (drawing_height < height) {
                drawing_height = height;
                drawing_width = (height*feeder->getWidth())/feeder->getHeight();
            }
            if (drawing_width < width) {
                drawing_width = width;
                drawing_height = (width*feeder->getHeight())/feeder->getWidth();
            }
            
            
            // verifying paddings
            //
            if (drawing_width > width) {
                padding_x = abs(drawing_width - width)/2;
            }
            else {
                padding_x = 0;
            }

            if (drawing_height > height) {
                padding_y = abs(drawing_height - height)/2;
            }
            else {
                padding_y = 0;
            }
        }
    }
}


//------------------------------------------------------------------
void CustomSyphonServer::inputAdded(ImageOutput* in_){
    drawNoInputs = false;
    feeder = in_;
    
    int index = aspectRatio;
    previous_index = -1;
    aspectRatioChanged(index);
}

//------------------------------------------------------------------
void CustomSyphonServer::inputRemoved(int id_){
    drawNoInputs = true;
    feeder = NULL;
    
    resetSizeToNoInputs();
}

//------------------------------------------------------------------
void CustomSyphonServer::editName(string& name_) {
    
    name = name_;
    title->setTitle( name_ );
    
    server.setName(name_);
}

//------------------------------------------------------------------
void CustomSyphonServer::_showHelp(){
    string name = "Node type name: Syphon Output";
    string description = "Description: Output of the pipeline";
    string use = "Use: Configure in the inspector the name of the server which the output will be published in the syphon server";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
}

//------------------------------------------------------------------
bool CustomSyphonServer::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    bool loaded = true;
    nId = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    aspectRatio = XML.getAttribute("NODE", "aspectRatio", 0, nTag_);
    previous_index = -1;
    
    XML.pushTag("NODE", nTag_);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
    
}

//------------------------------------------------------------------
bool CustomSyphonServer::saveSettings(ofxXmlSettings &XML) {
    bool saved = true;
    
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    if (feeder != NULL) XML.addAttribute("NODE", "inputId", feeder->getId(), lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "aspectRatio", aspectRatio, lastPlace);
    
    XML.pushTag("NODE", lastPlace);
    saved = ofxPatch::saveSettings(XML, true, lastPlace);
    XML.popTag();
    saved = true;

    return saved;
}

//------------------------------------------------------------------
bool CustomSyphonServer::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
        
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SYPHON_OUTPUT", lastPlace);
    XML.addAttribute("NODE", "aspectRatio", aspectRatio, lastPlace);
    if (feeder != NULL && newIdsMap[feeder->getId()]) {
        XML.addAttribute("NODE", "inputId", feeder->getId(), lastPlace);
    }
    
    XML.pushTag("NODE", lastPlace);
    saved = ofxPatch::saveSettings(XML, true, lastPlace);
    if (saved) {
        XML.popTag();
    }
    
    return saved;
}
