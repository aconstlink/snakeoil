
#version 330

layout( location = 0 ) out vec4 out_color ;



in vertex_data
{
    vec2 tx ;
} psi ;

void main()
{
    
    
    out_color = vec4(1.0, 0.0, 0.0, 1.0);
}
