#version 330 core

layout (location = 0) in vec3 position;    // POSITION_INDEX
layout (location = 4) in float color;    // POSITION_INDEX

uniform mat4 ciModelViewProjection;

out float vColor;
void main(void)
{
    vColor = color;
    gl_Position = ciModelViewProjection * vec4(position, 1.0);

}

