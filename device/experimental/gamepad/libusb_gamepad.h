#pragma once

namespace so_device
{
    namespace so_experimental
    {
        // for later use
        #if 0
        class libusb_gamepad
        {
            
            void_t init( void_t ) 
            {
                libusb_device **devs;
                size_t const cnt = libusb_get_device_list( NULL, &devs );

                if( cnt <= 0 )
                {
                    so_log::global_t::status( "][ : no USB Devices detected" ) ;
                    return ;
                }

                for( size_t i = 0; i < cnt; ++i )
                {
                    libusb_device_descriptor desc;
                    int_t r = libusb_get_device_descriptor( devs[ i ], &desc );
                    if( r < 0 )
                    {
                        so_log::global_t::status( "][ : failed to get device descriptor" ) ;
                        continue ;
                    }

                    // interesting if nes controller
                    if( desc.idVendor != 0x0810 || desc.idProduct != 0xe501 )            
                    {
                        if( desc.idVendor != 0x0079 || desc.idProduct != 0x0011 )
                            continue ;
                    }

                    libusb_device_handle * handle ;
                    r = libusb_open( devs[ i ], &handle ) ;
                    if( r < 0 )
                    {
                        so_log::global_t::status( "][ : Interesting device but can not open handle" ) ;
                        continue ;
                    }

                    this_t::nes_data_t rd ;
                    rd.dev_handle = handle ;

                    libusb_config_descriptor *conf_desc;
                    {
                        int_t err = libusb_get_config_descriptor( devs[ i ], 0, &conf_desc ) ;
                        if( err < 0 )
                        {
                            so_log::global_t::error( "Can not get config descriptor" );
                            libusb_close( handle ) ;
                        }
                    }

                    libusb_interface const * inter = &conf_desc->interface[ 0 ] ;
                    libusb_interface_descriptor const * inter_desc = &inter->altsetting[ 0 ];
                    libusb_endpoint_descriptor const * ep_desc = &inter_desc->endpoint[ 0 ] ;

                    rd.ep_desc = ep_desc ;

                    {
                        int_t const err = libusb_set_auto_detach_kernel_driver( handle, 1 );
                        if( err < 0 )
                        {
                            so_log::global_t::error( "libusb_set_auto_detach_kernel_driver failed - not supported on win" );
                            //libusb_free_config_descriptor( conf_desc ) ;
                            //libusb_close( handle ) ;
                            //continue ;
                        }
                    }

                    {
                        int_t const err = libusb_claim_interface( handle, 0 );
                        if( err < 0 )
                        {
                            so_log::global_t::error( "libusb_claim_interface failed" );
                            libusb_free_config_descriptor( conf_desc ) ;
                            libusb_close( handle ) ;
                            continue ;
                        }
                    }

                    _nes_datas.push_back( rd ) ;
                    if( desc.idVendor == 0x0810 )
                        _nes_controllers.push_back( new nes_0810_e501_controller_t() ) ;
                    else if( desc.idVendor == 0x0079 )
                        _nes_controllers.push_back( new nes_0079_0011_controller_t() ) ;
                }

                libusb_free_device_list( devs, 1 ) ;

                std::thread( [=] ( void_t )
                {
                    while( _nes_update_run ) 
                    {
                        {
                            so_thread::lock_guard_t lk( this->_nes_input_mtx ) ;
                            this->_nes_input.clear() ;

                            for( auto const & rd : _nes_datas )
                            {
                                int_t read ;
                                uint8_t data[ 8 ] ;
                                auto const res = libusb_interrupt_transfer( rd.dev_handle, rd.ep_desc->bEndpointAddress, data, 8, &read, 100 ) ;
                                if( LIBUSB_ERROR_TIMEOUT == res )
                                {
                                    so_log::global_t::warning( "libusb_interrupt_transfer - timeout " ) ;
                                    return ;
                                }

                                std::array< byte_t, 8 > track ;

                                std::stringstream ss ;
                                for( size_t i = 0; i < 8; ++i )
                                {
                                    track[ i ] = data[ i ] ;
                                }

                                this->_nes_input.push_back( std::move( track ) ) ;
                            }
                        }

                        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) ) ;
                    }

                } ).detach() ;
            }

            //************************************************************************
            void_t update( void_t ) 
            {
                {
                    so_thread::lock_guard_t lk( this->_nes_input_mtx ) ;
                    _nes_input_local.clear() ;

                    for( auto & item : _nes_input )
                    {
                        _nes_input_local.push_back( std::move( item ) ) ;
                    }
                    _nes_input.clear() ;
                }


                {
                    size_t i = 0 ;
                    for( auto const & item : _nes_input_local )
                    {
                        _nes_controllers[ i++ ]->update( item ) ;
                    }
                }
            }
        };
        #endif
    }
}