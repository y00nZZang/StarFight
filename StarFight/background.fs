#version 330 core

in vec2 toTexCoord;
in vec4 toColor;

out vec4 FragColor;

// texture sampler
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, toTexCoord);
    //FragColor = vec4(1,0,0,1);
}
