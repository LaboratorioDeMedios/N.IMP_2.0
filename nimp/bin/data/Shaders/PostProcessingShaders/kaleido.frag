#version 150

//uniform sampler2DRect tex0;
//uniform sampler2DRect maskTex;
//uniform float texOpacity;
//uniform float maskOpacity;
//
//in vec2 varyingtexcoord;
//out vec4 outputColor;
//
//void main (void){
//    vec2 pos = varyingtexcoord;
//    vec4 src = texture(tex0, pos);
//    outputColor = vec4( src.r, 0, src.b, 1.0);
////    float mask = texture(maskTex, pos).r;
////    outputColor = vec4( src.rgb * texOpacity , clamp( min(src.a,mask) , maskOpacity, 1.0));
//}

uniform sampler2DRect tex0;
//uniform sampler2DRect maskTex;
//uniform float texOpacity;
//uniform float maskOpacity;
uniform float segments;

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void){
//    vec2 pos = varyingtexcoord;
//    vec4 src = texture(tex0, pos);
//    outputColor = vec4( src.r, 0, src.b, 1.0);
    
    vec2 uv = varyingtexcoord;
    vec2 normed = 2.0 * uv - 1.0;
    float r = length(normed);
    float theta = atan(normed.y / abs(normed.x));
    theta *= segments;
    
    vec2 newUv = (vec2(r * cos(theta), r * sin(theta)) + 1.0) / 2.0;
    
    outputColor = texture(tex0, newUv);
}

//string fragShaderSrc = GLSL(150,
//                            uniform sampler2DRect tex;
//                            uniform float segments;
//                            
//                            in vec2 varyingtexcoord;
//                            out vec4 outputcolor;
//                            
//                            void main()
//                            {
//                                vec2 uv = varyingtexcoord;
//                                vec2 normed = 2.0 * uv - 1.0;
//                                float r = length(normed);
//                                float theta = atan(normed.y / abs(normed.x));
//                                theta *= segments;
//                                
//                                vec2 newUv = (vec2(r * cos(theta), r * sin(theta)) + 1.0) / 2.0;
//                                
//                                outputcolor = texture(tex, newUv);
//                            }
//                            );
