#include "ofApp.h"

using namespace cv;
using namespace ofxCv;


/* ================================================ */
/*                      SETUP                       */
/* ================================================ */

void ofApp::setup() {
    
    ofSetEscapeQuitsApp(false);
    
    //*** MAIN SETUP ***//
    //
    xmlFileName            = "Untitled.xml";
    xmlFilePath            = "";
    loadingOK              = true;
    isFullScreen           = false;
    midiLearnActive        = false;
    editAudioInActive      = false;
    editOSCActive          = false;
    audioAnalizer          = NULL;
    currentViewer          = 0;
    holdingCommand         = false;
    holdingControl         = false;
    holdingDelete          = false;
    holdingShift           = false;
    showInspectors         = false;
    activatePatches        = false;
    
    //populating string dictionaries for simple comparison used in LoadFromXML
    inputTypes.insert(std::pair<string,InputType>("AUDIO_ANALIZER",AUDIO_ANALIZER));
    inputTypes.insert(std::pair<string,InputType>("CAM",CAM));
    inputTypes.insert(std::pair<string,InputType>("IMAGE_AND_VIDEO_LIST",IMAGE_AND_VIDEO_LIST));
    inputTypes.insert(std::pair<string,InputType>("PARTICLE",PARTICLE));
    inputTypes.insert(std::pair<string,InputType>("LEFT_AUDIO_IN",LEFT_AUDIO_IN));
    inputTypes.insert(std::pair<string,InputType>("RIGHT_AUDIO_IN",RIGHT_AUDIO_IN));
    inputTypes.insert(std::pair<string,InputType>("OSC_RECEIVER",OSC_RECEIVER));
    inputTypes.insert(std::pair<string,InputType>("SYPHON_CLIENT",SYPHON_CLIENT));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("IKEDA", IKEDA));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("GLITCH_1", GLITCH_1));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("GLITCH_2", GLITCH_2));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("IMAGE_PROCESSOR", IMAGE_PROCESSOR));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("SHADER", SHADER));
    mixerTypes.insert(std::pair<string,MixerType>("MASK", MASK));
    mixerTypes.insert(std::pair<string,MixerType>("SIMPLE_BLEND", SIMPLE_BLEND));
    mixerTypes.insert(std::pair<string,MixerType>("MULTI_CHANNEL", MULTI_CHANNEL));
    inputGenTypes.insert(std::pair<string,paramInputType>("MIDI", MIDI));
    inputGenTypes.insert(std::pair<string,paramInputType>("FFT", FFT));
    inputGenTypes.insert(std::pair<string,paramInputType>("OSC", OSC));
    
    
    //*** CAMERA SETUP ***//
    //
    cam.setDistance(600);
    cam.disableMouseInput();
    cam.enableOrtho();
    cam.setVFlip(true);
    scale = 1.f;
    
    
    //*** TRACKPAD SETUP ***//
    //
    pad = ofxMultiTouchPad();
    

    //*** TOP MENU ***//
    //
    menu = new ofxUISuperCanvas("menu", 0, MENU_TOP_PADDING, ofGetWidth(), MENU_HEIGHT);
    menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    menu->setColorBack(ofxUIColor(140, 140, 140,255));
    ofxUISpacer* spacer;
    new menuItem(menu, "MultiImageButton", "New Patcher (cmd+n)", "assets/new_file.png", false, RIGHT_MENU_WIDTH, 20);
    new menuItem(menu, "MultiImageButton", "Open Patcher (cmd+o)", "assets/open_file.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE + MENU_ITEM_PADDING, 20);
    new menuItem(menu, "MultiImageButton", "Save Patcher (cmd+s)", "assets/save_file.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*2 + MENU_ITEM_PADDING*2, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*3 + MENU_ITEM_PADDING*3, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Create Node (n)", "assets/node.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*3 + MENU_ITEM_PADDING*4, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*4 + MENU_ITEM_PADDING*5, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Save Snippet (cmd+k)", "assets/save_snippet.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*4 + MENU_ITEM_PADDING*6, 20);
    new menuItem(menu, "MultiImageButton", "Open Snippet (cmd+j)", "assets/open_snippet.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*5 + MENU_ITEM_PADDING*7, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*6 + MENU_ITEM_PADDING*8, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Straight Links", "assets/line.png", true, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*6 + MENU_ITEM_PADDING*9, 20);
    new menuItem(menu, "MultiImageToggle", "Curved Links", "assets/curve_line.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*7 + MENU_ITEM_PADDING*10, 20);
    new menuItem(menu, "MultiImageToggle", "Segmented Links", "assets/path_line.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*8 + MENU_ITEM_PADDING*11, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*9 + MENU_ITEM_PADDING*12, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Edit Mode on/off (F2)", "assets/edit_mode.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*9 + MENU_ITEM_PADDING*13, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*10 + MENU_ITEM_PADDING*14, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Console on/off (cmd+c)", "assets/console.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*10 + MENU_ITEM_PADDING*15, 20);
    new menuItem(menu, "MultiImageButton", "Clear Console (cmd+shift+c)", "assets/clear_console.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*11 + MENU_ITEM_PADDING*16, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*12 + MENU_ITEM_PADDING*17, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Encapsulate", "assets/encapsulate.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*12 + MENU_ITEM_PADDING*18, 20);
    new menuItem(menu, "MultiImageButton", "Uncapsulate", "assets/uncapsulate.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*13 + MENU_ITEM_PADDING*19, 20);
    new menuItem(menu, "MultiImageButton", "Open encapsulated (cmd+e)", "assets/open_encapsulated.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*14 + MENU_ITEM_PADDING*20, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*15 + MENU_ITEM_PADDING*21, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Midi Learn", "assets/midi_learn.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*15 + MENU_ITEM_PADDING*22, 20);
    
    new menuItem(menu, "MultiImageButton", "Print Shortcuts (F1)", "assets/help.png", false, ofGetWidth() - 30, 20);
    
    ofAddListener(menu->newGUIEvent,this,&ofApp::menuEvent);
    
    
    //*** RIGHT MENU ***//
    //
    right_menu = new ofxUISuperCanvas("menu", 0, MENU_HEIGHT + MENU_TOP_PADDING, RIGHT_MENU_WIDTH, ofGetHeight() - (MENU_HEIGHT + MENU_TOP_PADDING));
    right_menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    right_menu->setColorBack(ofxUIColor(140, 140, 140,255));
    
    new menuItem(right_menu, "MultiImageButton", "Zoom In", "assets/zoom_in.png", false, 3, right_menu->getRect()->getHeight()-30);
    new menuItem(right_menu, "MultiImageButton", "Zoom Out", "assets/zoom_out.png", false, 3, right_menu->getRect()->getHeight()-60);
    new menuItem(right_menu, "MultiImageToggle", "Analizer", "assets/analizer.png", false, 3, right_menu->getRect()->getHeight()-100);
    
    ofAddListener(right_menu->newGUIEvent,this,&ofApp::menuEvent);
    
    
    //*** MAIN CANVAS ***//
    //
    gui = new ofxUISuperCanvas("", RIGHT_MENU_WIDTH, MENU_HEIGHT + MENU_TOP_PADDING, ofGetWidth() - RIGHT_MENU_WIDTH, ofGetHeight() - (MENU_HEIGHT +MENU_TOP_PADDING));
    gui->setColorBack(ofxUIColor(255,255,255,0));
    gui->setDraggable(false);
    
    
    //*** SYPHON INPUT SERVER ***//
    //
    SyphonClientHandler::getInstance();
    
    
    //*** NODE VIEWR SETUP (COMPOSER) ***//
    //
    NodeViewer* nV = new NodeViewer(xmlFileName);
    nV->setNodesCount(0);
    nV->setParent(cam);
    nV->setLinkType(STRAIGHT_LINKS);
    gui->setMainComposer(nV);
    right_menu->setMainComposer(nV);
    menu->setMainComposer(nV);
    nodeViewers.push_back(nV);
    setCurrentViewer(0);
    
    
    //*** SCROLL BAR SETUP ***//
    //
    this->scrollBars = new scrollBar(nodeViewers[currentViewer], &this->pad, &cam, SCROLL_BAR_EVENT_PRIORITY);
    scrollBars->setup();
    
    
    //*** CONSOLE SETUP ***//
    //
    console = ConsoleLog::getInstance();
    console->setupScrollBar(&pad);
    ofSetLogLevel(OF_LOG_ERROR);
    ofAddListener(console->changeConsoleHeight, this, &ofApp::consoleHeightChanged);
    
    
    //*** AUDIO SETUP ***//
    //
    setupAudio();
    
    //*** OSC SETUP ***//
//    oscInputGeneratorPortMap = new map<int, OscInputGenerator*>();

}

//------------------------------------------------------------------
void ofApp::setupAudio(){
    
    bufferCounter	= 0;
   
    left = new float[BUFFER_SIZE];
    right = new float[BUFFER_SIZE];
    
    // creating audio analizer
    //
    if (audioAnalizer == NULL) {
        initAudioAnalizer();
    }
    
    setSelectedForAudioIn();
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // BUFFER_SIZE samples per buffer
    // 4 num buffers (latency)
    
    ofSoundStreamSetup(0, 2, this, 44100, BUFFER_SIZE, 4);
}

//------------------------------------------------------------------
void ofApp::setSelectedForAudioIn(){
    
    // setting nodes attributes selected for audio in
    //
    AudioIn* audioInNode;
    vector<DTAudioMap*>* am;
    bool isLeftChannel = false;
    std::map<int, ImageOutput*>::iterator node_;
    
    for (int i = 0; i < audioListeners.size(); i++){
        
        am = audioListeners[i]->getAudioMap();
        audioInNode = (AudioIn*)nodes.find(audioListeners[i]->getNodeID())->second;
        isLeftChannel = audioInNode->getAudioInType() == LEFT;
        
        for (int j = 0; j < am->size(); j++){
            node_ = nodes.find(am->at(j)->nodeId);
            if (node_ != nodes.end()) {
                node_->second->setAttributesForAudioIn(am->at(j)->paramId, isLeftChannel, audioListeners[i]->getNodeID());
            }
        }
        
        listenToAudioInEvents(audioInNode, true);
    }
}

//------------------------------------------------------------------
void ofApp::setSelectedForOSC(){
    
    // setting nodes attributes selected for OSC
    //
    OSCReceiver* oscNode;
    std::map<string, DTOscMap*>* om;
    std::map<int, ImageOutput*>::iterator node_;
    
    for(map<int,OscInputGenerator*>::iterator it = oscInputGeneratorPortMap.begin(); it != oscInputGeneratorPortMap.end(); it++ ){
            
        om = it->second->oscMap;

        for (std::map<string,DTOscMap* >::iterator it2 = om->begin(); it2 != om->end(); it2++) {
            
            oscNode = (OSCReceiver*)findOSCNodeForAddress(it2->first, it->first);
            if (oscNode != NULL) {
            
                for (int j = 0; j < it2->second->nodeId.size(); j++) {
                    
                    node_ = nodes.find(it2->second->nodeId[j]);
                    if (node_ != nodes.end()) {
                        node_->second->setAttributesForOSC(it2->second->paramId[j], oscNode->getId());
                    }
                }
                
                listenToOSCEvents(oscNode, true);
            }
        }
    }
}


//------------------------------------------------------------------
ImageOutput* ofApp::findOSCNodeForAddress(string address_, int port_) {
    
    for(map<int,ImageOutput*>::iterator it = nodes.begin(); it != nodes.end(); it++ ){
        if (it->second->getTypeName() == "OSC Receiver") {
            if ((((OSCReceiver*)it->second)->getAddress() == address_) && (((OSCReceiver*)it->second)->getPort() == port_)) {
                return it->second;
            }
        }
    }
    
    return NULL;
}

//------------------------------------------------------------------
void ofApp::setSelectedForMIDI(){
    
    // setting nodes attributes selected for OSC
    //
    std::map<int,vector<DTMidiMap*> > midiMaps;
    std::map<int, ImageOutput*>::iterator node_;
    
    for (int i = 0; i < inputGenerators.size(); i++){
        
        if (inputGenerators[i]->getParamInputType() == MIDI) {
            
            midiMaps = ((MidiInputGenerator*)inputGenerators[i])->midiControlMaps;
            
            for (std::map<int,vector<DTMidiMap*> >::iterator it = midiMaps.begin(); it != midiMaps.end(); it++) {
                for (int j = 0; j < it->second.size(); j++) {
                    
                    node_ = nodes.find(it->second[j]->nodeId);
                    if (node_ != nodes.end()) {
                        node_->second->setAttributesForMIDI(it->second[j]->paramId, it->second[j]->control);
                    }
                }
            }
        }
    }
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void ofApp::update() {
    
    if(loadingOK){
        
        //if pressing zoom in/out buttons
        if (menu_zoom_in) {
            scale = cam.getScale().x;
            float scaleAux = scale - 10*SCALE_SENSITIVITY;
            if(scaleAux > MIN_SCALE){
                scale -= 10*SCALE_SENSITIVITY;
                cam.setScale(scale);
            } else {
                ConsoleLog::getInstance()->pushWarning("Max zoom in/out reached");
                menu_zoom_in = false;
            }
        }
        else if (menu_zoom_out) {
            scale = cam.getScale().x;
            float scaleAux = scale + 10*SCALE_SENSITIVITY;
            if(scaleAux < MAX_SCALE){
                scale += 10*SCALE_SENSITIVITY;
                cam.setScale(scale);
            } else {
                ConsoleLog::getInstance()->pushWarning("Max zoom in/out reached");
                menu_zoom_out = false;
            }
        }
        
        //getting new paraminputs
        for (int i=0; i<inputGenerators.size(); ++i) {
            ParamInputGenerator* p = inputGenerators[i];
            
            int maxParams = 0;
            bool continueProcessing = true;
            while(maxParams<=5 && continueProcessing){
                Param* param = p->getNextInputMessage();
                
                //sending param info to the destination node
                if(param != NULL){
                    
                    if (p->getParamInputType() == MIDI && midiLearnActive) {
                        
                        if (holdingControl) {
                            for (int i=0; i<nodesVector.size(); ++i) {
                                nodesVector[i]->setMidiControlActive(param->midiControl);
                            }
                        }
                        else {
                            
                            map<int, vector<string> > attributesSelected = nodeViewers[currentViewer]->getAttributesClicked();
                            std::map<int, ImageOutput*>::iterator node_;
                            
                            // editing midi control maps
                            //
                            if (attributesSelected.size() > 0) {
                                
                                // reseting active midi control on GUI params
                                //
                                for (int i=0; i<nodesVector.size(); ++i) {
                                    nodesVector[i]->resetMidiControlActive(param->midiControl);
                                }
                                
                                for(map<int, vector<string> >::iterator it = attributesSelected.begin(); it != attributesSelected.end(); it++ ){
                                    node_ = nodes.find(it->first);
                                    if (node_ != nodes.end()) {
                                        ((MidiInputGenerator*)p)->addNewMidiMap(param->midiControl, node_->second, it->second);
                                    }
                                }
                            }
                            // removing all midi control maps
                            //
                            else if (holdingDelete){
                                
                                // removing all midi control active on GUI params
                                //
                                for (int i=0; i<nodesVector.size(); ++i) {
                                    nodesVector[i]->resetMidiControlActive(param->midiControl);
                                }
                                
                                ((MidiInputGenerator*)p)->midiControlMaps[param->midiControl].clear();
                            }
                        }
                    }
                    else if (!midiLearnActive){
                        ImageOutput* destinationNode = NULL;
                        std::map<int,ImageOutput*>::iterator it;

                        it = nodes.find(param->imageInputId);
                        
                        if(it!=nodes.end()){
                            it->second->updateParameter(param);
                            delete param;
                        }
                        maxParams += 1;
                    }
                }
                else{
                    continueProcessing = false;
                }
            }
        }

        //delete removed widgets
        if (widgetsToDelete.size() > 0) {
            for(auto widget : widgetsToDelete) {
                if (newNodeInput == widget)
                    newNodeInput = NULL;
                gui->removeWidget(widget);
            }
            widgetsToDelete.clear();
        }
    
        //update right menu menuitems in case window have been resized
        right_menu->getWidget("Zoom In")->getRect()->setY(right_menu->getRect()->getHeight()-30);
        right_menu->getWidget("Zoom Out")->getRect()->setY(right_menu->getRect()->getHeight()-60);
        right_menu->getWidget("Analizer")->getRect()->setY(right_menu->getRect()->getHeight()-100);
        menu->getWidget("Print Shortcuts (F1)")->getRect()->setX(menu->getRect()->getWidth()-30);
        
        scrollBars->update();
        nodeViewers[currentViewer]->update();
        
        if (showConsole) {
            console->update();
        }
    }
}

//------------------------------------------------------------------
void ofApp::draw() {
    //create bg
    ofClear(35);

    // draw nodes
    if(loadingOK){
        nodeViewers[currentViewer]->draw();
    }
    else{
        ofDrawBitmapString("ERROR LOADING XML", 50, 50);
    }
    
    //update menu's width and height
    menu->setWidth(ofGetWidth());
    right_menu->setHeight(ofGetHeight() - (MENU_HEIGHT + MENU_TOP_PADDING));
    
    //draw console
    if (showConsole) {
        console->draw();
    }
    
    //draw scrollbars
    scrollBars->draw();
    
    //draw framerate
    ofDrawBitmapString(ofToString(ofGetFrameRate(),0), ofGetWidth() - 50, ofGetHeight()-35);
}

//------------------------------------------------------------------
void ofApp::updateSyphon(ofFbo & img){
    if(loadingOK){
        
    }
}

//------------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // samples are "interleaved"
    for (int i = 0; i < bufferSize; i++){
        left[i] = input[i*2];
        right[i] = input[i*2+1];
    }
    
    for (int i=0; i<audioListeners.size(); i++){
        // for each audio listener, i copy the audiobuffer and send it to it.
        
        float * leftToGo = new float[BUFFER_SIZE];
        float * rightToGo = new float[BUFFER_SIZE];
        
        memcpy ( leftToGo, left, sizeof (float)*BUFFER_SIZE );
        memcpy ( rightToGo, right, sizeof (float)*BUFFER_SIZE );
        
        if(!audioListeners[i]->fillNewData(leftToGo, rightToGo, BUFFER_SIZE)){
            // if data copy was not successful, then we delete the memory (because the input gen wont delete it)
            delete[] leftToGo;
            delete[] rightToGo;
        }
    }
    bufferCounter++;
    
    if (audioAnalizer != NULL) audioAnalizer->analyze(input);
}
/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                     EVENTS                       */
/* ================================================ */

void ofApp::keyPressed  (int key){
    bool notAvailable = false;
    switch(key){
        case OF_KEY_LEFT_COMMAND: case OF_KEY_RIGHT_COMMAND:
            holdingCommand = true;
            break;
        case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
            holdingControl = true;
            break;
        case OF_KEY_BACKSPACE:
            holdingDelete = true;
            break;
        case OF_KEY_SHIFT:
            holdingShift = true;
            break;
        case 'f': case 'F':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                isFullScreen = !isFullScreen;
                ofSetFullscreen(isFullScreen);
            }
            break;
        case OF_KEY_DEL: case OF_KEY_ESC:
            if ((newNodeInput != NULL) && ((ofxUITextInput*) newNodeInput)->isClicked()) {
                gui->removeWidget(newNodeInput);
                newNodeInput = NULL;
            }
            break;
        case 'n': case 'N' :
            if(!holdingCommand){
                if (nodeViewers[currentViewer]->getEdit()) {
                    if (newNodeInput == NULL)
                        this->createNodeInput();
                    else if (!newNodeInput->isClicked()){
                        newNodeInput->getRect()->setX(ofGetMouseX());
                        newNodeInput->getRect()->setY(ofGetMouseY());
                        
                        newNodeInput->getDropdownList()->getRect()->setX(ofGetMouseX());
                        newNodeInput->getDropdownList()->getRect()->setY(ofGetMouseY());
                    }
                }
                else {
                    console->pushWarning("Unable to create nodes while Edit Mode is off");
                }
            } else {
                if (nodeViewers[currentViewer]->getPatches().size() > 1) {
                    saveToXML();
                }
                xmlFileName = "Untitle.xml";
                xmlFilePath = "";
                deleteEverything();
                
                initAudioAnalizer();
            }
            break;
        case 'j': case 'J' :
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                loadSnippet();
            }
            break;
        case 'k': case 'K' :
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                saveSnippet();
            }
            break;
        case 'e': case 'E':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                int encapsulatedId = nodeViewers[currentViewer]->getSelectedEncapsulated();
                EventHandler::getInstance()->setEncapsulatedIdDraw(encapsulatedId);
            }
            break;
        case 'i': case 'I':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                showInspectors = !showInspectors;
                nodeViewers[currentViewer]->deactivateAllPatches();
                nodeViewers[currentViewer]->setDrawInspectors(showInspectors);
            }
            break;
        case 'A': case 'a':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                activatePatches = !activatePatches;
                activatePatches ? nodeViewers[currentViewer]->activateAllPatches() : nodeViewers[currentViewer]->deactivateAllPatches();
            }
            break;
        case 's': case 'S':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                saveToXML();
            }
            break;
        case 'o': case 'O':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                loadFromXML();
            }
            break;
        case 'c': case 'C':
            if (holdingCommand && ((newNodeInput == NULL) || (!newNodeInput->isClicked()))) {
                if (holdingShift && showConsole) {
                    console->clearMessages();
                }
                else {
                    if(showConsole){
                        scrollBars->changeWindowHeight(1);
                    }else{
                        scrollBars->changeWindowHeight(console->getWindowRatio());
                    }
                    showConsole = !showConsole;
                    console->setConsoleVisible(showConsole);
                }
            }
            break;
        case OF_KEY_F1:
            printShortcuts();
        default:
            notAvailable = true;
            break;
    }
    
    // sending keyboard input to all input generators
    for (int i = 0; i<inputGenerators.size();i++){
        inputGenerators[i]->keyPressed(key);
    }
    
}

