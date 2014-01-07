
#version 400

layout(location = 0) in vec3 vertexPosition;

uniform vec4 vertexColor;
uniform mat4 allMat;

out vec4 outColor;

void main ()
{
    outColor = vertexColor;
    gl_Position = allMat * vec4(vertexPosition, 1.0);
}
