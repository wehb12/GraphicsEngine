#version 330 core

in vec4 VertexColour;
in vec2 VertexTexCoord;

out vec4 FragmentColour;

uniform sampler2D Texture;
uniform vec4 LightColour;

void main()
{
    float AmbientLightStrength = 0.1;
    vec4 Ambient = AmbientLightStrength * LightColour;

    FragmentColour = Ambient * texture(Texture, VertexTexCoord);
}
