#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <numeric>
#include <sstream>
#include "MPSSCommunicationProvider.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

namespace devices {

MPSSProxy MPSSCommunicationProvider::proxy;

MPSSCommunicationProvider::MPSSCommunicationProvider( DeviceIdentifier::idType id ) :
		device{ getDeviceById( id ) },
		powerLimitHelper{ device } {
}

bool MPSSCommunicationProvider::init( void ) {
	auto returnVal = proxy.init();
	MPSSPowerLimitHelper::init( proxy );
	return returnVal;
}

bool MPSSCommunicationProvider::shutdown( void ) {
	return proxy.shutdown();
}

unsigned MPSSCommunicationProvider::getCurrentPowerLimit( void ) const {
	return powerLimitHelper.getPowerLimit();
}

void MPSSCommunicationProvider::setPowerLimit( unsigned watts ) {
	powerLimitHelper.setPowerLimit( watts );
}

std::pair<unsigned, unsigned> MPSSCommunicationProvider::getPowerLimitConstraints( void ) const {
	return { powerLimitHelper.getPowerLimitLowerConstraint(), powerLimitHelper.getPowerLimitUpperConstraint() };
}

unsigned MPSSCommunicationProvider::getCurrentPowerUsage( void ) const {
	return powerLimitHelper.getCurrentPowerUsage();
}

std::vector<int> MPSSCommunicationProvider::listDevices( void ) {
	std::vector<int> list( getDevicesList().getCount() );
	std::iota( list.begin(), list.end(), 0 );

	LOG( INFO ) << "Total number of devices: " << list.size();

	return list;
}

DeviceIdentifier::idType MPSSCommunicationProvider::getPrimaryId( int index ) {
	return getUUID( index );
}

DeviceInformation::InfoContainer MPSSCommunicationProvider::getInfo( int index ) {
	DeviceInformation::InfoContainer info;
	auto devPtr = getDeviceByIndex( index );
	auto dev = devPtr.get();

	info[ "UUID" ] = getUUID( index );

	info[ "Name" ] = proxy.mic_get_device_name( dev );

	char stringBuf[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;
	MPSS_ERROR_CHECK( proxy.mic_get_silicon_sku( dev, stringBuf, &size ) );
	info[ "SiliconSKU" ] = stringBuf;

	MPSS_ERROR_CHECK( proxy.mic_get_serial_number( dev, stringBuf, &size ) );
	info[ "SerialNumber" ] = stringBuf;

	mic_cores_info* coresInfo;
	MPSS_ERROR_CHECK( proxy.mic_get_cores_info( dev, &coresInfo ) );
	std::shared_ptr<mic_cores_info> coresInfoRAIIGuard{ coresInfo, []( mic_cores_info* ptr ) { proxy.mic_free_cores_info( ptr ); } };

	std::uint32_t coresCount;
	MPSS_ERROR_CHECK( proxy.mic_get_cores_count( coresInfo, &coresCount ) );
	info[ "CoresCount" ] = std::to_string( coresCount );

	fillVersionInfo( info, dev );
	fillProcessorInfo( info, dev );
	fillPciConfigInfo( info, dev );
	fillMemoryInfo( info, dev );

	return info;
}

void MPSSCommunicationProvider::fillVersionInfo( DeviceInformation::InfoContainer& info, mic_device* dev ) {
	mic_version_info* version;
	MPSS_ERROR_CHECK( proxy.mic_get_version_info( dev, &version ) );
	std::shared_ptr<mic_version_info> versionRAIIGuard{ version, []( mic_version_info* ptr ) { proxy.mic_free_version_info( ptr ); } };

	char stringBuf[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;
	MPSS_ERROR_CHECK( proxy.mic_get_flash_version( version, stringBuf, &size ) );
	info[ "FlashVersion" ] = stringBuf;

	MPSS_ERROR_CHECK( proxy.mic_get_uos_version( version, stringBuf, &size ) );
	info[ "uOSVersion" ] = stringBuf;
}

void MPSSCommunicationProvider::fillProcessorInfo( DeviceInformation::InfoContainer& info, mic_device* dev ) {
	mic_processor_info* processorInfo;
	MPSS_ERROR_CHECK( proxy.mic_get_processor_info( dev, &processorInfo ) );
	std::shared_ptr<mic_processor_info> processorInfoRAIIGuard{ processorInfo, []( mic_processor_info* ptr ) { proxy.mic_free_processor_info( ptr ); } };

	std::uint16_t model, model_ext;
	MPSS_ERROR_CHECK( proxy.mic_get_processor_model( processorInfo, &model,	&model_ext ) );
	info[ "ProcessorModel" ] = std::to_string( model );
	info[ "ProcessorModelExtension" ] = std::to_string( model_ext );

	std::uint16_t type;
	MPSS_ERROR_CHECK( proxy.mic_get_processor_type( processorInfo, &type ) );
	info[ "ProcessorType" ] = std::to_string( type );

	std::uint32_t id;
	MPSS_ERROR_CHECK( proxy.mic_get_processor_steppingid( processorInfo, &id ) );
	info[ "ProcessorSteppingId" ] = std::to_string( id );

	char stringBuf[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;
	MPSS_ERROR_CHECK( proxy.mic_get_processor_stepping( processorInfo, stringBuf, &size ) );
	info[ "ProcessorStepping" ] = stringBuf;
}

void MPSSCommunicationProvider::fillPciConfigInfo( DeviceInformation::InfoContainer& info, mic_device* dev ) {
	mic_pci_config* pciConfig;
	MPSS_ERROR_CHECK( proxy.mic_get_pci_config( dev, &pciConfig ) );
	std::shared_ptr<mic_pci_config> pciConfigRAIIGuard{ pciConfig, []( mic_pci_config* ptr ) { proxy.mic_free_pci_config( ptr ); } };

	std::uint16_t vendorId;
	MPSS_ERROR_CHECK( proxy.mic_get_vendor_id( pciConfig, &vendorId ) );
	info[ "VendorId" ] = "0x" + utility::toHexString( vendorId );

	std::uint16_t deviceId;
	MPSS_ERROR_CHECK( proxy.mic_get_device_id( pciConfig, &deviceId ) );
	info[ "DeviceId" ] = "0x" + utility::toHexString( deviceId );

	std::uint8_t revisionId;
	MPSS_ERROR_CHECK( proxy.mic_get_revision_id( pciConfig, &revisionId ) );
	info[ "RevisionId" ] = "0x" + utility::toHexString( revisionId );

	std::uint16_t subsystemId;
	MPSS_ERROR_CHECK( proxy.mic_get_subsystem_id( pciConfig, &subsystemId ) );
	info[ "SubsysId" ] = "0x" + utility::toHexString( subsystemId );
}

void MPSSCommunicationProvider::fillMemoryInfo( DeviceInformation::InfoContainer& info, mic_device* dev ) {
	mic_device_mem* memoryInfo;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_info(dev, &memoryInfo ) );
	std::shared_ptr<mic_device_mem> memoryInfoRAIIGuard{ memoryInfo, []( mic_device_mem* ptr ) { proxy.mic_free_memory_info( ptr ); } };

	char stringBuf[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_vendor( memoryInfo, stringBuf, &size ) );
	info[ "MemoryVendor" ] = stringBuf;

	std::uint32_t revision;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_revision( memoryInfo, &revision ) );
	info[ "MemoryRevision" ] = std::to_string( revision );

	std::uint32_t density;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_density( memoryInfo, &density ) );
	info[ "MemoryDensity" ] = std::to_string( density );

	std::uint32_t memorySize;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_size( memoryInfo, &memorySize ) );
	info[ "MemorySize" ] = std::to_string( memorySize );

