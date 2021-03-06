
#version 330 

in vec3 in_pos ;

// layout:
// vec4( pos.xy, scale.xy )
// vec4( rot.z, pivot.xy, unused )
// vec4( texcoords.xy, texcoords.wh )
// vec4( color.rgba )
uniform samplerBuffer u_image_info ;
uniform uint u_offset ;

uniform mat4 u_proj ;

out vertex_data
{
  vec2 tx ;
  vec4 color ;
} vso ;

mat2 rotate( float r )
{
  return mat2( 
              cos(r), sin(r), 
              -sin(r), cos(r) ) ;
}

void main()
{
  int image_id = (gl_VertexID >> 2) + int(u_offset) ; 
  int index = image_id << 2 ;

  vec4 d0 = texelFetch( u_image_info, index + 0 ) ;
  vec4 d1 = texelFetch( u_image_info, index + 1 ) ;
  vec4 d2 = texelFetch( u_image_info, index + 2 ) ;
  vec4 d3 = texelFetch( u_image_info, index + 3 ) ;

  vso.color = d3 ;

  vec2 tx  = sign(in_pos.xy) * vec2(0.5) + vec2(0.5) ;
  vso.tx = d2.xy + tx * d2.zw ;

  vec2 rpos = sign(in_pos.xy) * vec2(0.5) + vec2(0.5) ;
  rpos = rpos - d1.yz ;

  vec2 pos = rotate(d1.x) * (rpos* d0.zw ) ;
  pos = pos + d0.xy ;
  gl_Position = u_proj * vec4( pos, 1.0, 1.0) ;
}
