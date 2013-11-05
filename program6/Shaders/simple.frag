
//#version 130
in vec4 smoothColor;

/*
void main()
{
	//gl_FragColor = vec4(0.1, 0.6, 0.8, 1.0);
	gl_FragColor = vec4(smoothColor.xyz,1);
}
*/

in vec3 fColor;
in float d;
in float s;
void main()
{
	vec4 origColor  = vec4(smoothColor.xyz,1);
	//vec4 origColor  = vec4(fColor.xyz,1);
	vec4 white = vec4(1);
	
	vec4 aColor = origColor * 0.1; //0.02;
	vec4 dColor = d * origColor;
	vec4 sColor = s * white;
	
	gl_FragColor =dColor+aColor+sColor;
	
}
	
