#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <numeric>
#include <sstream>
#include "MPSSCommunicationProvider.hpp"
#include "utility/Logging.hpp"

#define S( x ) #x
#define STR( x ) S( x )
#define MPSS_ERROR_CHECK( statusCode ) checkMPSSErrors( "MPSS error at line:" STR( __LINE__ ), statusCode )

namespace devices {

MPSSProxy MPSSCommunicationProvider::proxy;

MPSSCommunicationProvider::MPSSCommunicationProvider( DeviceIdentifier::idType id ) :
deviceHandle{ getDeviceById( id ) } {
}

bool MPSSCommunicationProvider::init( void ) {
	return proxy.init();
}

bool MPSSCommunicationProvider::shutdown( void ) {
	return proxy.shutdown();
}

unsigned MPSSCommunicationProvider::getCurrentPowerLimit( void ) const {
	mic_power_limit* powerLimit;
	proxy.mic_get_power_limit( deviceHandle.get(), &powerLimit );
	std::shared_ptr<mic_power_limit> powerLimitRAIIGuard{ powerLimit, []( mic_power_limit* ptr ) { proxy.mic_free_power_limit( ptr ); } };

	std::uint32_t limit;
	MPSS_ERROR_CHECK( proxy.mic_get_power_phys_limit( powerLimit, &limit ) );
	MPSS_ERROR_CHECK( proxy.mic_get_power_hmrk( powerLimit, &limit ) ); //power limit 0
	MPSS_ERROR_CHECK( proxy.mic_get_power_lmrk( powerLimit, &limit ) ); //power limit 1

//	std::uint32_t window;
//	MPSS_ERROR_CHECK( proxy.mic_get_time_window0( powerLimit, &window );
//	MPSS_ERROR_CHECK( proxy.mic_get_time_window1( powerLimit, &window );

	return 0;
}

void MPSSCommunicationProvider::setPowerLimit( unsigned milliwatts ) {
	auto dev = deviceHandle.get();

	std::uint32_t hmrk, lmrk;
	hmrk = lmrk = milliwatts;
	MPSS_ERROR_CHECK( proxy.mic_set_power_limit0( dev, hmrk, TIME_WINDOW_0 ) );
	MPSS_ERROR_CHECK( proxy.mic_set_power_limit1( dev, lmrk, TIME_WINDOW_1 ) );
}

std::pair<unsigned, unsigned> MPSSCommunicationProvider::getPowerLimitConstraints( void ) const {
	return {}; //TODO: implement
}

std::vector<int> MPSSCommunicationProvider::listDevices( void ) {
	auto devList = getDevicesList();

	std::vector<int> list{ devList.getCount() };
	std::iota( list.begin(), list.end(), 0 );

	return list;
}

devices::DeviceIdentifier::idType MPSSCommunicationProvider::getPrimaryId( int index ) {
	return getUUID( index );
}

std::map<std::string, std::string> MPSSCommunicationProvider::getInfo( int index ) {
	std::map<std::string, std::string> info;
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

void MPSSCommunicationProvider::fillVersionInfo( std::map<std::string, std::string>& info, mic_device* dev ) {
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

void MPSSCommunicationProvider::fillProcessorInfo( std::map<std::string, std::string>& info, mic_device* dev ) {
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

void MPSSCommunicationProvider::fillPciConfigInfo( std::map<std::string, std::string>& info, mic_device* dev ) {
	//TODO: sposob wypisywania tych wszystkich id? w examples.c jest uzywany hex, sprawdzic po odpaleniu na aplu
	mic_pci_config* pciConfig;
	MPSS_ERROR_CHECK( proxy.mic_get_pci_config( dev, &pciConfig ) );
	std::shared_ptr<mic_pci_config> pciConfigRAIIGuard{ pciConfig, []( mic_pci_config* ptr ) { proxy.mic_free_pci_config( ptr ); } };

	std::uint16_t vendorId;
	MPSS_ERROR_CHECK( proxy.mic_get_vendor_id( pciConfig, &vendorId ) );
	info[ "VendorId" ] = std::to_string( vendorId );

	std::uint16_t deviceId;
	MPSS_ERROR_CHECK( proxy.mic_get_device_id( pciConfig, &deviceId ) );
	info[ "DeviceId" ] = std::to_string( deviceId );

	std::uint8_t revisionId;
	MPSS_ERROR_CHECK( proxy.mic_get_revision_id( pciConfig, &revisionId ) );
	info[ "RevisionId" ] = std::to_string( revisionId );

	std::uint16_t subsystemId;
	MPSS_ERROR_CHECK( proxy.mic_get_subsystem_id( pciConfig, &subsystemId ) );
	info[ "SubsysId" ] = std::to_string( subsystemId );

/*	std::uint32_t linkWidth;
	// Has to be su to run this, else there is access violation error
	MPSS_ERROR_CHECK( proxy.mic_get_link_width( pciConfig, &linkWidth ) );
	info[ "LinkWidth" ] = std::to_string( linkWidth );

	std::uint32_t payload;
	MPSS_ERROR_CHECK( proxy.mic_get_max_payload( pciConfig, &payload ) );
	info[ "MaxPayload" ] = std::to_string( payload );

	std::uint32_t readreq;
	MPSS_ERROR_CHECK( proxy.mic_get_max_readreq( pciConfig, &readreq ) );
	info[ "MaxReadRequests" ] = std::to_string( readreq );

	char stringBuf[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;
	MPSS_ERROR_CHECK( proxy.mic_get_link_speed( pciConfig, stringBuf, &size ) );
	info[ "LinkSpeed" ] = stringBuf;*/
}

void MPSSCommunicationProvider::fillMemoryInfo( std::map<std::string, std::string>& info, mic_device* dev ) {
	mic_device_mem* memoryInfo;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_info(dev, &memoryInfo ) );
	std::shared_ptr<mic_device_mem> memoryInfoRAIIGuard{ memoryInfo, []( mic_device_mem* ptr ) { proxy.mic_free_memory_info( ptr ); } };

	char stringBuf[ MAX_NAME_LENGTH ];
	size_t size = MAX_NAME_LENGTH;
	MPSS_ERROR_CHECK( proxy.mic_get_memory_vendor( memoryInfo, stringBuf, &size ) );
	info[ "LinkSpeed" ] = stringBuf;

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

void MPSSCommunicationProvider::checkMPSSErrors( const char* source, int status ) {
	if ( status != E_MIC_SUCCESS ) {
		std::ostringstream err;
		err << proxy.mic_get_error_string() << ": " << strerror( errno );
		throw MPSSError( source, err.str());
	}
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

	std::ostringstream UUIDss;
	UUIDss << std::setfill( '0' ) << std::hex;
	for ( unsigned i = 0; i < size; ++i ) {
		UUIDss << std::setw( 2 ) << ( static_cast<int>( UUID[ i ] ) & 0x000000ff ); //TODO: czy ten and potrzebny
	}

	return UUIDss.str();
}

} // namespace devices