//------------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    switch(key){
        case 'n': case 'N' :
            if (newNodeInput != NULL && !newNodeInput->isClicked())
                newNodeInput->setFocus(true);
        break;
            
        case OF_KEY_LEFT_CONTROL: case OF_KEY_RIGHT_CONTROL:
            if (midiLearnActive) {
                for (int i = 0; i < nodesVector.size(); ++i) {
                    nodesVector[i]->setMidiControlActive(-1);
                }
            }
            holdingControl = false;
        break;
        case OF_KEY_LEFT_COMMAND: case OF_KEY_RIGHT_COMMAND:
            holdingCommand = false;
            break;
        case OF_KEY_BACKSPACE:
            holdingDelete = false;
            break;
        case OF_KEY_SHIFT:
            holdingShift = false;
            break;
        case 'e': case 'E':
            if(EventHandler::getInstance()->getEncapsulatedIdDraw() != MAIN_WINDOW){
                EventHandler::getInstance()->setEncapsulatedIdDraw(MAIN_WINDOW);
            }
        break;
    }
}

//------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    if (showConsole && console->getContainer().inside(x,y)) {
        nodeViewers[currentViewer]->deactivateAllPatches();
    }
}

//------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    menu_zoom_in = false;
    menu_zoom_out = false;
}

//------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
    if( dragInfo.files.size() > 0 ){
        
        bool foundNode = false;
        
        for(int i = 0; i < dragInfo.files.size(); i++){
            
            textInputEvent* ev = new textInputEvent();
            ofFile file = ofFile(dragInfo.files[i]);
            string ext = file.getExtension();
            
            if ((ext == "jpg") || (ext == "JPG") ||
                (ext == "jpeg")|| (ext == "JPEG")||
                (ext == "png") || (ext == "PNG") ||
                (ext == "gif") || (ext == "GIF") ||
                (ext == "bmp") || (ext == "BMP") ||
                (ext == "mov") || (ext == "MOV") ||
                (ext == "mpg") || (ext == "MPG") ||
                (ext == "mp4") || (ext == "MP4") ||
                (ext == "m4v") || (ext == "M4V") ){
                
                ev->type = "image or video";
            }
            else if ((ext == "mov") || (ext == "MOV") ||
                     (ext == "mpg") || (ext == "MPG") ||
                     (ext == "mp4") || (ext == "MP4") ||
                     (ext == "m4v") || (ext == "M4V") ){
                
                 ev->type = "video player";
            }
            else if( (ext == "frag") || (ext == "FRAG") ||
                     (ext == "fs") || (ext == "FS") ) {
                ev->type = "shader";
            }
            else {
                console->pushError("Not valid file format to create a node.");
                return;
            }
            
            foundNode = false;
            for (int j = 0; j < inputs.size(); j ++) {
                if (inputs[j]->getTypeName() == "Image & Video List" && inputs[j]->isOver(dragInfo.position*cam.getGlobalTransformMatrix())){
                    ((ImageAndVideoInputList*)inputs[j])->loadImage(file.getFileName(), dragInfo.files[i]);
                    console->pushSuccess("File " + file.getFileName()
                                         + " was succesfully added as a new sequence to the node "
                                         + inputs[j]->getName() + ".");
                    file.close();
                    foundNode = true;
                    break;
                }
            }
            
            if (!foundNode) {
                ev->path   = dragInfo.files[i];
                ev->point  = dragInfo.position;
                ev->name   = file.getFileName();
                ev->widget = NULL;
                createNode(*ev);
                file.close();
            }
        }
    }
}

//------------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    ofShowCursor();

}

