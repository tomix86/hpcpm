#pragma once
#include <miclib.h>
#include "utility/RLLProxy.hpp"

namespace devices {

class MPSSProxy : public utility::RLLProxy {
public:
	MPSSProxy( void );

	int ( *mic_get_power_limit )( mic_device *mdh, mic_power_limit **limit  );
	const char* ( *mic_get_device_name )( mic_device *device );
	int ( *mic_get_power_phys_limit )( mic_power_limit *limit, uint32_t *phys_lim );
	int ( *mic_get_power_hmrk )( mic_power_limit *limit, uint32_t *hmrk );
	int ( *mic_get_power_lmrk )( mic_power_limit *limit, uint32_t *lmrk );
	int ( *mic_get_time_window0 )( mic_power_limit *limit, uint32_t *time_window );
	int ( *mic_get_time_window1 )( mic_power_limit *limit, uint32_t *time_window );
	int ( *mic_free_power_limit )( mic_power_limit *limit );
	int ( *mic_set_power_limit0 )( mic_device *mdh, uint32_t power, uint32_t time_window );
	int ( *mic_set_power_limit1 )( mic_device *mdh, uint32_t power, uint32_t time_window );
	int ( *mic_get_uuid )( mic_device *mdh, uint8_t *uuid, size_t *size );
	int ( *mic_get_silicon_sku )( mic_device *mdh, char *sku, size_t *size );
	int ( *mic_get_serial_number )( mic_device *mdh, char *serial, size_t *size );
	int ( *mic_get_cores_info )( mic_device *mdh,  mic_cores_info **cores );
	int ( *mic_get_cores_count )( mic_cores_info *core, uint32_t *num_cores );
	int ( *mic_free_cores_info )( mic_cores_info *cores );
	int ( *mic_get_version_info )( mic_device *mdh,  mic_version_info **version );
	int ( *mic_get_uos_version )( mic_version_info *version, char *uos, size_t *size );
	int ( *mic_get_flash_version )( mic_version_info *version, char *flash, size_t *size );
	int ( *mic_free_version_info )( mic_version_info *version );
	int ( *mic_get_processor_info )( mic_device *mdh,  mic_processor_info **processor );
	int ( *mic_get_processor_model )( mic_processor_info *processor, uint16_t *model, uint16_t *model_ext );
	int ( *mic_get_processor_family )( mic_processor_info *processor, uint16_t *family, uint16_t *family_ext );
	int ( *mic_get_processor_type )( mic_processor_info *processor, uint16_t *type );
	int ( *mic_get_processor_steppingid )( mic_processor_info *processor, uint32_t *id );
	int ( *mic_get_processor_stepping )( mic_processor_info *processor, char *stepping, size_t *size );
	int ( *mic_free_processor_info )( mic_processor_info *processor );
	int ( *mic_get_pci_config )( mic_device *mdh,  mic_pci_config **conf );
	int ( *mic_free_pci_config )( mic_pci_config *conf );
	int ( *mic_get_pci_domain_id )( mic_pci_config *conf, uint16_t *domain );
	int ( *mic_get_bus_number )( mic_pci_config *conf, uint16_t *bus_no );
	int ( *mic_get_device_number )( mic_pci_config *conf, uint16_t *dev_no );
	int ( *mic_get_vendor_id )( mic_pci_config *conf, uint16_t *id );
	int ( *mic_get_device_id )( mic_pci_config *conf, uint16_t *id );
	int ( *mic_get_revision_id )( mic_pci_config *conf, uint8_t *id );
	int ( *mic_get_subsystem_id )( mic_pci_config *conf, uint16_t *id );
	int ( *mic_get_link_speed )( mic_pci_config *conf, char *speed, size_t *size );
	int ( *mic_get_link_width )( mic_pci_config *conf, uint32_t *width );
	int ( *mic_get_max_payload )( mic_pci_config *conf, uint32_t *payload );
	int ( *mic_get_max_readreq )( mic_pci_config *conf, uint32_t *readreq );
	int ( *mic_get_memory_info )( mic_device *mdh,  mic_device_mem **meminfo );
	int ( *mic_get_memory_vendor )( mic_device_mem *mem, char *vendor, size_t *bufsize );
	int ( *mic_get_memory_revision )( mic_device_mem *mem, uint32_t *revision );
	int ( *mic_get_memory_density )( mic_device_mem *mem, uint32_t *density );
	int ( *mic_get_memory_size )( mic_device_mem *mem, uint32_t *size );
	int ( *mic_get_memory_speed )( mic_device_mem *mem, uint32_t *speed );
	int ( *mic_get_memory_type )( mic_device_mem *mem, char *type, size_t *bufsize );
	int ( *mic_get_memory_frequency )( mic_device_mem *mem, uint32_t *buf );
	int ( *mic_get_memory_voltage )( mic_device_mem *mem, uint32_t *buf );
	int ( *mic_get_ecc_mode )( mic_device_mem *mem, uint16_t *ecc );
	int ( *mic_free_memory_info )( mic_device_mem *mem );
	int ( *mic_get_devices )( mic_devices_list **devices );
	int ( *mic_free_devices )( mic_devices_list *devices );
	int ( *mic_get_ndevices )( mic_devices_list *devices, int *ndevices );
	int ( *mic_get_device_at_index )( mic_devices_list *devices, int index, int *device );
	int ( *mic_open_device )( mic_device **device, uint32_t device_num );
	int ( *mic_close_device )( mic_device *device );
	const char* ( *mic_get_error_string )( void );

private:
	bool loadSymbols( void );
};

} // namespace devices
