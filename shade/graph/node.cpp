//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "node.h"

#include "../slot/iinput_slot.h"
#include "../slot/ioutput_slot.h"

using namespace so_shade ;
using namespace so_shade::so_graph ;

//*************************************************************************************
node::node( void_t ) 
{
}

//*************************************************************************************
node::node( so_std::string_cref_t name ) : _name(name)
{}

//*************************************************************************************
node::node( this_rref_t rhv ) : input_slots( std::move(rhv) ), output_slots( std::move(rhv) )
{
    _name = std::move(rhv._name) ;
}

//*************************************************************************************
node::~node( void_t )
{
}

//*************************************************************************************
so_std::string_t node::get_name( void_t ) const 
{
    return _name ;
}
