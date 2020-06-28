#version 330 core

struct MaterialStruct
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct LightStruct
{
    vec3 Position;
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec4 VertexColour;
in vec3 VertexNormal;
in vec2 VertexTexCoord;
in vec3 FragmentPosition;

out vec4 FragmentColour;

uniform sampler2D Texture;
uniform vec3 CameraPosition;
uniform MaterialStruct Material;
uniform LightStruct Light;

void main()
{
    vec3 Ambient = Light.Ambient * Material.Ambient;
    
    vec3 Normal = normalize(VertexNormal);
    vec3 LightDirection = normalize(Light.Position - FragmentPosition);
    float DiffuseStrength = max(dot(Normal, LightDirection), 0.0);
    vec3 Diffuse = DiffuseStrength * Light.Diffuse * Material.Diffuse;
    
    vec3 ViewDirection = normalize(CameraPosition - FragmentPosition);
    vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    float Shininess = 32;
    float SpecularPower = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Material.Shininess);
    vec3 Specular = SpecularPower * Light.Diffuse * Material.Specular;

    vec4 TextureColour = texture(Texture, VertexTexCoord);
    FragmentColour = vec4((Ambient + Diffuse + Specular), 1.0) * TextureColour;
}
