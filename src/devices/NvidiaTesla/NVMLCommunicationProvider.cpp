#include "NVMLCommunicationProvider.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

#define S( x ) #x
#define STR( x ) S( x )
#define NVML_ERROR_CHECK( statusCode ) checkNVMLErrors( "NVML error at line:" STR( __LINE__ ), statusCode )

//TODO: use nvmlDeviceGetEnforcedPowerLimit ?
namespace devices {

NVMLProxy NVMLCommunicationProvider::proxy;

NVMLCommunicationProvider::NVMLCommunicationProvider( DeviceIdentifier::idType deviceId ) {
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetHandleByUUID( deviceId.c_str(), &deviceHandle ) );
}

bool NVMLCommunicationProvider::init( void ) {
	if ( !proxy.init() ) {
		return false;
	}

	try {
		NVML_ERROR_CHECK( proxy.nvmlInit() );

		LOG ( INFO ) << "...NVML Communication Provider initialized ";

		char NVMLVersionString[ NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE ];
		NVML_ERROR_CHECK( proxy.nvmlSystemGetNVMLVersion( NVMLVersionString, NVML_SYSTEM_NVML_VERSION_BUFFER_SIZE ) );
		LOG ( INFO ) << "......NVML version: " << NVMLVersionString;

		char driverVersionString[ NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE ];
		NVML_ERROR_CHECK( proxy.nvmlSystemGetDriverVersion( driverVersionString, NVML_SYSTEM_DRIVER_VERSION_BUFFER_SIZE ) );
		LOG ( INFO ) << "......Driver version: " << driverVersionString;

		return true;
	}
	catch ( const NVMLError& err ) {
		LOG( ERROR ) << err.info();
		return false;
	}
}

bool NVMLCommunicationProvider::shutdown( void ) {
	try {
		NVML_ERROR_CHECK( proxy.nvmlShutdown() );
		LOG ( INFO ) << "...NVML Communication Provider shut down";

		if ( !proxy.shutdown() ) {
			return false;
		}

		return true;
	}
	catch ( const NVMLError& err ) {
		LOG( ERROR ) << err.info();
		return false;
	}
}

std::vector<nvmlDevice_t> NVMLCommunicationProvider::listDevices( void ) {
	unsigned int devCount;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetCount( &devCount ) );

	std::vector<nvmlDevice_t> devices;
	for ( size_t i = 0; i < devCount; i++ ) {
		nvmlDevice_t devtmp;
		try {
			NVML_ERROR_CHECK( proxy.nvmlDeviceGetHandleByIndex( i, &devtmp ) );
			devices.push_back( devtmp );
		}
		catch ( const NVMLError& err ) {
			LOG( ERROR ) << "Failed to acquire device handle for device with index " << i << ": " << err.message();
		}
	}

	LOG( INFO ) << "Total number of devices: " << devices.size();

	return devices;
}

devices::DeviceIdentifier::idType NVMLCommunicationProvider::getPrimaryId( nvmlDevice_t deviceHandle ) {
	char UUID[ NVML_DEVICE_UUID_BUFFER_SIZE ];
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetUUID( deviceHandle, UUID, NVML_DEVICE_UUID_BUFFER_SIZE ) );

	return UUID;
}

