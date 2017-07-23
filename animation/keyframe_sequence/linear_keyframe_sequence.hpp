//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_ANIMATION_KEYFRAME_SEQUENCE_LINEAR_KEYFRAME_SEQUENCE_HPP_
#define _SNAKEOIL_ANIMATION_KEYFRAME_SEQUENCE_LINEAR_KEYFRAME_SEQUENCE_HPP_

#include "../typedefs.h"
#include "../result.h"

#include "../keyframe/keyframe.hpp"

#include "../evaluation_result.h"

#include <snakeoil/spline/linear/linear_spline.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_ani
{
    template< typename T >
    class linear_keyframe_sequence
    {
        so_this_typedefs( linear_keyframe_sequence<T> ) ;

    public:

        so_typedefs( keyframe<T>, keyframe ) ;
        so_typedefs( so_std::vector< keyframe_t >, keyframes ) ;

        so_typedefs( T, value ) ;

        so_typedefs( typename keyframe_t::time_stamp_t, time_stamp ) ;
        
        so_typedefs( so_spline::linear_spline<T>, value_spline ) ;
        so_typedefs( so_spline::linear_spline<float_t>, time_spline ) ;

        so_typedefs( so_std::vector< float_t >, scalings ) ;

        typedef std::function< float_t( float_t ) > time_funk_t ;
        so_typedefs( so_std::vector<time_funk_t>, time_funks ) ;
        
    private:

        keyframes_t _keyframes ;
        value_spline_t _value_spline ;
        
        time_funks_t _time_funks ;

    public:

        linear_keyframe_sequence( void_t )
        {
        }

        linear_keyframe_sequence( this_cref_t rhv )
        {
            _keyframes = rhv._keyframes ;
            _value_spline = rhv._value_spline ;
            _time_funks = rhv._time_funks ;
        }

        linear_keyframe_sequence( this_rref_t rhv ) 
        {
            _keyframes = std::move( rhv._keyframes ) ;
            _value_spline = std::move( rhv._value_spline ) ;
            _time_funks = std::move( rhv._time_funks ) ;
        }

        ~linear_keyframe_sequence( void_t ) 
        {}

    public: // operator = 

        this_ref_t operator = ( this_rref_t rhv ) 
        {
            _keyframes = std::move( rhv._keyframes ) ;
            _value_spline = std::move( rhv._value_spline ) ;
            _time_funks = std::move( rhv._time_funks ) ;

            return *this ;
        }

        this_ref_t operator = ( this_cref_t rhv ) 
        {
            _keyframes = rhv._keyframes ;
            _value_spline = rhv._value_spline ;
            _time_funks = rhv._time_funks ;

            return *this ;
        }

    public:

        bool_t insert( keyframe_cref_t kf )
        {
            if( kf.get_time() == time_stamp_t(-1) )
                return false ;

            // 1. clear splines. It is easier to insert
            // data after keyframe was inserted into right
            // position of keyframes array
            _value_spline.clear() ;
            _time_funks.clear() ;

            // 2. insert keyframe
            _keyframes.insert( std::lower_bound( _keyframes.begin(), _keyframes.end(), kf ), kf ) ;
            
            // 3. rebuild splines
            for( auto const & kf : _keyframes )
            {
                _value_spline.push_back( kf.get_value() ) ;
            }

            // 4. compute scalings
            {
                // num_segments
                size_t const ns = _keyframes.size()-1 ;

                for( size_t i = 1; i < _keyframes.size(); ++i )
                {
                    float_t const t1 = this_t::to_zero_one(_keyframes[i-0].get_time()) ;
                    float_t const t0 = this_t::to_zero_one(_keyframes[i-1].get_time()) ;

                    float_t const m = 1.0f / (float_t( ns ) * (t1-t0)) ;

                    float_t const t_off = ( float_t( i ) / float_t( ns ) ) - t1 * m ;

                    _time_funks.push_back( [=]( float_t const t )
                    {
                        return t * m + t_off ;
                    } ) ;
                }
            }

            return true ;
        }

    public:

        so_ani::evaluation_result operator () ( time_stamp_t const ts, value_out_t vo ) const
        {
            float_t t ;
            
            // before we can evaluate the anything, we need to check if
            // 1. the sequence has enough keyframes
            // 2. the time stamp is in time range of the sequence
            {
                auto const res = this_t::is_in_range( ts, t ) ;

                if( so_core::is_not( so_ani::is_value_usable( res ) ) )
                    return res ;

                if( so_ani::is_out_of_range( res ) )
                {
                    this_t::get_extreme_value( ts, vo ) ;
                    return res ;
                }
            }
            

            // @note the segment index si can not be calculated
            // by a simple multiplication like t*ns, because the
            // time stamps are NOT equally distributed!
            // So for now, we need to search it.
            {
                size_t si = 0 ;

                // do linear search
                while( _keyframes[ si++ ].get_time() < ts ) ;

                size_t const ns = _keyframes.size() - 1 ;

                // need to do -2 because si++ adds one after 
                // we found the keyframe index and since we need
                // the segment index but searched for keyframe index.
                si = std::min( si-2, ns - 1 ) ;
                t = _time_funks[ si ]( t ) ;
            }

            {
                auto const res = _value_spline( t, vo ) ;
            }

            return so_ani::evaluation_result::in_range ;
        }

    public:

        /// checks if ts is in range and if so, returns where
        /// ts is in that range in [0,1]. The direct return value
        /// is true if in range, false otherwise.
        so_ani::evaluation_result is_in_range( time_stamp_t const ts, float_out_t at ) const
        {
            if( _keyframes.size() < 2 ) 
                return so_ani::evaluation_result::invalid ;

            time_stamp_t const b = _keyframes.front().get_time() ;
            time_stamp_t const e = _keyframes.back().get_time() ;
            
            // if ts not E [b, e]
            {
                if( ts < b )
                {
                    at = 0.0f ;
                    return so_ani::evaluation_result::out_of_range ;
                }
                else if( ts > e )
                {
                    at = 1.0f ;
                    return so_ani::evaluation_result::out_of_range ;
                }
            }

            at = float_t( double_t( ts - b ) / double_t( e - b ) ) ;

            return so_ani::evaluation_result::in_range ;
        }

    private:

        //***************************************************************************
        float_t to_zero_one( time_stamp_t const ts ) const 
        {
            time_stamp_t const b = _keyframes.front().get_time() ;
            time_stamp_t const e = _keyframes.back().get_time() ;

            return float_t( double_t( ts - b ) / double_t( e - b ) ) ;
        }

        //***************************************************************************
        bool_t get_extreme_value( time_stamp_t const ts, value_out_t vout ) const
        {
            time_stamp_t const b = _keyframes.front().get_time() ;
            time_stamp_t const e = _keyframes.back().get_time() ;

            if( ts < b )
            {
                vout = _keyframes.front().get_value() ;
                return true ;
            }
            else if( ts > e )
            {
                vout = _keyframes.back().get_value() ;
                return true ;
            }
            return false ;
        }
    };
}

#endif
