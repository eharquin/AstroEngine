#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout( push_constant ) uniform constants
{
	mat4 data;
	mat4 transform_matrix;
} pushConstants;

void main() {
    gl_Position = pushConstants.transform_matrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}