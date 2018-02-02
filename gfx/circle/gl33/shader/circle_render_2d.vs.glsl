
#version 330

in vec3 in_pos ;

// layout:
// vec4( pos.xy, scale.xy )
// vec4( pivot.xy, unused.xy )
// vec4( color.rgba )
uniform samplerBuffer u_rect_info ;

uniform uint u_per_circ_verts ;
uniform uint u_offset ;
uniform mat4 u_proj ;

out vertex_data
{
  vec4 color ;
} vso ;

void main( void )
{
  int rect_id = (gl_VertexID /int(u_per_circ_verts)) + int(u_offset) ;
  int index = rect_id * 3 ;

  vec4 d0 = texelFetch( u_rect_info, index + 0 ) ;
  vec4 d1 = texelFetch( u_rect_info, index + 1 ) ;
  vec4 d2 = texelFetch( u_rect_info, index + 2 ) ;
  
  vso.color = d2 ;
  
  vec2 rpos = (in_pos.xy) * vec2(0.5) + vec2(0.5) ;
  rpos = rpos - d1.xy ;

  vec2 pos = rpos * d0.zw + d0.xy ;
  gl_Position = u_proj * vec4( pos, 1.0, 1.0) ;
}
