//#version 130

uniform mat4 M;

attribute vec3 pos;
varying vec4 smoothColor;



attribute vec3 norm;
varying float s;
varying float d;
uniform mat4 P;
uniform mat4 C;
void main()
{
	vec4 p = vec4(pos, 1);
	gl_Position = M*p;
    if(pos.z > 0.){
        smoothColor = vec4(1, 1, 1, 1.0);
    }
    else
    {
        smoothColor = vec4(.9, .9, .9, 1.0);
    }



	// for testing
	vec3 norm = vec3(0.,0.,1.);

	mat4 R =mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	vec4 lightPos = vec4(0,0,2,1);
	vec4 p2 = C*vec4(pos, 1);
	vec4 lPos = C*lightPos;
	vec4 l = normalize(lPos - p2); 
	vec4 n = vec4(norm,0.);  //normal vector, unchanged, so should still be normalized
	n = C*R*n; 
	d = clamp(dot(l,n), 0., 1.);
	vec4 camPos = vec4(0);  //camera is always at origin in view coords!
	vec4 v = normalize(camPos-p2);  //view vector goes to camera, normalized
	vec4 r = reflect(-l,n);  //light vector points away from surface, but reflect() needs incident, so invert
	s = clamp(dot(v,r), 0., 1.);  //only valid in range [0,1]
	s = pow(s, 10.);
	
}