#version 330 core

layout (location =  0) in vec3 Position;
layout (location =  1) in vec4 Colour;
layout (location =  2) in vec3 Normal;
layout (location =  3) in vec2 TexCoord;

out vec4 VertexColour;
out vec3 VertexNormal;
out vec2 VertexTexCoord;
out vec3 FragmentPosition;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;

void main()
{
    gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(Position, 1.0);
    FragmentPosition = vec3(ModelMatrix * vec4(Position, 1.0));
    VertexColour = Colour;
    VertexNormal = Normal;
    VertexTexCoord = TexCoord;
}
