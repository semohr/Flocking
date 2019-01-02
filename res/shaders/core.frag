#version 440 core
in vec3 ourColor;
//in vec2 TexCoord;

out vec4 color;

//Texture samplers
//uniform sampler2D ourTexture1;

void main()
{
		color = vec4(ourColor,1.0f);
}