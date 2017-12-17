
#version 330 core

layout( location = 0 ) out vec4 out_color ;

uniform sampler2D u_smp ;

in vertex_data
{
  vec2 tx ;
  vec4 color ;
} psi ;

void main()
{
  vec4 color = texture2D( u_smp, psi.tx ) ;
  out_color = vec4( color *  psi.color  ) ;
}
