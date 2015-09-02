#include "NVMLCommunicationProvider.hpp"
#include "utility/Logging.hpp"

#define S( x ) #x
#define STR( x ) S( x )
#define NVML_ERROR_CHECK( statusCode ) checkNVMLErrors( "NVML error at line:" STR( __LINE__ ), statusCode )

//TODO: use nvmlDeviceGetEnforcedPowerLimit ?
//TODO: think about error handling
//TOOO: should we also reported devices which are not capable of power management via NVML?
namespace devices {
NVMLCommunicationProvider::NVMLCommunicationProvider( DeviceIdentifier::idType deviceId ) {
	NVML_ERROR_CHECK( nvmlDeviceGetHandleByUUID( deviceId.c_str(), &deviceHandle ) ); //TODO: this line should be changed when we decide what should be used as device's primary id
}


void NVMLCommunicationProvider::init( void ) {
	NVML_ERROR_CHECK( nvmlInit() );

	LOG ( INFO ) << "...NVML Communication Provider initialized ";

	char NVMLVersionString[ NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlSystemGetNVMLVersion( NVMLVersionString, NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE ) );
	LOG ( INFO ) << "......NVML version: " << NVMLVersionString;

	char driverVersionString[ NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlSystemGetDriverVersion( driverVersionString, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE ) );
	LOG ( INFO ) << "......Driver version: " << driverVersionString;
}

void NVMLCommunicationProvider::shutdown( void ) {
	NVML_ERROR_CHECK( nvmlShutdown() );
	LOG ( INFO ) << "...NVML Communication Provider shut down";
}

std::vector<nvmlDevice_t> NVMLCommunicationProvider::listDevices( void ) {
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

	return devices;
}

devices::DeviceIdentifier::idType NVMLCommunicationProvider::getPrimaryId( nvmlDevice_t deviceHandle ) {
	std::map<std::string, std::string> info;

	char UUID[ NVML_DEVICE_UUID_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetUUID( deviceHandle, UUID, NVML_DEVICE_UUID_BUFFER_SIZE ) );
	info[ "UUID" ] = UUID;

	char serial[ NVML_DEVICE_SERIAL_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetSerial( deviceHandle, serial, NVML_DEVICE_SERIAL_BUFFER_SIZE ) );
	info[ "SerialNumber" ] = serial;

	unsigned index;
	NVML_ERROR_CHECK( nvmlDeviceGetIndex( deviceHandle, &index ) );
	info[ "Index" ] = std::to_string( index );

	nvmlPciInfo_t pciInfo;
	NVML_ERROR_CHECK( nvmlDeviceGetPciInfo( deviceHandle, &pciInfo ) );
	info[ "PciBusId" ] = pciInfo.busId;



	nvmlComputeMode_t computeMode;
	NVML_ERROR_CHECK( nvmlDeviceGetComputeMode( deviceHandle, &computeMode ) );
	switch( computeMode ) {
	case NVML_COMPUTEMODE_DEFAULT:
		info[ "ComputeMode" ] = "Default"; break;
	case NVML_COMPUTEMODE_EXCLUSIVE_THREAD:
		info[ "ComputeMode" ] = "Exclusive thread"; break;
	case NVML_COMPUTEMODE_PROHIBITED:
		info[ "ComputeMode" ] = "Prohibited"; break;
	case NVML_COMPUTEMODE_EXCLUSIVE_PROCESS:
		info[ "ComputeMode" ] = "Exclusive process"; break;
	case NVML_COMPUTEMODE_COUNT:
	default:
		info[ "ComputeMode" ] = "Unknown";
	}

	unsigned pcieLinkGeneration;
	NVML_ERROR_CHECK( nvmlDeviceGetCurrPcieLinkGeneration( deviceHandle, &pcieLinkGeneration ) );
	info[ "PcieLinkGeneration" ] = std::to_string( pcieLinkGeneration );

	unsigned pcieLinkWidth;
	NVML_ERROR_CHECK( nvmlDeviceGetCurrPcieLinkWidth( deviceHandle, &pcieLinkWidth ) );
	info[ "PcieLinkWidth" ] = std::to_string( pcieLinkWidth );

	nvmlEnableState_t currentEccMode, pendingEccMode;
	NVML_ERROR_CHECK( nvmlDeviceGetEccMode( deviceHandle, &currentEccMode, &pendingEccMode ) );
	info[ "CurrentECCMode" ] = ( currentEccMode == NVML_FEATURE_ENABLED ? "Enabled" : "Disabled" );
	info[ "PendingECCMode" ] = ( pendingEccMode == NVML_FEATURE_ENABLED ? "Enabled" : "Disabled" );

	nvmlGpuOperationMode_t currentGpuOperationMode, pendingGpuOperationMode;
	NVML_ERROR_CHECK( nvmlDeviceGetGpuOperationMode( deviceHandle, &currentGpuOperationMode, &pendingGpuOperationMode ) );
	switch( currentGpuOperationMode ) {
	case NVML_GOM_ALL_ON:
		info[ "CurrentGpuOperationMode" ] = "All on"; break;
	case NVML_GOM_COMPUTE:
		info[ "CurrentGpuOperationMode" ] = "Compute"; break;
	case NVML_GOM_LOW_DP:
		info[ "CurrentGpuOperationMode" ] = "Low DP"; break;
	default:
		info[ "CurrentGpuOperationMode" ] = "Unknown"; break;
	}
	switch( pendingGpuOperationMode ) {
	case NVML_GOM_ALL_ON:
		info[ "PendingGpuOperationMode" ] = "All on"; break;
	case NVML_GOM_COMPUTE:
		info[ "PendingGpuOperationMode" ] = "Compute"; break;
	case NVML_GOM_LOW_DP:
		info[ "PendingGpuOperationMode" ] = "Low DP"; break;
	default:
		info[ "PendingGpuOperationMode" ] = "Unknown"; break;
	}

	char inforomImageVersion[ NVML_DEVICE_INFOROM_VERSION_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetInforomImageVersion( deviceHandle, inforomImageVersion, NVML_DEVICE_INFOROM_VERSION_BUFFER_SIZE ) );
	info[ "InforomImageVersion" ] = inforomImageVersion;

	unsigned clock;
	NVML_ERROR_CHECK( nvmlDeviceGetMaxClockInfo( deviceHandle, NVML_CLOCK_GRAPHICS, &clock ) );
	info[ "MaxGraphicsClock" ] = std::to_string( clock );
	NVML_ERROR_CHECK( nvmlDeviceGetMaxClockInfo( deviceHandle, NVML_CLOCK_SM, &clock ) );
	info[ "MaxSMClock" ] = std::to_string( clock );
	NVML_ERROR_CHECK( nvmlDeviceGetMaxClockInfo( deviceHandle, NVML_CLOCK_MEM, &clock ) );
	info[ "MaxMemoryClock" ] = std::to_string( clock );

	char name[ NVML_DEVICE_NAME_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetName( deviceHandle, name, NVML_DEVICE_NAME_BUFFER_SIZE ) );
	info[ "Name" ] = name;

	nvmlEnableState_t persistenceMode;
	NVML_ERROR_CHECK( nvmlDeviceGetPersistenceMode( deviceHandle, &persistenceMode ) );
	info[ "PersistenceMode" ] = ( persistenceMode == NVML_FEATURE_ENABLED ? "Enabled" : "Disabled" );

	unsigned defaultLimit;
	NVML_ERROR_CHECK( nvmlDeviceGetPowerManagementDefaultLimit( deviceHandle, &defaultLimit ) );
	info[ "DefaultPowerManagementLimit" ] = std::to_string( defaultLimit );

	char vbiosVersion[ NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetVbiosVersion( deviceHandle, vbiosVersion, NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE ) );
	info[ "VBiosVersion" ] = vbiosVersion;

	return UUID;
}

std::map<std::string, std::string> NVMLCommunicationProvider::getInfo( nvmlDevice_t deviceHandle ) {
	std::map<std::string, std::string> info;
	char name[ NVML_DEVICE_NAME_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetName( deviceHandle, name, NVML_DEVICE_NAME_BUFFER_SIZE ) );

	return info;
}

unsigned NVMLCommunicationProvider::getCurrentPowerLimit( void ) const {
	unsigned currentLimit;
	NVML_ERROR_CHECK( nvmlDeviceGetPowerManagementLimit( deviceHandle, &currentLimit ) );
	return currentLimit;
}

void NVMLCommunicationProvider::setPowerLimit( unsigned milliwatts ) {
	NVML_ERROR_CHECK( nvmlDeviceSetPowerManagementLimit( deviceHandle, milliwatts ) );
}

std::pair<unsigned, unsigned> NVMLCommunicationProvider::getPowerLimitConstraints( void ) const {
	unsigned minLimit, maxLimit;
	NVML_ERROR_CHECK( nvmlDeviceGetPowerManagementLimitConstraints( deviceHandle, &minLimit, &maxLimit ) );
	return std::make_pair( minLimit, maxLimit );
}

void NVMLCommunicationProvider::checkNVMLErrors( const char* source, nvmlReturn_t status ) {
	if ( status != NVML_SUCCESS ) {
		throw NVMLError{ source, status };
	}
}

} // namespace devices
