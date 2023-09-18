#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 texCoord;

// Out to fragment shader
out vec3 model_color; 
out vec2 model_uv; 

// Engine time
uniform float delta_time;
uniform float time;
uniform float aspect_ratio;

// Game object world matrix
uniform mat4 transform;

// Camera matrices 
uniform mat4 view;
uniform mat4 projection;

// Sprite 

// Rect for texture atlas 
uniform vec4 sprite_tile_rect;
uniform vec2 sprite_texture_res;

vec2 GetAtlasUV(vec2 uv)
{
    if(sprite_tile_rect.z + sprite_tile_rect.w == 0)
        return uv;

    vec2 size = vec2(1.0f  / (length(sprite_texture_res) / sprite_tile_rect.z), 1.0f / (length(sprite_texture_res) / sprite_tile_rect.w));
    vec2 offset = vec2(size.x * sprite_tile_rect.x, size.y * sprite_tile_rect.y);   

    return uv * size + offset;
}

void main()
{
    gl_Position = projection * view * transform * vec4((aPos / aspect_ratio), 1.0);
    model_uv = GetAtlasUV(texCoord);
    model_color = aColor;
}