//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "components_component.h"

#include "../../component/icomponent.h"

#include <algorithm>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*****************************************************************************************
components_component::components_component( void_t )
{

}

//*****************************************************************************************
components_component::components_component( this_rref_t rhv )
{
    *this = std::move( rhv ) ;
}

//*****************************************************************************************
components_component::~components_component( void_t )
{
    for( auto * cptr : _components )
    {
        cptr->destroy() ;
    }
}

//*****************************************************************************************
components_component::this_ref_t components_component::operator = ( this_rref_t rhv )
{
    _components = std::move( rhv._components ) ;
    return *this ;
}

//*****************************************************************************************
bool_t components_component::add_component( so_ui::icomponent_ptr_t cptr )
{
    auto const iter = std::find_if( _components.begin(), _components.end(), [&] ( so_ui::icomponent_ptr_t iptr ) 
    { 
        return cptr->is_same_type( iptr ) ;
    } ) ;
    
    if( iter != _components.end() ) return false ;
    
    _components.push_back( cptr ) ;

    return true ;
}

//*****************************************************************************************
void_t components_component::foreach_component( components_component::foreach_funk_t fe )
{
    for( auto * cptr : _components )
    {
        if( fe( cptr ) ) break ;
    }
}

//*****************************************************************************************