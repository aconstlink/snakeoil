//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_CORE_MOVE_H_
#define _SNAKEOIL_CORE_MOVE_H_

/// moves a pointer - reduces code by one line in rval functions
#define so_move_ptr( to, from ) \
    to = from ; \
    from = nullptr

/// moves a pointer - reduces code by one line in rval functions
#define so_move_member_ptr( var_name, from ) \
    var_name = from.var_name ; \
    from.var_name = nullptr 

#define so_move( var_name, from ) \
    var_name = std::move( from.var_name ) 

#endif


