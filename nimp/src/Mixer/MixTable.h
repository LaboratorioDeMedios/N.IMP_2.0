/*
 *  MixTable.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MixTable
#define _MixTable

#include "ofMain.h"
#include "VisualLayer.h"
#include "ImageOutput.h"

// Basic class for mixtables
class MixTable : public ImageOutput{
	
public:
    
    MixTable(string name_ = "mixtable", string typeName_ = "MIXER", int id_ = -1);
    
};

#endif
