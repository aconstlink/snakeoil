
#version 330

in vec3 in_pos ;

// layout:
// vec4( pos.xy, scale.xy )
// vec4( color.rgba )
uniform samplerBuffer u_rect_info ;

uniform uint u_offset ;

out vertex_data
{
  vec4 color ;
} vso ;

void main( void )
{
  int rect_id = (gl_VertexID >> 2) + int(u_offset) ;
  int index = rect_id << 1 ;

  vec4 d0 = texelFetch( u_rect_info, index + 0 ) ;
  vec4 d1 = texelFetch( u_rect_info, index + 1 ) ;
  
  vso.color = d1 ;
  
  vec2 pos = in_pos.xy * d0.zw + d0.xy ;
  gl_Position = vec4( pos, 0.0, 1.0) ;
}
