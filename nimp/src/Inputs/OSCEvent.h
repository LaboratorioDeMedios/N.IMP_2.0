//
//  OSCEvent.h
//  nimp
//
//  Created by Mili Garicoits on 3/9/16.
//
//

#ifndef OSCEvent_h
#define OSCEvent_h

class OSCEvent : ofEventArgs {
    
public:
    int nodeId;
    int port;
    int oldPort;
    int min;
    int max;
    string address;
    string oldAddress;
};

#endif /* OSCEvent_h */