//------------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//------------------------------------------------------------------
void ofApp::menuEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
    
    if (name == "New Patcher (cmd+n)") {
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            if (nodeViewers[currentViewer]->getPatches().size() > 1) {
                saveToXML();
            }
            xmlFileName = "Untitle.xml";
            xmlFilePath = "";
            deleteEverything();
            
            initAudioAnalizer();
        }
    }
    else if (name == "Open Patcher (cmd+o)") {
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            loadFromXML();
        }
    }
    else if (name == "Straight Links") {
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        nodeViewers[currentViewer]->setLinkType(STRAIGHT_LINKS);
    }
    else if (name == "Curved Links") {
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        nodeViewers[currentViewer]->setLinkType(CURVE_LINKS);
    }
    else if (name == "Segmented Links") {
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        nodeViewers[currentViewer]->setLinkType(PATH_LINKS);
    }
    else if (name == "Create Node (n)") {
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            if (nodeViewers[currentViewer]->getEdit()) {
                if (newNodeInput == NULL) {
                    this->createNodeInput((ofGetWidth()/2)-75, ofGetHeight()/2);
                }
                else if (!newNodeInput->isClicked()){
                    newNodeInput->getRect()->setX((ofGetWidth()/2)-75);
                    newNodeInput->getRect()->setY(ofGetHeight()/2);
                    
                    newNodeInput->getDropdownList()->getRect()->setX((ofGetWidth()/2)-75);
                    newNodeInput->getDropdownList()->getRect()->setY(ofGetHeight()/2);
                    
                    newNodeInput->setFocus(true);
                }
            }
            else {
                console->pushWarning("Unable to create nodes while Edit Mode is off");
            }
        }
    }
    else if (name == "Edit Mode on/off (F2)") {
        
        if (nodeViewers[currentViewer]->getEdit())
            nodeViewers[currentViewer]->setEdit(false);
        else nodeViewers[currentViewer]->setEdit(true);
    }
    else if (name == "Zoom In"){
        
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            menu_zoom_in = true;
        }
    }
    else if (name == "Zoom Out"){
        
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            menu_zoom_out = true;
        }
    }
    else if (name == "Analizer"){
        
        if(((ofxUIMultiImageToggle*)e.widget)->getValue() == false){
            audioAnalizer->setDrawAudioAnalizer(false);
            audioAnalizer->setDrawInspector(false);
        }
        else {
            audioAnalizer->setDrawAudioAnalizer(true);
        }
    }
    else if (name == "Save Snippet (cmd+k)"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            saveSnippet();
        }
    }
    else if (name == "Open Snippet (cmd+j)"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            loadSnippet();
        }
    }
    else if (name == "Save Patcher (cmd+s)"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            saveToXML();
        }
    }
    else if (name == "Console on/off (cmd+c)"){
        if(showConsole){
            scrollBars->changeWindowHeight(1);
        }else{
            scrollBars->changeWindowHeight(console->getWindowRatio());
        }
        showConsole = !showConsole;
        console->setConsoleVisible(showConsole);
    }
    else if (name == "Clear Console (cmd+shift+c)"){
        console->clearMessages();
    }
    
    else if (name == "Encapsulate"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            int encapsulatedId = nodeViewers[currentViewer]->encapsulate();
            encapsulatedIds.push_back(encapsulatedId);
        }
    }
    
    else if (name == "Uncapsulate"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            int encapsulatedId = nodeViewers[currentViewer]->getSelectedEncapsulated();
            if(encapsulatedId < 0){
                console->pushError("No encapsulated node is selected");
            } else {
                int lastPatchId = nodeViewers[currentViewer]->getLastPatchEncapsulated(encapsulatedId);
                nodeViewers[currentViewer]->restoreOutputEncapsulated(lastPatchId);
                nodeViewers[currentViewer]->uncapsulate(encapsulatedId);
                for (std::vector<int>::iterator it = encapsulatedIds.begin() ; it != encapsulatedIds.end(); ++it){
                    if(*it == encapsulatedId){
                        encapsulatedIds.erase(it);
                        break;
                    }
                }
            }
        }
    }
    else if (name == "Open encapsulated (cmd+e)"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            if(EventHandler::getInstance()->getEncapsulatedIdDraw() != MAIN_WINDOW){
                EventHandler::getInstance()->setEncapsulatedIdDraw(MAIN_WINDOW);
            } else {
                int encapsulatedId = nodeViewers[currentViewer]->getSelectedEncapsulated();
                if(encapsulatedId < 0){
                    console->pushError("No encapsulated node is selected");
                } else {
                    EventHandler::getInstance()->setEncapsulatedIdDraw(encapsulatedId);
                }
            }
        }
    }
    else if (name == "Midi Learn") {
        
        if (!editAudioInActive && !editOSCActive) {
            midiLearnActive = !midiLearnActive;
            nodeViewers[currentViewer]->setMidiLearnActive(midiLearnActive);
            
            int i = 0;
            for (int i = 0; i < inputGenerators.size(); i++) {
                if(inputGenerators[i]->getParamInputType() == MIDI) {
                    ((MidiInputGenerator*)inputGenerators[i])->setMidiLearnActive(midiLearnActive);
                }
            }
            
            if (midiLearnActive) {
                console->pushMessage("");
                console->pushMessage("*** MIDI Learn is ON ***");
                console->pushMessage("Click on any nodes parameters to create a map to a MIDI control.");
                console->pushMessage("Delete all MIDI control mappings with DEL key.");
                console->pushMessage("To view what is map to a MIDI control, press CONTROL.");
            }
            else {
                console->pushMessage("*** MIDI Learn is OFF ***");
                console->pushMessage("");
            }
        }
        else {
            ((ofxUIMultiImageToggle*)menu->getWidget("Midi Learn"))->setValue(false);
        }
    }
    else if (name == "Print Shortcuts (F1)") {
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            printShortcuts();
        }
    }
}
/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                 OTHER FUNCTIONS                  */
/* ================================================ */

void ofApp::nextViewer(){
    int tViewer = currentViewer;
    ++tViewer;
    tViewer %= nodeViewers.size();
    setCurrentViewer(tViewer);
}

//------------------------------------------------------------------
void ofApp::previousViewer(){
    int tViewer = currentViewer;
    tViewer = (tViewer == 0) ? nodeViewers.size()-1 : --tViewer;
    setCurrentViewer(tViewer);
}

//------------------------------------------------------------------
void ofApp::setCurrentViewer(int currentViewer_){
    if (currentViewer_ >= 0 && currentViewer_ < nodeViewers.size()) {
        currentViewer = currentViewer_;
    }else{
        ofLogNotice() << "choosen viewer not available";
    }
}

//------------------------------------------------------------------
void ofApp::createNodeInput(float _x, float _y){
    
    textInput *node = new textInput("", "", 250, 20, _x, _y);
    vector<string> nodes;
    node->setMidiIn(&midiIn);
    ofxUIDropDownList *dlist = new ofxUIDropDownList("", nodes, 250, _x, _y);
    
    gui->addWidget(dlist);
    gui->addWidget(node);
    
    node->setAutoUnfocus(false);
    node->setAutoClear(false);
    node->setColorBack(ofxUIColor (80,80,80,250));
    node->setColorOutlineHighlight(ofxUIColor(150,150,250));
    node->setDropdownList(dlist);
    dlist->setColorBack(ofxUIColor (80,80,80,0));
    dlist->setColorFill(ofxUIColor (80,80,80,0));
    dlist->setColorFillHighlight(ofxUIColor (80,80,80,0));
    dlist->getLabelWidget()->setColorBack(ofxUIColor (80,80,80,0));
    dlist->getLabelWidget()->setColorFill(ofxUIColor (80,80,80,0));
    dlist->getLabelWidget()->setColorFillHighlight(ofxUIColor (80,80,80,0));
    
    ofAddListener( node->createNode , this, &ofApp::createNode);
    
    newNodeInput = node;
}

//------------------------------------------------------------------
void ofApp::createNode(textInputEvent &args){
    
    NodeElement* nE;
    ImageOutput* newPatch = NULL;
    ofVec3f position(args.point.x, args.point.y, 0.0);
    position = position*cam.getGlobalTransformMatrix();
    bool exist = false;
    
    if (args.type == "audio in - left channel") {
        newPatch = new AudioIn(gui, "Audio In - Left Channel", "New Audio In - Left Channel");
        ((AudioIn*)newPatch)->setChannel(0);
        inputs.push_back((AudioIn*)newPatch);
    }
    else if (args.type == "audio in - right channel") {
        newPatch = new AudioIn(gui, "Audio In - Right Channel", "New Audio In - Right Channel");
        ((AudioIn*)newPatch)->setChannel(1);
        inputs.push_back((AudioIn*)newPatch);
    }
    else if (args.type == "osc receiver") {
        newPatch = new OSCReceiver();
        ((OSCReceiver*)newPatch)->setPort(6666);
        inputs.push_back((OSCReceiver*)newPatch);
    }
    else if (args.type == "camera") {
        newPatch = new InputCamera();
        inputs.push_back((InputCamera*)newPatch);
    }
    else if (args.type == "glitch 1") {
        newPatch = new GlitchLayer();
        visualLayers.push_back((GlitchLayer*)newPatch);
    }
    else if (args.type == "glitch 2") {
        newPatch = new GlitchLayerAlt();
        visualLayers.push_back((GlitchLayerAlt*)newPatch);
    }
    else if (args.type == "ikeda") {
        newPatch = new IkedaLayer();
        visualLayers.push_back((IkedaLayer*)newPatch);
    }
    else if (args.type == "shader") {
        newPatch = new ShaderLayer();
        bool loaded = ((ShaderLayer*)newPatch)->loadShader(args.path, args.name);
        if(loaded){
            visualLayers.push_back((ShaderLayer*)newPatch);
        } else {
            delete newPatch;
            newPatch = NULL;
        }
    }
    else if (args.type == "image or video") {
        newPatch = new ImageAndVideoInputList();
        ((ImageAndVideoInputList*)newPatch)->loadImage(args.name, args.path);
        inputs.push_back((ImageAndVideoInputList*)newPatch);
    }
    else if (args.type == "image processor") {
        newPatch = new ImageProcessor();
        visualLayers.push_back((ImageProcessor*)newPatch);
    }
    else if (args.type == "mix mask") {
        newPatch = new MixMask();
        mixtables.push_back((MixMask*)newPatch);
    }
    else if (args.type == "mix simple blend") {
        newPatch = new MixSimpleBlend();
        mixtables.push_back((MixSimpleBlend*)newPatch);
    }
    else if (args.type == "multi channel switch") {
        newPatch = new MultiChannelSwitch();
        mixtables.push_back((MultiChannelSwitch*)newPatch);
    }
    else if (args.type == "particle generator") {
        newPatch = new ParticleGenerator();
        inputs.push_back((ParticleGenerator*)newPatch);
    }
    else if (args.type == "syphon client"){
        newPatch = SyphonClientHandler::getInstance()->createSyphonPatch();
        inputs.push_back((InputSyphon*)newPatch);
    }
    else if (args.type == "syphon output"){
        newPatch = new CustomSyphonServer();
        syphonServers.push_back((CustomSyphonServer*)newPatch);
    }
    else {
        for(vector<string>::iterator it = midiIn.getPortList().begin(); it != midiIn.getPortList().end(); it++ ){
            if (args.type == it->data()) {
                int j = 0;
                while (j < inputGenerators.size()) {
                    if (inputGenerators[j]->getParamInputType() == MIDI
                        && ((MidiInputGenerator*)inputGenerators[j])->getMidiDeviceName() == args.type) {
                        
                        j = inputGenerators.size()+1;
                    }
                    else {
                        j++;
                    }
                }
                if (j <= inputGenerators.size()) {
                    MidiInputGenerator* mI = new MidiInputGenerator("midi", args.type);
                    inputGenerators.push_back(mI);
                    mI->setMidiIn(&midiIn);
                    mI->setup();
                    mI->start();
                }
            }
        }
    }
    
    if (newPatch && !exist) {
        nE = new NodeElement(newPatch);
        nodeViewers[currentViewer]->addElement(nE, position);
        nodes.insert(std::pair<int, ImageOutput*>(newPatch->getId(), newPatch));
        nodesVector.push_back(newPatch);
        initNode(newPatch);
        newPatch->resetSize();
        
        if (newPatch->getIsAudio()){
            
            AudioInputGenerator* aI = new AudioInputGenerator(newPatch->getName(), newPatch->getId());
            aI->setChannel(((AudioIn*)newPatch)->getChannel());
            aI->setBand(((AudioIn*)newPatch)->getBand());
            
            inputGenerators.push_back(aI);
            audioListeners.push_back(aI);
            aI->setup();
            aI->start();
            
            listenToAudioInEvents((AudioIn*)newPatch, true);
        }
        else if (newPatch->getIsOSCReceiver()) {
            
            // search for OSC Input Generator with port 6666
            std::map<int, OscInputGenerator*>::iterator it;
            it = oscInputGeneratorPortMap.find(6666);
            
            if (it != oscInputGeneratorPortMap.end()) {
                it->second->oscReceiverIn();
            }
            // create new OSC Input Generator with port 6666
            else {
                OscInputGenerator* oscI = new OscInputGenerator("");
                oscI->setPort(6666);
            
                inputGenerators.push_back(oscI);
                oscInputGeneratorPortMap.insert(std::pair<int,OscInputGenerator*>(6666,oscI));
                oscI->setup();
                oscI->start();
            }
            listenToOSCEvents((OSCReceiver*)newPatch, true);
        }
    }
    if (args.widget != NULL) {
        ofRemoveListener(((textInput*)args.widget)->createNode , this, &ofApp::createNode);
        widgetsToDelete.push_back(args.widget); // delete input from canvas
    }
}

