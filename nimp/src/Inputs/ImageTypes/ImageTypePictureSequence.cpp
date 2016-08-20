/*
 *  ImageTypePictureSequence.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypePictureSequence.h"

ImageTypePictureSequence::ImageTypePictureSequence(string name_ ,string path_): ImageType(name_,path_){
    mediaType = T_IMAGE_SEQUENCE;
    ofDirectory dir;
    dir.listDir(path_);
    player.loadMovie(path_);
    player.setLoopState(OF_LOOP_NORMAL);
    img.loadImage(dir.getPath(0));
    
    ofIsStringInString(path_, ".png") ? ext = PNG : ext = OTHER;
}

//------------------------------------------------------------------
void ImageTypePictureSequence::update(ofImage& _img, ofFbo& _fbo){
    if (isPlaying) {
        player.update();
        if (player.isFrameNew()) {
            _img.setFromPixels(player.getPixels(), player.getWidth(), player.getHeight(), OF_IMAGE_COLOR_ALPHA);
        }
    }
}

//------------------------------------------------------------------
void ImageTypePictureSequence::setLoopState(ofLoopType l){
    player.setLoopState(l);
    
}

//------------------------------------------------------------------
int ImageTypePictureSequence::getFrameRate(){
    return player.getFrameRate();
}

//------------------------------------------------------------------
float ImageTypePictureSequence::getPosition(){
    return player.getPosition();
}

//------------------------------------------------------------------
void ImageTypePictureSequence::calculateFPS(){
    float fps;
    if (!isMatchBpmToSequenceLength) {
        fps = bpm*((float)bpmMultiplier)/60.0;
    }else{
        float ds = (float)bpm/60.0;
        fps = player.getTotalNumFrames() * ds / ((float)bpmMultiplier);
    }
    player.setFrameRate(fps);
}

//------------------------------------------------------------------
void ImageTypePictureSequence::setPosition(float p, ofImage& _img){
    player.setPosition(p);
    _img.setFromPixels(player.getPixels(), player.getWidth(), player.getHeight(), OF_IMAGE_COLOR_ALPHA);
}

//------------------------------------------------------------------
void ImageTypePictureSequence::activate(ofImage& _img, ofFbo& _fbo){
    player.play();
    _img.setFromPixels(player.getPixels(), player.getWidth(), player.getHeight(), OF_IMAGE_COLOR_ALPHA);
}

//------------------------------------------------------------------
bool ImageTypePictureSequence::isVideoDone(){
    if (isPlaying) {
        return player.getCurrentFrame() == player.getTotalNumFrames();
    }
    return false;
}

//------------------------------------------------------------------
float ImageTypePictureSequence::getHeight() {
    return img.getHeight();
}

//------------------------------------------------------------------
float ImageTypePictureSequence::getWidth() {
    return img.getWidth();
}

//------------------------------------------------------------------
float ImageTypePictureSequence::getCurrentSecond() {
    float totalFrames = player.getTotalNumFrames();
    float curFrame = player.getCurrentFrame();
    float totalDuration = player.getDuration();
    return curFrame/totalFrames*totalDuration;
}

