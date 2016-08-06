uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

const float resolutionXMax = 1024.0;
const float resolutionXMin = 240.0;
const float resolutionYMax = 968.0;
const float resolutionYMin = 240.0;
const float mouseXMax = 1024.0;
const float mouseXMin = 240.0;
const float mouseYMax = 968.0;
const float mouseYMin = 240.0;

const float resolutionXDefault = 300.0;
const float resolutionYDefault = 350.0;
const float mouseXDefault = 300.0;
const float mouseYDefault = 350.0;

void main( void ) {

	vec2 position = ( gl_FragCoord.xy / resolution.xy ) + mouse / 3.0;

	float color = 0.0;
	color += sin( position.x * cos( time / 10.0 ) * 60.0 ) + cos( position.y * cos( time / 5.0 ) * 10.0 );
	color += sin( position.y * sin( time / 5.0 ) * 20.0 ) + cos( position.x * sin( time / 5.0 ) * 40.0 );
	color += sin( position.x * sin( time / 5.0 ) * 30.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
	color *= sin( time / 10.0 ) * 0.5;

	gl_FragColor = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );

}