//------------------------------------------------------------------
void ofApp::initNode(ofxPatch* node) {
    node->title->setParent(cam);
    node->getPanel()->setParent(cam);
    node->setParent(cam);
    node->setup();
    node->setLinkType(nodeViewers[currentViewer]->getLinkType());
    ofAddListener( node->title->close , this, &ofApp::closePatch);
    
    ofAddListener(((ImageOutput*)node)->editOSCInputs , this, &ofApp::editOSCInputs);
    
//    ImageOutput* oscNode_;
//    
//    for(map<int,OscInputGenerator*>::iterator it = oscInputGeneratorPortMap.begin(); it != oscInputGeneratorPortMap.end(); it++ ){
//        
//        for (std::map<string,DTOscMap* >::iterator it2 = it->second->oscMap->begin(); it2 != it->second->oscMap->end(); it2++) {
//            
//            oscNode_ = (OSCReceiver*)findOSCNodeForAddress(it2->first);
//            if (oscNode_ != NULL) {
//                ofAddListener(((ImageOutput*)node)->editOSCInputs , this, &ofApp::editOSCInputs);
//            }
//        }
//    }
    
    
//    std::map<int, ImageOutput*>::iterator oscNode_;
//    
//    for (int i = 0; i < inputGenerators.size(); ++i) {
//        ParamInputGenerator* p = inputGenerators[i];
//        if ((p->getParamInputType() == OSC)) {
//            oscNode_ = nodes.find(((OscInputGenerator*)p)->getNodeID());
//            
//            if (oscNode_ != nodes.end()) {
//                ofAddListener(((ImageOutput*)node)->editOSCInputs , this, &ofApp::editOSCInputs);
//            }
//        }
//    }
}

//------------------------------------------------------------------
void ofApp::initAudioAnalizer() {
    
    audioAnalizer = new AudioAnalizer();
    audioAnalizer->setDrawAudioAnalizer(false);
    ((ofxUIMultiImageToggle*)right_menu->getWidget("Analizer"))->setValue(false);
    initNode(audioAnalizer);
    nodeViewers[currentViewer]->addPatch(audioAnalizer, ofPoint((ofGetWidth()/2)-100, (ofGetHeight()/2)-50));
}

//------------------------------------------------------------------
void ofApp::closePatch(int &_nID) {
    
    if (audioAnalizer->getId() == _nID) {
        audioAnalizer->setDrawAudioAnalizer(false);
        audioAnalizer->setDrawInspector(false);
        ((ofxUIMultiImageToggle*)right_menu->getWidget("Analizer"))->setValue(false);
    }
    else {
        ImageOutput* nodeToDelete = nodes.find(_nID)->second;
        int i = 0;
        
        // If is Audio or OSC
        // delete it from input generators
        //
        if (nodeToDelete->getIsAudio() || nodeToDelete->getIsOSCReceiver()) {
            if (nodeToDelete->getIsAudio()) {
                i = 0;
                while (i < audioListeners.size()) {
                    if (audioListeners[i]->getParamInputType() == FFT && ((AudioListenerInput*)audioListeners[i])->getNodeID() == _nID) {
                        audioListeners.erase(audioListeners.begin() + i);
                    }
                    i++;
                }
                listenToAudioInEvents((AudioIn*)nodeToDelete, false);
            }
            
            
            OscInputGenerator* oscToDelete = NULL;
            if (nodeToDelete->getIsOSCReceiver()) {
                std::map<int, OscInputGenerator*>::iterator it = oscInputGeneratorPortMap.find(((OSCReceiver*)nodeToDelete)->getPort());
                
                if (it != oscInputGeneratorPortMap.end()) {
                    // remove osc mappings for node to delete
                    it->second->getOSCMapForAddress(((OSCReceiver*)nodeToDelete)->getAddress());
                    
                    if (it->second->getNumberOSCReceiver() == 0) {
                        oscToDelete = it->second;
                        oscInputGeneratorPortMap.erase(it);
                    }
                }
            }
            
            while (i < inputGenerators.size()) {
                
                if ((inputGenerators[i]->getParamInputType() == FFT && ((AudioListenerInput*)inputGenerators[i])->getNodeID() == _nID)
                    || (inputGenerators[i]->getParamInputType() == OSC && ((OscInputGenerator*)inputGenerators[i]) == oscToDelete)) {
                    
                    delete inputGenerators[i];
                    inputGenerators.erase(inputGenerators.begin() + i);
                }
                i++;
            }
        }
        // else if not
        // delete params from input generators
        //
        else {
            ImageOutput* oscNode;
            
            for(map<int,OscInputGenerator*>::iterator it = oscInputGeneratorPortMap.begin(); it != oscInputGeneratorPortMap.end(); it++ ){
                
                for (std::map<string,DTOscMap* >::iterator it2 = it->second->oscMap->begin(); it2 != it->second->oscMap->end(); it2++) {
                    
                    oscNode = (OSCReceiver*)findOSCNodeForAddress(it2->first, it->first);
                    if (oscNode != NULL) {

                        ((OSCReceiver*)oscNode)->removeNodeParams(_nID);
                        it->second->removeNodeFromParams(_nID);
                        ofRemoveListener(nodeToDelete->editOSCInputs , this, &ofApp::editOSCInputs);
                    }
                }
            }
        }
        
        // delete the Patch
        //
        bool found = false;
        i = 0;
        
        if (nodeToDelete->getNodeType() == INPUT) {
            
            if (editAudioInActive) {
                editAudioInActive = !editAudioInActive;
                
                ((AudioIn*)nodeToDelete)->getChannel() == 0
                ? nodeViewers[currentViewer]->setEditLeftAudioInActive(false, -1)
                : nodeViewers[currentViewer]->setEditRightAudioInActive(false, -1);
            }
            else if (editOSCActive){
                editOSCActive = !editOSCActive;
                nodeViewers[currentViewer]->setEditOSCActive(false, -1);
            }
            
            while (i < inputs.size() && !found) {
                if (inputs[i]->getId() == _nID) {
                    inputs.erase(inputs.begin() + i);
                    found = true;
                }
                i++;
            }
        }
        else if (nodeToDelete->getNodeType() == VISUAL_LAYER) {
            while (i < visualLayers.size() && !found) {
                if (visualLayers[i]->getId() == _nID) {
                    visualLayers.erase(visualLayers.begin() + i);
                    found = true;
                }
                i++;
            }
        }
        else if (nodeToDelete->getNodeType() == MIXER) {
            while (i < mixtables.size() && !found) {
                if (mixtables[i]->getId() == _nID) {
                    mixtables.erase(mixtables.begin() + i);
                    found = true;
                }
                i++;
            }
        }
        else if (nodeToDelete->getIsSyphonServer()) {
            while (i < syphonServers.size() && !found) {
                if (syphonServers[i]->getId() == _nID) {
                    syphonServers.erase(syphonServers.begin() + i);
                    found = true;
                }
                i++;
            }
        }

        ofRemoveListener(nodeToDelete->title->close , this, &ofApp::closePatch);
        
        found = false;
        i = 0;
        while (i < nodesVector.size()) {
            if(nodesVector[i]->getId() == _nID) {
                nodesVector.erase(nodesVector.begin() + i);
            }
            i++;
        }
        
        nodes.erase(_nID);
        nodeViewers[currentViewer]->closePatch(_nID);
    }
}

//------------------------------------------------------------------
void ofApp::editAudioInBand(AudioInEvent &e_){

    for (int i = 0; i < audioListeners.size(); ++i) {
        if (audioListeners[i]->getNodeID() == e_.nodeId) {
            ((AudioInputGenerator*)audioListeners[i])->setBand(e_.band);
            break;
        }
    }
}

//------------------------------------------------------------------
void ofApp::editAudioIn(AudioInEvent &edit_){
    
    if (midiLearnActive) {
        ((ofxUIMultiImageToggle*)menu->getWidget("Midi Learn"))->setValue(false);
        
        midiLearnActive = false;
        
        for (int i = 0; i < inputGenerators.size(); i++) {
            if (inputGenerators[i]->getParamInputType() == MIDI) {
                ((MidiInputGenerator*)inputGenerators[i])->setMidiLearnActive(false);
            }
        }
    }
    
    if (editAudioInActive){
        
        for (int i=0; i<inputGenerators.size(); ++i) {
            ParamInputGenerator* p = inputGenerators[i];
            if ((p->getParamInputType() == FFT) && (((AudioInputGenerator*)p)->getNodeID() == edit_.nodeId)) {
                
                std::map<int, ImageOutput*>::iterator audioInNode_;
                audioInNode_ = nodes.find(edit_.nodeId);
                
                if (audioInNode_ != nodes.end()) {
                
                    ((AudioInputGenerator*)p)->clearAudioMap();
                    
                    map<int, vector<string> > attributesSelected = nodeViewers[currentViewer]->getAttributesClicked();
                    std::map<int, ImageOutput*>::iterator node_;
                    
                    for(map<int, vector<string> >::iterator it = attributesSelected.begin(); it != attributesSelected.end(); it++ ){
                        node_ = nodes.find(it->first);
                        if (node_ != nodes.end()) {
                            ((AudioInputGenerator*)p)->addNewAudioMap(((AudioIn*)audioInNode_->second)->getBand(), ((AudioIn*)audioInNode_->second)->getChannel(), node_->second, it->second);
                        }
                    }
                }
            }
        }
    }
    
    editAudioInActive = edit_.active;
    edit_.channel == 0
        ? nodeViewers[currentViewer]->setEditLeftAudioInActive(edit_.active, edit_.nodeId)
        : nodeViewers[currentViewer]->setEditRightAudioInActive(edit_.active, edit_.nodeId);
};

//------------------------------------------------------------------
void ofApp::editAudioInSaturation(AudioInEvent &edit_){
    
    for (int i = 0; i < audioListeners.size(); ++i) {
        if (audioListeners[i]->getNodeID() == edit_.nodeId) {
            ((AudioInputGenerator*)audioListeners[i])->setSaturation(edit_.saturation);
            break;
        }
    }
}

//------------------------------------------------------------------
void ofApp::editAudioInEnabled(AudioInEvent &edit_){
    
    for (int i = 0; i < audioListeners.size(); ++i) {
        if (audioListeners[i]->getNodeID() == edit_.nodeId) {
            ((AudioInputGenerator*)audioListeners[i])->setEnable(edit_.enable);
            break;
        }
    }
}

//------------------------------------------------------------------
void ofApp::listenToOSCEvents(OSCReceiver *osc, bool listen) {
    
    if (listen){
        ofAddListener(osc->editOSCPort , this, &ofApp::editOSCPort);
        ofAddListener(osc->editOSCMinMaxValues , this, &ofApp::editOSCMinMaxValues);
        ofAddListener(osc->editOSCInputsActive , this, &ofApp::editOSCInputsActive);
        
        for(std::map<int, ImageOutput*>::iterator it = nodes.begin(); it != nodes.end(); it++ ){
            if (it->second != osc) {
                ofAddListener(it->second->editOSCInputs , this, &ofApp::editOSCInputs);
            }
        }
    }
    else {
        ofRemoveListener(osc->editOSCPort , this, &ofApp::editOSCPort);
        ofRemoveListener(osc->editOSCMinMaxValues , this, &ofApp::editOSCMinMaxValues);
        ofRemoveListener(osc->editOSCInputsActive , this, &ofApp::editOSCInputsActive);
        
        for(std::map<int, ImageOutput*>::iterator it = nodes.begin(); it != nodes.end(); it++ ){
            if (it->second != osc) {
                ofRemoveListener(it->second->editOSCInputs , this, &ofApp::editOSCInputs);
            }
        }
    }
}

//------------------------------------------------------------------
void ofApp::listenToAudioInEvents(AudioIn* audio, bool listen) {
    
    if (listen){
        ofAddListener(audio->editAudioIn , this, &ofApp::editAudioIn);
        ofAddListener(audio->editAudioInBand , this, &ofApp::editAudioInBand);
        ofAddListener(audio->editAudioInSaturation , this, &ofApp::editAudioInSaturation);
        ofAddListener(audio->editAudioInEnabled , this, &ofApp::editAudioInEnabled);
    }
    else {
        ofRemoveListener(audio->editAudioIn , this, &ofApp::editAudioIn);
        ofRemoveListener(audio->editAudioInBand , this, &ofApp::editAudioInBand);
        ofRemoveListener(audio->editAudioInSaturation , this, &ofApp::editAudioInSaturation);
        ofRemoveListener(audio->editAudioInEnabled , this, &ofApp::editAudioInEnabled);
    }
}

