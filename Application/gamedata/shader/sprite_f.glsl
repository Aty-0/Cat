#version 330 core
out vec4 FragColor;

in vec4 drawable_color;
in vec2 drawable_uv;
uniform sampler2D drawable_texture;


void main()
{ 
    vec4 texture = texture(drawable_texture, drawable_uv);
    if(texture.a < 0.1)
        discard;
        
    FragColor = texture * drawable_color;
}