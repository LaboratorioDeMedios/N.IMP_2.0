//
//  AudioInEvent.h
//  nimp
//
//  Created by Mili Garicoits on 2/27/16.
//
//

#ifndef AudioInEvent_h
#define AudioInEvent_h

class AudioInEvent : ofEventArgs {
    
public:
    int   nodeId;
    int   band;
    int   channel;
    bool  active;
    bool  enable;
    float saturation;
};

#endif /* AudioInEvent_h */
