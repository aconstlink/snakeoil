//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "shader.h"

using namespace so_shade ;
using namespace so_shade::so_shader ;


//*************************************************************************************
shader::shader( void_t ) 
{

}

//*************************************************************************************
shader::shader( this_rref_t rhv ) : custom_bindings_t(std::move(rhv))
{

}

//*************************************************************************************
shader::shader( this_cref_t rhv ) : custom_bindings_policy( rhv )
{
}

//*************************************************************************************
shader::~shader( void_t ) 
{

}

//*************************************************************************************
shader::this_ref_t shader::operator = ( this_cref_t rhv ) 
{
    custom_bindings_policy::operator=( rhv ) ;
    return *this ;
}

//*************************************************************************************
bool_t shader::add_custom_binding( icustom_binding_ptr_t ptr ) 
{
    return so_shade::success( custom_bindings_t::add_binding( ptr ) ) ;
}

//*************************************************************************************
void_t shader::for_each_custom_binding( custom_bindings_t::for_each_funk_c_t funk ) const 
{
    custom_bindings_t::for_each( funk ) ;
}
