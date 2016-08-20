//
//  EventHandler.cpp
//  nimp
//
//  Created by Nicolas Peri on 1/6/16.
//
//

#include "EventHandler.h"
#include "enumerations.h"

EventHandler* EventHandler::instance = NULL;

EventHandler::EventHandler() {
    encapsulatedIdDraw = MAIN_WINDOW;
}

//------------------------------------------------------------------
EventHandler::~EventHandler(){
}

//------------------------------------------------------------------
EventHandler* EventHandler::getInstance(){
    if(!instance){
        instance = new EventHandler();
    }
    return instance;
}

//------------------------------------------------------------------
int EventHandler::getEncapsulatedIdDraw(){
    return encapsulatedIdDraw;
}

//------------------------------------------------------------------
void EventHandler::setEncapsulatedIdDraw(int encapsulatedId){
    this->encapsulatedIdDraw = encapsulatedId;
}
