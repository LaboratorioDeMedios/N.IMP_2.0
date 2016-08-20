#ifndef _InputSyphon
#define _InputSyphon

#include "InputSource.h"
#include "ofxGui.h"
#include "ofxSyphon.h"

class InputSyphon : public InputSource{
	
public:
    InputSyphon(ofxSyphonServerDirectory* serverDir, string name, int id_);
    ~InputSyphon();
	
	void setup();
    void update();
    
    ofTexture*  getTexture();

    void changeToNextServer();
    void changeToPrevServer();
    void serverRetired(string serverName, string appName);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);

private:
    
    void _showHelp();
    
    ofImage serverDownImg;
    
    int     dirIdx;
    string  serverName;
    string  appName;
    ofParameter<string> serverSelectedName;
    
    ofxSyphonServerDirectory* dir;
    ofxSyphonClient client;
    
    bool serverDown;

    void updateParameter(Param* inputParam){};
    float getMidiMin(string param_){};
    float getMidiMax(string param_){};
    
    ofxButton nextServer;
    ofxButton prevServer;
	
};

#endif
