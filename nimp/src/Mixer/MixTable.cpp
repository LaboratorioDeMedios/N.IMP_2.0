/*
 *  MixTable.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixTable.h"

MixTable::MixTable(string name_, string typeName_, int id_):ImageOutput(name_, typeName_, id_){
    ofFbo::Settings s;
    s.width			 = width;
    s.height		 = height;
    s.internalformat = GL_RGBA;
    s.useDepth		 = true;
    fbo.allocate(s);
    
    nodeType = MIXER;
    
    addInputDot();
}
