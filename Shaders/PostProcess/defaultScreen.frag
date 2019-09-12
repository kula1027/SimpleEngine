#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
	FragColor = texture(screenTexture, TexCoords);		
	//FragColor = vec4(1, 0, 1, 1);
}