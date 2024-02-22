#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2,TexCoord), 1);
    vec4 Color = texture(texture1, TexCoord);
    Color.a *= 0.01f;
    clamp(Color.a, 0.0f ,1.0f );
    FragColor = Color;
}
