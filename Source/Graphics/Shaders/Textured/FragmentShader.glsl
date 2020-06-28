#version 330 core

struct MaterialStruct
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

in vec4 VertexColour;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec3 FragmentPosition;

out vec4 FragmentColour;

uniform sampler2D Texture;
uniform vec4 LightColour;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform MaterialStruct Material;

void main()
{
    float AmbientLightStrength = 0.1;
    vec3 Ambient = vec3(AmbientLightStrength * LightColour) * Material.Ambient;
    
    vec3 Normal = normalize(VertexNormal);
    vec3 LightDirection = normalize(LightPosition - FragmentPosition);
    float DiffuseStrength = max(dot(Normal, LightDirection), 0.0);
    vec3 Diffuse = vec3(DiffuseStrength * LightColour) * Material.Diffuse;
    
    float SpecularStrength = 0.5;
    vec3 ViewDirection = normalize(CameraPosition - FragmentPosition);
    vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    float Shininess = 32;
    float SpecularPower = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Material.Shininess);
    vec3 Specular = vec3(SpecularStrength * SpecularPower * LightColour) * Material.Specular;

    vec4 TextureColour = texture(Texture, VertexTexCoord);
    FragmentColour = vec4((Ambient + Diffuse + Specular), 1.0) * TextureColour;
}
