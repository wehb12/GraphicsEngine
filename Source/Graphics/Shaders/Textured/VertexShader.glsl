#version 330 core

layout (location =  0) in vec3 Position;
layout (location =  2) in vec3 Normal;
layout (location =  3) in vec3 Tangent;
layout (location =  4) in vec2 TexCoord;

out vec2 FragmentTexCoord;
out vec3 FragmentPosition;
out vec3 TangentSpaceLightPosition;
out vec3 TangentSpaceCameraPosition;
out vec3 TangentSpaceFragmentPosition;

struct LightStruct
{
    vec3 Position;
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

uniform mat4 ModelMatrix;
uniform mat4 ProjectionViewMatrix;
uniform vec3 CameraPosition;
uniform LightStruct Light;

void main()
{
    gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(Position, 1.0);
    FragmentPosition = vec3(ModelMatrix * vec4(Position, 1.0));
    FragmentTexCoord = TexCoord;

    mat3 NormalMatrix = transpose(inverse(mat3(ModelMatrix)));
    vec3 T = normalize(vec3(NormalMatrix * Tangent));
    vec3 N = normalize(vec3(NormalMatrix * Normal));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentSpaceLightPosition = TBN * Light.Position;
    TangentSpaceCameraPosition = TBN * CameraPosition;
    TangentSpaceFragmentPosition = TBN * FragmentPosition;
}
