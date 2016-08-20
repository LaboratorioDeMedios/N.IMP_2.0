//
//  ConsoleLog.cpp
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#include "ConsoleLog.h"
#include "enumerations.h"

ConsoleLog* ConsoleLog::instance = NULL;

//------------------------------------------------------------------
ConsoleLog::ConsoleLog() {
    ofTrueTypeFont::setGlobalDpi(72);
    
    font.loadFont("Fonts/verdana.ttf", 14);
    font.setLineHeight(18.0f);
    font.setLetterSpacing(1.037);
    
    maxConsoleMessages = 300;
    consoleVisible = false;
    pressingConsole = false;
    
    container.set(RIGHT_MENU_WIDTH, 3*ofGetHeight()/4, ofGetWidth()-RIGHT_MENU_WIDTH, ofGetHeight()/4 +1);
    windowRatio = CONSOLE_ASPECT_RATIO;
    
    ofAddListener(ofEvents().mousePressed, this, &ConsoleLog::mousePressed, CONSOLE_EVENT_PRORITY);
    ofAddListener(ofEvents().mouseDragged, this, &ConsoleLog::mouseDragged, CONSOLE_EVENT_PRORITY);
    ofAddListener(ofEvents().mouseReleased, this, &ConsoleLog::mouseReleased, CONSOLE_EVENT_PRORITY);
}

//------------------------------------------------------------------
ConsoleLog::~ConsoleLog(){
    ofRemoveListener(ofEvents().mousePressed, this, &ConsoleLog::mousePressed, CONSOLE_EVENT_PRORITY);
    ofRemoveListener(ofEvents().mouseDragged, this, &ConsoleLog::mouseDragged, CONSOLE_EVENT_PRORITY);
    ofRemoveListener(ofEvents().mouseReleased, this, &ConsoleLog::mouseReleased, CONSOLE_EVENT_PRORITY);
    delete this->instance->scrollBar;
}

//------------------------------------------------------------------
ConsoleLog* ConsoleLog::getInstance(){
    if(!instance){
        instance = new ConsoleLog();
    }
    return instance;
}

//------------------------------------------------------------------
void ConsoleLog::setupScrollBar(ofxMultiTouchPad* pad){
    startY = container.getY() + 10;
    this->instance->scrollBar = new ConsoleScrollBar(pad, CONSOLE_SCROLL_EVENT_PRORITY);
    this->instance->scrollBar->setup(windowRatio);
}

//------------------------------------------------------------------
void ConsoleLog::restartStartY(float winRatio) {
    container.height = ofGetHeight() - ofGetHeight()*windowRatio ;
    container.setPosition(RIGHT_MENU_WIDTH, ofGetHeight()*windowRatio);
    startY = container.getY() + 10;
}

//------------------------------------------------------------------
void ConsoleLog::update(){
    
}

//------------------------------------------------------------------
void ConsoleLog::draw(){
    
    ofPushStyle();
    
    ofSetColor(0);
    ofRect(container);
    ofSetColor(150);
    ofLine(RIGHT_MENU_WIDTH, container.getY(), ofGetWidth(), container.getY());
    printMessages();
    
    ofPopStyle();
}

//------------------------------------------------------------------
bool ConsoleLog::mouseDragged(ofMouseEventArgs &e){

    if(pressingConsole) {
        
        ofVec3f mouse = ofVec3f(e.x, e.y,0);
        ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
        ofVec3f diffVec = ofVec3f(0,0,0);
        
        diffVec.y = mouseLast.y - mouse.y;
        container.setHeight(container.getHeight() + diffVec.y);
        windowRatio = 1 - container.getHeight()/ofGetHeight();
        
        
        container.setPosition(RIGHT_MENU_WIDTH, ofGetHeight() - container.height);
        startY -= diffVec.y;
        
        this->instance->scrollBar->setup(windowRatio);
        ofNotifyEvent(changeConsoleHeight, windowRatio);
        
        return true;
    }
    
    return false;
}

//------------------------------------------------------------------
bool ConsoleLog::mouseReleased(ofMouseEventArgs &e){
    
    pressingConsole = false;
    
    return false;
}

//------------------------------------------------------------------
bool ConsoleLog::mousePressed(ofMouseEventArgs &e){

    pressingConsole = consoleVisible && container.inside(e.x, e.y);
    return pressingConsole;
}

//------------------------------------------------------------------
bool ConsoleLog::mouseMoved(ofMouseEventArgs &e){

    return false;
}

//------------------------------------------------------------------
void ConsoleLog::pushMessage(string message, bool firstMesssage){
    message = processMessage(message, firstMesssage);
    Message m = Message(DEFAULT, message);
    push(m);
}

//------------------------------------------------------------------
void ConsoleLog::pushError(string error, bool firstMesssage){
    error = processMessage(error, firstMesssage);
    Message m = Message(ERROR, error);
    push(m);
    ofLog(OF_LOG_ERROR, error);
}

//------------------------------------------------------------------
void ConsoleLog::pushSuccess(string success, bool firstMesssage){
    success = processMessage(success, firstMesssage);
    Message m = Message(SUCCESS, success);
    push(m);
}

//------------------------------------------------------------------
void ConsoleLog::pushWarning(string warning, bool firstMesssage){
    warning = processMessage(warning, firstMesssage);
    Message m = Message(WARNING, warning);
    push(m);
}

//------------------------------------------------------------------
void ConsoleLog::push(Message m){
    if(messages.size() == maxConsoleMessages){
        messages.erase(messages.begin());
    }
    messages.push_back(m);
}

//------------------------------------------------------------------
void ConsoleLog::clearMessages(){
    messages.clear();
    startY = container.getY() + 10;
}

//------------------------------------------------------------------
void ConsoleLog::printMessages(){
    instance->scrollBar->update();
    instance->scrollBar->draw();
    float y = startY + font.getLineHeight();
    ofPushStyle();
    for (int i = 0; i < messages.size();i++){
        if(messages.at(i).messageType == DEFAULT){
            ofSetColor(255, 255, 255);
        } else if(messages.at(i).messageType == ERROR){
            ofSetColor(255, 0, 0);
        } else if(messages.at(i).messageType == WARNING){
            ofSetColor(255, 255, 0);
        } else if(messages.at(i).messageType == SUCCESS){
            ofSetColor(0, 255, 0);
        }
        
        if (y > container.y + font.getLineHeight()) {
            font.drawString(messages.at(i).message, RIGHT_MENU_WIDTH + 10.f, y);
        }
        y += font.getLineHeight();
    }
    ofPopStyle();
}

//------------------------------------------------------------------
int ConsoleLog::getLowestCoord(){
    return (messages.size() + 2)*font.getLineHeight() + startY - container.getY();
}

//------------------------------------------------------------------
int ConsoleLog::getHighestCoord(){
    return startY - container.getY() - (font.getLineHeight()/2);
}

//------------------------------------------------------------------
void ConsoleLog::setDiffStartY(int diffY){
    startY += diffY;
}

//------------------------------------------------------------------
string ConsoleLog::processMessage(string message, bool firstMessage) {
    if(firstMessage){
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime (&rawtime);
        ostringstream oss;
        if(timeinfo->tm_hour < 10){
            oss << "0";
        }
        oss << timeinfo->tm_hour << ":";
        if(timeinfo->tm_min < 10){
            oss << "0";
        }
        oss << timeinfo->tm_min << ":";
        if(timeinfo->tm_sec < 10){
            oss << "0";
        }
        oss << timeinfo->tm_sec << " - " << message;
        return oss.str();
    } else {
        return message = " \t " + message;
    }
    
}
