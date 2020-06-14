#version 330 core

in vec4 VertexColour;

out vec4 FragmentColour;

uniform vec4 LightColour;

void main()
{
    float AmbientLightStrength = 0.1;
    vec4 Ambient = AmbientLightStrength * LightColour;
    
    FragmentColour = Ambient * VertexColour;
}
