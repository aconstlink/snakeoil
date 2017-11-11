
#version 330 core

layout( location = 0 ) out vec4 out_color ;

in vertex_data
{
  vec4 color ;
} psi ;

void main()
{    
    out_color = psi.color ;
}
