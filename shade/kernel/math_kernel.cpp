//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "math_kernel.h"

using namespace so_shade ;
using namespace so_shade::so_kernel ;

//**********************************************************************************************
cos::cos( void_t ) : kernel( "cos" ) {}

//**********************************************************************************************
cos::cos( this_rref_t rhv ) : kernel( std::move( rhv ) ) {}

//**********************************************************************************************
cos::~cos( void_t ) {}

//**********************************************************************************************
cos::this_ptr_t cos::create( so_memory::purpose_cref_t p ) 
{
    return so_shade::so_kernel::create_kernel<this_t>( this_t(), p ) ;
}

//**********************************************************************************************
void_t cos::destroy( void_t )
{
    so_shade::so_kernel::destroy_kernel( this ) ;
}

//**********************************************************************************************
size_t cos::get_num_inputs( void_t ) const
{
    return 1;
}

//**********************************************************************************************
size_t cos::get_num_outputs( void_t ) const
{
    return 1;
}

//**********************************************************************************************
bool_t cos::validate_input_types( types_cref_t inputs ) const
{
    for( auto t : inputs )
    {
        so_shade::type_base const tb = t.get_type_base();

        switch( tb ) 
        {
        case so_shade::type_base::tfloat:
        case so_shade::type_base::tdouble:
            continue ;
        default:
            return false ;
        }        
    }

    for( auto t : inputs )
    {
        so_shade::type_struct const ts = t.get_type_struct();

        switch( ts ) 
        {
        case so_shade::type_struct::vector1:
        case so_shade::type_struct::vector2:
        case so_shade::type_struct::vector3:
        case so_shade::type_struct::vector4:
            continue ;
        default:
            return false ;
        }
    }

    return true;
}

//**********************************************************************************************
types_t cos::determine_output_types( types_cref_t types_in ) const
{
    if( types_in.size() != this_t::get_num_inputs() )
        return types_t();

    types_t ret_types( { so_shade::type_t() } );

    // determine type struct
    {
        so_shade::type_struct arg0 = types_in[0].get_type_struct();

        switch( arg0 )
        {
        case so_shade::type_struct::vector1:
            ret_types[0] = so_shade::type_struct::vector1;
            break;

        case so_shade::type_struct::vector2:
            ret_types[0] = so_shade::type_struct::vector2;
            break;

        case so_shade::type_struct::vector3:
            ret_types[0] = so_shade::type_struct::vector3;
            break;

        case so_shade::type_struct::vector4:
            ret_types[0] = so_shade::type_struct::vector4;
            break;

        default: break;
        }
    }

    // determine type_base
    {
        type_base arg0 = types_in[0].get_type_base();

        switch( arg0 )
        {
        case so_shade::type_base::tfloat:
            ret_types[0] = so_shade::type_base::tfloat;
            break;
        case so_shade::type_base::tdouble:
            ret_types[0] = so_shade::type_base::tdouble;
            break;
        default:
            break;
        }
    }

    return std::move( ret_types );
}

//**********************************************************************************************
multiply::multiply( void_t ) : kernel( "mult" ) 
{}

//**********************************************************************************************
multiply::multiply( this_rref_t rhv ) : kernel( std::move( rhv ) ) 
{}

//**********************************************************************************************
multiply::~multiply( void_t ) 
{}

//**********************************************************************************************
multiply::this_ptr_t multiply::create( so_memory::purpose_cref_t p ) 
{
    return so_shade::so_kernel::create_kernel<this_t>( this_t(), p ) ;
}

//**********************************************************************************************
void_t multiply::destroy( void_t )
{
    so_shade::so_kernel::destroy_kernel( this ) ;
}

//**********************************************************************************************
size_t multiply::get_num_inputs( void_t ) const
{
    return 2;
}

//**********************************************************************************************
size_t multiply::get_num_outputs( void_t ) const
{
    return 1;
}

//**********************************************************************************************
bool_t multiply::validate_input_types( types_cref_t inputs ) const
{
    for( auto t : inputs )
    {
        so_shade::type_base const tb = t.get_type_base();

        switch( tb )
        {
        case so_shade::type_base::tint:
        case so_shade::type_base::tuint:
        case so_shade::type_base::tfloat:
        case so_shade::type_base::tdouble:
            continue;
        default:
            return false;
        }
    }

    for( auto t : inputs )
    {
        so_shade::type_struct const ts = t.get_type_struct();

        switch( ts )
        {
        case so_shade::type_struct::vector1:
        case so_shade::type_struct::vector2:
        case so_shade::type_struct::vector3:
        case so_shade::type_struct::vector4:
        case so_shade::type_struct::matrix2:
        case so_shade::type_struct::matrix3:
        case so_shade::type_struct::matrix4:
            continue;
        default:
            return false;
        }
    }

    return true;
}

