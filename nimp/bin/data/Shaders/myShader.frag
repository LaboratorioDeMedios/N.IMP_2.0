#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect maskTex;
uniform float texOpacity;
uniform float maskOpacity;

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void){
    vec2 pos = varyingtexcoord;
    vec4 src = texture(tex0, pos);
//    outputColor = vec4( src.rgb, 1.0);
    float mask = texture(maskTex, pos).r;
    outputColor = vec4( src.rgb * texOpacity , clamp( min(src.a,mask) , maskOpacity, 1.0));
}
