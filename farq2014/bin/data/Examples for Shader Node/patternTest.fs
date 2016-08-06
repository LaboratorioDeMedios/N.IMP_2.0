uniform vec2 resolution;
uniform float time;

const float resolutionXMax = 1024.0;
const float resolutionXMin = 240.0;
const float resolutionYMax = 968.0;
const float resolutionYMin = 240.0;

const float resolutionXDefault = 300.0;
const float resolutionYDefault = 350.0;

const float tot = 2.0*3.1415926;

void main(void){
    vec2 p;
    p.x = -1.0 + 2.0 * gl_FragCoord.x / resolution.x;
    p.y = -1.0 + 2.0 * gl_FragCoord.y / resolution.y;
    p.y *= resolution.y/resolution.x;
    p *= 100.0;
    const float n = 9.0;
    const float df = tot/n; 
    float c = 1.;
    float t = time*1.0;

    for (float phi =0.0; phi < tot; phi+=df){
        c+=cos(cos(phi)*p.x+sin(phi)*p.y+t*phi/tot);
    }

    gl_FragColor = vec4(c,c,c,1.0);
}