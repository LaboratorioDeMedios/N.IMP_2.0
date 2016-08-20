//
//  ConsoleLog.h
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#ifndef __nimp__ConsoleLog__
#define __nimp__ConsoleLog__

#include "ofMain.h"
#include "ConsoleScrollBar.h"
#include "ConsoleMessageType.cpp"

struct Message{
    Message(ConsoleMessageType mT, string m){
        messageType = mT;
        message = m;
    }
    
    ConsoleMessageType messageType;
    string             message;
};

class ConsoleLog {
    
public:
    
    static ConsoleLog* getInstance();
    
    //*** LOOPS ***//
    //
    void update();
    void draw();
    
    
    //*** EVENTS ***//
    //
    bool mouseMoved(ofMouseEventArgs &e);
    bool mousePressed(ofMouseEventArgs &e);
    bool mouseReleased(ofMouseEventArgs &e);
    bool mouseDragged(ofMouseEventArgs &e);
    ofEvent<float> changeConsoleHeight;
    
    //*** FUNCTIONS ***//
    //
    void pushMessage(string message, bool firstMessage = true);
    void pushError(string error, bool firstMessage = true);
    void pushSuccess(string success, bool firstMessage = true);
    void pushWarning(string warning, bool firstMessage = true);
    void clearMessages();
    void printMessages();
    
    int getLowestCoord();
    int getHighestCoord();
    float getWindowRatio() { return windowRatio; };
    ofRectangle getContainer() { return container; };
    
    void setDiffStartY(int newY);
    void setupScrollBar(ofxMultiTouchPad* pad);
    void setConsoleVisible(bool visible) { consoleVisible = visible; };
    
    void restartStartY(float winRatio);
    
private:
    ConsoleLog();
    ConsoleLog(ConsoleLog const&){};             // copy constructor is private
    ConsoleLog& operator=(ConsoleLog const&){};  // assignment operator is private

    ~ConsoleLog();
    static ConsoleLog* instance;
    
    vector<Message> messages;
    ofTrueTypeFont font;
    int startY;
    ConsoleScrollBar* scrollBar;
    
    int maxConsoleMessages;
    void push(Message m);
    
    ofRectangle container;
    float windowRatio;
    bool consoleVisible;
    bool pressingConsole;
    
    // if a same message is going to be printed in several lines, set 'firstMessage' to false
    string processMessage(string message, bool firstMessage);
};



#endif /* defined(__nimp__ConsoleLog__) */
