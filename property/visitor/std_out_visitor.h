//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_PROPERTY_VISITOR_STD_OUT_VISITOR_H_
#define _SNAKEOIL_PROPERTY_VISITOR_STD_OUT_VISITOR_H_

#include "../ivisitor.h"

#include <snakeoil/std/string/utf8.hpp>

namespace so_property
{
    class SNAKEOIL_PROPERTY_API std_out_visitor : public so_property::visitor
    {
        so_this_typedefs( std_out_visitor )  ;

    private:

        size_t _level = 0 ;

    public: 

        std_out_visitor( void_t ) ;
        std_out_visitor( this_cref_t ) = delete ;
        std_out_visitor( this_rref_t ) ;
        virtual ~std_out_visitor( void_t ) ;

    public:


        virtual void_t visit( group_property_ptr_t ) ;
        virtual void_t post_visit( group_property_ptr_t ) ;

        virtual void_t visit( decorator_property_ptr_t ) ;
        virtual void_t post_visit( decorator_property_ptr_t ) ;

        virtual void_t visit( single_choice_property_ptr_t ) ;
        virtual void_t post_visit( single_choice_property_ptr_t ) ;

        virtual void_t visit( multi_choice_property_ptr_t ) ;
        virtual void_t post_visit( multi_choice_property_ptr_t ) ;

        virtual void_t visit( so_property::iproperty_ptr_t ) ;

    private:

        void_t print_property_name( so_property::iproperty_ptr_t, so_std::utf8_cref_t ) const ;
        so_std::utf8_t level_to_string( void_t ) const ;

    };
}

#endif