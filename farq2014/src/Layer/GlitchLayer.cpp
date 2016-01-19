//
//  GlitchLayer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayer.h"


//------------------------------------------------------------------
GlitchLayer::GlitchLayer(string name_, int id_):VisualLayer(name_, "Glitch Layer", id_){
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(do_CONVERGENCE.setup("CONVERGENCE", do_CONVERGENCE, 100,20));
    gui.add(do_GLOW.setup("GLOW", do_GLOW, 100,20));
    gui.add(do_SHAKER.setup("SHAKER", do_SHAKER, 100,20));
    gui.add(do_CUTSLIDER.setup("CUTSLIDER", do_CUTSLIDER, 100,20));
    gui.add(do_TWIST.setup("TWIST", do_TWIST, 100,20));
    gui.add(do_OUTLINE.setup("OUTLINE", do_OUTLINE, 100,20));
    gui.add(do_NOISE.setup("NOISE", do_NOISE, 100,20));
    gui.add(do_SLITSCAN.setup("SLITSCAN", do_SLITSCAN, 100,20));
    gui.add(do_SWELL.setup("SWELL", do_SWELL, 100,20));
    gui.add(do_INVERT.setup("INVERT", do_INVERT, 100,20));
    
    gui.add(do_CR_HIGHCONTRAST.setup("HIGHCONTRAST", do_CR_HIGHCONTRAST, 100,20));
    gui.add(do_CR_BLUERAISE.setup("BLUERAISE", do_CR_BLUERAISE, 100,20));
    gui.add(do_CR_REDRAISE.setup("REDRAISE", do_CR_REDRAISE, 100,20));
    gui.add(do_CR_GREENRAISE.setup("GREENRAISE", do_CR_GREENRAISE, 100,20));
    gui.add(do_CR_BLUEINVERT.setup("BLUEINVERT", do_CR_BLUEINVERT, 100,20));
    gui.add(do_CR_REDINVERT.setup("REDINVERT", do_CR_REDINVERT, 100,20));
    gui.add(do_CR_GREENINVERT.setup("GREENINVERT", do_CR_GREENINVERT, 100,20));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void GlitchLayer::setup() {

    if(input.size()) {
        
//        height = input[0]->getHeight();
//        width  = input[0]->getWidth();
//        updateFromInputCoorners(getTextureCoorners().getVertices()[0]);
//        drawNoInputs = false;
        
        //drawFbo = true;
        
        fbo.allocate(width, height);
        myGlitch.setup(&fbo);
    }
    else {
        drawNoInputs = true;
    }
}

//------------------------------------------------------------------
void GlitchLayer::draw(int x,int y, float scale) {
    
//    ofSetColor(255, 255, 255);
//    ofPushMatrix();
//    glMultMatrixf(glMatrix);
//    img.draw(0, 0);
//    ofPopMatrix();
}

//------------------------------------------------------------------
void GlitchLayer::update(){
    //process pipeline
    
    if(input.size()) {
        
        //img.setFromPixels(input[0]->getImage()->getPixels(), width, height, OF_IMAGE_COLOR);

        if(isEnabled){
            
            setGlitchParameters();
            
            fbo.begin();
            
            ofClear(0, 0, 0,255);
            ofSetColor(255);
            //img.draw(0, 0);
            input[0]->getTextureReference().draw(0,0,width,height);
            
            fbo.end();
            
            /* Apply effects */
            myGlitch.generateFx();
        }
        else {
            fbo.begin();
            input[0]->getTextureReference().draw(0,0,width,height);
            fbo.end();
        }
        
        fbo.readToPixels(buff);
        img.setFromPixels(buff);
        
        img.update();
        //tex = img.getTextureReference();
    }
}

//------------------------------------------------------------------
void GlitchLayer::setGlitchParameters(){
    myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, do_CONVERGENCE);
    myGlitch.setFx(OFXPOSTGLITCH_GLOW			, do_GLOW);
    myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, do_SHAKER);
    myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, do_CUTSLIDER);
    myGlitch.setFx(OFXPOSTGLITCH_TWIST			, do_TWIST);
    myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, do_OUTLINE);
    myGlitch.setFx(OFXPOSTGLITCH_NOISE			, do_NOISE);
    myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, do_SLITSCAN);
    myGlitch.setFx(OFXPOSTGLITCH_SWELL			, do_SWELL);
    myGlitch.setFx(OFXPOSTGLITCH_INVERT			, do_INVERT);
    
    myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, do_CR_HIGHCONTRAST);
    myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, do_CR_BLUERAISE);
    myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, do_CR_REDRAISE);
    myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, do_CR_GREENRAISE);
    myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, do_CR_BLUEINVERT);
    myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, do_CR_REDINVERT);
    myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, do_CR_GREENINVERT);

}

