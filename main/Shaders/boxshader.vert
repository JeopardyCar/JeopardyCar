//#version 130

uniform mat4 M;

attribute vec3 pos;
varying vec4 smoothColor;

void main()
{
	vec4 p = vec4(pos, 1);
    
	gl_Position = M*p;
    
    if(pos.z > 0.){
        smoothColor = vec4(0.2, 0.2, 0.2, 1.0);
    }
    else
    {
        smoothColor = vec4(0.7, 0.7, 0.7, 1.0);
    }
}