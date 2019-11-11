
#version 330

layout( location = 0 ) out vec4 out_color ;

uniform sampler2D u_smp_color ;

in vertex_data
{
    vec2 tx ;
} psi ;

void main()
{
    vec4 color_image = texture( u_smp_color, psi.tx );
    out_color = color_image ;    
    //out_color = vec4(1.0);
}
