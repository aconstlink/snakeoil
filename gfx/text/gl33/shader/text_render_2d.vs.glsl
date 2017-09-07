#version 330 core

in vec3 in_pos ;

// layout: 
// vec4( start_x, start_y, width, height )
// vec4( layer, bearing_y, reserved, reserved ) 
uniform samplerBuffer u_glyph_info ;

// layout: vec4( glyph_index, pos_x, pos_y, reserved )
// layout: vec4( red, gree, blue, reserved )
uniform samplerBuffer u_text_info ;

out vertex_data
{
    vec2 txc ;
    flat int layer ;
    flat vec3 color ;
} vso ;

void main( )
{
    // 
    // Calculate glyph index in text
    int glyph_index = gl_VertexID / 4 ;
    
    //
    // Calculate the texture coordinates
    //
    vec2 txc = sign(in_pos.xy) * 0.5 + 0.5 ;
    
    vec4 text_data_00 = texelFetch( u_text_info, (glyph_index<<1)+0 ) ;
    vec4 text_data_01 = texelFetch( u_text_info, (glyph_index<<1)+1 ) ;
    
    vec4 glyph_data_00 = texelFetch( u_glyph_info, int(text_data_00.x*2.0)+0 ) ;
    vec4 glyph_data_01 = texelFetch( u_glyph_info, int(text_data_00.x*2.0)+1 ) ;
    
    vec2 smp_start = glyph_data_00.xy ;
    vec2 smp_dims = glyph_data_00.zw ;
    float bearing = glyph_data_01.y ;
    
    vso.txc = txc * smp_dims + smp_start ;
    vso.layer = int( glyph_data_01.x ) ;
    vso.color = text_data_01.xyz ;
    
    //
    // Calcualte the glyph dimensions for the quad
    //
    vec3 pos = sign(in_pos) * 0.5 + 0.5 ; 
    pos = pos * vec3( smp_dims.xy, 1.0 ) + vec3( text_data_00.yz+vec2(0.0, bearing), 0.0 ) ;
    gl_Position = vec4( pos, 1.0 ) ;
}
