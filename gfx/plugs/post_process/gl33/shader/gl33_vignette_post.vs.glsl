#version 330

in vec3 in_pos ;

uniform vec2 u_bb_dims ;
uniform vec2 u_fb_dims ;

out vertex_data
{
    vec2 tx ;
} vso ;

void main()
{
    // aspect ratio back buffer
    float a = u_bb_dims.x / u_bb_dims.y ;
    vec2 b = u_fb_dims / u_bb_dims ;

    float sx = b.x ;
    float sy = b.y ;

    vso.tx = sign( in_pos.xy ) * 0.5 + 0.5 ;
    
    gl_Position = vec4( sign( in_pos.xy ) * vec2(1.0,sx/sy), 0.0, 1.0 ) ;
}
