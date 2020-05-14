#version 330 core

in vec4 VertexColour;

out vec4 FragmentColour;

void main()
{
    FragmentColour = VertexColour;
}
