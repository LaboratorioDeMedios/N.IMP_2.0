//
//  enumerations.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#ifndef NIMP_Prototipe_enumerations_h
#define NIMP_Prototipe_enumerations_h

#define BUFFER_SIZE 256
#define NUM_WINDOWS 80

#define NODE_WIDTH 640  //1024
#define NODE_HEIGHT 480 //576

#define MENU_ITEM_SIZE 20
#define MENU_ITEM_PADDING 7
#define MENU_HEIGHT 42
#define MENU_TOP_PADDING -17
#define RIGHT_MENU_WIDTH 25
#define RIGHT_MENU_LONG_WIDTH 250

#define AFTER_MENU_BAR_EVENT_PRIORITY 1
#define SCROLL_BAR_EVENT_PRIORITY 201
#define COMPOSER_EVENT_PRIORITY 202
#define PATCH_EVENT_PRIORITY 203
#define CONSOLE_LOG_EVENT_PRORITY 204

#define SCALE_SENSITIVITY 0.001f
#define ZOOM_SENSITIVITY .001f
#define ZOOM_DIFF .5f
#define ZOOM_UNIT 1.f

#define KEY_SCROLL_SENSITIVITY 10.f
#define SCROLL_TOLERANCE 10
#define GRIP_TOLERANCE .002f
#define SCROLL_BAR_WIDTH 13.f
#define CONSOLE_SCROLL_TOLERANCE 2

#define INSPECTOR_WIDTH 200

#define SCALE_RATIO 0.4

#define MAIN_WINDOW 1
#define CONSOLE_WINDOW 0

enum nodeLinkType
{
    STRAIGHT_LINKS = 0,
    CURVE_LINKS = 1,
    PATH_LINKS = 2
};

enum nodeType
{
    INPUT = 0,
    VISUAL_LAYER = 1,
    MIXER = 2
};

enum paramInputType
{
    MIDI,
    OSC,
    FFT
};

enum InputType
{
    VIDEO,
    CAM,
    IMAGE,
    PARTICLE,
    RIGHT_AUDIO_IN,
    LEFT_AUDIO_IN
};

enum VisualLayerType
{
    IKEDA,
    GLITCH_1,
    GLITCH_2,
    IMAGE_PROCESSOR
};

enum MixerType
{
    SIMPLE_BLEND,
    MASK,
    MULTI_CHANNEL
};

enum imgType
{
    T_IMAGE,
    T_IMAGE_SEQUENCE,
    T_MOVIE
};

enum imgExt
{
    PNG,
    OTHER
};

enum AudioInType
{
    LEFT,
    RIGHT
};


#endif
