//Fragment shader
#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
void main()
{
    FragColor = vec4(lightColor, 0.5f);
}
