
#version 330 core

layout( location = 0 ) out vec4 out_color ;

in vertex_data
{
    flat vec3 color ;
} psi ;

void main()
{    
    out_color = vec4(1.0,0.0,0.0,1.0) ;
}
