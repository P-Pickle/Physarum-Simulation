#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D TrailMap;
//uniform sampler2D texture2;

void main()
{
 
    FragColor = texture(TrailMap,TexCoord);
}