//------------------------------------------------------------------
void ofApp::editOSCPort(OSCEvent &e_) {
    
    // search for OSC Input Generator with old port
    std::map<int, OscInputGenerator*>::iterator it;
    it = oscInputGeneratorPortMap.find(e_.oldPort);
    
    if (it != oscInputGeneratorPortMap.end()) {
        
        // change address
        if (e_.oldAddress != e_.address) {
            it->second->setAddress(e_.oldAddress, e_.address);
        }
        // change port
        else if (e_.oldPort != e_.port) {
            
            DTOscMap* oscmap = NULL;
            
            // removing osc mappings from old port
            if (it != oscInputGeneratorPortMap.end()) {
                oscmap = it->second->getOSCMapForAddress(e_.address);
                
                
                if (it->second->getNumberOSCReceiver() == 0) {
                    
                    // delete Osc Input Generator
                    OscInputGenerator* oscToDelete = it->second;
                    oscInputGeneratorPortMap.erase(it);
                    
                    int i = 0;
                    while (i < inputGenerators.size()) {
                        
                        if ((inputGenerators[i]->getParamInputType() == OSC && ((OscInputGenerator*)inputGenerators[i]) == oscToDelete)) {
                            
                            delete inputGenerators[i];
                            inputGenerators.erase(inputGenerators.begin() + i);
                            i = inputGenerators.size();
                        }
                        i++;
                    }
                }
            }
            
            // add osc mappings to new port
            it = oscInputGeneratorPortMap.find(e_.port);
            if (it != oscInputGeneratorPortMap.end()) {
                if (oscmap != NULL) {
                    it->second->addOSCMapForAddress(e_.address, oscmap);
                }
                else {
                    it->second->oscReceiverIn();
                }
            }
            // create new OSC Input Generator
            else {
                OscInputGenerator* newOSCInput = new OscInputGenerator("");
                newOSCInput->setPort(e_.port);
                
                if (oscmap != NULL) {
                    newOSCInput->oscMap->insert(std::pair<string,DTOscMap* >(e_.address,oscmap));
                }
                oscInputGeneratorPortMap.insert(std::pair<int,OscInputGenerator*>(e_.port,newOSCInput));
                inputGenerators.push_back(newOSCInput);
                newOSCInput->setup();
                newOSCInput->start();
            }
        }
    }
}

//------------------------------------------------------------------
void ofApp::editOSCMinMaxValues(OSCEvent &e_) {
    
    std::map<int, OscInputGenerator*>::iterator it;
    it = oscInputGeneratorPortMap.find(e_.port);
    
    if (it != oscInputGeneratorPortMap.end()) {
        it->second->setMin(e_.address, e_.min);
        it->second->setMax(e_.address, e_.max);
    }
}

//------------------------------------------------------------------
void ofApp::editOSCInputsActive(OSCEvent &edit_) {

    editOSCActive = !editOSCActive;
    nodeViewers[currentViewer]->setEditOSCActive(editOSCActive, edit_.nodeId);
}

//------------------------------------------------------------------
void ofApp::editOSCInputs(ofxOSCGuiEvent &event_) {

    if (editOSCActive){
        
        std::map<int, ImageOutput*>::iterator oscNode_;
        oscNode_ = nodes.find(event_.oscNodeId);
        
        if (oscNode_ != nodes.end()) {
            
            // get OSC Input Generator for port: event_.oscNodePort
            std::map<int, OscInputGenerator*>::iterator it;
            it = oscInputGeneratorPortMap.find(((OSCReceiver*)oscNode_->second)->getPort());
            
            if (it != oscInputGeneratorPortMap.end()) {
                
                std::map<int, ImageOutput*>::iterator nodeToAdd_ = nodes.find(event_.nodeId);
                
                if (nodeToAdd_ != nodes.end()) {
                    if (event_.add) {
                        it->second->addNewOSCMap(((OSCReceiver*)oscNode_->second)->getOSCMin(), ((OSCReceiver*)oscNode_->second)->getOscMax(),
                                                 ((OSCReceiver*)oscNode_->second)->getAddress(), nodeToAdd_->second, event_.paramName);
                        ((OSCReceiver*)oscNode_->second)->addParameter(nodeToAdd_->second->getId(), event_.paramName);
                    }
                    else {
                        it->second->removeOSCMap(((OSCReceiver*)oscNode_->second)->getAddress(), nodeToAdd_->second, event_.paramName);
                        ((OSCReceiver*)oscNode_->second)->removeParameter(nodeToAdd_->second->getId(), event_.paramName);
                    }
                }
            }
        }
    }
}

//ofApp::editOSCInputs(ofxOSCGuiEvent &event_) {
//    
//    if (editOSCActive){
//        for (int i = 0; i < inputGenerators.size(); ++i) {
//            ParamInputGenerator* p = inputGenerators[i];
//            if ((p->getParamInputType() == OSC) &&  (((OscInputGenerator*)p)->getNodeID() == event_.oscNodeId)) {
//                std::map<int, ImageOutput*>::iterator oscNode_;
//                oscNode_ = nodes.find(event_.oscNodeId);
//                
//                if (oscNode_ != nodes.end()) {
//                    
//                    std::map<int, ImageOutput*>::iterator nodeToAdd_ = nodes.find(event_.nodeId);
//                    
//                    if (nodeToAdd_ != nodes.end()) {
//                        if (event_.add) {
//                            ((OscInputGenerator*)p)->addNewOSCMap(((OSCReceiver*)oscNode_->second)->getAddress(), nodeToAdd_->second, event_.paramName);
//                            ((OSCReceiver*)oscNode_->second)->addParameter(nodeToAdd_->second->getId(), event_.paramName);
//                        }
//                        else {
//                            ((OscInputGenerator*)p)->removeOSCMap(((OSCReceiver*)oscNode_->second)->getAddress(), nodeToAdd_->second, event_.paramName);
//                            ((OSCReceiver*)oscNode_->second)->removeParameter(nodeToAdd_->second->getId(), event_.paramName);
//                        }
//                    }
//                }
//            }
//        }
//    }
//}

//------------------------------------------------------------------
void ofApp::consoleHeightChanged(float &ratio){
    
    scrollBars->changeWindowHeight(ratio); 
}

//------------------------------------------------------------------
bool ofApp::loadFromXML(){
    
    bool loadingOK = true;
    string errorMsg;
    nodeLinkType nodeLinkType;
    int nodesCount;
    
    ofDrawBitmapString("LOADING XML ...", 50, 50);
    
    ofFileDialogResult openFileResult;
    openFileResult = ofSystemLoadDialog("Select a NIMP settings file (.xml)");
    
    if (openFileResult.bSuccess){
        
        ofFile file (openFileResult.getPath());
        
        if (file.exists()){
            string fileExtension = ofToUpper(file.getExtension());
            
            if(fileExtension == "XML"){
                xmlFileName = openFileResult.getName();
                xmlFilePath = openFileResult.getPath();;
            }
            else {
                loadingOK = false;
                errorMsg = "File \"" + file.getFileName() + "\" is not valid.";
            }
        }
        file.close();
    }
    
    if (xmlFilePath == ""){
        return;
    }
    
    if( XML.loadFile(xmlFilePath) ){
        
        deleteEverything();
        
        int numMainSettingsTag = XML.getNumTags("MAIN_SETTINGS");
        if(numMainSettingsTag == 1) {
            
            XML.pushTag("MAIN_SETTINGS");
            
            int linkType = XML.getValue("link_type", 0);
            switch (linkType) {
                case 0:
                    nodeLinkType = STRAIGHT_LINKS;
                    ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(true);
                    break;
                case 1:
                    nodeLinkType = CURVE_LINKS;
                    ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(true);
                    ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
                    break;
                case 2:
                    nodeLinkType = PATH_LINKS;
                    ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(true);
                    ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
                    break;
            }
            
            nodesCount  = XML.getValue("total_nodes", -1);
            
            int numSettingsTag = XML.getNumTags("SETTINGS");
            
            if(numSettingsTag==1){
                
                XML.pushTag("SETTINGS");

                errorMsg = this->loadNodes(XML);
                loadingOK = errorMsg == "";
                
                XML.popTag();
            }
            else {
                loadingOK = false;
                errorMsg = "SETTINGS tag missing.";
            }
            
            if(loadingOK){
                //PROCESSING NODE_VIEWS
                
                int numNodeViews = XML.getNumTags("NODE_VIEWS");
                
                if(numNodeViews==1){
                    
                    XML.pushTag("NODE_VIEWS");
                    
                    int numNodeView = XML.getNumTags("NODE_VIEW");
                    
                    for(int i = 0; i < numNodeView; i++){
                        string nodeViewName = XML.getAttribute("NODE_VIEW","name","default",i);
                        
                        nodeViewers[currentViewer]->setName(nodeViewName);
                        nodeViewers[currentViewer]->setNodesCount(nodesCount);
                        
                        XML.pushTag("NODE_VIEW",i);
                        int numNODETag = XML.getNumTags("NODE");
                        
                        for(int j = 0; j < numNODETag; j++){
                            int    nodeId   = XML.getAttribute("NODE","id",0,j);
                            string nodeName = XML.getAttribute("NODE","name","default",j);
                            
                            int x            = ofToInt(XML.getAttribute("NODE","x","20",j));
                            int y            = ofToInt(XML.getAttribute("NODE","y","20",j));
                            int guiX         = ofToInt(XML.getAttribute("NODE","guiX","20",j));
                            int guiY         = ofToInt(XML.getAttribute("NODE","guiY","20",j));
                            int guiWidth     = ofToInt(XML.getAttribute("NODE","guiWidth","120",j));
                            float imageScale = ofToFloat(XML.getAttribute("NODE","imageScale","1",j));
                            
                            std::map<int,ImageOutput*>::iterator it;
                            
                            it = nodes.find(nodeId);
                            
                            if(it != nodes.end()){
                                ImageOutput* iO = it->second;
                                NodeElement* nE = new NodeElement(iO, x, y, guiX, guiY, guiWidth, imageScale);
                                
                                nodeViewers[currentViewer]->addElement(nE);
                            }
                            else{
                                loadingOK = false;
                                errorMsg = "node \"" + nodeName + "\" not found.";
                            }
                            
                        }
                        
                        //NODE_VIEW
                        XML.popTag();
                        
                    }
                    
                    //NODE_VIEWS POP
                    XML.popTag();
                    
                }
                else{
                    loadingOK = false;
                    errorMsg = "NODE_VIEWS tag missing.";
                }
                
                // PROCESSING PARAM INPUT GENERATORS
                
                if (loadingOK){
                    
                    int numParamInputs = XML.getNumTags("PARAM_INPUT_GENERATORS");
                    if(numParamInputs==1){
                        
                        XML.pushTag("PARAM_INPUT_GENERATORS");

                        int numInputGen = XML.getNumTags("INPUT_GEN");
                        
                        for(int j = 0; j < numInputGen; j++){
                            string inputName = XML.getAttribute("INPUT_GEN","name","default",j);
                            string inputType = XML.getAttribute("INPUT_GEN","type","MIDI",j);
                            string midiDeviceName = XML.getAttribute("INPUT_GEN","midiDeviceName","Oxygen 25",j);
                            int    nodeID = XML.getAttribute("INPUT_GEN","nodeId",-1,j);
                            XML.pushTag("INPUT_GEN",j);
                            switch(inputGenTypes[inputType]){
                                case MIDI:
                                {
                                    MidiInputGenerator* mI = new MidiInputGenerator(inputName, midiDeviceName);
                                    mI->loadSettings(XML, nodes);
                                    mI->setMidiIn(&midiIn);
                                    
                                    inputGenerators.push_back(mI);
                                    
                                    break;
                                }
                                case FFT:
                                {
                                    std::map<int,ImageOutput*>::iterator it = nodes.find(nodeID);
                                    
                                    if(it!=nodes.end()){
                                        AudioInputGenerator* aI = new AudioInputGenerator(inputName, nodeID);
                                        aI->loadSettings(XML);
                                        
                                        inputGenerators.push_back(aI);
                                        audioListeners.push_back(aI);
                                    }
                                    break;
                                }
                                case OSC:
                                {
                                    OscInputGenerator* oI = new OscInputGenerator(inputName);
                                    oI->loadSettings(XML);
                                    
                                    inputGenerators.push_back(oI);
                                    oscInputGeneratorPortMap.insert(std::pair<int,OscInputGenerator*>(oI->getPort(),oI));
                                    
                                    break;
                                }
                                default:
                                {
                                    loadingOK = false;
                                    errorMsg = "Unknown generator node type '" + inputType + "'.";
                                    break;
                                };
                                    
                            }
                            XML.popTag();
                            
                        }
                        
                        //Popping PARAM_INPUT_GENERATORS tags
                        XML.popTag();

                    }
                    else{
                        loadingOK = false;
                        errorMsg = "PARAM_INPUT_GENERATORS tag missing";
                    }
                }
            }
        }
        else{
            loadingOK = false;
            errorMsg = "File " + xmlFileName + " is not elegible for N.IMP";
        }
        
        if(!loadingOK){
            ofLog(OF_LOG_ERROR, errorMsg);
            console->pushError("XML couldn't be loaded: " + errorMsg);
            deleteEverything();
            initAudioAnalizer();
            
            ((ofxUIImageToggle*)menu->getWidget("Console on/off (cmd+c)"))->setValue(true);
            showConsole = true;
            return;
        }
    }
    else {
        loadingOK = false;
    }
    
    if(loadingOK){
        //populating the nodes vector for fast iteration in the main loop.
        for(int i=0; i<inputs.size();i++){
            nodesVector.push_back(inputs[i]);
        }
        for(int i=0; i<visualLayers.size();i++){
            nodesVector.push_back(visualLayers[i]);
        }
        for(int i=0; i<mixtables.size();i++){
            nodesVector.push_back(mixtables[i]);
        }
        for(int i=0; i<syphonServers.size();i++){
            nodesVector.push_back(syphonServers[i]);
        }
        
        //setting inputs to every node
        bool error = false;
        for(int i=0; i<nodesVector.size(); i++){
            error = nodesVector[i]->findAndAssignInputs(nodes);
            if(error){
                errorMsg = "Error assigning inputs to node '" + nodesVector[i]->getName() + "'.";
                loadingOK = false;
                break;
            }
        }
        
        //init node view
        setCurrentViewer(0);
        nodeViewers[currentViewer]->setParent(cam);
        gui->setMainComposer(nodeViewers[currentViewer]);
        
        //create connections in nodeView
        for (int i=0; i<nodeViewers.size(); ++i) {
            nodeViewers[i]->createConnections();
            nodeViewers[i]->setLinkType(nodeLinkType);
        }
        
        if (audioAnalizer == NULL) {
            audioAnalizer = new AudioAnalizer();
            audioAnalizer->setDrawAudioAnalizer(false);
            ((ofxUIMultiImageToggle*)right_menu->getWidget("Analizer"))->setValue(false);
        }
        initNode(audioAnalizer);
        ((ofxUIMultiImageToggle*)right_menu->getWidget("Analizer"))->setValue(audioAnalizer->getDrawAudioAnalizer());
        nodeViewers[currentViewer]->addPatch(audioAnalizer, ofPoint(audioAnalizer->getLowestXCoord(),audioAnalizer->getHighestYCoord()));
    }
    
    if(loadingOK){
        
        console->pushMessage("All nodes loaded correctly.");
        
        //*** invoking input generators setup functions  ***//
        //
        for(int i=0; i<inputGenerators.size(); i++){
            inputGenerators[i]->setup();
        }
        
        //*** invoking nodes setup functions and setting main canvas and camera  ***//
        //
        for (int i=0; i<nodesVector.size(); ++i) {
            initNode(nodesVector[i]);
        }
        
        //*** starting input generators theads (the not threadeds will not start)  ***//
        //
        for(int i=0; i<inputGenerators.size(); i++){
            inputGenerators[i]->start();
        }
        
        //*** starting syphon servers  ***//
        //
        for(int i=0; i<syphonServers.size();i++){
            syphonServers[i]->setup();
            initNode(syphonServers[i]);
        }
        
        setSelectedForAudioIn();
        setSelectedForOSC();
        setSelectedForMIDI();
    }
    else {
        ofLog(OF_LOG_ERROR, errorMsg);
        console->pushError("XML couldn't be loaded: " + errorMsg);
    }
    
    return loadingOK;
}


