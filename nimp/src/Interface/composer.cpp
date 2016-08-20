//
//  composer.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#include "composer.h"

composer::composer() : ofxComposer() {
    // Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &composer::_mouseMoved, COMPOSER_EVENT_PRIORITY);
    ofAddListener(ofEvents().mousePressed, this, &composer::_mousePressed, COMPOSER_EVENT_PRIORITY);
    ofAddListener(ofEvents().mouseReleased, this, &composer::_mouseReleased, COMPOSER_EVENT_PRIORITY);
    ofAddListener(ofEvents().keyPressed, this, &composer::_keyPressed, COMPOSER_EVENT_PRIORITY);
    ofAddListener(ofEvents().windowResized, this, &composer::_windowResized, COMPOSER_EVENT_PRIORITY);
    
    // nico SrollBar
    ofAddListener(ofEvents().mouseDragged, this, &composer::_mouseDragged, COMPOSER_EVENT_PRIORITY);
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void composer::update(){
    
    this->ofxComposer::update();
}

void composer::customDraw(){
    
    ofPushStyle();
        this->ofxComposer::customDraw();
    ofPopStyle();
}

/* ================================================ */
/*                      EVENTS                      */
/* ================================================ */

void composer::_mouseDragged(ofMouseEventArgs &e){
    ofxComposer::_mouseDragged(e);
}

void composer::_mouseReleased(ofMouseEventArgs &e){
    ofxComposer::_mouseReleased(e);
}

void composer::_mousePressed(ofMouseEventArgs &e){
    ofxComposer::_mousePressed(e);
}

void composer::_mouseMoved(ofMouseEventArgs &e){
    ofxComposer::_mouseMoved(e);
}

void composer::_keyPressed(ofKeyEventArgs &e){
    if (e.key == 'j') {
        loadSnippet();
    }
    if (e.key == 'k') {
        saveSnippet();
    }
    
    ofxComposer::_keyPressed(e);
}

void composer::_windowResized(ofResizeEventArgs &e){
    ofxComposer::_windowResized(e);
}

/* ================================================ */
/*       GETTERS | SETTERS | OTHER FUNCTIONS        */
/* ================================================ */

bool composer::getEdit(){
    return this->bEditMode;
}

void composer::setLinkType (nodeLinkType type) {
    for(map<int,patch*>::iterator it = this->patches.begin(); it != this->patches.end(); it++ ){
        it->second->setLinkType(type);
    }
}

map<int,patch*> composer::getPatches() {
    return patches;
}

/************************************** EMPIEZA SNIPPETS *********************************/
void ofxComposer::loadSnippet() {
    
    string snippetName = "";
    
    ofFileDialogResult openFileResult;
    openFileResult = ofSystemLoadDialog("Select a snippet (.xml)");

    if (openFileResult.bSuccess){
        ofFile file (openFileResult.getPath());
        if (file.exists()){
            string fileExtension = ofToUpper(file.getExtension());
            
            //We only want images
//            if (fileExtension == "SNI" ||
//                fileExtension == "XML") {
            if(fileExtension == "XML"){
                snippetName = openFileResult.getPath();
            } else return;
        }
        file.close();
    }
    ofxXmlSettings XML;
    
    int previousPatchesSize = patches.size();
    int a = getMaxIdPatch();
    deactivateAllPatches();
    
    if (XML.loadFile(snippetName)){
        
#ifdef USE_OFXGLEDITOR
        editor.setup(XML.getValue("general:console:font", "menlo.ttf"));
#endif
        int totalPatchs = XML.getNumTags("surface");
        
        // Load each surface present on the xml file
        //
        for(int i = 0; i < totalPatchs ; i++){
            patch *nPatch = new patch();
            bool loaded = nPatch->loadSnippetPatch(snippetName, i, previousPatchesSize);
            if (loaded){
                
#ifdef USE_OFXGLEDITOR
                if (nPatch->getType() == "ofxGLEditor"){
                    ofLog(OF_LOG_NOTICE,"ofxComposer: ofxGLEditor loaded");
                    nPatch->setTexture( editorFbo.getTextureReference(), 0);
                    bGLEditorPatch = true;
                }
#endif
                // Listen to close bottom on the titleBar
                //
                ofAddListener( nPatch->title->close , this, &ofxComposer::closePatch);
                
                // Insert the new patch into the map
                //
                patches[nPatch->getId()] = nPatch;
                
                //mili
                nPatch->setMainCanvas(this->gui);
                //
                
                nPatch->bActive = true;
            }
        }
        
        // Load links between Patchs
        //
        for(int i = 0; i < totalPatchs ; i++){
            if (XML.pushTag("surface", i)){
                int fromID = XML.getValue("id", -1);
                
                if (XML.pushTag("out")){
                    
                    int totalLinks = XML.getNumTags("dot");
                    for(int j = 0; j < totalLinks ; j++){
                        
                        if (XML.pushTag("dot",j)){
                            int toID = XML.getValue("to", 0);
                            int nTex = XML.getValue("tex", 0);
                            
                            // If everything goes ok "i" will match the position of the vector
                            // with the position on the XML, in the same place of the vector array
                            // defined on the previus loop
                            //
                            connect( fromID + previousPatchesSize, toID + previousPatchesSize, nTex);
                            
                            XML.popTag();
                        }
                    }
                    XML.popTag();
                }
                XML.popTag();
            }
        }
    }
}


bool ofxComposer::saveSnippet() {
    string snippetName = "";
    ofxXmlSettings XML;
    
    ofFileDialogResult openFileResult;
    openFileResult = ofSystemSaveDialog("snippet.xml", "Save your Snippet");
    
    if(openFileResult.bSuccess){
        snippetName = openFileResult.getPath();
    }
    
    bool saveOk = true;
    bool a;
    bool b;
    // Delete and create xml file
    if (XML.loadFile(snippetName)) {
        XML.clear();
//        a = XML.saveFile();
    } else {
        b = XML.saveFile(snippetName);
        XML.loadFile(snippetName);
    }
    
    
    map<int,int> idMap;// = new map<int, int>;
    int idAux = 1;
    for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
        if(it->second->bActive) {
            idMap[it->second->getId()] = idAux;
            idAux++;
        }
    }
    for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++ ){
        saveOk = saveOk && it->second->saveSnippetPatch(snippetName, idMap, XML);
    }
    
    XML.saveFile(snippetName);
    
    return saveOk;
}
/*****************************************************************************************/

int ofxComposer::getMaxIdPatch(){
    vector<int> v;
    for(map<int,patch*>::iterator it = patches.begin(); it != patches.end(); it++) {
        v.push_back(it->first);
    }
    
    return *(std::max_element(v.begin(), v.end()));
}

void ofxComposer::addPatch(patch *p){
    
    patches[p->getId()] = p;
    
    ofAddListener( p->title->close , this, &ofxComposer::closePatch);
    
    p->setMainCanvas(this->gui);
    p->setParent(*this->getParent());
}








