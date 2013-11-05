//#version 150

uniform mat4 M;
attribute vec3 move;
attribute vec3 pos;
//in vec2 pos;
attribute vec3 colorIn;

uniform mat4 P;
uniform mat4 C;
uniform float time;
varying float d;
varying float s;
attribute vec3 norm;
varying vec3 fColor;
vec4 lightPos = vec4(2,2,1.1,1);



varying vec4 smoothColor;
mat4 rotateY(float a)
{
	return mat4(
		cos(a), -sin(a),  0, 0,
		sin(a), cos(a), 0, 0,
		0, 0, 1, 0,
		
		0, 0, 0, 1);
}
/*
void main()
{	
	vec4 p = vec4(pos, 1);
	gl_Position = M*p + vec4(move,0);
	smoothColor = vec4(colorIn.xyz,1);
}
*/

void main()
{		
	//mat4 R = rotateY(time*0.6);
	mat4 R =mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	
	lightPos = vec4(3.+2.*cos(time)*2.,3.+2.*sin(time)*2.,1.5,1.);

	vec4 p = C*vec4(pos, 1);  //4d position that is rotating, in view coords

	vec4 lPos = C*lightPos;
	// diffuse coefficent //
	vec4 l = normalize(lPos - p);  //vector to the light, normalized
	vec4 n = vec4(norm,0);  //normal vector, unchanged, so should still be normalized 
	n = C*R*n;  //normals must move with triangle faces
	d = clamp(dot(l,n), 0., 1.);  //only valid in range [0,1]
	
	
	// specular coefficient //
	vec4 camPos = vec4(0);  //camera is always at origin in view coords!
	vec4 v = normalize(camPos-p);  //view vector goes to camera, normalized
	vec4 r = reflect(-l,n);  //light vector points away from surface, but reflect() needs incident, so invert
	s = clamp(dot(v,r), 0., 1.);  //only valid in range [0,1]
	s = pow(s, 10.);  //exponent controls sharpness of specular highlight
	


	//gl_Position = P*p;  //project point into clip space
	gl_Position = P*p + vec4(move,0);
	fColor = norm;
	smoothColor = vec4(colorIn.xyz,1);
}
