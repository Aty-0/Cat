#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 texCoord;

uniform float delta_time;
uniform float time;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 model_color; 
out vec2 model_uv; 

void main()
{
    gl_Position =  projection * view * transform * vec4(aPos, 1.0);
    model_uv = texCoord;


    model_color = vec3(sin(time), cos(time), sin(time)) + vec3(cos(time), 1.0f, sin(time));
}