
#version 330

in vec3 in_pos ;

// layout:
// vec4( pos0.xy, pos1.xy )
// vec4( color.rgba )
uniform samplerBuffer u_line_info ;

uniform uint u_offset ;
uniform mat4 u_proj ;
uniform mat4 u_view ;

out vertex_data
{
  vec4 color ;
  
} vso ;

void main( void )
{
  int line_id  = (gl_VertexID >> 1) + int(u_offset) ; 
  int index = line_id << 1 ;

  vec4 line_info_00 = texelFetch( u_line_info, index + 0 ) ;
  vec4 line_info_01 = texelFetch( u_line_info, index + 1 ) ;

  //vec2 beg[2] = vec2[2]( vec2(0.0,0.0), line_info_00.xy ) ;
  //vec2 end[2] = vec2[2]( line_info_00.xy, line_info_00.zw ) ;

  //vec2 pos = beg[gl_VertexID%2] + end[gl_VertexID%2] ;
  
  vec2 points[2] = vec2[2]( line_info_00.xy, line_info_00.zw ) ;
  vec2 pos = points[ gl_VertexID % 2]  ;
  
  vso.color = line_info_01 ;

  gl_Position = u_proj * u_view * vec4(pos, 0.0, 1.0) ;
}
