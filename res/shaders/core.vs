#version 440 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;
//out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;


void main()
{
		//gl_Position = vec4(position, 1.0f);
    gl_Position = projection * camera * model * vec4(position, 1.0f);
    ourColor = color;
    //TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}