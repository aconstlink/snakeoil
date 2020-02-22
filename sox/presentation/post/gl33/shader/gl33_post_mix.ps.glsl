
#version 330

layout( location = 0 ) out vec4 out_color ;

uniform sampler2D u_smp_color_a ; // scene a
uniform sampler2D u_smp_color_b ; // scene b
uniform sampler2D u_smp_color_x ; // transition color
uniform sampler2D u_smp_mask ; // transition mask

in vertex_data
{
    vec2 tx ;
} psi ;

void main()
{
    vec4 tex_a = texture( u_smp_color_a, psi.tx );
    vec4 tex_b = texture( u_smp_color_b, psi.tx );

    vec4 tex_x = texture( u_smp_color_x, psi.tx );
    vec4 tex_m = texture( u_smp_mask, psi.tx );

    vec4 color_a = mix( tex_a, tex_b, tex_m.r ) ;
    vec4 color_b = mix( tex_x, color_a, tex_m.g ) ;
      
    out_color = color_b ;
    //out_color = mix( color_a, color_b, tex_x.a) ;    
    //out_color = vec4(.8,0.8,.8,1.0);
}