	size = sizeof( stringBuf );
	MPSS_ERROR_CHECK( proxy.mic_get_memory_type( memoryInfo, stringBuf, &size ) );
	info[ "MemoryType" ] = stringBuf;

	std::uint32_t frequency;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_frequency( memoryInfo, &frequency ) );
	info[ "MemoryFrequency" ] = std::to_string( frequency );

	std::uint16_t ecc;
	MPSS_ERROR_CHECK( proxy.mic_get_ecc_mode( memoryInfo, &ecc ) );
	info[ "ECCMode" ] = std::to_string( ecc );
}

MPSSCommunicationProvider::DevicesListWrapper MPSSCommunicationProvider::getDevicesList( void ) {
	mic_devices_list* micDevicesList;

	auto result = proxy.mic_get_devices( &micDevicesList );
	if ( result == E_MIC_DRIVER_NOT_LOADED ) {
		throw MPSSError( "MPSSCommunicationProvider::listDevices", "The driver is not loaded" );
	}
	else if ( result == E_MIC_ACCESS ) {
		throw MPSSError( "MPSSCommunicationProvider::listDevices", "Access to the driver denied, do you have neccessary permissions?" );
	}
	else {
		MPSS_ERROR_CHECK( result );
	}
	std::shared_ptr<mic_devices_list> micDevicesListRAIIGuard{ micDevicesList, []( mic_devices_list* ptr ) { proxy.mic_free_devices( ptr ); } };

	int devicesCount;
	MPSS_ERROR_CHECK( proxy.mic_get_ndevices( micDevicesList, &devicesCount ) != E_MIC_SUCCESS);

	return { micDevicesListRAIIGuard, devicesCount };
}

