/*
 *  Param.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Param
#define _Param
#include "ofMain.h"


class Param {
	
  public:
	
	Param();
    string  name;
    int     imageInputId;
    int     intVal;
    string  stringVal;
    float   floatVal;
    float   value;
    int     midiControl;
    int     inputMin;
    int     inputMax;
};

#endif
