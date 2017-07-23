//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "can_xl_system.h"

#include <snakeoil/log/log.h>

#include <thread>

using namespace so_net ;
using namespace so_net::so_can ;

//**********************************************************************************
can_xl_system::can_xl_system( void_t )
{
    
}

//**********************************************************************************
can_xl_system::can_xl_system( this_rref_t rhv )
{
    _released = rhv._released ;
    rhv._released = true ;
    _recvs = std::move( rhv._recvs ) ;
}

//**********************************************************************************
can_xl_system::~can_xl_system( void_t )
{
    if( so_core::is_not( _released ) )
    {
        this_t::release() ;
    }
}

//**********************************************************************************
so_net::result can_xl_system::init( void_t )
{
    if( so_core::is_not( _released ) )
        return so_net::ok ;

    {
        auto const res = xlOpenDriver() ;
        if( res != XL_SUCCESS )
        {
            so_log::log::error( "[can_xl_system::init] : xlOpenDriver" ) ;
            return so_net::failed ;
        }
    }

    {
        auto const res = xlGetDriverConfig( &_config ) ;
        if( res != XL_SUCCESS )
        {
            so_log::log::error( "[can_xl_system::init] : xlGetDriverConfig" ) ;
            return so_net::failed ;
        }
    }
        
    _released = false ;
    return so_net::ok ;
}

//**********************************************************************************
so_net::result can_xl_system::release( void_t )
{
    if( _released )
        return so_net::ok ;

    for( auto * dptr : _recvs )
    {
        xlClosePort( dptr->hnd_port ) ;
        so_net::memory::dealloc( dptr ) ;
    }

    xlCloseDriver() ;

    _released = true ;
    return so_net::ok ;
}