//------------------------------------------------------------------
void GlitchLayer::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
ofImage* GlitchLayer::getImage(){
    if (drawNoInputs)
        return &noInputs;
    else
        return &img;
}

//------------------------------------------------------------------
ofTexture* GlitchLayer::getTexture(){
    if (drawNoInputs)
        return &noInputs.getTextureReference();
    else
        return &img.getTextureReference();
}

//------------------------------------------------------------------
bool GlitchLayer::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    do_CONVERGENCE      = ofToBool(XML.getAttribute("NODE","do_CONVERGENCE","false",nTag_));
    do_GLOW             = ofToBool(XML.getAttribute("NODE","do_GLOW","false",nTag_));
    do_SHAKER           = ofToBool(XML.getAttribute("NODE","do_SHAKER","false",nTag_));
    do_CUTSLIDER        = ofToBool(XML.getAttribute("NODE","do_CUTSLIDER","false",nTag_));
    do_TWIST            = ofToBool(XML.getAttribute("NODE","do_TWIST","false",nTag_));
    do_OUTLINE          = ofToBool(XML.getAttribute("NODE","do_OUTLINE","false",nTag_));
    do_NOISE            = ofToBool(XML.getAttribute("NODE","do_NOISE","false",nTag_));
    do_SLITSCAN         = ofToBool(XML.getAttribute("NODE","do_SLITSCAN","false",nTag_));
    do_SWELL            = ofToBool(XML.getAttribute("NODE","do_SWELL","false",nTag_));
    do_INVERT           = ofToBool(XML.getAttribute("NODE","do_INVERT","false",nTag_));
    
    do_CR_HIGHCONTRAST  = ofToBool(XML.getAttribute("NODE","do_CR_HIGHCONTRAST","false",nTag_));
    do_CR_BLUERAISE     = ofToBool(XML.getAttribute("NODE","do_CR_BLUERAISE","false",nTag_));
    do_CR_REDRAISE      = ofToBool(XML.getAttribute("NODE","do_CR_REDRAISE","false",nTag_));
    do_CR_GREENRAISE    = ofToBool(XML.getAttribute("NODE","do_CR_GREENRAISE","false",nTag_));
    do_CR_BLUEINVERT    = ofToBool(XML.getAttribute("NODE","do_CR_BLUEINVERT","false",nTag_));
    do_CR_REDINVERT     = ofToBool(XML.getAttribute("NODE","do_CR_REDINVERT","false",nTag_));
    do_CR_GREENINVERT   = ofToBool(XML.getAttribute("NODE","do_CR_GREENINVERT","false",nTag_));
    
    nId                 = XML.getAttribute("NODE", "id", -1, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type                = XML.getValue("type","none");
    bVisible            = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool GlitchLayer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the patch ID to update information
    // If the patch ID doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of nodes of the same type ...
    //
    int totalNodes = XML.getNumTags("NODE");
    
    // ... and search for the right id for loading
    //
    for (int i = 0; i <= totalNodes; i++){
        
        // Once it found the right surface that match the id ...
        //
        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
            
            XML.setAttribute("NODE", "name", name, i);
            if (input.size())
                XML.setAttribute("NODE", "inputSource", input[0]->getId(), i);
            else
                XML.removeAttribute("NODE", "inputSource", i);
            
            XML.setAttribute("NODE","do_CONVERGENCE", do_CONVERGENCE, i);
            XML.setAttribute("NODE","do_GLOW", do_GLOW, i);
            XML.setAttribute("NODE","do_SHAKER", do_SHAKER, i);
            XML.setAttribute("NODE","do_CUTSLIDER", do_CUTSLIDER, i);
            XML.setAttribute("NODE","do_TWIST", do_TWIST, i);
            XML.setAttribute("NODE","do_OUTLINE", do_OUTLINE, i);
            XML.setAttribute("NODE","do_NOISE", do_NOISE, i);
            XML.setAttribute("NODE","do_SLITSCAN", do_SLITSCAN, i);
            XML.setAttribute("NODE","do_SWELL", do_SWELL, i);
            XML.setAttribute("NODE","do_INVERT", do_INVERT, i);
            
            XML.setAttribute("NODE","do_CR_HIGHCONTRAST", do_CR_HIGHCONTRAST, i);
            XML.setAttribute("NODE","do_CR_BLUERAISE", do_CR_BLUERAISE, i);
            XML.setAttribute("NODE","do_CR_REDRAISE", do_CR_REDRAISE, i);
            XML.setAttribute("NODE","do_CR_GREENRAISE", do_CR_GREENRAISE, i);
            XML.setAttribute("NODE","do_CR_BLUEINVERT", do_CR_BLUEINVERT, i);
            XML.setAttribute("NODE","do_CR_REDINVERT", do_CR_REDINVERT, i);
            XML.setAttribute("NODE","do_CR_GREENINVERT", do_CR_GREENINVERT, i);
            
            XML.pushTag("NODE", i);
            
            ofxPatch::saveSettings(XML, false, i);
            
            XML.popTag();
            break;
        }
        
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            
            XML.addAttribute("NODE", "id", nId, lastPlace);
            XML.addAttribute("NODE", "name", name, lastPlace);
            XML.addAttribute("NODE", "type", "GLITCH_1", lastPlace);
            if (input.size()) XML.setAttribute("NODE", "inputSource", input[0]->getId(), lastPlace);
            
            XML.addAttribute("NODE","do_CONVERGENCE", do_CONVERGENCE, lastPlace);
            XML.addAttribute("NODE","do_GLOW", do_GLOW, lastPlace);
            XML.addAttribute("NODE","do_SHAKER", do_SHAKER, lastPlace);
            XML.addAttribute("NODE","do_CUTSLIDER", do_CUTSLIDER, lastPlace);
            XML.addAttribute("NODE","do_TWIST", do_TWIST, lastPlace);
            XML.addAttribute("NODE","do_OUTLINE", do_OUTLINE, lastPlace);
            XML.addAttribute("NODE","do_NOISE", do_NOISE, lastPlace);
            XML.addAttribute("NODE","do_SLITSCAN", do_SLITSCAN, lastPlace);
            XML.addAttribute("NODE","do_SWELL", do_SWELL, lastPlace);
            XML.addAttribute("NODE","do_INVERT", do_INVERT, lastPlace);
            
            XML.addAttribute("NODE","do_CR_HIGHCONTRAST", do_CR_HIGHCONTRAST, lastPlace);
            XML.addAttribute("NODE","do_CR_BLUERAISE", do_CR_BLUERAISE, lastPlace);
            XML.addAttribute("NODE","do_CR_REDRAISE", do_CR_REDRAISE, lastPlace);
            XML.addAttribute("NODE","do_CR_GREENRAISE", do_CR_GREENRAISE, lastPlace);
            XML.addAttribute("NODE","do_CR_BLUEINVERT", do_CR_BLUEINVERT, lastPlace);
            XML.addAttribute("NODE","do_CR_REDINVERT", do_CR_REDINVERT, lastPlace);
            XML.addAttribute("NODE","do_CR_GREENINVERT", do_CR_GREENINVERT, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}

