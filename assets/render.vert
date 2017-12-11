#version 330 core

layout (location = 0) in vec3 position;    // POSITION_INDEX

uniform mat4 ciModelViewProjection;


void main(void)
{
    gl_Position = ciModelViewProjection * vec4(position, 1.0);

}