//------------------------------------------------------------------
bool ofApp::saveToXML() {
    
    ofxXmlSettings XML;
    bool file = true;
    bool backupFile = false;
    bool success = true;
    string backupFilePath;
    
    // Open/create clean xml settings file
    //
    if (xmlFileName == "Untitle.xml") {
        
        ofFileDialogResult saveFileResult = ofSystemSaveDialog(xmlFileName, "Save your project");
        if (saveFileResult.bSuccess){
            xmlFileName = saveFileResult.getName();
            nodeViewers[currentViewer]->setName(xmlFileName);
            xmlFilePath = saveFileResult.getPath();
        }
        else {
            return;
        }
    }
    
    ofFile backup;
    if (backup.doesFileExist(xmlFilePath)) {
        backupFile = backup.open(xmlFilePath);
        if (backupFile) {
            backup.copyTo("~" + xmlFileName, true);
            backupFilePath = backup.getEnclosingDirectory() + "~" + xmlFileName;
            backup.close();
            backup.open(backupFilePath);
        }
    }
    
    file = XML.loadFile(xmlFilePath);
    
    if (file) {
        XML.clear();
    } else {
        file = XML.saveFile(xmlFilePath);
        XML.loadFile(xmlFilePath);
    }
    
    if (file){
        
        XML.clear();
        
        XML.addTag("MAIN_SETTINGS");
        XML.pushTag("MAIN_SETTINGS");
        
        XML.setValue("link_type", nodeViewers[currentViewer]->getLinkType() );
        XML.setValue("total_nodes", nodeViewers[currentViewer]->getNodesCount() );
        XML.setValue("show_console", showConsole );
        
        XML.addTag("SETTINGS");
        XML.pushTag("SETTINGS");
        
        // Save Inputs
        //
        XML.addTag("INPUTS");
        XML.pushTag("INPUTS");
        audioAnalizer->saveSettings(XML);
        for (int i = 0; i < inputs.size() && success; i++) {
            success = inputs[i]->saveSettings(XML);
        }
        XML.popTag();
        
        
        // Save Visual Layers
        //
        XML.addTag("VISUAL_LAYERS");
        XML.pushTag("VISUAL_LAYERS");
        for (int vl = 0; vl < visualLayers.size() && success; vl++) {
            success = visualLayers[vl]->saveSettings(XML);
        }
        XML.popTag();
        
        
        // Save Mixers
        //
        XML.addTag("MIXERS");
        XML.pushTag("MIXERS");
        for (int m = 0; m < mixtables.size() && success; m++) {
            success = mixtables[m]->saveSettings(XML);
        }
        XML.popTag();
        
        
        // Save Encapsulated Nodes
        //
        XML.addTag("ENCAPSULATED_NODES");
        XML.pushTag("ENCAPSULATED_NODES");
        for(int e = 0; e < encapsulatedIds.size() && success; e++){
            success = nodeViewers[currentViewer]->saveEncapsulatedSettings(XML, encapsulatedIds[e]);
        }
        XML.popTag();
        
        
        // Save Syphon Servers
        XML.addTag("SYPHON_SERVERS");
        XML.pushTag("SYPHON_SERVERS");
        for (int ss = 0; ss < syphonServers.size() && success; ss++) {
            success = syphonServers[ss]->saveSettings(XML);
        }
        XML.popTag(); // tag SYPHON_SERVERS
        
        
        XML.popTag(); // tag SETTINGS
        
        
        // Save Node Views
        //
        XML.addTag("NODE_VIEWS");
        XML.pushTag("NODE_VIEWS");
        for (int nv = 0; nv < nodeViewers.size() && success; nv++) {
            success = nodeViewers[nv]->saveSettings(XML);
        }
        XML.popTag(); // tag NODE_VIEWS
        
        
        // Save Param Input Generators
        //
        XML.addTag("PARAM_INPUT_GENERATORS");
        XML.pushTag("PARAM_INPUT_GENERATORS");
        for (int ig = 0; ig < inputGenerators.size() && success; ig++) {
            success = inputGenerators[ig]->saveSettings(XML);
        }
        XML.popTag(); // tag PARAM_INPUT_GENERATORS
        
        XML.popTag(); // tag MAIN_SETTINGS
        
        if (success && XML.saveFile()) {
            console->pushSuccess("Settings saved succesfully.");
        }
        else {
            if (backupFile) {
                backup.copyTo(xmlFileName, true, true);
            }
            else {
                XML.clear();
                XML.saveFile();
            }
            console->pushError("Couldn't save settings. An error occurred.");
        }
    }
    else {
        
        if (backupFile) {
            backup.copyTo(xmlFileName, true, true);
        }
        else {
            XML.clear();
            XML.saveFile();
        }
        console->pushError("Couldn't load the .xml file. The file " + xmlFileName + " was not found.");
    }
    
    if (backupFile) {
        backup.remove();
        backup.close();
    }
}

//------------------------------------------------------------------
string ofApp::loadNodes(ofxXmlSettings &XML){
    
    string errorMsg = "";
    bool result = true;
    int  numInputTag = XML.getNumTags("INPUTS");
    
    // LOADING INPUTS
    
    if(numInputTag==1){
        XML.pushTag("INPUTS");
        
        int numInputTag = XML.getNumTags("NODE");
        for(int i=0; i < numInputTag; i++){
            int    inputId   = XML.getAttribute("NODE","id",-1,i);
            string inputName = XML.getAttribute("NODE","name","default",i);
            string inputType = XML.getAttribute("NODE","type","CAM",i);
            
            switch(inputTypes[inputType]){
                case AUDIO_ANALIZER:
                {
                    audioAnalizer = new AudioAnalizer(inputName, inputId);
                    
                    if (!audioAnalizer->loadSettings(XML, i)) {
                        result = false;
                        errorMsg = "No Audio Analizer found";
                    }
                    
                    break;
                };
                case CAM:
                {
                    InputCamera* iC = new InputCamera(inputName, inputId);
                    iC->loadSettings(XML, i);
                    
                    inputs.push_back(iC);
                    nodes.insert(std::pair<int,ImageOutput*>(inputId,iC));
                    
                    break;
                };
                case IMAGE_AND_VIDEO_LIST:
                {
                    ImageAndVideoInputList* iI = new ImageAndVideoInputList(inputName, inputId);
                    
                    if (iI->loadSettings(XML, i)) {
                        inputs.push_back(iI);
                        nodes.insert(std::pair<int,ImageOutput*>(inputId,iI));
                    }
                    else {
                        result = false;
                        errorMsg = "Error loading node '" + inputName + "'. No images or videos found.";
                    }
                    
                    break;
                };
                case PARTICLE:
                {
                    ParticleGenerator* iI = new ParticleGenerator(inputName, inputId);
                    iI->loadSettings(XML, i);
                    
                    inputs.push_back(iI);
                    nodes.insert(std::pair<int,ImageOutput*>(inputId,iI));
                    
                    break;
                };
                case RIGHT_AUDIO_IN:
                {
                    AudioIn* aI = new AudioIn(gui, "Audio In - Right Channel", inputName, inputId);
                    aI->loadSettings(XML, i);
                    
                    inputs.push_back(aI);
                    nodes.insert(std::pair<int,ImageOutput*>(inputId,aI));
                    
                    break;
                };
                case LEFT_AUDIO_IN:
                {
                    AudioIn* aI = new AudioIn(gui, "Audio In - Left Channel", inputName, inputId);
                    aI->loadSettings(XML, i);
                    
                    inputs.push_back(aI);
                    nodes.insert(std::pair<int,ImageOutput*>(inputId,aI));
                    
                    break;
                };
                case OSC_RECEIVER:
                {
                    OSCReceiver* oI = new OSCReceiver(inputName, inputId);
                    oI->loadSettings(XML, i);
                    
                    inputs.push_back(oI);
                    nodes.insert(std::pair<int,ImageOutput*>(inputId,oI));

                    break;
                };
                case SYPHON_CLIENT:
                {
                    InputSyphon* iS = SyphonClientHandler::getInstance()->createSyphonPatch(inputName, inputId);
                    iS->setup();
                    iS->loadSettings(XML, i);
                    inputs.push_back(iS);
                    nodes.insert(std::pair<int,ImageOutput*>(inputId,iS));
                    break;
                };
                default:
                {
                    result = false;
                    errorMsg = "Unknown node type '" + inputType + "'.";
                    break;
                };
            }
            
            if(!result){
                //there has been an error
                //exit the loop
                break;
            }
            
        }
        XML.popTag();
        
    }
    else{
        errorMsg = "INPUTS tag missing.";
        result = false;
    }
    
    //LOADING VISUAL LAYERS
    
    if(result){
        int numVLsTag = XML.getNumTags("VISUAL_LAYERS");
        if(numVLsTag==1){
            
            XML.pushTag("VISUAL_LAYERS");
            
            int numVLTag = XML.getNumTags("NODE");
            
            for(int i = 0; i < numVLTag; i++){
                int    layerId    = XML.getAttribute("NODE","id",-1,i);
                string layerName  = XML.getAttribute("NODE","name","default",i);
                string layerType  = XML.getAttribute("NODE","type","IKEDA",i);
                int inputSourceId = XML.getAttribute("NODE","inputSource",0,i);
                
                switch(visualLayerTypes[layerType]){
                    case IKEDA:
                    {
                        IkedaLayer* iL = new IkedaLayer(layerName, layerId);
                        if (inputSourceId != 0)
                            iL->addInputIdentifier(inputSourceId);
                        iL->loadSettings(XML, i);
                        
                        visualLayers.push_back(iL);
                        nodes.insert(std::pair<int,ImageOutput*>(layerId,iL));
                        
                        break;
                    };
                    case GLITCH_1:
                    {
                        GlitchLayer* gL = new GlitchLayer(layerName, layerId);
                        if (inputSourceId != 0)
                            gL->addInputIdentifier(inputSourceId);
                        gL->loadSettings(XML, i);
                        
                        visualLayers.push_back(gL);
                        nodes.insert(std::pair<int,ImageOutput*>(layerId,gL));
                        
                        break;
                    };
                    case GLITCH_2:
                    {
                        GlitchLayerAlt* gLA = new GlitchLayerAlt(layerName, layerId);
                        if (inputSourceId != 0)
                            gLA->addInputIdentifier(inputSourceId);
                        gLA->loadSettings(XML, i);
                        
                        visualLayers.push_back(gLA);
                        nodes.insert(std::pair<int,ImageOutput*>(layerId,gLA));
                        
                        break;
                    };
                    case IMAGE_PROCESSOR:
                    {
                        
                        ImageProcessor* gLA = new ImageProcessor(layerName, layerId);
                        if (inputSourceId != 0)
                            gLA->addInputIdentifier(inputSourceId);
                        gLA->loadSettings(XML, i);
                        
                        
                        visualLayers.push_back(gLA);
                        nodes.insert(std::pair<int,ImageOutput*>(layerId,gLA));
                        
                        break;
                    };
                    case SHADER:
                    {
                        ShaderLayer* gLA = new ShaderLayer(layerName, layerId);
                        if (inputSourceId != 0)
                            gLA->addInputIdentifier(inputSourceId);
                        gLA->loadSettings(XML, i);
                        
                        
                        visualLayers.push_back(gLA);
                        nodes.insert(std::pair<int,ImageOutput*>(layerId,gLA));
                        
                        break;
                    }
                    default:
                    {
                        result = false;
                        errorMsg = "Unknown node type '" + layerType + "'.";
                        break;
                    };
                }
                
                if(!result){
                    //there has been an error
                    //exit the loop
                    break;
                }
            }
            
            XML.popTag();
        }
        else {
            errorMsg = "VISUAL_LAYERS tag missing.";
            result = false;
        }
    }
    
    //LOADING MIXERS
    if(result){
        int numMXsTag = XML.getNumTags("MIXERS");
        if(numMXsTag==1){
            XML.pushTag("MIXERS");
            
            int numMXTag = XML.getNumTags("NODE");
            
            for(int i = 0; i < numMXTag; i++){
                int    mixerId   = XML.getAttribute("NODE","id",-1,i);
                string mixerName = XML.getAttribute("NODE","name","default",i);
                string mixerType = XML.getAttribute("NODE","type","SIMPLE_BLEND", i);
                
                switch(mixerTypes[mixerType]){
                    case SIMPLE_BLEND:
                    {
                        MixSimpleBlend* mSB = new MixSimpleBlend(mixerName, mixerId);
                        mSB->loadSettings(XML, i);
                        
                        mixtables.push_back(mSB);
                        nodes.insert(std::pair<int, ImageOutput*>(mixerId, mSB));
                        
                        break;
                    };
                    case MASK:
                    {
                        MixMask* mMM = new MixMask(mixerName, mixerId);
                        mMM->loadSettings(XML, i);
                        
                        mixtables.push_back(mMM);
                        nodes.insert(std::pair<int, ImageOutput*>(mixerId, mMM));
                        
                        break;
                    };
                    case MULTI_CHANNEL:
                    {
                        MultiChannelSwitch* mMM = new MultiChannelSwitch(mixerName, mixerId);
                        mMM->loadSettings(XML, i);
                        
                        mixtables.push_back(mMM);
                        nodes.insert(std::pair<int, ImageOutput*>(mixerId, mMM));
                        
                        break;
                    };
                    default:
                    {
                        result = false;
                        errorMsg = "Unknown node type '" + mixerType + "'.";
                        break;
                    };
                        
                }
                
                if(!result){
                    //there has been an error
                    //exit the loop
                    break;
                }
                
            }
            //MIXERS POP
            XML.popTag();
            
        }
        
    }
    else{
        errorMsg = "MIXERS tag missing.";
        result = false;
    }
    
    // LOAD ENCAPSULATED NODES
    if(result){
        int numENsTag = XML.getNumTags("ENCAPSULATED_NODES");
        if(numENsTag==1){
            XML.pushTag("ENCAPSULATED_NODES");
            
            int numENTag = XML.getNumTags("ENCAPSULATED_NODE");
            
            for(int i = 0; i < numENTag; i++){
                int encapsulatedId     = XML.getAttribute("ENCAPSULATED_NODE","id",-1,i);
                int lastEncapsulatedId = XML.getAttribute("ENCAPSULATED_NODE","lastEncapsulatedId",false,i);

                nodes.at(lastEncapsulatedId)->setEncapsulatedId(encapsulatedId);
                nodes.at(lastEncapsulatedId)->setLastEncapsulated(true);
                
                XML.pushTag("ENCAPSULATED_NODE", i);
                int numEncapsulatedTag = XML.getNumTags("NODE");
                for(int j = 0; j < numEncapsulatedTag; j++){
                    int patchId = XML.getValue("NODE", -1, j);
                    if(patchId == -1){
                        result = false;
                        errorMsg = "Encapsulated data is wrong.";
                        break;
                    }
                    nodes.at(patchId)->setEncapsulatedId(encapsulatedId);
                    nodes.at(patchId)->setToEncapsulatedId(lastEncapsulatedId);
                }
                
                encapsulatedIds.push_back(encapsulatedId);
                
                // ENCAPSULATED_NODE POP
                XML.popTag();
                
                if(!result){
                    //there has been an error
                    //exit the loop
                    break;
                }
                
            }
            //ENCAPSULATED_NODES POP
            XML.popTag();
            
        } else {
            result = false;
            errorMsg = "ENCAPSULATED_NODES tag missing.";
        }
        
    }
    
    // PROCESSING SYPHON SERVERS
    if(result){
        int numServers = XML.getNumTags("SYPHON_SERVERS");
        if(numServers==1){
            XML.pushTag("SYPHON_SERVERS");
            
            int numSyphonServer = XML.getNumTags("NODE");
            
            for(int j=0; j<numSyphonServer; j++){
                int nodeId          = XML.getAttribute("NODE","id",-1,j);
                int inputSourceId   = XML.getAttribute("NODE","inputId",-1,j);
                string name         = XML.getAttribute("NODE","name","SyphonName",j);
                
                CustomSyphonServer* cSS;
                
                std::map<int,ImageOutput*>::iterator it;
                it=nodes.find(inputSourceId);
                
                if(it!=nodes.end()){
                    ImageOutput* iO = it->second;
                    cSS = new CustomSyphonServer(name, iO, nodeId);
                    cSS->addInputIdentifier(inputSourceId);
                }
                else{
                    cSS = new CustomSyphonServer(name, NULL, nodeId);
                }
                
                result = cSS->loadSettings(XML, j);
                
                if (result) {
                    syphonServers.push_back(cSS);
                    nodes.insert(std::pair<int, ImageOutput*>(nodeId, cSS));
                }
                else {
                    errorMsg = "Error loading Syphon Server '" + name + "'.";
                    break;
                }
            }
            //Popping SYPHON_SERVERS tags
            XML.popTag();
        }
        else {
            result = false;
            errorMsg = "SYPHON_SERVERS tag missing.";
        }
    }
    
    return errorMsg;
}

