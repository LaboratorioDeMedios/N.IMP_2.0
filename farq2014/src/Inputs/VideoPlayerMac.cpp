/*
 *  VideoPlayerMac.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "VideoPlayerMac.h"

VideoPlayerMac::VideoPlayerMac(string name) : InputSource(name){
    //there always has to exist at least one player.
    currentPlayer=0;
    img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    
    play.addListener(this, &VideoPlayerMac::playVideo);
    stop.addListener(this, &VideoPlayerMac::stopVideo);
    
    gui.add(play.setup("Play"));
    gui.add(stop.setup("Stop"));

    
}

//------------------------------------------------------------------
void VideoPlayerMac::setup() {
	
	
}


//------------------------------------------------------------------
void VideoPlayerMac::update() {
    if (players[currentPlayer]->isLoaded()) {
        players[currentPlayer]->update();
        if (players[currentPlayer]->isFrameNew()){
            img.setFromPixels(players[currentPlayer]->getPixels(), players[currentPlayer]->getWidth(), players[currentPlayer]->getHeight(), OF_IMAGE_COLOR_ALPHA);
            //img.resize(width, height);
            tex = img.getTextureReference();
        }
    }
}


//------------------------------------------------------------------
void VideoPlayerMac::draw(int x,int y, float scale) {
	if (players[currentPlayer]->isLoaded()) {
        float ratio = (float)height/(float)width;
        int w = 640*scale;
        int h = w*ratio;
        players[currentPlayer]->draw(x, y, w,h);
    }
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}

void VideoPlayerMac::loadVideo(string path) {
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    ofQTKitPlayer* vP = new ofQTKitPlayer();
    vP->setPixelFormat(OF_PIXELS_RGBA);
    vP->loadMovie(path, decodeMode);
    //ofVideoPlayer* vP = new ofVideoPlayer();
    //vP->loadMovie(path);
    players.push_back(vP);
}

void VideoPlayerMac::nextVideo(){
    //stopping current video
    players[currentPlayer]->stop();
    //incrementing to next video index
    currentPlayer += 1;
    if (currentPlayer > players.size()){
        currentPlayer = 0 ;
    }
    //playing current video
    players[currentPlayer]->play();
}

void VideoPlayerMac::prevVideo(){
    //stopping current video
    players[currentPlayer]->stop();
    //decrementing to next video index
    currentPlayer -= 1;
    if (currentPlayer < 0){
        currentPlayer = players.size()-1 ;
    }
    //playing current video
    players[currentPlayer]->play();
}

void VideoPlayerMac::playVideo(){
    players[currentPlayer]->play();
}

void VideoPlayerMac::stopVideo(){
    players[currentPlayer]->stop();
}

void VideoPlayerMac::setSpeed(float speed){
    players[currentPlayer]->setSpeed(speed);
}

float VideoPlayerMac::getSpeed(){
    return players[currentPlayer]->getSpeed();
}

void VideoPlayerMac::updateParameter(Param* inputParam){
    
}
