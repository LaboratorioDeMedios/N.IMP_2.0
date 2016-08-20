//
//  ImageInput.cpp
//  ofApp
//
//  Created by Christian Clark on 12/7/14.
//
//

#include "ImageInput.h"


ImageInput::ImageInput(string name) : InputSource(name){
    lastFrameSent = ofGetElapsedTimeMillis();
    
    isImageSequence = false;
    isVideo = false;
    img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
}

void ImageInput::setup(){
    
}

void ImageInput::update(){
    if (isImageSequence && isPlaying) {
        player.update();
        if (player.isFrameNew()) {
            tex = *player.getTexture();
        }
    }else if (isVideo && isPlaying){
        videoPlayer.update();
        if (videoPlayer.isFrameNew()){
            img.setFromPixels(videoPlayer.getPixels(), videoPlayer.getWidth(), videoPlayer.getHeight(), OF_IMAGE_COLOR_ALPHA);
            tex = img.getTextureReference();
        }
    }
}

void ImageInput::draw(int x,int y, float scale){
    tex.draw(x, y,640*scale,480*scale);
    ofSetColor(255, 255, 255);
    string desc = name;
    if (isImageSequence) {
        desc += "\nfps: " + ofToString(player.getFrameRate());
        desc += "\nposition: " + ofToString(player.getPosition());
    }else if (isVideo){
        desc += "\nfps: " + ofToString(videoPlayer.getTotalNumFrames()/videoPlayer.getDuration()*videoPlayer.getSpeed());
        desc += "\nposition: " + ofToString(videoPlayer.getPosition());
    }
    ofSetColor(0, 0, 0);
    ofRect(x + 5, y + 5, 180, 50);
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(desc, x + 10, y + 18);

}

void ImageInput::loadImage(string path_){
    //load image sequence
    if (ofIsStringInString(path_, ".mov")) {
        ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
        videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
        videoPlayer.loadMovie(path_, decodeMode);
        isVideo = true;
        
        isPalindromLoop.addListener(this, &ImageInput::videoLoopTypeChanged);
        isPlayingBackwards.addListener(this,&ImageInput::isPlayingBackwardsChanged);
        setOriginalPlaySpeed.addListener(this, &ImageInput::setOriginalPlaySpeedChanged);
        bpm.addListener(this, &ImageInput::videoBpmChanged);
        bpmMultiplier.addListener(this, &ImageInput::videoBpmMultiplierChanged);
        nextFrame.addListener(this, &ImageInput::videoNextFrameChanged);
        previousFrame.addListener(this, &ImageInput::videoPreviousFrameChanged);
        isMatchBpmToSequenceLength.addListener(this, &ImageInput::videoIsMatchBpmToSequenceLengthChanged);
        playPosition.addListener(this, &ImageInput::videoPlayPositionChanged);
        
        gui.add(isPlaying.set("Play",true));
        gui.add(isPalindromLoop.set("LoopPalindrom",false));
        gui.add(isPlayingBackwards.set("Backwards", false));
        gui.add(setOriginalPlaySpeed.setup("original playspeed"));
        gui.add(isMatchBpmToSequenceLength.set("match BPM to Sequence Length",false));
        gui.add(bpm.set("bpm", 100, 10, 200));
        gui.add(bpmMultiplier.set("bpmMultiplier", 4, 1, 40));
        
        gui.add(nextFrame.setup("nextFrame"));
        gui.add(previousFrame.setup("previousFrame"));
        gui.add(playPosition.set("playPosition",0.0,0.0,1.0));
        
        videoPlayer.play();
    }
    else if (!ofIsStringInString(path_, ".")) {
        ofDirectory dir;
        dir.listDir(path_);
        player.loadMovie(path_);
        player.setLoopState(OF_LOOP_NORMAL);
        
        img.loadImage(dir.getPath(0));
        unsigned char* p = img.getPixels();
        tex.loadData(p, width, height, GL_RGB);
        
        isImageSequence = true;
        
        //create image sequence gui
        isPalindromLoop.addListener(this, &ImageInput::loopTypeChanged);
        bpm.addListener(this, &ImageInput::bpmChanged);
        setOriginalPlaySpeed.addListener(this, &ImageInput::setOriginalPlaySpeedChanged);
        bpmMultiplier.addListener(this, &ImageInput::bpmMultiplierChanged);
        nextFrame.addListener(this, &ImageInput::nextFrameChanged);
        previousFrame.addListener(this, &ImageInput::previousFrameChanged);
        isMatchBpmToSequenceLength.addListener(this, &ImageInput::isMatchBpmToSequenceLengthChanged);
        playPosition.addListener(this, &ImageInput::playPositionChanged);
        
        gui.add(isPlaying.set("Play",true));
        gui.add(setOriginalPlaySpeed.setup("original playspeed"));
        gui.add(isPalindromLoop.set("LoopPalindrom",false));
        gui.add(isMatchBpmToSequenceLength.set("match BPM to Sequence Length",false));
        gui.add(bpm.set("bpm", 100, 10, 200));
        gui.add(bpmMultiplier.set("bpmMultiplier", 4, 1, 40));
        player.setFrameRate(bpm*((float)bpmMultiplier)/60.0);
        
        gui.add(nextFrame.setup("nextFrame"));
        gui.add(previousFrame.setup("previousFrame"));
        gui.add(playPosition.set("playPosition",0.0,0.0,1.0));
        player.play();
    }
    //load single image
    else{
        img.loadImage(path_);
        img.resize(width, height);
        unsigned char* p = img.getPixels();
        tex.loadData(p, width, height, GL_RGB);
        isImageSequence = false;
    }
}

