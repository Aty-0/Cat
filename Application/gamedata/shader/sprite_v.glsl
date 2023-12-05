#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor; 
layout (location = 2) in vec2 texCoord;


struct base_t
{
    float delta_time;
    float time;
    float aspect_ratio;
};

uniform base_t base;
struct transform_t
{
    mat4 world;
    mat4 view;
    mat4 projection;
};
uniform transform_t transform;
  
struct drawable_t
{
    vec4 color;
};
uniform drawable_t drawable;

out vec4 drawable_color;
out vec2 drawable_uv;

void main()
{
    gl_Position = transform.projection * transform.view * transform.world * vec4(aPos, 1.0);
    drawable_color = drawable.color;
    drawable_uv = texCoord;
}