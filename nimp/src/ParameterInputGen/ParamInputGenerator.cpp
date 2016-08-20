//
//  ParameterInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#include "ParamInputGenerator.h"


ParamInputGenerator::ParamInputGenerator(string name_, bool isThreaded_){
    generatorName = name_;
    isConfigured = false;
    isThreaded = isThreaded_;
    inputBuffer = new queue<Param*>();
}

ParamInputGenerator::~ParamInputGenerator(){
    isRunning = false;
    deleteAllMessages();
    stop();
}

void ParamInputGenerator::setup(){
    samplingMs = 33; // default sampling is 30 fps -- this only works with threaded generators
    isConfigured = true;
}

void ParamInputGenerator::start(){
    if(isConfigured && isThreaded){
        isRunning = true;
        startThread(false);    // non blocking
    }
}

void ParamInputGenerator::stop(){
//    waitForThread(true);
    lock();
    if(isThreaded){
        stopThread();
    }
    unlock();
}

Param* ParamInputGenerator::getNextInputMessage(){
    
    Param* p = NULL;
    if(isThreaded){
        if(lock()){
            if(inputBuffer->size()>0){
                p = inputBuffer->front();
                inputBuffer->pop();
            }
            unlock();
        }
    }
    else{
        if(inputBuffer->size()>0){
            p = inputBuffer->front();
            inputBuffer->pop();
        }
    }
    return p;
    
}

void ParamInputGenerator::storeMessage(Param* p){
    // IMPORTANT: this function has to be called between lock and unlock ALWAYS!
    
    if(inputBuffer->size()>10){
        Param* firstP = inputBuffer->front();
        inputBuffer->pop();
        //delete firstP;
        
    }
    inputBuffer->push(p);
}

void ParamInputGenerator::threadedFunction(){
    
    while(isThreadRunning() && isRunning) {
        processInput();
        sleep(samplingMs); // we limit the sampling to avoid wasting resources.
    }
    
//    deleteAllMessages();
}

void ParamInputGenerator::deleteAllMessages(){
    while(inputBuffer->size()>0){
        Param* p = inputBuffer->front();
        inputBuffer->pop();
        delete p;
    }
}

void ParamInputGenerator::keyPressed (int key){
    // this method should be thread safe using lock and unlock
}