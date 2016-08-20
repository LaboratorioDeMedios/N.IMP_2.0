#version 150

uniform sampler2DRect backbuffer;
uniform sampler2DRect tex0;
uniform vec2  size0;               // tex0 resolution
uniform vec2  resolution;          // Patch resolution
uniform vec2  window;              // Window resolution
uniform vec2  screen;              // Screen resolution
uniform vec2  mouse;               // Mouse position on the screen
uniform float time;                // seconds

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void){
    vec2 pos = varyingtexcoord;
    vec4 color = texture(tex0, pos);
//    outputColor = vec4( src.rgb, 1.0);
//    float mask = texture(maskTex, pos).r;
    outputColor = vec4( color.rgb, color.a);
}
