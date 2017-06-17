#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 test = texture2D(texture_1, texcoord);
	if(test.w < 0.5f) discard;
	else 
	{
		vec4 color1 = texture2D(texture_1, texcoord);
		vec4 color2 = texture2D(texture_2, texcoord);
		out_color = mix(color1, color2, 0.7);
	}
}