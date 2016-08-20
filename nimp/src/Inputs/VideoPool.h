//
//  VideoPool.h
//  ofApp
//
//  Created by Brian Eschrich on 13.01.15.
//  Copyright (c) 2015 . All rights reserved.
//

#ifndef ofApp_VideoPool_cpp
#define ofApp_VideoPool_cpp

#include "ofMain.h"
#define MAX_PLAYERS 8

class VideoPool{

    public:
        static VideoPool* getInstance();
        
        void update();
        ofBaseVideoPlayer* getPlayer(string path, string name);
        void releasePlayer(ofBaseVideoPlayer* player);
        bool isHapVideo(string path);
        ofBaseVideoPlayer* getCommonPlayer();
        ofBaseVideoPlayer* getHapPlayer();

    protected:
        VideoPool() {};

    private:
        static VideoPool* instance;
        void setup();
        vector<std::pair<bool,ofBaseVideoPlayer*> > pool;
    
};


#endif