//------------------------------------------------------------------
void ofApp::deleteEverything() {
    
    for (int i = 0; i < inputGenerators.size(); i++){
        delete inputGenerators[i];
        inputGenerators[i] = NULL;
    }
    inputGenerators.clear();
    audioListeners.clear();
    oscInputGeneratorPortMap.clear();
    
    for (int i = 0; i < nodes.size(); i++){
        if (nodes[i] != NULL) ofRemoveListener( nodes[i]->title->close , this, &ofApp::closePatch);
    }
    
    if (audioAnalizer != NULL)
        ofRemoveListener( audioAnalizer->title->close , this, &ofApp::closePatch);
    
    for (int i = 0; i < nodeViewers.size(); i++){
        nodeViewers[i]->deleteEverything();
    }
    nodes.clear();
    nodesVector.clear();
    inputs.clear();
    visualLayers.clear();
    mixtables.clear();
    
    encapsulatedIds.clear();
    syphonServers.clear();
    
    audioAnalizer = NULL;
    
    nodeViewers[currentViewer]->setNodesCount(0);
}

/* ================================================ */
/* ================================================ */

// -----------------------------------------------------------
// ------------------------------------------------- SNNIPPETS
// -----------------------------------------------------------
bool ofApp::loadSnippet() {
    
    string snippetName = "";
    string errorMsg = "";
    bool result = true;
    
    ofFileDialogResult openFileResult;
    openFileResult = ofSystemLoadDialog("Select a snippet (.xml)");
    
    if (openFileResult.bSuccess){
        ofFile file (openFileResult.getPath());
        if (file.exists()){
            string fileExtension = ofToUpper(file.getExtension());
            
            if(fileExtension == "XML"){
                snippetName = openFileResult.getPath();
            } else return false;
        }
        file.close();
    }
    ofxXmlSettings XML;
    
    nodeViewers[currentViewer]->deactivateAllPatches();
    int nodesCount = nodeViewers[currentViewer]->getNodesCount();
    
    if (XML.loadFile(snippetName)){
        
        vector<ImageOutput*> aux_nodesVector;
        map<int, ImageOutput*> aux_nodes;
        vector<NodeElement*> snnipetNodeElements;
        bool result = true;
        
        if (XML.getNumTags("SNIPPET") == 1) {
            
            XML.pushTag("SNIPPET");
            int  numNodes = XML.getNumTags("NODE");
            for(int i = 0; i < numNodes; i++){
                
                int    nodeId     = XML.getAttribute("NODE","id",0,i) + nodesCount;
                string nodeName   = XML.getAttribute("NODE","name","default",i);
                string nodeType   = XML.getAttribute("NODE","type","CAM",i);
                int inputSourceId = XML.getAttribute("NODE","inputSource",0,i) + nodesCount;
                
                if (nodeType == "CAM") {
                     
                    InputCamera* iC = new InputCamera(nodeName, nodeId);
                    result = iC->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(iC);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,iC));
                        aux_nodesVector.push_back(iC);
                    }
                }
                else if (nodeType == "IMAGE_AND_VIDEO_LIST") {

                    ImageAndVideoInputList* iI = new ImageAndVideoInputList(nodeName, nodeId);
                    result = iI->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(iI);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,iI));
                        aux_nodesVector.push_back(iI);
                    }
                    else {
                        result = false;
                        errorMsg = "Error loading node '" + nodeName + "'. No images or videos found.";
                        break;
                    }
                }
                else if (nodeType == "PARTICLE") {
                    
                    ParticleGenerator* iI = new ParticleGenerator(nodeName, nodeId);
                    result = iI->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(iI);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,iI));
                        aux_nodesVector.push_back(iI);
                    }
                }
                else if (nodeType == "RIGHT_AUDIO_IN") {
                    
                    AudioIn* aI = new AudioIn(gui, "Audio In - Right Channel", nodeName, nodeId);
                    result = aI->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(aI);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,aI));
                        aux_nodesVector.push_back(aI);
                    }
                }
                else if (nodeType == "LEFT_AUDIO_IN") {
                    AudioIn* aI = new AudioIn(gui, "Audio In - Left Channel", nodeName, nodeId);
                    result = aI->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(aI);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,aI));
                        aux_nodesVector.push_back(aI);
                    }
                }
                else if (nodeType == "OSC_RECEIVER") {
                    OSCReceiver* oI = new OSCReceiver(nodeName, nodeId);
                    result = oI->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(oI);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,oI));
                        aux_nodesVector.push_back(oI);
                    }
                }
                else if (nodeType == "IKEDA") {
                    
                    IkedaLayer* iL = new IkedaLayer(nodeName, nodeId);
                    if (inputSourceId != nodesCount)
                        iL->addInputIdentifier(inputSourceId);
                    result = iL->loadSettings(XML, i, nodesCount);
                    if (result) {
                        visualLayers.push_back(iL);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,iL));
                        aux_nodesVector.push_back(iL);
                    }
                }
                else if (nodeType == "GLITCH_1") {
                    
                    GlitchLayer* gL = new GlitchLayer(nodeName, nodeId);
                    if (inputSourceId != nodesCount)
                        gL->addInputIdentifier(inputSourceId);
                    result = gL->loadSettings(XML, i, nodesCount);
                    if (result) {
                        visualLayers.push_back(gL);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,gL));
                        aux_nodesVector.push_back(gL);
                    }
                }
                else if (nodeType == "GLITCH_2") {
                    
                    GlitchLayerAlt* gLA = new GlitchLayerAlt(nodeName, nodeId);
                    if (inputSourceId != nodesCount)
                        gLA->addInputIdentifier(inputSourceId);
                    result = gLA->loadSettings(XML, i, nodesCount);
                    if (result) {
                        visualLayers.push_back(gLA);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,gLA));
                        aux_nodesVector.push_back(gLA);
                    }
                }
                else if (nodeType == "IMAGE_PROCESSOR") {
                    
                    ImageProcessor* gLA = new ImageProcessor(nodeName, nodeId);
                    if (inputSourceId != nodesCount)
                        gLA->addInputIdentifier(inputSourceId);
                    result = gLA->loadSettings(XML, i, nodesCount);
                    if (result) {
                        visualLayers.push_back(gLA);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,gLA));
                        aux_nodesVector.push_back(gLA);
                    }
                }
                else if (nodeType == "SHADER") {
                    
                    ShaderLayer* sL = new ShaderLayer(nodeName, nodeId);
                    if (inputSourceId != nodesCount)
                        sL->addInputIdentifier(inputSourceId);
                    result = sL->loadSettings(XML, i, nodesCount);
                    if (result) {
                        visualLayers.push_back(sL);
                        aux_nodes.insert(std::pair<int,ImageOutput*>(nodeId,sL));
                        aux_nodesVector.push_back(sL);
                    }
                }
                else if (nodeType == "SIMPLE_BLEND") {
                    
                    MixSimpleBlend* mSB = new MixSimpleBlend(nodeName, nodeId);
                    result = mSB->loadSettings(XML, i, nodesCount);
                    if (result) {
                        mixtables.push_back(mSB);
                        aux_nodes.insert(std::pair<int, ImageOutput*>(nodeId, mSB));
                        aux_nodesVector.push_back(mSB);
                    }
                }
                else if (nodeType == "MASK") {
                    
                    MixMask* mMM = new MixMask(nodeName, nodeId);
                    result = mMM->loadSettings(XML, i, nodesCount);
                    if (result) {
                        mixtables.push_back(mMM);
                        aux_nodes.insert(std::pair<int, ImageOutput*>(nodeId, mMM));
                        aux_nodesVector.push_back(mMM);
                    }
                }
                else if (nodeType == "MULTI_CHANNEL") {
                    
                    MultiChannelSwitch* mMM = new MultiChannelSwitch(nodeName, nodeId);
                    result = mMM->loadSettings(XML, i, nodesCount);
                    if (result) {
                        mixtables.push_back(mMM);
                        aux_nodes.insert(std::pair<int, ImageOutput*>(nodeId, mMM));
                        aux_nodesVector.push_back(mMM);
                    }
                }
                else if (nodeType == "SYPHON_CLIENT") {
                    InputSyphon* iS = SyphonClientHandler::getInstance()->createSyphonPatch(nodeName, nodeId);
                    iS->setup();
                    result = iS->loadSettings(XML, i, nodesCount);
                    if (result) {
                        inputs.push_back(iS);
                        aux_nodes.insert(std::pair<int, ImageOutput*>(nodeId, iS));
                        aux_nodesVector.push_back(iS);
                    }
                }
                else if (nodeType == "SYPHON_OUTPUT") {
                    CustomSyphonServer* So = new CustomSyphonServer(nodeName, NULL, nodeId);
                    So->setup();
                    result = So->loadSettings(XML, i, nodesCount);
                    if (result) {
                        syphonServers.push_back(So);
                        aux_nodes.insert(std::pair<int, ImageOutput*>(nodeId, So));
                        aux_nodesVector.push_back(So);
                    }
                }
                else {
                    result = false;
                    errorMsg = "Unknown node type '" + nodeType + "'.";
                    break;
                }
            }
            
            if (result) {
                int numENsTag = XML.getNumTags("ENCAPSULATED_NODES");
                if(numENsTag == 1){
                    
                    XML.pushTag("ENCAPSULATED_NODES");
                    
                    int numENTag = XML.getNumTags("ENCAPSULATED_NODE");
                    
                    for(int i = 0; i < numENTag; i++){
                        int encapsulatedId     = XML.getAttribute("ENCAPSULATED_NODE","id",-1,i) + nodesCount;
                        int lastEncapsulatedId = XML.getAttribute("ENCAPSULATED_NODE","lastEncapsulatedId",false,i) + nodesCount;
                        
                        aux_nodes.at(lastEncapsulatedId)->setEncapsulatedId(encapsulatedId);
                        aux_nodes.at(lastEncapsulatedId)->setLastEncapsulated(true);
                        
                        result = XML.pushTag("ENCAPSULATED_NODE", i);
                        if (result) {
                            
                            int numEncapsulatedTag = XML.getNumTags("NODE");
                            for(int j = 0; j < numEncapsulatedTag; j++){
                                
                                int patchId = XML.getValue("NODE", -1, j) + nodesCount;
                                if(patchId == -1){
                                    result = false;
                                    errorMsg = "Encapsulated data is wrong";
                                    break;
                                }
                                aux_nodes.at(patchId)->setEncapsulatedId(encapsulatedId);
                                aux_nodes.at(patchId)->setToEncapsulatedId(lastEncapsulatedId);
                            }
                            
                            encapsulatedIds.push_back(encapsulatedId);
                            
                            XML.popTag(); // tag ENCAPSULATED_NODE
                        }
                        
                        if(!result){
                            errorMsg = "Unable to load snippet " + snippetName + ". Some error occurred loading the encapsulated nodes.";
                            break;
                        }
                        
                    }
                    //ENCAPSULATED_NODES POP
                    XML.popTag();
                    
                }
            }
            
            if (result) {
                int numParamInputs = XML.getNumTags("PARAM_INPUT_GENERATORS");
                if(numParamInputs==1){
                    
                    result = XML.pushTag("PARAM_INPUT_GENERATORS");
                    if (result) {
                        
                        int numInputGen = XML.getNumTags("INPUT_GEN");
                        
                        for(int j = 0; j < numInputGen; j++){
                            
                            string inputName = XML.getAttribute("INPUT_GEN","name","default",j);
                            string inputType = XML.getAttribute("INPUT_GEN","type","MIDI",j);
                            string midiDeviceName = XML.getAttribute("INPUT_GEN","midiDeviceName","Oxygen 25",j);
                            int    nodeID = XML.getAttribute("INPUT_GEN","nodeId",-1,j) + nodesCount;
                            
                            result = XML.pushTag("INPUT_GEN",j);
                            if (result) {

                                switch(inputGenTypes[inputType]){
                                    case MIDI:
                                    {
                                        MidiInputGenerator* mI = new MidiInputGenerator(inputName, midiDeviceName);
                                        result = mI->loadSettings(XML, aux_nodes, nodesCount);
                                        if (result) {
                                            mI->setMidiIn(&midiIn);
                                            inputGenerators.push_back(mI);
                                        }
                                        break;
                                    }
                                    case FFT:
                                    {
                                        std::map<int,ImageOutput*>::iterator it = aux_nodes.find(nodeID);
                                        
                                        if(it!=aux_nodes.end()){
                                            AudioInputGenerator* aI = new AudioInputGenerator(inputName, nodeID);
                                            result = aI->loadSettings(XML, nodesCount);
                                            if (result) {
                                                inputGenerators.push_back(aI);
                                                audioListeners.push_back(aI);
                                            }
                                        }
                                        break;
                                    }
                                    case OSC:
                                    {
                                        OscInputGenerator* oI = new OscInputGenerator(inputName);
                                        result = oI->loadSettings(XML, nodesCount);
                                        if (result) {
                                            inputGenerators.push_back(oI);
                                        }
                                        break;
                                    }
                                    default:
                                    {
                                        result = false;
                                        errorMsg = "Unknown generator node type '" + inputType + "'.";
                                        break;
                                    };
                                        
                                }
                                XML.popTag(); //tag INPUT_GEN
                            }
                        }
                        
                        XML.popTag(); //tag PARAM_INPUT_GENERATORS
                    }
                }
            }
        
            if (result) {
                //setting inputs to every node
                for(int i = 0; i < aux_nodesVector.size(); i++){
                    if (aux_nodesVector[i]->findAndAssignInputs(aux_nodes)) {
                        errorMsg = "Error assigning inputs to node '" + nodesVector[i]->getName() + "'.";
                        result = false;
                        break;
                    }
                    NodeElement* nE = new NodeElement(aux_nodesVector[i]);
                    snnipetNodeElements.push_back(nE);
                    nodeViewers[currentViewer]->addElement(nE);
                    nodesVector.push_back(aux_nodesVector[i]);
                }
            }
            
            if (result) {
                //create connections in nodeView
                nodeViewers[currentViewer]->createConnections(snnipetNodeElements);
                nodeViewers[currentViewer]->setNodesCount(nodesCount + aux_nodes.size());
                nodes.insert(aux_nodes.begin(), aux_nodes.end());
                
                for(int i = 0; i < aux_nodesVector.size(); i++){
                    initNode(aux_nodesVector[i]);
                }
                
                for(int i=0; i<inputGenerators.size(); i++){
                    inputGenerators[i]->setup();
                    inputGenerators[i]->start();
                }
                
                setSelectedForAudioIn();
                setSelectedForOSC();
            }
            
            XML.popTag(); // tag SNIPPET
        }
        else {
            errorMsg = "File " + snippetName + " is not eligible for Snippet.";
        }
    }
    
    if (!result) {
        console->pushError("Unable to load snippet '" + snippetName + "': " + errorMsg);
    }
    
    return result;
}

