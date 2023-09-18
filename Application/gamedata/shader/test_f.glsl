#version 330 core
out vec4 FragColor;

in vec3 model_color;
in vec2 model_uv;
uniform sampler2D model_texture;

void main()
{
    FragColor = vec4(model_color,1.0f);
     
   // FragColor = texture(model_texture, model_uv);
}