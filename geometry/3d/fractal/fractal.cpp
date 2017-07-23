//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "fractal.h"

using namespace so_geo ;
using namespace so_geo::so_3d ;

//*************************************************************************************
fractal::objects_t fractal::menger_sponge( fractal_object_cref_t parent ) 
{
    objects_t objs ;

    float_t const step = parent.side_width / 3.0f ;
    float_t const child_width = step ;

    so_math::vec3f_t const start_pos = 
        parent.pos - so_math::vec3f_t(step) ;
    
    //////////////////////////////////////////////////////
    // left 8 cubes
    {
        so_math::vec3f_t ls = start_pos ;

        //
        // front
        //
        {
            so_math::vec3f_t pos = ls ;
            
            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.y() += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.y() += 2.0f * child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        //
        // middle
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.z() += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        /*{
            so_math::vec3f_t pos = ls ;
            pos.y += child_width ;
            pos.z += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }*/

        {
            so_math::vec3f_t pos = ls ;
            pos.y() += 2.0f * child_width ;
            pos.z() += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        //
        // back
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.y() += child_width ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.y() += 2.0f * child_width ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }
    }

    //////////////////////////////////////////////////////
    // middle 4 cubes
    {
        so_math::vec3f_t ls = start_pos ;

        //
        // front
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.x() += child_width ;
            
            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        // no pos here
        {}

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += child_width ;
            pos.y() += 2.0f * child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        //
        // middle - middle has no positions
        //
        {}

        //
        // back
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.x() += child_width ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        // no pos here
        {}

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += child_width ;
            pos.y() += 2.0f * child_width ;
            pos.z() += 2.0f * child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }
    }

    //////////////////////////////////////////////////////
    // right 8 cubes
    {
        so_math::vec3f_t ls = start_pos ;
    
        //
        // front
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.y() += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.y() += 2.0f * child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        //
        // middle
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.z() += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        /*{
            so_math::vec3f_t pos = ls ;
            pos.y += child_width ;
            pos.z += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }*/

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.y() += 2.0f * child_width ;
            pos.z() += child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        //
        // back
        //
        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.y() += child_width ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }

        {
            so_math::vec3f_t pos = ls ;
            pos.x() += 2.0f * child_width ;
            pos.y() += 2.0f * child_width ;
            pos.z() += 2.0f*child_width ;

            fractal_object o ;
            o.pos = pos ;
            o.side_width = child_width ;

            objs.push_back( o ) ;
        }
    }

    return std::move( objs ) ;
}

//*************************************************************************************
fractal::objects_t fractal::sierpinski_tetra( fractal_object_cref_t parent ) 
{
    objects_t objs ;

    float_t const step = parent.side_width / 2.0f ;
    float_t const child_width = step ;

    so_math::vec3f_t start_pos = parent.pos ;


    //////////////////////////////////////////////////////
    // left tet
    {
        so_math::vec3f_t ls = start_pos ;
        ls.x() -= step ;
        ls.y() -= step ;
        ls.z() -= step ;

        fractal_object o ;
        o.pos = ls ;
        o.side_width = child_width ;

        objs.push_back( o ) ;
    }

    //////////////////////////////////////////////////////
    // right tet
    {
        so_math::vec3f_t ls = start_pos ;
        ls.x() += step ;
        ls.y() -= step ;
        ls.z() -= step ;

        fractal_object o ;
        o.pos = ls ;
        o.side_width = child_width ;

        objs.push_back( o ) ;
    }

    //////////////////////////////////////////////////////
    // back tet
    {
        so_math::vec3f_t ls = start_pos ;
        ls.y() -= step ;
        ls.z() += step ;

        fractal_object o ;
        o.pos = ls ;
        o.side_width = child_width ;

        objs.push_back( o ) ;
    }

    //////////////////////////////////////////////////////
    // top tet
    {
        so_math::vec3f_t ls = start_pos ;
        ls.y() += step ;

        fractal_object o ;
        o.pos = ls ;
        o.side_width = child_width ;

        objs.push_back( o ) ;
    }
    return std::move( objs ) ;
}

//*************************************************************************************
fractal::divider_funk_t fractal::menger_sponge_funk( void_t ) 
{
    return [=]( fractal_object_cref_t parent )
    {
        return std::move(this_t::menger_sponge( parent )) ;
    } ;
}

//*************************************************************************************
fractal::divider_funk_t fractal::sierpinski_tetra_funk( void_t ) 
{
    return [=]( fractal_object_cref_t parent )
    {
        return std::move(this_t::sierpinski_tetra( parent )) ;
    } ;
}

//*************************************************************************************
fractal::objects_t fractal::iterate( divider_funk_t funk, fractal_object_cref_t parent, size_t num_divs ) 
{
    this_t::objects_t objs = { parent } ;

    this_t::objects_t store(10000000) ;

    for( size_t i=0; i<num_divs; ++i )
    {
        size_t cur_pos = 0 ;

        for( auto const & obj : objs )
        {
            this_t::objects_t tmps = std::move( funk( obj ) ) ;
            
            if( store.size() <= cur_pos )
            {
                store.resize( store.size() + 1000000 ) ;
            }            
            
            for( size_t j=0; j<tmps.size(); ++j )
            {
                store[cur_pos+j] = tmps[j] ;
            }
            cur_pos += tmps.size() ;
        }
        //objs = std::move( store ) ;

        objs.resize( cur_pos ) ;
        memcpy( objs.data(), store.data(), sizeof(fractal_object_t)*cur_pos ) ;
        
    }

    return std::move( objs ) ;
}
