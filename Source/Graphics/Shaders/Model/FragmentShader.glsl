#version 330 core

struct MaterialStruct
{
    sampler2D Diffuse;
    sampler2D Specular;
    sampler2D Normal;

    float Shininess;
};

struct LightStruct
{
    vec3 Position;
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec2 FragmentTexCoord;
in vec4 VertexColour;
in vec3 VertexNormal;
in vec3 FragmentPosition;
in vec3 TangentSpaceLightPosition;
in vec3 TangentSpaceCameraPosition;
in vec3 TangentSpaceFragmentPosition;

out vec4 FragmentColour;

uniform MaterialStruct Material;
uniform LightStruct Light;

void main()
{
    vec4 DiffuseTextureColour = texture(Material.Diffuse, FragmentTexCoord);
 
    vec3 Ambient = Light.Ambient * vec3(DiffuseTextureColour);   

    vec4 NormalTextureColour = texture(Material.Normal, FragmentTexCoord);
    vec3 Normal = (VertexNormal * NormalTextureColour.rgb) * 2.0 - 1.0;
    Normal = normalize(Normal);
    vec3 LightDirection = normalize(TangentSpaceLightPosition - TangentSpaceFragmentPosition);
    float DiffuseStrength = max(dot(Normal, LightDirection), 0.0);
    vec3 Diffuse = DiffuseStrength * Light.Diffuse * vec3(DiffuseTextureColour);
    
    vec3 ViewDirection = normalize(TangentSpaceCameraPosition - TangentSpaceFragmentPosition);
    vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    float Shininess = 32;
    float SpecularPower = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), Shininess);
    vec4 SpecularTextureColour = texture(Material.Specular, FragmentTexCoord);
    vec3 Specular = SpecularPower * Light.Diffuse * vec3(SpecularTextureColour);

    FragmentColour = vec4((Ambient + Diffuse + Specular), DiffuseTextureColour.a);
}
