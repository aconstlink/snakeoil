//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "funk_node.h"
#include "leaf/arg_node.h"

#include <algorithm>

using namespace so_shade ;
using namespace so_shade::so_tree ;

//**********************************************************************************************
funk_node::funk_node( so_std::string_cref_t funk_name ) 
{
    _name = funk_name ;
}

//**********************************************************************************************
funk_node::funk_node( this_rref_t rhv ) : base_t( std::move(rhv) )
{
    _name = std::move(rhv._name) ;
}

//**********************************************************************************************
funk_node::~funk_node( void_t )
{
    for( auto item : _args )
    {
        if( so_core::is_not_nullptr(item.rep_ptr) )
            item.arg_ptr->destroy() ;
    }
}

//**********************************************************************************************
so_std::string_cref_t funk_node::get_name( void_t ) const 
{
    return _name ;
}

//**********************************************************************************************
so_shade::result funk_node::create_arg( so_std::string_cref_t name ) 
{
    auto * arg_node_ptr = so_shade::so_tree::arg_node_t::create( 
        "[so_shade::so_tree::funk_node::create_arg] : arg node: " + name ) ;

    auto const res = this_t::add_arg( name, arg_node_ptr ) ;

    if( so_shade::no_success(res) )
        so_shade::so_tree::arg_node_t::destroy( arg_node_ptr ) ;

    return res ;
}

//**********************************************************************************************
so_shade::result funk_node::add_arg( so_std::string_cref_t arg_name, 
    so_shade::so_tree::arg_node_ptr_t nptr )
{
    if( so_core::is_nullptr(nptr) )
        return so_shade::invalid_pointer ;

    auto iter = std::find_if( _args.begin(), _args.end(), [&]( arg_item_cref_t item )
    {
        return item.name == arg_name ;
    } ) ;

    if( iter != _args.end() )
    {
        return nptr == iter->arg_ptr ? so_shade::ok : so_shade::invalid_pointer ;
    }

    if( this_t::find( nptr ) )
        return so_shade::invalid_pointer ;

    arg_item_t argi ;
    argi.arg_ptr = nptr ;
    argi.rep_ptr = nullptr ;
    argi.name = arg_name ;
    
    _args.push_back( std::move( argi ) ) ;

    return this_t::add_node( nptr ) ;
}

//**********************************************************************************************
so_shade::result funk_node::exchange_by_name( so_std::string_cref_t arg_name, node_ptr_t nptr )
{
    if( so_core::is_nullptr(nptr) )
        return so_shade::invalid_pointer ;

    auto iter = std::find_if( _args.begin(), _args.end(), [&]( arg_item_cref_t item )
    {
        return item.name == arg_name ;
    } ) ;

    if( iter == _args.end() )
        return so_shade::invalid_argname ;

    so_shade::so_tree::node_ptr_t ex_ptr = so_core::is_not_nullptr(iter->rep_ptr) ? 
        iter->rep_ptr : iter->arg_ptr ;

    auto const res = this_t::exchange( ex_ptr, nptr ) ;
    if( so_shade::no_success( res ) )
        return res ;
    
    if( so_core::is_not_nullptr(iter->rep_ptr) )
    {
        ex_ptr->destroy() ;
    }

    iter->rep_ptr = nptr ;

    return so_shade::ok ;
}

//**********************************************************************************************
funk_node::string_list_t funk_node::get_arg_name_list( void_t ) const
{
    string_list_t sl ;
    for( auto item : _args )
    {
        sl.push_back( item.name ) ;
    }

    return std::move(sl) ;
}