MicDevicePtr MPSSCommunicationProvider::getDeviceByIndex( int index ) {
	auto micDevicesListWrapper = getDevicesList();

	if ( index >= micDevicesListWrapper.getCount() ) {
		throw MPSSError( "MPSSCommunicationProvider::getDeviceById", "Index: " + std::to_string( index ) + " out of bounds" );
	}

	int deviceHandle;
	MPSS_ERROR_CHECK( proxy.mic_get_device_at_index( micDevicesListWrapper.getPtr(), index, &deviceHandle ) != E_MIC_SUCCESS );

	mic_device* device;
	MPSS_ERROR_CHECK( proxy.mic_open_device( &device, deviceHandle ) != E_MIC_SUCCESS );

	auto deviceDeleter = []( mic_device* ptr ) { proxy.mic_close_device( ptr ); };
	MicDevicePtr devPtr{ device, deviceDeleter };

	return devPtr;
}

MicDevicePtr MPSSCommunicationProvider::getDeviceById( DeviceIdentifier::idType id ) {
	auto micDevicesListWrapper = getDevicesList();

	for( int i = 0; i < micDevicesListWrapper.getCount(); ++i ) {
		if( getPrimaryId( i ) == id ) {
			return getDeviceByIndex( i );
		}
	}

	throw MPSSError( "MPSSCommunicationProvider::getDeviceById", "Device with id: " + id + " not found" );
}

std::string MPSSCommunicationProvider::getUUID( int index ) {
	auto devPtr = getDeviceByIndex( index );
	std::uint8_t UUID[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;

	MPSS_ERROR_CHECK( proxy.mic_get_uuid( devPtr.get(), UUID, &size ) != E_MIC_SUCCESS );

	std::string UUIDstring;
	for ( unsigned i = 0; i < size; ++i ) {
		UUIDstring += utility::toHexString( UUID[ i ] );
	}

	return UUIDstring;
}

void MPSSCommunicationProvider::checkMPSSErrors( const char* source, int status ) {
	if ( status != E_MIC_SUCCESS ) {
		std::ostringstream err;
		err << proxy.mic_get_error_string() << ": " << strerror( errno );
		throw MPSSError( source, err.str());
	}
}

} // namespace devices
