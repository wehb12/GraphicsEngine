#version 330 core

in vec4 VertexColour;
in vec2 VertexTexCoord;

out vec4 FragmentColour;

uniform sampler2D Texture;

void main()
{
    FragmentColour = texture(Texture, VertexTexCoord);
}
