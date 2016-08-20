#include "SyphonClientHandler.h"
#include "ConsoleLog.h"

SyphonClientHandler* SyphonClientHandler::instance = NULL;

SyphonClientHandler::SyphonClientHandler(){
}

SyphonClientHandler::~SyphonClientHandler(){
}

void SyphonClientHandler::setup(){
    dir.setup();
    
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &SyphonClientHandler::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &SyphonClientHandler::serverRetired);
}

SyphonClientHandler* SyphonClientHandler::getInstance(){
    if(!instance){
        instance = new SyphonClientHandler();
        instance->setup();
    }
    return instance;
}

//------------------------------------------------------------------

InputSyphon* SyphonClientHandler::createSyphonPatch(string name, int patchId){
    InputSyphon* newClient = new InputSyphon(&dir, name, patchId);
    clients.push_back(newClient);
    return newClient;
}

void SyphonClientHandler::removeClient(InputSyphon* client){
    // only remove it from list, its delete is called from another place
    for(int i=0; i < clients.size(); i++){
        if(client == clients[i]){
            clients.erase(clients.begin() + i);
        }
    }
}

//------------------------------------------------------------------

void SyphonClientHandler::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg) {
    for( auto& dir : arg.servers ){
        ConsoleLog::getInstance()->pushMessage("New Syphon server published");
        ConsoleLog::getInstance()->pushMessage("\t Server Name: " + dir.serverName + ". Application Name: " + dir.appName);
    }
}

void SyphonClientHandler::serverRetired(ofxSyphonServerDirectoryEventArgs &arg) {
    for( auto& dir : arg.servers ){
        ConsoleLog::getInstance()->pushMessage("Syphon server retired");
        ConsoleLog::getInstance()->pushMessage("\t Server Name: " + dir.serverName + ". Application Name: " + dir.appName);
        for(int i=0; i < clients.size(); i++){
            clients[i]->serverRetired(dir.serverName, dir.appName);
        }
    }
}

