#version 330 core

layout (location =  0) in vec3 Position;
layout (location =  1) in vec4 Colour;

out vec4 VertexColour;

uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;

void main()
{
    gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(Position, 1.0);
    VertexColour = Colour;
}
