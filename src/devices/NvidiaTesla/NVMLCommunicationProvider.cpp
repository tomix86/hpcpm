#include "NVMLCommunicationProvider.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

#define S( x ) #x
#define STR( x ) S( x )
#define NVML_ERROR_CHECK( statusCode ) checkNVMLErrors( "NVML error at line:" STR( __LINE__ ), statusCode )

//TODO: use nvmlDeviceGetEnforcedPowerLimit ?
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
			LOG( ERROR ) << "Failed to acquire device handle for device with index: " << i << ", error: " << nvmlErrorString( err.code );
		}
	}

	LOG( INFO ) << "Total number of devices: " << devices.size();

	return devices;
}

devices::DeviceIdentifier::idType NVMLCommunicationProvider::getPrimaryId( nvmlDevice_t deviceHandle ) {
	char UUID[ NVML_DEVICE_UUID_BUFFER_SIZE ];
	NVML_ERROR_CHECK( nvmlDeviceGetUUID( deviceHandle, UUID, NVML_DEVICE_UUID_BUFFER_SIZE ) );

	return UUID;
}

std::map<std::string, std::string> NVMLCommunicationProvider::getInfo( nvmlDevice_t deviceHandle ) {
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

	info[ "PowerManagementCapable" ] = utility::toString( isDevicePowerManagementCapable( deviceHandle ) );

	nvmlComputeMode_t computeMode;
	NVML_ERROR_CHECK( nvmlDeviceGetComputeMode( deviceHandle, &computeMode ) );
	info[ "ComputeMode" ] = computeModeToString( computeMode );

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
	info[ "CurrentGpuOperationMode" ] = gpuOperationModeToString( currentGpuOperationMode );
	info[ "PendingGpuOperationMode" ] = gpuOperationModeToString( pendingGpuOperationMode );

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

std::string NVMLCommunicationProvider::computeModeToString( nvmlComputeMode_t computeMode ) {
	switch( computeMode ) {
	case NVML_COMPUTEMODE_DEFAULT:
		return "Default";
	case NVML_COMPUTEMODE_EXCLUSIVE_THREAD:
		return "Exclusive thread";
	case NVML_COMPUTEMODE_PROHIBITED:
		return "Prohibited";
	case NVML_COMPUTEMODE_EXCLUSIVE_PROCESS:
		return "Exclusive process";
	case NVML_COMPUTEMODE_COUNT:
	default:
		return "Unknown";
	}
}

std::string NVMLCommunicationProvider::gpuOperationModeToString( nvmlGpuOperationMode_t gpuOperationMode ) {
	switch( gpuOperationMode ) {
	case NVML_GOM_ALL_ON:
		return "All on";
	case NVML_GOM_COMPUTE:
		return "Compute";
	case NVML_GOM_LOW_DP:
		return "Low DP";
	default:
		return "Unknown";
	}
}

bool NVMLCommunicationProvider::isDevicePowerManagementCapable( nvmlDevice_t deviceHandle ) {
	nvmlEnableState_t powerManagementEnabled;
	try {
		NVML_ERROR_CHECK( nvmlDeviceGetPowerManagementMode( deviceHandle, &powerManagementEnabled ) );

		if ( powerManagementEnabled == NVML_FEATURE_DISABLED ) {
			char name[ NVML_DEVICE_NAME_BUFFER_SIZE ];
			NVML_ERROR_CHECK( nvmlDeviceGetName( deviceHandle, name, NVML_DEVICE_NAME_BUFFER_SIZE ) );
			char UUID[ NVML_DEVICE_UUID_BUFFER_SIZE ];
			NVML_ERROR_CHECK( nvmlDeviceGetUUID( deviceHandle, UUID, NVML_DEVICE_UUID_BUFFER_SIZE ) );
			LOG( INFO ) << name << " with UUID: " << UUID << " doesn't support power management.";
		}
	}
	catch ( NVMLError& ex ) {
		LOG( INFO ) << "Failed to get power management mode for device with handle: " << deviceHandle
					<< " exception: " << ex.info();
		return false;
	}

	return powerManagementEnabled == NVML_FEATURE_ENABLED;
}

} // namespace devices
