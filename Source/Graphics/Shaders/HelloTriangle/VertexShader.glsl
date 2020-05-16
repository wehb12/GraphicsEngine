#version 330 core

layout (location =  0) in vec3 Position;
layout (location =  1) in vec4 Colour;
layout (location =  2) in vec2 TexCoord;

out vec4 VertexColour;
out vec2 VertexTexCoord;

void main()
{
    gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
    VertexColour = Colour;
    VertexTexCoord = TexCoord;
}
