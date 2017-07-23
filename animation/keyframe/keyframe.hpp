//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_ANIMATION_KEYFRAME_KEYFRAME_HPP_
#define _SNAKEOIL_ANIMATION_KEYFRAME_KEYFRAME_HPP_

namespace so_ani
{
    template< typename T >
    class keyframe
    {
        so_this_typedefs( keyframe<T> ) ;

    public:

        so_typedefs( T, type ) ;
        so_typedefs( T, value ) ;
        so_typedefs( size_t, time_stamp ) ;
        
    private:

        value_t _value ;
        time_stamp_t _time ;

    public:

        keyframe( void_t ) : _value(0), _time(time_stamp_t(-1))
        {}

        keyframe( time_stamp_t ts ) : _value( 0 ), _time(ts)
        {}

        keyframe( time_stamp_t ts, value_cref_t v ) : _value(v), _time(ts)
        {}

        keyframe( this_cref_t rhv ) : _value(rhv._value), _time(rhv._time)
        {}

        keyframe( this_rref_t rhv ) : _value( rhv._value ), _time( rhv._time )
        {}

        ~keyframe( void_t ) 
        {}

    public:

        bool_t operator < ( this_cref_t rhv ) const
        {
            return _time < rhv._time ;
        }

        bool_t operator > ( this_cref_t rhv ) const
        {
            return _time > rhv._time ;
        }

        bool_t operator <= ( this_cref_t rhv ) const
        {
            return _time <= rhv._time ;
        }

        bool_t operator >= ( this_cref_t rhv ) const
        {
            return _time >= rhv._time ;
        }

        bool_t operator == ( this_cref_t rhv ) const
        {
            return _time == rhv._time ;
        }

        this_ref_t operator = ( this_cref_t rhv )
        {
            _time = rhv._time ;
            _value = rhv._value ;
            return *this ;
        }

    public:

        time_stamp_t get_time( void_t ) const
        {
            return _time ;
        }

        value_cref_t get_value( void_t ) const
        {
            return _value ;
        }
    };
}

#endif
