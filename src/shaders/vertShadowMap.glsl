#version 460 core
layout(location = 0) in vec3 aPos;

uniform shadowMapMatrix {
    mat4 lightSpaceMatrix;
};

layout(std140) uniform instances {
    mat4 modelMatrices[20];
};

void main()
{
    mat4 model = modelMatrices[gl_InstanceID];
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
