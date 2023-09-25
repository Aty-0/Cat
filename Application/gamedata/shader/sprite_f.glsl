#version 330 core
out vec4 FragColor;

in vec4 drawable_color;

uniform sampler2D texture;


void main()
{ 
    //FragColor = texture(model_texture, model_uv);
    FragColor = drawable_color;
}