#version 400

in vec4 vcol;
out vec4 frag_colour;

void main ()
{
  frag_colour = vcol;
}
