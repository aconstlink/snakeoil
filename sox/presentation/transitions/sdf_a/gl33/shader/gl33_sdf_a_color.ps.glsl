
#version 330

layout( location = 0 ) out vec4 out_color ;

in vertex_data
{
    vec2 tx ;
} psi ;

void main()
{
    out_color = vec4(psi.tx, 0.0, 1.0);

    float d = dot( psi.tx*2.0-1.0, normalize(vec2(1.0,1.0))) ;
    out_color = d < 0 ? vec4(1.0,0.0,0.0,1.0) : vec4(0.0,0.0,0.0,0.0) ;
}
