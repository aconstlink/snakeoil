
#version 330

in vec3 in_pos  ;

out vertex_data
{
    vec4 color ;
} vso ;

uniform mat4 u_proj ;
uniform mat4 u_view ;
uniform vec4 u_pos_wh ;

void main( void ) 
{
    vec3 pos = in_pos+vec3(0.5) ;
    pos *= 10.0 ;
    //pos.y *= 4.0 ;
    pos.y *= u_pos_wh.w ;
    pos.xy += u_pos_wh.xy ;
    gl_Position = u_proj * u_view * vec4( pos, 1.0 ) ;
    //gl_Position = vec4( in_pos, 1.0 ) ;
}