void ImageInput::loopTypeChanged(bool &b){
    if (b) {
        player.setLoopState(OF_LOOP_PALINDROME);
    }else{
        player.setLoopState(OF_LOOP_NORMAL);
    }
}

void ImageInput::isPlayingBackwardsChanged(bool &b){
    calculateFPS();
}

void ImageInput::bpmChanged(float &b){
    calculateFPS();
}
void ImageInput::bpmMultiplierChanged(int &b){//bpm to fps
    calculateFPS();
}
void ImageInput::isMatchBpmToSequenceLengthChanged(bool &b){
    if (b) {
        bpmMultiplier.set("loop length in beats", 1, 1, 16 );
    }
    else{
        bpmMultiplier.set("bpmMultiplier", 4, 1, 40);
    }
    calculateFPS();
}


void ImageInput::calculateFPS(){
    float fps;
    if (!isMatchBpmToSequenceLength) {
        fps = bpm*((float)bpmMultiplier)/60.0;
    }else{
        float ds = bpm/60.0;
        fps = player.getTotalNumFrames() * ds / ((float)bpmMultiplier);
    }
    float speed = fps/(player.getTotalNumFrames()/player.getDuration());
    if (isPlayingBackwards) {
        speed *= -1;
    }
    player.setSpeed(speed);
    
}

void ImageInput::previousFrameChanged(){
    player.previousFrame();
    tex = *player.getTexture();
}

void ImageInput::nextFrameChanged(){
    player.nextFrame();
    tex = *player.getTexture();
}

void ImageInput::playPositionChanged(float &pos){
    isPlaying = false;
    player.setPosition(pos);
    tex = *player.getTexture();
}

void ImageInput::videoLoopTypeChanged(bool &b){
    if (b) {
        videoPlayer.setLoopState(OF_LOOP_PALINDROME);
    }else{
        videoPlayer.setLoopState(OF_LOOP_NORMAL);
    }
}

void ImageInput::videoBpmChanged(float &b){
    videoCalculateFPS();
}
void ImageInput::videoBpmMultiplierChanged(int &b){//bpm to fps
    videoCalculateFPS();
}
void ImageInput::videoIsMatchBpmToSequenceLengthChanged(bool &b){
    if (b) {
        bpmMultiplier.set("loop length in beats", 1, 1, 16 );
    }
    else{
        bpmMultiplier.set("bpmMultiplier", 4, 1, 40);
    }
    videoCalculateFPS();
}


void ImageInput::videoCalculateFPS(){
    float fps;
    if (!isMatchBpmToSequenceLength) {
        fps = bpm*((float)bpmMultiplier)/60.0;
    }else{
        float ds = bpm/60.0;
        fps = videoPlayer.getTotalNumFrames() * ds / ((float)bpmMultiplier);
    }
    float speed = fps/(videoPlayer.getTotalNumFrames()/videoPlayer.getDuration());
    if (isPlayingBackwards) {
     speed *= -1;
     }
    videoPlayer.setSpeed(speed);
}

void ImageInput::videoPreviousFrameChanged(){
    videoPlayer.previousFrame();
    tex = *videoPlayer.getTexture();
}

void ImageInput::videoNextFrameChanged(){
    videoPlayer.nextFrame();
    tex = *videoPlayer.getTexture();
}

void ImageInput::videoPlayPositionChanged(float &pos){
    isPlaying = false;
    videoPlayer.setPosition(pos);
    videoPlayer.update();
    img.setFromPixels(videoPlayer.getPixels(),videoPlayer.getWidth(),videoPlayer.getHeight(), OF_IMAGE_COLOR_ALPHA);
    tex = img.getTextureReference();
}

void ImageInput::setOriginalPlaySpeedChanged(){
    videoPlayer.setSpeed(1);
}

void ImageInput::updateParameter(Param* inputParam){
    
}
