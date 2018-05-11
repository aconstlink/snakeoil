
#version 330

layout( location = 0 ) out vec4 out_color ;

uniform sampler2D u_smp_color ;

in vertex_data
{
    vec2 tx ;
} psi ;

void main()
{
    //vec2 v = psi.tx * vec2(2.0) - vec2(1.0) ;
    //float l = length( v * vec2(0.3,0.9) ) ;
    //l = pow( l, 2 ) ;
    
    //float vignette = 1.0 - pow( l, 1 ) ;

    vec4 color_image = texture( u_smp_color, psi.tx );

    out_color = color_image * vec4(0.9,0.9,0.9,1.0) ;
    //out_color *= vignette ;
 
    //out_color = vec4( 1.0,1.0,1.0,1.0 ) ;

    //out_color = vec4(1.0, 0.0, 0.0, 1.0 ) ;
}
