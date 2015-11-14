#pragma once
#include <nvml.h>
#include "utility/RLLProxy.hpp"

namespace devices {

class NVMLProxy : public utility::RLLProxy {
public:
	NVMLProxy( void );

	nvmlReturn_t ( *nvmlInit )( void );
	nvmlReturn_t ( *nvmlShutdown )( void );
	nvmlReturn_t ( *nvmlSystemGetNVMLVersion )( char *version, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetHandleByUUID )( const char *uuid, nvmlDevice_t *device );
	nvmlReturn_t ( *nvmlSystemGetDriverVersion )( char *version, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetCount )( unsigned int *deviceCount );
	nvmlReturn_t ( *nvmlDeviceGetHandleByIndex )( unsigned int index, nvmlDevice_t *device );
	nvmlReturn_t ( *nvmlDeviceGetHandleBySerial )( const char *serial, nvmlDevice_t *device );
	nvmlReturn_t ( *nvmlDeviceGetHandleByPciBusId )( const char *pciBusId, nvmlDevice_t *device );
	nvmlReturn_t ( *nvmlDeviceGetName )( nvmlDevice_t device, char *name, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetUUID )( nvmlDevice_t device, char *uuid, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetSerial )( nvmlDevice_t device, char *serial, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetIndex )( nvmlDevice_t device, unsigned int *index );
	nvmlReturn_t ( *nvmlDeviceGetPciInfo )( nvmlDevice_t device, nvmlPciInfo_t *pci );
	nvmlReturn_t ( *nvmlDeviceGetComputeMode )( nvmlDevice_t device, nvmlComputeMode_t *mode );
	nvmlReturn_t ( *nvmlDeviceGetCurrPcieLinkGeneration )( nvmlDevice_t device, unsigned int *currLinkGen );
	nvmlReturn_t ( *nvmlDeviceGetCurrPcieLinkWidth )( nvmlDevice_t device, unsigned int *currLinkWidth );
	nvmlReturn_t ( *nvmlDeviceGetEccMode )( nvmlDevice_t device, nvmlEnableState_t *current, nvmlEnableState_t *pending );
	nvmlReturn_t ( *nvmlDeviceGetGpuOperationMode )( nvmlDevice_t device, nvmlGpuOperationMode_t *current, nvmlGpuOperationMode_t *pending );
	nvmlReturn_t ( *nvmlDeviceGetInforomImageVersion )( nvmlDevice_t device, char *version, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetMaxClockInfo )( nvmlDevice_t device, nvmlClockType_t type, unsigned int *clock );
	nvmlReturn_t ( *nvmlDeviceGetPersistenceMode )( nvmlDevice_t device, nvmlEnableState_t *mode );
	nvmlReturn_t ( *nvmlDeviceGetPowerManagementDefaultLimit )( nvmlDevice_t device, unsigned int *defaultLimit );
	nvmlReturn_t ( *nvmlDeviceGetVbiosVersion )( nvmlDevice_t device, char *version, unsigned int length );
	nvmlReturn_t ( *nvmlDeviceGetPowerManagementLimit )( nvmlDevice_t device, unsigned int *limit );
	nvmlReturn_t ( *nvmlDeviceSetPowerManagementLimit )( nvmlDevice_t device, unsigned int limit );
	nvmlReturn_t ( *nvmlDeviceGetPowerManagementLimitConstraints )( nvmlDevice_t device, unsigned int *minLimit, unsigned int *maxLimit );
	nvmlReturn_t ( *nvmlDeviceGetPowerManagementMode )( nvmlDevice_t device, nvmlEnableState_t *mode );
	nvmlReturn_t ( *nvmlDeviceGetPowerUsage )( nvmlDevice_t device, unsigned int *power);
	const char* ( *nvmlErrorString )( nvmlReturn_t result );

private:
	bool loadSymbols( void );
};

} // namespace devices