//**********************************************************************************
so_net::result can_xl_system::recv( recv_init_data_cref_t initd, so_net::so_can::can_recv_funk_t funk )
{
    this_t::recv_data_t rcd ;

    if( initd.channel >= _config.channelCount )
    {
        so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
        so_log::log::error( "[can_xl_system::recv] : invalid channel index" ) ;
        return so_net::failed ;
    }

    XLchannelConfig & usedc = _config.channel[ initd.channel ] ;
    
    XLuint64 channel_mask = 0 ;
    XLuint64 permission_mask = 0 ;

    bool_t fd_support = false ;

    /*{
        if( ( usedc.channelCapabilities & XL_CHANNEL_FLAG_CANFD_ISO_SUPPORT ) > 0 ){
            fd_support = true ;
        }
    }*/

    // check status, get config
    {
        bool_t const bus_active = (usedc.channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN) > 0 ;
        if( so_core::is_not( bus_active ) )
        {
            so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
            so_log::log::error( "[can_xl_system::recv] : can not access bus. Is not active" ) ;
            return so_net::failed ;
        }

        usedc.channelMask ;
        usedc.channelIndex ;

        // permission 
        channel_mask |= usedc.channelMask;
        permission_mask = usedc.channelMask ;
    }
    
    XLportHandle port = XL_INVALID_PORTHANDLE ;

    // create port
    {
        XLstatus res ;

        if( fd_support )
        {
            res = xlOpenPort( &port, "snakeoil VectorXL", channel_mask, &permission_mask,
                16000, XL_INTERFACE_VERSION_V4, XL_BUS_TYPE_CAN ) ;
        }
        else
        {
            res = xlOpenPort( &port, "snakeoil VectorXL", channel_mask, &permission_mask,
                RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN );
        }

        if( ( res != XL_SUCCESS ) || ( port == XL_INVALID_PORTHANDLE ) )
        {
            xlClosePort( port );
            so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
            so_log::log::error( "[can_xl_system::recv] : xlOpenPort" ) ;
            return so_net::failed ;
        }
    }
    

    // set bitrate
    if( channel_mask == permission_mask )
    {
        if( fd_support )
        {
            XLcanFdConf fdParams;

            memset( &fdParams, 0, sizeof( fdParams ) );

            // arbitration bitrate
            fdParams.arbitrationBitRate = 1000000;
            fdParams.tseg1Abr = 6;
            fdParams.tseg2Abr = 3;
            fdParams.sjwAbr = 2;

            // data bitrate
            fdParams.dataBitRate = fdParams.arbitrationBitRate * 2;
            fdParams.tseg1Dbr = 6;
            fdParams.tseg2Dbr = 3;
            fdParams.sjwDbr = 2;

            auto const res = xlCanFdSetConfiguration( port, channel_mask, &fdParams );
            if( res != XL_SUCCESS )
            {
                xlClosePort( port );
                so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
                so_log::log::error( "[can_xl_system::recv] : xlCanSetChannelBitrate" ) ;
                return so_net::failed ;
            }
        }
        else
        {
            unsigned long const baudrate = 500000 ;
            auto const res = xlCanSetChannelBitrate( port, channel_mask, baudrate );
            if( res != XL_SUCCESS )
            {
                xlClosePort( port );
                so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
                so_log::log::error( "[can_xl_system::recv] : xlCanSetChannelBitrate" ) ;
                return so_net::failed ;
            }
        }
    }


    // activate channel
    {
    auto const res = xlActivateChannel( port, channel_mask,
        XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK ) ;

    if( res != XL_SUCCESS )
    {
        xlClosePort( port );
        so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
        so_log::log::error( "[can_xl_system::recv] : xlActivateChannel" ) ;
        return so_net::failed ;
    }
    }


    rcd.hnd_port = port ;
    rcd.fd_support = fd_support ;

    this_t::recv_data_ptr_t data_ptr = so_net::memory::alloc( std::move( rcd ),
        "[can_xl_system::recv] : recv_data" ) ;

    _recvs.push_back( data_ptr ) ;

    std::thread( [=]( void_t )
    {

        size_t sleepfor = 100 ;

        while( true )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( sleepfor ) ) ;

            so_net::so_can::can_data_t data ;

            if( fd_support )
            {
                XLcanRxEvent evt ;
                auto const res = xlCanReceive( data_ptr->hnd_port, &evt );
                if( res == XL_ERR_QUEUE_IS_EMPTY ) continue ;
                if( res != XL_SUCCESS ) break ;

                /*for( size_t i = 0; i < 8; ++i )
                {
                    data[ i ] = evt.tagData. .msg.data[ i ] ;
                }*/
            }
            else
            {
                XLevent xlEvent ;
                unsigned int msgsrx = RECEIVE_EVENT_SIZE;

                auto const res = xlReceive( data_ptr->hnd_port, &msgsrx, &xlEvent );
                if( res == XL_ERR_QUEUE_IS_EMPTY ) continue ;
                if( res != XL_SUCCESS ) break ;

                for( size_t i = 0; i < 8; ++i )
                {
                    data[ i ] = xlEvent.tagData.msg.data[ i ] ;
                }
            }

            auto const res = funk( data ) ;
            if( res == size_t( -1 ) ) break ;

            sleepfor = res ;
        }

        if( so_core::is_not_nullptr( initd.finish_sync_ptr ) )
            initd.finish_sync_ptr->set_and_signal() ;

    } ).detach() ;


    return so_net::ok ;
}

