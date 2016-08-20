/*
 *  ImageTypePicture.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypePicture.h"

ImageTypePicture::ImageTypePicture(string name_ ,string path_, bool isResize, int width, int height): ImageType(name_,path_){
    mediaType = T_IMAGE;

    img.setUseTexture(false);
    img.loadImage(path_);
    if (isResize) {
        img.resize(width, height);
    }
    
    ofIsStringInString(path_, ".png") ? ext = PNG : ext = OTHER;
    
}

//------------------------------------------------------------------
ImageTypePicture::~ImageTypePicture(){
    img.clear();
}

//------------------------------------------------------------------
void ImageTypePicture::activate(ofImage& _img, ofFbo& _fbo){
    _img.setFromPixels(img.getPixelsRef());
}

//------------------------------------------------------------------
void ImageTypePicture::update(ofImage& _img, ofFbo& _fbo){
    _img.setFromPixels(img.getPixelsRef());
}

//------------------------------------------------------------------
int ImageTypePicture::getFrameRate(){
    return -1;
}

//------------------------------------------------------------------
float ImageTypePicture::getPosition(){
    return -1;
}

//------------------------------------------------------------------
void ImageTypePicture::calculateFPS(){
    
}

//------------------------------------------------------------------
void ImageTypePicture::setPosition(float p, ofImage& _img){
    
}

//------------------------------------------------------------------
bool ImageTypePicture::isVideoDone(){
    return false;
}

//------------------------------------------------------------------
float ImageTypePicture::getHeight() {
    return img.getHeight();
}

//------------------------------------------------------------------
float ImageTypePicture::getWidth() {
    return img.getWidth();
}

//------------------------------------------------------------------
float ImageTypePicture::getCurrentSecond() {
    return 0;
}
