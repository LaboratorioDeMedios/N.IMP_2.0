uniform float time;
uniform vec2 param4;

const float param4XMax = 1024.0;
const float param4XMin = 240.0;
const float param4YMax = 968.0;
const float param4YMin = 240.0;
const float param4ZMax = 1024.0;
const float param4ZMin = 240.0;
const float param4WMax = 968.0;
const float param4WMin = 240.0;

const float param4XDefault = 300.0;
const float param4YDefault = 350.0;
const float param4ZDefault = 300.0;
const float param4WDefault = 350.0;

void main( void ) {

	vec2 position = ( gl_FragCoord.xy / param4.xy ) + param4.zw / 3.0;

	float color = 0.0;
	color += sin( position.x * cos( time / 10.0 ) * 60.0 ) + cos( position.y * cos( time / 5.0 ) * 10.0 );
	color += sin( position.y * sin( time / 5.0 ) * 20.0 ) + cos( position.x * sin( time / 5.0 ) * 40.0 );
	color += sin( position.x * sin( time / 5.0 ) * 30.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
	color *= sin( time / 10.0 ) * 0.5;

	gl_FragColor = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );

}