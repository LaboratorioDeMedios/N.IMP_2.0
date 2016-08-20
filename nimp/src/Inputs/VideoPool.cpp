//
//  VideoPool.cpp
//  ofApp
//
//  Created by Brian Eschrich on 13.01.15.
//  Copyright (c) 2015 . All rights reserved.
//


#include "VideoPool.h"
#include "ofxHapPlayer.h"



VideoPool* VideoPool::instance = 0;

VideoPool* VideoPool::getInstance()
{
	if ( instance == 0)
	{
		instance = new VideoPool();
		instance->setup();
	}
	return instance;
}

void VideoPool::setup() {
    
}

void VideoPool::update() {
    
}

ofBaseVideoPlayer* VideoPool::getPlayer(string path, string name) {
    ofBaseVideoPlayer* vP = NULL;
    
    bool found = false;
    int index = 0;
    
    while(!found && index<pool.size()){
        if(pool[index].first==true){
            found = true;
            vP = pool[index].second;
            pool[index].first = false;
        }
        index++;
    }
    
    if(!found){
        if(pool.size()<MAX_PLAYERS){
            ofxHapPlayer* a = new ofxHapPlayer();
            bool b = a->loadMovie(path);
            if(!b){
                delete a;
                vP = new ofQTKitPlayer();
                ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
                vP->setPixelFormat(OF_PIXELS_RGBA);
            } else {
                vP = a;
            }
            pool.push_back(std::pair<bool,ofBaseVideoPlayer*>(false,vP));
        }
    }
    
    return vP;
    
}
void VideoPool::releasePlayer(ofBaseVideoPlayer* player) {
    if (player!=NULL) {
        player->close();
        
        bool found = false;
        int index = 0;
        
        while(!found && index<pool.size()){
            if(pool[index].second==player){
                found = true;
                pool[index].first = true;
            }
            index++;
        }
    }
}

ofBaseVideoPlayer* VideoPool::getCommonPlayer() {
    ofBaseVideoPlayer* vP = new ofQTKitPlayer();
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    vP->setPixelFormat(OF_PIXELS_RGBA);
    pool.push_back(std::pair<bool,ofBaseVideoPlayer*>(false,vP));
    return vP;
}

ofBaseVideoPlayer* VideoPool::getHapPlayer() {
    ofBaseVideoPlayer* vP = new ofxHapPlayer();
    pool.push_back(std::pair<bool,ofBaseVideoPlayer*>(false,vP));
    return vP;
}


bool VideoPool::isHapVideo(string path) {
    ofxHapPlayer* vP = new ofxHapPlayer();
    bool b = vP->loadMovie(path);
    delete vP;
    return b;
}