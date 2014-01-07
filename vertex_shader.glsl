#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec4 vertexColor;

uniform mat4 allMat;

out vec4 vcol;

void main ()
{
    vcol = vertexColor;
    gl_Position = allMat * vec4 (vertex_position, 1.0);
}