//------------------------------------------------------------------
bool ofApp::saveSnippet() {
    
    string snippetName = "";
    ofxXmlSettings XML;
    
    ofFileDialogResult openFileResult;
    openFileResult = ofSystemSaveDialog("snippet.xml", "Save your Snippet");
    
    if(openFileResult.bSuccess){
        snippetName = openFileResult.getPath();
    }
    else {
        return;
    }
    
    bool saveOk = true;
    bool a;
    bool b;
    // Delete and create xml file
    if (XML.loadFile(snippetName)) {
        XML.clear();
    } else {
        b = XML.saveFile(snippetName);
        XML.loadFile(snippetName);
    }
    
    vector <OscInputGenerator*> oscInputs;
    vector <AudioInputGenerator*> audioInputs;
    vector<int> activeEncapsulatedPatches;
    map<int,ofxPatch*> activePatches = nodeViewers[currentViewer]->getActivePatches();
    map<int,int> newIdsMap;
    newIdsMap.clear();
    int nextNodeId = 1;
    
    
    // Look for all selected nodes
    //
    for(map<int,ofxPatch*>::iterator it = activePatches.begin(); it != activePatches.end(); it++ ){
        newIdsMap[it->second->getId()] = nextNodeId;
        nextNodeId++;
    }
    
    XML.addTag("SNIPPET");
    saveOk = XML.pushTag("SNIPPET");
    if (saveOk) {
        
        // Save nodes and syphon servers
        //
        for(map<int,ofxPatch*>::iterator it = activePatches.begin(); it != activePatches.end() && saveOk; it++ ){
            saveOk = ((ImageOutput*)it->second)->saveSettingsToSnippet(XML, newIdsMap);
            
            if (saveOk) {
                
                if (it->second->isLastEncapsulated()) {
                    activeEncapsulatedPatches.push_back(it->second->getEncapsulatedId());
                }
                if (((ImageOutput*)it->second)->getTypeName() == "OSC Receiver") {
                    
                    for (int i = 0; i < inputGenerators.size(); ++i) {
                        
                        std::map<int, OscInputGenerator*>::iterator it;
                        it = oscInputGeneratorPortMap.find(((OSCReceiver*)it->second)->getPort());
                        
                        if (it != oscInputGeneratorPortMap.end()) {
                            oscInputs.push_back(it->second);
                            break;
                        }
                    }
                }
                else if (((ImageOutput*)it->second)->getTypeName() == "Audio In - Left Channel" ||
                         ((ImageOutput*)it->second)->getTypeName() == "Audio In - Right Channel") {
                    
                    for (int i = 0; i < audioListeners.size(); ++i) {
                        if (audioListeners[i]->getNodeID() == ((ImageOutput*)it->second)->getId()) {
                            audioInputs.push_back((AudioInputGenerator*)audioListeners[i]);
                            break;
                        }
                    }
                }
            }
        }
        
        
        // Save Encapsulated Nodes
        //
        XML.addTag("ENCAPSULATED_NODES");
        saveOk = XML.pushTag("ENCAPSULATED_NODES");
        if (saveOk) {
            
            for(int e = 0; e < activeEncapsulatedPatches.size() && saveOk; e++){
                saveOk = nodeViewers[currentViewer]->saveEncapsulatedSettingsToSnippet(XML, activeEncapsulatedPatches[e], newIdsMap);
            }
            
            XML.popTag(); // tag ENCAPSULATED_NODES
        }
        
        
        // Save Param Input Generators
        //
        XML.addTag("PARAM_INPUT_GENERATORS");
        saveOk = XML.pushTag("PARAM_INPUT_GENERATORS");
        if (saveOk) {
            
            for (int i = 0; i < audioInputs.size() && saveOk; i++) {
                saveOk = audioInputs[i]->saveSettingsToSnippet(XML, newIdsMap);
            }
            for (int j = 0; j < oscInputs.size() && saveOk; j++) {
                saveOk = oscInputs[j]->saveSettingsToSnippet(XML, newIdsMap);
            }
            
            XML.popTag(); // tag PARAM_INPUT_GENERATORS
        }
        
         XML.popTag(); // SNIPPET
        
        // If everything OK, save file
        //
        if (saveOk) {
            XML.saveFile(snippetName);
        }
    }
    
    return saveOk;
}

/* ================================================ */
/* ================================================ */

void ofApp::printShortcuts(){
    
    if(!showConsole){
        scrollBars->changeWindowHeight(console->getWindowRatio());
    }
    showConsole = true;
    console->setConsoleVisible(showConsole);
    
    console->pushMessage("", false);
    console->pushMessage("************************", false);
    console->pushMessage("*** Shortcuts for N.IMP ***", false);
    console->pushMessage("************************", false);
    console->pushMessage("Create a Node - n", false);
    console->pushMessage("Cancel node creation - esc", false);
    console->pushMessage("Load Snippet - command + j", false);
    console->pushMessage("Save Snippet - command + k", false);
    console->pushMessage("Show Console - command + c", false);
    console->pushMessage("Clear Console - command + shift + c", false);
    console->pushMessage("Fullscreen - command + f", false);
    console->pushMessage("Show Encapsulated Node - command + e", false);
    console->pushMessage("Activate\\Deactivate all nodes - command + a", false);
    console->pushMessage("Show\\Hide all inspectors - command + i", false);
    console->pushMessage("New Patcher - command + n", false);
    console->pushMessage("Open Patcher - command + o", false);
    console->pushMessage("Save Patcher - command + s", false);
    console->pushMessage("Show shortcuts - F1", false);
    console->pushMessage("Edit Mode On\\Off - F2", false);
    console->pushMessage("", false);
    
}