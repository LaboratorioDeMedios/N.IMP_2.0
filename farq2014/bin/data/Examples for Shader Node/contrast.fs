uniform sampler2DRect tex0;
uniform float Contrast;
uniform float Brightness;

const float ContrastMax = 1.0;
const float ContrastMin = 0.1;
const float BrightnessMax = 1.0;
const float BrightnessMin = 0.1;
const float ContrastDefault = 1.0;
const float BrightnessDefault = 1.0;

void main()
{

	vec4 color = texture2DRect(tex0,gl_TexCoord[0].st);

	//adjust brightness and convert to grayscale
	float p = 0.3 *color.g + 0.59*color.r + 0.11*color.b;
	p = p * Brightness;
	color = vec4(p,p,p,1.0);
	color = mix( vec4(1.0,1.0,1.0,1.0),color,Contrast);
	
	gl_FragColor =  vec4(color.r , color.g, color.b, 1.0);
}
