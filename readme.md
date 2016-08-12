N.IMP
=====

Node-based Image Processor

Building
---------------
To build this project go to /bin/data path and run setup.sh script


Description
---------------
N.IMP is an audiovisual real-time content generator made in Open Frameworks. It is a node-based arquitecture which has different types of inputs to feed content to the application, then layers and mixers to process them and generates a new output. It has a syphon node to export the generated content.

For the moment it's only available for mac.

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

- IKEDA Layer, Glitch1 Layer, Glitch2 Layer, Image Processor

Greyscale effects, Canny's algorithm, several other effects.

- Shaders

Create your own shaders to apply different transformations. It supports GLSL version 120


### Mixers

- Multi-Channel Switch (up to 8 inputs), Mix Simple Blend, Mix Mask

Mix different inputs and apply transformations


### Outputs

- Syphon Output

Export generated content to syphon server.



### Screenshot
![alt tag](/screenShot.png?raw=true "Demo")


### Used OF addons
Forked this addons and made our customization.

https://github.com/patriciogonzalezvivo/ofxComposer

https://github.com/rezaali/ofxUI

https://github.com/neilmendoza/ofxPostProcessing

https://github.com/astellato/ofxSyphon

https://github.com/danomatika/ofxMidi

https://github.com/jens-a-e/ofxMultiTouchPad

https://github.com/Akira-Hayasaka/ofxPSBlend

https://github.com/maxillacult/ofxPostGlitch

https://github.com/bangnoise/ofxHapPlayer

https://github.com/brinoausrino/ofxImageSequencePlayer

https://github.com/2bbb/ofxJpegGlitch

https://github.com/kronick/ofxAVFVideoPlayer

https://github.com/leozimmerman/ofxFilterbank

