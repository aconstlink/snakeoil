#version 330

in vec3 in_pos ;

out vertex_data
{
    vec2 tx ;
} vso ;

void main()
{
    vso.tx = sign( in_pos.xy ) * 0.5 + 0.5 ;
    
    gl_Position = vec4( sign( in_pos.xy ), 0.0, 1.0 ) ;
}