//**********************************************************************************
so_net::result can_xl_system::send( send_init_data_cref_t initd, so_net::so_can::can_send_funk_t funk )
{
    this_t::send_data_t scd ;

    if( initd.channel >= _config.channelCount )
    {
        so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
        so_log::log::error( "[can_xl_system::send] : invalid channel index" ) ;
        return so_net::failed ;
    }

    XLchannelConfig & usedc = _config.channel[ initd.channel ] ;

    XLuint64 channel_mask = 0 ;
    XLuint64 permission_mask = 0 ;

    bool_t fd_support = false ;

    /*{
    if( ( usedc.channelCapabilities & XL_CHANNEL_FLAG_CANFD_ISO_SUPPORT ) > 0 ){
    fd_support = true ;
    }
    }*/

    // check status, get config
    {
        bool_t const bus_active = ( usedc.channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN ) > 0 ;
        if( so_core::is_not( bus_active ) )
        {
            so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
            so_log::log::error( "[can_xl_system::send] : can not access bus. Is not active" ) ;
            return so_net::failed ;
        }

        usedc.channelMask ;
        usedc.channelIndex ;

        // permission 
        channel_mask |= usedc.channelMask;
        permission_mask = usedc.channelMask ;
    }

    XLportHandle port = XL_INVALID_PORTHANDLE ;

    // create port
    {
        XLstatus res ;

        if( fd_support )
        {
            res = xlOpenPort( &port, "snakeoil VectorXL", channel_mask, &permission_mask,
                16000, XL_INTERFACE_VERSION_V4, XL_BUS_TYPE_CAN ) ;
        }
        else
        {
            res = xlOpenPort( &port, "snakeoil VectorXL", channel_mask, &permission_mask,
                RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN );
        }

        if( ( res != XL_SUCCESS ) || ( port == XL_INVALID_PORTHANDLE ) )
        {
            xlClosePort( port );
            so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
            so_log::log::error( "[can_xl_system::send] : xlOpenPort" ) ;
            return so_net::failed ;
        }
    }


    // set bitrate
    if( channel_mask == permission_mask )
    {
        if( fd_support )
        {
            XLcanFdConf fdParams;

            memset( &fdParams, 0, sizeof( fdParams ) );

            // arbitration bitrate
            fdParams.arbitrationBitRate = 1000000;
            fdParams.tseg1Abr = 6;
            fdParams.tseg2Abr = 3;
            fdParams.sjwAbr = 2;

            // data bitrate
            fdParams.dataBitRate = fdParams.arbitrationBitRate * 2;
            fdParams.tseg1Dbr = 6;
            fdParams.tseg2Dbr = 3;
            fdParams.sjwDbr = 2;

            auto const res = xlCanFdSetConfiguration( port, channel_mask, &fdParams );
            if( res != XL_SUCCESS )
            {
                xlClosePort( port );
                so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
                so_log::log::error( "[can_xl_system::send] : xlCanSetChannelBitrate" ) ;
                return so_net::failed ;
            }
        }
        else
        {
            unsigned long const baudrate = 500000 ;
            auto const res = xlCanSetChannelBitrate( port, channel_mask, baudrate );
            if( res != XL_SUCCESS )
            {
                xlClosePort( port );
                so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
                so_log::log::error( "[can_xl_system::send] : xlCanSetChannelBitrate" ) ;
                return so_net::failed ;
            }
        }
    }


    // activate channel
    {
        auto const res = xlActivateChannel( port, channel_mask,
            XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK ) ;

        if( res != XL_SUCCESS )
        {
            xlClosePort( port );
            so_thread::sync_object_t::set_and_signal( initd.finish_sync_ptr ) ;
            so_log::log::error( "[can_xl_system::send] : xlActivateChannel" ) ;
            return so_net::failed ;
        }
    }


    scd.hnd_port = port ;
    scd.fd_support = fd_support ;

    this_t::send_data_ptr_t data_ptr = so_net::memory::alloc( std::move( scd ),
        "[can_xl_system::send] : send_data" ) ;

    _sends.push_back( data_ptr ) ;

    std::thread( [=]( void_t )
    {

        size_t sleepfor = 100 ;

        while( true )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( sleepfor ) ) ;

            so_net::so_can::can_data_t data ;

            if( fd_support )
            {
                // no right now
            }
            else
            {
                uint_t mcount = 1 ;

                XLevent xlEvent ;

                memset( &xlEvent, 0, sizeof( xlEvent ) );

                xlEvent.tag = XL_TRANSMIT_MSG;
                xlEvent.tagData.msg.id = 1;
                xlEvent.tagData.msg.dlc = 8;
                xlEvent.tagData.msg.flags = 0;
                xlEvent.tagData.msg.data[ 0 ];
                xlEvent.tagData.msg.data[ 1 ] = 2;
                xlEvent.tagData.msg.data[ 2 ] = 3;
                xlEvent.tagData.msg.data[ 3 ] = 4;
                xlEvent.tagData.msg.data[ 4 ] = 5;
                xlEvent.tagData.msg.data[ 5 ] = 6;
                xlEvent.tagData.msg.data[ 6 ] = 7;
                xlEvent.tagData.msg.data[ 7 ] = 8;

                auto const res = xlCanTransmit( port, channel_mask, &mcount, &xlEvent );
                if( res != XL_SUCCESS )
                    break ;
            }

            auto const res = funk( data ) ;
            if( res == size_t( -1 ) ) break ;

            sleepfor = res ;
        }

        if( so_core::is_not_nullptr( initd.finish_sync_ptr ) )
            initd.finish_sync_ptr->set_and_signal() ;

    } ).detach() ;


    return so_net::ok ;
}