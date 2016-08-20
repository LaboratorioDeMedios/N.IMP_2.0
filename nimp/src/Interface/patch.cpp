//
//  patch.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/31/15.
//
//

#include "patch.h"
#include <GLUT/glut.h>

patch::patch() : ofxPatch() {

    selectedLink = -1;
    selectedLinkPath = -1;
    bInspector = false;
    
    ofAddListener(ofEvents().mousePressed, this, &patch::_mousePressed, PATCH_EVENT_PRIORITY);
    ofAddListener(ofEvents().mouseDragged, this, &patch::_mouseDragged, PATCH_EVENT_PRIORITY);
    ofAddListener(ofEvents().mouseReleased, this, &patch::_mouseReleased, PATCH_EVENT_PRIORITY);
    ofAddListener(ofEvents().keyPressed, this, &patch::_keyPressed, PATCH_EVENT_PRIORITY);
    
    title->addButton('i', &bInspector, TOGGLE_BUTTON);
    title->setParent(*this);
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void patch::update() {

    if (imageSrc != "") {
        delete image;
        image   = new ofImage();
        image->loadImage( imageSrc );
        
        if (width != image->getWidth() || height != image->getHeight()) {
            this->resetSize(image->getWidth(), image->getHeight());
        }
    }
    
    ofxPatch::update();

    /*if (videoPlayer != NULL){
        videoInterface->setVideoFrame(videoPlayer->getPosition());
    }*/
    //title->setTitle("");
}

void patch::customDraw() {
    
    ofxPatch::customDraw();
    
    if (bEditMode) {
        
        // Draw video interface
        /*if (type.compare("ofVideoPlayer") == 0)
            videoInterface->draw();*/
        
        if (type.compare("ofImage") == 0) {
            if (bInspector) {
                ofVec3f scale = ((ofCamera*)this->getParent())->getScale();
                inspector->setPosition(textureCorners[1].x/scale.x+2, textureCorners[1].y/scale.y);
                inspector->setVisible(true);
            }
            else inspector->setVisible(false);
        }
    
        // Draw the links
        //
        for (int i = 0; i < outPut.size(); i++){
            if (outPut[i].to != NULL){
                ofFill();
                ofCircle(outPut[i].pos, 3);
                if (linkType == STRAIGHT_LINKS)
                    ofLine(outPut[i].pos, outPut[i].to->pos);
                else if (linkType == CURVE_LINKS) {
                    ofNoFill();
                    ofBezier(outPut[i].pos.x, outPut[i].pos.y, outPut[i].pos.x+55, outPut[i].pos.y, outPut[i].to->pos.x-55, outPut[i].to->pos.y, outPut[i].to->pos.x, outPut[i].to->pos.y);
                    ofFill();
                }
                else {
                    if (outPut[i].path_coorners.size() > 0) {
                        ofNoFill();
                        for(int j = 0; j < outPut[i].path_coorners.size(); j++){
                            ofCircle( outPut[i].path_coorners[j], 4);
                        }
                    }
                    
                    outPut[i].path_line.clear();
                    outPut[i].path_line.addVertex(outPut[i].pos);
                    if (outPut[i].path_coorners.size() > 0)
                        outPut[i].path_line.addVertices(outPut[i].path_coorners);
                    outPut[i].path_line.addVertex(outPut[i].to->pos);
                    outPut[i].path_line.draw();
                    
                    ofFill();
                }
                
                ofCircle(outPut[i].to->pos, 3);
            }
        }
    }
    else {
        if (bInspector) {
            inspector->setVisible(false);
        }
    }
}


/* ================================================ */
/*                      EVENTS                      */
/* ================================================ */

void patch::_mousePressed(ofMouseEventArgs &e) {
    
    ofVec3f mouse_transformed = ofVec3f(e.x, e.y, 0.0)*this->getGlobalTransformMatrix();
    
    if (bEditMode){
        
        // check is mouse is pressing over the inspector
        if (inspector != NULL and inspector->isVisible() and inspector->isHit(mouse_transformed.x, mouse_transformed.y)) {
            gui->setOtherSelected(true);
            return;
        }
    }
    
    ofxPatch::_mousePressed(e);
    
    // Is over link dot ?
    if (bEditMode){
        
        bool overDot = false;
        for (int i = 0; i < outPut.size() and !overDot; i++){
            
            for (int j = 0; j < outPut[i].path_coorners.size(); j++){
                
                if ( ofDist(mouse_transformed.x, mouse_transformed.y, outPut[i].path_coorners[j].x, outPut[i].path_coorners[j].y) <= 10 ){
                    if ((e.button == 2) || (glutGetModifiers() == GLUT_ACTIVE_CTRL)) {
                        outPut[i].path_coorners.erase(outPut[i].path_coorners.begin()+j);
                    }
                    else {
                        selectedLinkPath = j;
                        selectedLink = i;
                    }
                    overDot = true;
                    ofxPatch::setLinkHit(true);
                }
            }
            
            if (!overDot and linkType == PATH_LINKS and outPut.size() > 0){
                vector<ofPoint> link_vertices = outPut[i].path_line.getVertices();
                
                if (link_vertices.size()){
                    int addNew = -1;
                    int tolerance = 3;
                    
                    for (int j = 0; j < link_vertices.size()-1; j++){
                        int next = (j+1)%link_vertices.size();
                        
                        if (is_between (mouse_transformed.x, link_vertices[j].x, link_vertices[j+1].x, tolerance) && is_between (mouse_transformed.y, link_vertices[j].y, link_vertices[j+1].y, tolerance))
                        {
                            if ((link_vertices[j+1].y - link_vertices[j].y) <= tolerance) // Horizontal line.
                            {
                                addNew = j;
                            }
                            
                            const float M = (link_vertices[j+1].y - link_vertices[j].y) / (link_vertices[j+1].x - link_vertices[j].x); // Slope
                            const float C = -(M * link_vertices[j].x) + link_vertices[j].y; // Y intercept
                            
                            // Checking if (x, y) is on the line passing through the end points.
                            if(std::fabs (mouse_transformed.y - (M * mouse_transformed.x + C)) <= tolerance) {
                                addNew = j;
                            }
                        }
                    }
    
                    if (addNew >= 0) {
                        ofxPatch::setLinkHit(true);
                        if (outPut[i].path_coorners.size() == 0)
                            outPut[i].path_coorners.push_back(ofVec3f(mouse_transformed.x, mouse_transformed.y, 0.0));
                        else if (addNew == 0)
                            outPut[i].path_coorners.insert(outPut[i].path_coorners.begin(), ofVec3f(mouse_transformed.x, mouse_transformed.y, 0.0));
                        else
                            outPut[i].path_coorners.insert(outPut[i].path_coorners.begin()+addNew, ofVec3f(mouse_transformed.x, mouse_transformed.y, 0.0));
                    }
                }
            }
        }
    }
}

void patch::_mouseDragged(ofMouseEventArgs &e) {
    
    if (!gui->getOtherSelected()) {
        ofxPatch::_mouseDragged(e);
        
        ofVec3f mouse = ofVec3f(e.x, e.y, 0.0);
        ofVec3f mouse_transformed = mouse*this->getGlobalTransformMatrix();
        
        if (bEditMode){
            if (selectedLink >= 0 and selectedLinkPath >= 0) {
                outPut[selectedLink].path_coorners[selectedLinkPath] = ofVec3f(mouse_transformed.x, mouse_transformed.y, 0.0);
            }
        }
    }
}

void patch::_mouseReleased(ofMouseEventArgs &e){
    
    // mouse is not longer pressing the inspector
    gui->setOtherSelected(false);
    
    ofxPatch::_mouseReleased(e);
    
    selectedLinkPath = -1;
    selectedLink = -1;
    ofxPatch::setLinkHit(false);
}

void patch::_keyPressed(ofKeyEventArgs &e){
    
    ofxPatch::_keyPressed(e);
    
    //if (ofGetKeyPressed(OF_KEY_CONTROL))
        cout << 'key ' << e.key << '\n';
}

/*void patch::_stopVideo(int &_nId) {
    videoPlayer->stop();
    videoPlayer->firstFrame();
    videoInterface->setVideoState(VIDEO_STOP);
}

void patch::_playVideo(int &_nId) {
    
    if (videoInterface->getVideoState() == VIDEO_PLAY) {
        videoPlayer->stop();
        videoInterface->setVideoState(VIDEO_PAUSE);
    } else {
        videoPlayer->play();
        videoInterface->setVideoState(VIDEO_PLAY);
    }
}*/

void patch::guiEvent(ofxUIEventArgs &e)
{
//	int kind = e.widget->getKind();
    string name = e.widget->getName();
    
    if (name == "Image src btn" && ((ofxUIButton*)e.widget)->getValue()) {
        
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image (.jpg, .jpeg, .png or .bmp)");
        
        if (openFileResult.bSuccess){
            
            ofFile file (openFileResult.getPath());
            
            if (file.exists()){
                
                string fileExtension = ofToUpper(file.getExtension());
                
                //We only want images
                if (fileExtension == "JPG"  ||
                    fileExtension == "PNG"  ||
                    fileExtension == "JPEG" ||
                    fileExtension == "GIF"  ||
                    fileExtension == "BMP"  ) {
                    imageSrc = openFileResult.getPath();
                    ((ofxUITextInput*)inspector->getWidget("Image src"))->setTextString(imageSrc);
                }
                else return;
            }
            file.close();
        }
    }
}


/* ================================================ */
/*       GETTERS | SETTERS | OTHER FUNCTIONS        */
/* ================================================ */

bool patch::loadFile(string _filePath, string _configFile) {
    
    ofxPatch::loadFile(_filePath, _configFile);
    
    /*if (type == "ofVideoPlayer") {
        videoInterface = new nodeVideoInterface(&box, &nId);
        ofAddListener( videoInterface->_stop , this, &patch::_stopVideo);
        ofAddListener( videoInterface->_play , this, &patch::_playVideo);
    }
    else*/ if (type == "ofImage") {
        imageSrc = _filePath;
        inspector = new ofxUICanvas();
        inspector->addLabel("INSPECTOR");
        inspector->addSpacer();
        inspector->addLabel("Image src:");
        ofxUITextInput* ti = inspector->addTextInput("Image src", imageSrc);
        ti->setDraggable(false);
        inspector->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        inspector->addImageButton("Image src btn", "assets/edit.png", false);
        inspector->autoSizeToFitWidgets();
        ofAddListener(inspector->newGUIEvent,this,&patch::guiEvent);
        //inspector->loadSettings("GUI/guiSettings.xml");
        inspector->setVisible(false);
    }
}

ofPolyline patch::getCoorners() {
    return textureCorners;
}

float patch::getHeight() {
    return height;
}

float patch::getWidth(){
    return width;
}

void patch::setLinkType(nodeLinkType type) {
    linkType = type;
}

void patch::setMainCanvas(ofxUISuperCanvas* _gui) {
    this->gui = _gui;
    this->setParent(*this->gui);
}

bool patch::is_between (float x, float bound1, float bound2, float tolerance) {
    // Handles cases when 'bound1' is greater than 'bound2' and when
    // 'bound2' is greater than 'bound1'.
    return (((x >= (bound1 - tolerance)) && (x <= (bound2 + tolerance))) ||
            ((x >= (bound2 - tolerance)) && (x <= (bound1 + tolerance))));
}

void patch::resetSize(int _width, int _height) {

    width = _width;
    height = _height;
    
    int offSet;
    if (title != NULL)
        offSet = 15;
    textureCorners[0].set(0.0, offSet);
    textureCorners[1].set(width, offSet);
    textureCorners[2].set(width, height + offSet);
    textureCorners[3].set(0.0, height + offSet);
    
    move( ofPoint(x,y) );
    scale(0.5);
    setPosition(getGlobalPosition()*((ofCamera*)getParent())->getScale());
}

// nico snippet
bool patch::loadSnippetPatch(string snippetName, int relativeId, int previousPatchesSize) {
    return ofxPatch::loadSnippetPatch(snippetName, relativeId, previousPatchesSize);
}

bool patch::saveSnippetPatch(string snippetName, map<int, int> idMap, ofxXmlSettings xml) {
   return ofxPatch::saveSnippetPatch(snippetName, idMap, xml);
}
