//#version 130
//varying vec4 smoothColor;
varying float s;
varying float d;
//varying vec2 UV;
uniform sampler2D Sampler;

void main()
{
	//gl_FragColor = smoothColor;
	vec4 smoothColor = texture2D(Sampler,gl_TexCoord[0].st);
    //vec4 color = vec4(0.1, 0.1, 0.8, 1.0);
	vec4 white = vec4(1);
	vec4 aColor = smoothColor*.2 ;
	vec4 dColor = d * smoothColor;
	vec4 sColor = s*white;
	gl_FragColor = smoothColor;
}
