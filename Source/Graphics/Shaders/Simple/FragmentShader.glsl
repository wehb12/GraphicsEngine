#version 330 core

in vec4 VertexColour;
in vec3 VertexNormal;
in vec3 FragmentPosition;

out vec4 FragmentColour;

uniform vec4 LightColour;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;

void main()
{
    float AmbientLightStrength = 0.1;
    vec3 Ambient = vec3(AmbientLightStrength * LightColour);
    
    vec3 Normal = normalize(VertexNormal);
    vec3 LightDirection = normalize(LightPosition - FragmentPosition);
    float DiffuseStrength = max(dot(Normal, LightDirection), 0.0);
    vec3 Diffuse = vec3(DiffuseStrength * vec3(LightColour));

    float SpecularStrength = 0.5;
    vec3 ViewDirection = normalize(CameraPosition - FragmentPosition);
    vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    float Shininess = 32;
    float SpecularPower = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Shininess);
    vec3 Specular = vec3(SpecularStrength * SpecularPower * LightColour);

    FragmentColour = vec4((Ambient + Diffuse + Specular), 1.0) * VertexColour;
}
