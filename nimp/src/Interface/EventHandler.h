//
//  EventHandler.h
//  nimp
//
//  Created by Nicolas Peri on 1/6/16.
//
//

#ifndef __nimp__EventHandler__
#define __nimp__EventHandler__

#include <stdio.h>


class EventHandler {
    
public:
    
    static EventHandler* getInstance();
    
    int  getEncapsulatedIdDraw();
    void setEncapsulatedIdDraw(int encapsulatedId);
    
private:
    EventHandler();
    EventHandler(EventHandler const&){};             // copy constructor is private
    EventHandler& operator=(EventHandler const&){};  // assignment operator is private
    
    ~EventHandler();
    static EventHandler* instance;

    int encapsulatedIdDraw;
    
};

#endif /* defined(__nimp__EventHandler__) */