DeviceInformation::InfoContainer NVMLCommunicationProvider::getInfo( nvmlDevice_t deviceHandle ) {
	DeviceInformation::InfoContainer info;

	char UUID[ NVML_DEVICE_UUID_BUFFER_SIZE ];
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetUUID( deviceHandle, UUID, NVML_DEVICE_UUID_BUFFER_SIZE ) );
	info[ "UUID" ] = UUID;

	char serial[ NVML_DEVICE_SERIAL_BUFFER_SIZE ];
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetSerial( deviceHandle, serial, NVML_DEVICE_SERIAL_BUFFER_SIZE ) );
	info[ "SerialNumber" ] = serial;

	unsigned index;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetIndex( deviceHandle, &index ) );
	info[ "Index" ] = std::to_string( index );

	nvmlPciInfo_t pciInfo;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetPciInfo( deviceHandle, &pciInfo ) );
	info[ "PciBusId" ] = pciInfo.busId;

	info[ "PowerManagementCapable" ] = utility::toString( isDevicePowerManagementCapable( deviceHandle ) );

	nvmlComputeMode_t computeMode;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetComputeMode( deviceHandle, &computeMode ) );
	info[ "ComputeMode" ] = computeModeToString( computeMode );

	unsigned pcieLinkGeneration;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetCurrPcieLinkGeneration( deviceHandle, &pcieLinkGeneration ) );
	info[ "PcieLinkGeneration" ] = std::to_string( pcieLinkGeneration );

	unsigned pcieLinkWidth;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetCurrPcieLinkWidth( deviceHandle, &pcieLinkWidth ) );
	info[ "PcieLinkWidth" ] = std::to_string( pcieLinkWidth );

	nvmlEnableState_t currentEccMode, pendingEccMode;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetEccMode( deviceHandle, &currentEccMode, &pendingEccMode ) );
	info[ "CurrentECCMode" ] = ( currentEccMode == NVML_FEATURE_ENABLED ? "Enabled" : "Disabled" );
	info[ "PendingECCMode" ] = ( pendingEccMode == NVML_FEATURE_ENABLED ? "Enabled" : "Disabled" );

	nvmlGpuOperationMode_t currentGpuOperationMode, pendingGpuOperationMode;
	auto ret = proxy.nvmlDeviceGetGpuOperationMode( deviceHandle, &currentGpuOperationMode, &pendingGpuOperationMode ) ;
	setIfSupported( ret, info[ "CurrentGpuOperationMode" ], gpuOperationModeToString( currentGpuOperationMode ) );
	setIfSupported( ret, info[ "PendingGpuOperationMode" ], gpuOperationModeToString( pendingGpuOperationMode ) );

	char inforomImageVersion[ NVML_DEVICE_INFOROM_VERSION_BUFFER_SIZE ];
	ret = proxy.nvmlDeviceGetInforomImageVersion( deviceHandle, inforomImageVersion, NVML_DEVICE_INFOROM_VERSION_BUFFER_SIZE );
	setIfSupported( ret, info[ "InforomImageVersion" ], inforomImageVersion );

	unsigned clock;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetMaxClockInfo( deviceHandle, NVML_CLOCK_GRAPHICS, &clock ) );
	info[ "MaxGraphicsClock" ] = std::to_string( clock );
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetMaxClockInfo( deviceHandle, NVML_CLOCK_SM, &clock ) );
	info[ "MaxSMClock" ] = std::to_string( clock );
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetMaxClockInfo( deviceHandle, NVML_CLOCK_MEM, &clock ) );
	info[ "MaxMemoryClock" ] = std::to_string( clock );

	char name[ NVML_DEVICE_NAME_BUFFER_SIZE ];
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetName( deviceHandle, name, NVML_DEVICE_NAME_BUFFER_SIZE ) );
	info[ "Name" ] = name;

	nvmlEnableState_t persistenceMode;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetPersistenceMode( deviceHandle, &persistenceMode ) );
	info[ "PersistenceMode" ] = ( persistenceMode == NVML_FEATURE_ENABLED ? "Enabled" : "Disabled" );

	unsigned defaultLimit;
	setIfSupported( proxy.nvmlDeviceGetPowerManagementDefaultLimit( deviceHandle, &defaultLimit ), info[ "DefaultPowerManagementLimit" ], std::to_string( defaultLimit ) );

	char vbiosVersion[ NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE ];
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetVbiosVersion( deviceHandle, vbiosVersion, NVML_DEVICE_VBIOS_VERSION_BUFFER_SIZE ) );
	info[ "VBiosVersion" ] = vbiosVersion;

	return info;
}

void NVMLCommunicationProvider::setIfSupported( nvmlReturn_t returnCode, ValueType& field, ValueType value ) {
	if ( returnCode == NVML_ERROR_NOT_SUPPORTED ) {
		field = "N/A";
	}
	else {
		NVML_ERROR_CHECK( returnCode );
		field = value;
	}
}

unsigned NVMLCommunicationProvider::getCurrentPowerLimit( void ) const {
	unsigned currentLimit;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetPowerManagementLimit( deviceHandle, &currentLimit ) );
	return currentLimit;
}

void NVMLCommunicationProvider::setPowerLimit( unsigned milliwatts ) {
	NVML_ERROR_CHECK( proxy.nvmlDeviceSetPowerManagementLimit( deviceHandle, milliwatts ) );
}

std::pair<unsigned, unsigned> NVMLCommunicationProvider::getPowerLimitConstraints( void ) const {
	unsigned minLimit, maxLimit;
	NVML_ERROR_CHECK( proxy.nvmlDeviceGetPowerManagementLimitConstraints( deviceHandle, &minLimit, &maxLimit ) );
	return std::make_pair( minLimit, maxLimit );
}

void NVMLCommunicationProvider::checkNVMLErrors( const char* source, nvmlReturn_t status ) {
	if ( status != NVML_SUCCESS ) {
		throw NVMLError{ source, "error code: " + std::to_string( status ) + ", description: " + proxy.nvmlErrorString( status ) };
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
		NVML_ERROR_CHECK( proxy.nvmlDeviceGetPowerManagementMode( deviceHandle, &powerManagementEnabled ) );

		if ( powerManagementEnabled == NVML_FEATURE_DISABLED ) {
			char name[ NVML_DEVICE_NAME_BUFFER_SIZE ];
			NVML_ERROR_CHECK( proxy.nvmlDeviceGetName( deviceHandle, name, NVML_DEVICE_NAME_BUFFER_SIZE ) );
			char UUID[ NVML_DEVICE_UUID_BUFFER_SIZE ];
			NVML_ERROR_CHECK( proxy.nvmlDeviceGetUUID( deviceHandle, UUID, NVML_DEVICE_UUID_BUFFER_SIZE ) );
			LOG( INFO ) << name << " with UUID: " << UUID << " doesn't support power management.";
		}
	}
	catch ( const NVMLError& ex ) {
		LOG( INFO ) << "Failed to get power management mode for device with handle: " << deviceHandle
					<< " exception: " << ex.info();
		return false;
	}

	return powerManagementEnabled == NVML_FEATURE_ENABLED;
}

} // namespace devices
