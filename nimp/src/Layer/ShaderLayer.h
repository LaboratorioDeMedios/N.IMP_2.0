
#ifndef _ShaderLayer
#define _ShaderLayer

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxPingPong.h"

struct ShaderIntParam{
    ofParameter<int> param;
    string           name;
};

struct ShaderFloatParam{
    ofParameter<float> param;
    string             name;
};

struct ShaderBoolParam{
    ofParameter<bool> param;
    string            name;
};

struct ShaderVec2Param{
    ofParameter<float> paramx;
    ofParameter<float> paramy;
    string               name;
};

struct ShaderVec3Param{
    ofParameter<float> paramx;
    ofParameter<float> paramy;
    ofParameter<float> paramz;
    string               name;
};

struct ShaderVec4Param{
    ofParameter<float> paramx;
    ofParameter<float> paramy;
    ofParameter<float> paramz;
    ofParameter<float> paramw;
    string               name;
};

class ShaderLayer : public VisualLayer {
  public:
	
    ShaderLayer(string name_ = "New Shader", int id_ = -1);
    ~ShaderLayer();
	
    void setup();

    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
    void updateParameter(Param* inputParam);
    
    bool loadShader(string path_, string fileName_);
    
private:
    
    void update();
    void _showHelp();
    void doFrame();
    
    ofxPingPong pingPong;
    ofFbo       *textures;
    ofShader    shader;
    vector<ShaderIntParam*>   shaderIntParams;
    vector<ShaderFloatParam*> shaderFloatParams;
    vector<ShaderBoolParam*>  shaderBoolParams;
    vector<ShaderVec2Param*>  shaderVec2Params;
    vector<ShaderVec3Param*>  shaderVec3Params;
    vector<ShaderVec4Param*>  shaderVec4Params;
    vector<string>            shaderTextureNames;
    void paramChanged(float &f);
    string      fragmentShader, vertexShader;
    int         nTextures, passes, internalFormat;
    bool        bFine;
    string      path;
    
    ofParameter<string> shaderFileName;
    ofParameter<string> shaderMaxInputs;
};

#endif
