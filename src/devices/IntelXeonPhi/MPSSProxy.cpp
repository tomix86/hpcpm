#include "MPSSProxy.hpp"

namespace devices {

MPSSProxy::MPSSProxy( void ) :
		RLLProxy{ "libmicmgmt.so" } {
}

bool MPSSProxy::loadSymbols( void ) {
	SYM_LOAD( mic_get_power_limit );
	SYM_LOAD( mic_get_power_limit );
	SYM_LOAD( mic_get_device_name );
	SYM_LOAD( mic_get_power_phys_limit );
	SYM_LOAD( mic_get_power_hmrk );
	SYM_LOAD( mic_get_power_lmrk );
	SYM_LOAD( mic_get_time_window0 );
	SYM_LOAD( mic_get_time_window1 );
	SYM_LOAD( mic_free_power_limit );
	SYM_LOAD( mic_set_power_limit0 );
	SYM_LOAD( mic_set_power_limit1 );
	SYM_LOAD( mic_get_uuid );
	SYM_LOAD( mic_get_silicon_sku );
	SYM_LOAD( mic_get_serial_number );
	SYM_LOAD( mic_get_cores_info );
	SYM_LOAD( mic_get_cores_count );
	SYM_LOAD( mic_free_cores_info );
	SYM_LOAD( mic_get_version_info );
	SYM_LOAD( mic_get_uos_version );
	SYM_LOAD( mic_get_flash_version );
	SYM_LOAD( mic_free_version_info );
	SYM_LOAD( mic_get_processor_info );
	SYM_LOAD( mic_get_processor_model );
	SYM_LOAD( mic_get_processor_family );
	SYM_LOAD( mic_get_processor_type );
	SYM_LOAD( mic_get_processor_steppingid );
	SYM_LOAD( mic_get_processor_stepping );
	SYM_LOAD( mic_free_processor_info );
	SYM_LOAD( mic_get_pci_config );
	SYM_LOAD( mic_free_pci_config );
	SYM_LOAD( mic_get_pci_domain_id );
	SYM_LOAD( mic_get_bus_number );
	SYM_LOAD( mic_get_device_number );
	SYM_LOAD( mic_get_vendor_id );
	SYM_LOAD( mic_get_device_id );
	SYM_LOAD( mic_get_revision_id );
	SYM_LOAD( mic_get_subsystem_id );
	SYM_LOAD( mic_get_link_speed );
	SYM_LOAD( mic_get_link_width );
	SYM_LOAD( mic_get_max_payload );
	SYM_LOAD( mic_get_max_readreq );
	SYM_LOAD( mic_get_memory_info );
	SYM_LOAD( mic_get_memory_vendor );
	SYM_LOAD( mic_get_memory_revision );
	SYM_LOAD( mic_get_memory_density );
	SYM_LOAD( mic_get_memory_size );
	SYM_LOAD( mic_get_memory_speed );
	SYM_LOAD( mic_get_memory_type );
	SYM_LOAD( mic_get_memory_frequency );
	SYM_LOAD( mic_get_memory_voltage );
	SYM_LOAD( mic_get_ecc_mode );
	SYM_LOAD( mic_free_memory_info );
	SYM_LOAD( mic_get_devices );
	SYM_LOAD( mic_free_devices );
	SYM_LOAD( mic_get_ndevices );
	SYM_LOAD( mic_get_device_at_index );
	SYM_LOAD( mic_open_device );
	SYM_LOAD( mic_close_device );
	SYM_LOAD( mic_get_error_string );
	return true;
}

} // namespace devices
