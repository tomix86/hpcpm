#include "NVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"

#define S( x ) #x
#define STR( x ) S( x )
#define NVML_ERROR_CHECK( statusCode ) checkNVMLErrors( "NVML error at line:" STR( __LINE__ ), statusCode )

namespace devices {
NVMLCommunicationProvider::NVMLCommunicationProvider( DeviceIdentifier::idType deviceId ) {
	// Initialize NVML
	NVML_ERROR_CHECK( nvmlInit() );

	NVML_ERROR_CHECK( nvmlDeviceGetHandleByUUID( deviceId.c_str(), &deviceHandle ) ); //TODO: this line should be changed when we decide what shoul be used as device's primary id
}

NVMLCommunicationProvider::~NVMLCommunicationProvider( void ) {
	// do the cleanup
	NVML_ERROR_CHECK( nvmlShutdown() );
}

std::vector<nvmlDevice_t> NVMLCommunicationProvider::listDevices( void ) {
	// Initialize NVML
	NVML_ERROR_CHECK( nvmlInit() ); //TODO: move initializations to one place... but where?

	// Enumerate devices and store them in container
	unsigned int devCount;
	NVML_ERROR_CHECK( nvmlDeviceGetCount( &devCount ) );
	std::vector<nvmlDevice_t> devices;
	for ( size_t i = 0; i < devCount; i++ ) {
		nvmlDevice_t devtmp;
		try {
			NVML_ERROR_CHECK( nvmlDeviceGetHandleByIndex( i, &devtmp ) );
			devices.push_back( devtmp );
		}
		catch ( NVMLError& err ) {
			LOG( ERROR ) << nvmlErrorString( err.code );
			// something has prevented us from acquiring device handle, take necessary actions here...
		}
	}

	LOG( INFO ) << "Total number of devices: " << devices.size();

	// In case of unsupported (i.e. not capable of power managment via NVML) devices
	// we can either remove them or store them in another container
	// at this moment we've chosen the former approach
	auto iter = std::remove_if( devices.begin(), devices.end(), [] ( nvmlDevice_t device ) {
		nvmlEnableState_t powerManagementEnabled;
		try {
			NVML_ERROR_CHECK( nvmlDeviceGetPowerManagementMode( device, &powerManagementEnabled ) );
			return powerManagementEnabled != nvmlEnableState_t::NVML_FEATURE_ENABLED;
		}
		catch ( NVMLError ) {
			// either device does not support power managment features or some other error has happened
			// in both cases we have no use of that device
			return true;
		}
	} );
	devices.erase( iter, devices.end() );

	LOG( INFO ) << "Number of devices supporting power management via NVML: " << devices.size();

	// do the cleanup
	NVML_ERROR_CHECK( nvmlShutdown() );

	return devices;
}

unsigned NVMLCommunicationProvider::getCurrentPowerLimit( void ) const {
	unsigned currentLimit;
	NVML_ERROR_CHECK( nvmlDeviceGetPowerManagementLimit( deviceHandle, &currentLimit ) );
	return currentLimit;
}

void NVMLCommunicationProvider::checkNVMLErrors( const char* source, nvmlReturn_t status ) {
	if ( status != NVML_SUCCESS ) {
		throw NVMLError{ source, status };
	}
}

} // namespace devices
