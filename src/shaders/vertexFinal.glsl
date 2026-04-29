#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform Transforms {
    mat4 view;
    mat4 projection;
};

layout(std140) uniform instances {
    mat4 modelMatrices[20];
};

uniform shadowMapMatrix {
    mat4 lightSpaceMatrix;
};

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

void main()
{
    mat4 model = modelMatrices[gl_InstanceID];
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}
