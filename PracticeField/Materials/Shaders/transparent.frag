#version 330 core

out vec4 out_color;

in vec2 UV;

uniform sampler2D texture_diffuse;

void main(){
	vec4 texColor = texture(texture_diffuse, UV);
	if(texColor.a < 0.05)discard;
	out_color = texColor;
}