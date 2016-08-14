N.IMP
=====

Node-based Image Processor

Building
---------------
To build this project go to /bin/data path and run setup.sh script

N.IMP uses Openframeworks v0.8.3.


Description
---------------
N.IMP is an audiovisual real-time content generator made in Open Frameworks. It is a node-based arquitecture which has different types of inputs to feed content to the application, then layers and mixers to process them and generates a new output. It has a syphon node to export the generated content.

For the moment it's only available for mac.

http://nimp.uy

License
---------------

Copyright (c) 2016 Christian Clark, Brian Eschrich, Milagros Garicoits, Nicolás Peri.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### Inputs

- Image and video Sequence

Drag and drop an image or video file to the workspace to create an input of this type. It supports HAP codec for videos.

- Camera

Recognizes every webcam connected to the host.

- Particle Generator

Random particle generator. Control lifetime and gravity/repulsion points.

- Syphon

Consume content published in syphon.

- Audio/OSC/MIDI

Control other node's parameters with audio or midi/osc messages.


### Visual Layers

- IKEDA Layer [deprecated], Glitch1 Layer, Glitch2 Layer [deprecated], Image Processor

Greyscale effects, Canny's algorithm, several other effects.

- Shaders

Create your own shaders to apply different transformations. It supports GLSL version 120


### Mixers

- Multi-Channel Switch (up to 16 inputs), Mix Simple Blend, Mix Mask

Mix different inputs and apply transformations


### Outputs

- Syphon Output

Export generated content to syphon server.



### Screenshot
![alt tag](/screenShot.png?raw=true "Demo")


### Used OF addons
To develop N.IMP, we have created these forks from some great addons:

git clone https://github.com/LaboratorioDeMedios/ofxPostProcessing_nimp

git clone https://github.com/LaboratorioDeMedios/ofxSyphon_nimp

git clone https://github.com/LaboratorioDeMedios/ofxMidi_nimp

git clone https://github.com/LaboratorioDeMedios/ofxMultiTouchPad_nimp

git clone https://github.com/LaboratorioDeMedios/ofxPSBlend_nimp

git clone https://github.com/LaboratorioDeMedios/ofxPostGlitch_nimp

git clone https://github.com/LaboratorioDeMedios/ofxHapPlayer_nimp

git clone https://github.com/LaboratorioDeMedios/ofxImageSequencePlayer_nimp

git clone https://github.com/LaboratorioDeMedios/ofxJpegGlitch_nimp

git clone https://github.com/LaboratorioDeMedios/ofxAVFVideoPlayer_nimp

git clone https://github.com/LaboratorioDeMedios/ofxComposer_nimp

git clone https://github.com/LaboratorioDeMedios/ofxGui_nimp

git clone https://github.com/LaboratorioDeMedios/ofxUI_nimp

git clone https://github.com/LaboratorioDeMedios/ofxFilterbank_nimp

git clone https://github.com/LaboratorioDeMedios/ofxMSAInteractiveObject_nimp

git clone https://github.com/LaboratorioDeMedios/ofxCv_nimp.git



