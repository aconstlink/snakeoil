
#version 330 core

layout( location = 0 ) out vec4 out_color ;

uniform sampler2D u_smp_glyph ;

in vertex_data
{
    vec2 txc ;
    flat int layer ;
    flat vec3 color ;
} psi ;

void main()
{    
    float mask = texture( u_smp_glyph, psi.txc )[psi.layer] ;
    out_color = vec4( psi.color * mask, mask ) ;

    //out_color = vec4(1.0,0.0,0.0,1.0) ;
}
