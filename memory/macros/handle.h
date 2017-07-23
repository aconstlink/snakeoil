//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MEMORY_MACROS_HANDLE_H_
#define _SNAKEOIL_MEMORY_MACROS_HANDLE_H_

#define so_mem_handle_typedef( class_name ) \
    typedef so_memory::handle< class_name > class_name ## _handle_t ; \
    typedef class_name ## _handle_t const & class_name ## _handle_cref_t ; \
    typedef class_name ## _handle_t && class_name ## _handle_rref_t 

#endif