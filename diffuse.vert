#version 330
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
}