//**********************************************************************************************
types_t multiply::determine_output_types( types_cref_t types_in ) const
{
    if( types_in.size() != 2 )
        return types_t();

    types_t ret_types( { so_shade::type_t() } );

    // determine type struct
    {
        so_shade::type_struct arg0 = types_in[0].get_type_struct();
        so_shade::type_struct arg1 = types_in[1].get_type_struct();

        switch( arg0 )
        {
        case so_shade::type_struct::vector1:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
                ret_types[0] = so_shade::type_struct::vector1;
                break;
            case so_shade::type_struct::vector2:
                ret_types[0] = so_shade::type_struct::vector2;
                break;
            case so_shade::type_struct::vector3:
                ret_types[0] = so_shade::type_struct::vector3;
                break;
            case so_shade::type_struct::vector4:
                ret_types[0] = so_shade::type_struct::vector4;
                break;
            case so_shade::type_struct::matrix2:
                ret_types[0] = so_shade::type_struct::matrix2;
                break;
            case so_shade::type_struct::matrix3:
                ret_types[0] = so_shade::type_struct::matrix3;
                break;
            case so_shade::type_struct::matrix4:
                ret_types[0] = so_shade::type_struct::matrix4;
                break;
            }
            break;

        case so_shade::type_struct::vector2:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
            case so_shade::type_struct::vector2:
                ret_types[0] = so_shade::type_struct::vector2;
                break;
            default: break;
            }
            break;
        case so_shade::type_struct::vector3:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
            case so_shade::type_struct::vector3:
                ret_types[0] = so_shade::type_struct::vector3;
                break;
            default: break;
            }
            break;
        case so_shade::type_struct::vector4:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
            case so_shade::type_struct::vector4:
                ret_types[0] = so_shade::type_struct::vector4;
                break;
            default: break;
            }
            break;
        case so_shade::type_struct::matrix2:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
            case so_shade::type_struct::matrix2:
                ret_types[0] = so_shade::type_struct::matrix2;
                break;
            default: break;
            }
            break;
        case so_shade::type_struct::matrix3:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
            case so_shade::type_struct::matrix3:
                ret_types[0] = so_shade::type_struct::matrix3;
                break;
            default: break;
            }
            break;
        case so_shade::type_struct::matrix4:
            switch( arg1 )
            {
            case so_shade::type_struct::vector1:
            case so_shade::type_struct::matrix4:
                ret_types[0] = so_shade::type_struct::matrix4;
                break;
            default: break;
            }
            break;

        default: break;
        }
    }

    // determine type_base
    {
        type_base arg0 = types_in[0].get_type_base();
        type_base arg1 = types_in[1].get_type_base();

        ret_types[0] = this_t::same_type_base_only( arg0, arg1 );
    }

    return std::move( ret_types );
}

//**********************************************************************************************
so_shade::type_base multiply::same_type_base_only( so_shade::type_base arg0, so_shade::type_base arg1 )
{
    return arg0 == arg1 ? arg0 : so_shade::type_base::invalid;
}

//**********************************************************************************************
dot::dot( void_t ) : kernel( "dot" ) 
{}

//**********************************************************************************************
dot::dot( this_rref_t rhv ) : kernel( std::move( rhv ) ) 
{}

//**********************************************************************************************
dot::~dot( void_t ) 
{}

//**********************************************************************************************
dot::this_ptr_t dot::create( so_memory::purpose_cref_t p )
{
    return so_shade::so_kernel::create_kernel<this_t>( this_t(), p );
}

//**********************************************************************************************
void_t dot::destroy( void_t )
{
    so_shade::so_kernel::destroy_kernel( this ) ;
}

//**********************************************************************************************
size_t dot::get_num_inputs( void_t ) const
{
    return 2;
}

//**********************************************************************************************
size_t dot::get_num_outputs( void_t ) const
{
    return 1;
}

//**********************************************************************************************
bool_t dot::validate_input_types( types_cref_t types_in ) const
{
    for( auto t : types_in )
    {
        so_shade::type_base const tb = t.get_type_base();
        so_shade::type_struct const ts = t.get_type_struct();

        if( tb != so_shade::type_base::tbyte ||
            tb != so_shade::type_base::tdouble ||
            tb != so_shade::type_base::tfloat ||
            tb != so_shade::type_base::tint ||
            tb != so_shade::type_base::tuint )
            return false;

        if( ts != so_shade::type_struct::vector2 ||
            ts != so_shade::type_struct::vector3 ||
            ts != so_shade::type_struct::vector4 )
            return false;
    }

    return true;
}

//**********************************************************************************************
types_t dot::determine_output_types( types_cref_t types_in ) const
{
    if( types_in.size() != 2 )
        return types_t();

    types_t ret_types( { so_shade::type_t() } );

    // determine type struct
    {
        so_shade::type_struct arg0 = types_in[0].get_type_struct();
        so_shade::type_struct arg1 = types_in[1].get_type_struct();

        switch( arg0 )
        {
        case so_shade::type_struct::vector2:
            if( arg1 == so_shade::type_struct::vector2 )
                ret_types[0] = so_shade::type_struct::vector2;
            break;

        case so_shade::type_struct::vector3:
            if( arg1 == so_shade::type_struct::vector3 )
                ret_types[0] = so_shade::type_struct::vector3;
            break;

        case so_shade::type_struct::vector4:
            if( arg1 == so_shade::type_struct::vector4 )
                ret_types[0] = so_shade::type_struct::vector4;
            break;

        default: break;
        }
    }

    // determine type_base
    {
        type_base arg0 = types_in[0].get_type_base();
        type_base arg1 = types_in[1].get_type_base();

        ret_types[0] = this_t::same_type_base_only( arg0, arg1 );
    }

    return std::move( ret_types );
}

//**********************************************************************************************
so_shade::type_base dot::same_type_base_only( so_shade::type_base arg0, so_shade::type_base arg1 )
{
    return arg0 == arg1 ? arg0 : so_shade::type_base::invalid;
}
