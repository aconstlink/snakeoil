
#version 330 

in vec3 in_pos ;

// layout:
// vec4( pos.xy, scale.xy )
// vec4( color.rgba )
// vec4( texcoords.xy, texcoords.wh )
// vec4( rot.z, vec3(unused) )
uniform samplerBuffer u_image_info ;
uniform uint u_offset ;

out vertex_data
{
  vec2 tx ;
} vso ;

void main()
{
  int image_id = (gl_VertexID >> 2) + int(u_offset) ; 
  int index = image_id << 2 ;

  vec4 d0 = texelFetch( u_image_info, index + 0 ) ;
  vec4 d1 = texelFetch( u_image_info, index + 1 ) ;
  vec4 d2 = texelFetch( u_image_info, index + 2 ) ;
  vec4 d4 = texelFetch( u_image_info, index + 3 ) ;

  vec2 tx  = sign(in_pos.xy) * vec2(0.5) + vec2(0.5) ;
  vso.tx = d2.xy + tx * d2.zw ;

  vec2 pos = sign(in_pos.xy) * d0.zw + d0.xy ;
  gl_Position = vec4( pos, 0.0, 1.0) ;
}
