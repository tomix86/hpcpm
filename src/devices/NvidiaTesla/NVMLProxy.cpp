#include "NVMLProxy.hpp"

namespace devices {

NVMLProxy::NVMLProxy( void ) :
RLLProxy{ "libnvidia-ml.so" } {
}

bool NVMLProxy::loadSymbols( void ) {
	SYM_LOAD( nvmlInit );
	SYM_LOAD( nvmlShutdown );
	SYM_LOAD( nvmlSystemGetNVMLVersion );
	SYM_LOAD( nvmlDeviceGetHandleByUUID );
	SYM_LOAD( nvmlSystemGetDriverVersion );
	SYM_LOAD( nvmlDeviceGetCount );
	SYM_LOAD( nvmlDeviceGetHandleByIndex );
	SYM_LOAD( nvmlDeviceGetHandleBySerial );
	SYM_LOAD( nvmlDeviceGetHandleByPciBusId );
	SYM_LOAD( nvmlDeviceGetName );
	SYM_LOAD( nvmlDeviceGetUUID );
	SYM_LOAD( nvmlDeviceGetSerial );
	SYM_LOAD( nvmlDeviceGetIndex );
	SYM_LOAD( nvmlDeviceGetPciInfo );
	SYM_LOAD( nvmlDeviceGetComputeMode );
	SYM_LOAD( nvmlDeviceGetCurrPcieLinkGeneration );
	SYM_LOAD( nvmlDeviceGetCurrPcieLinkWidth );
	SYM_LOAD( nvmlDeviceGetEccMode );
	SYM_LOAD( nvmlDeviceGetGpuOperationMode );
	SYM_LOAD( nvmlDeviceGetInforomImageVersion );
	SYM_LOAD( nvmlDeviceGetMaxClockInfo );
	SYM_LOAD( nvmlDeviceGetPersistenceMode );
	SYM_LOAD( nvmlDeviceGetPowerManagementDefaultLimit );
	SYM_LOAD( nvmlDeviceGetVbiosVersion );
	SYM_LOAD( nvmlDeviceGetPowerManagementLimit );
	SYM_LOAD( nvmlDeviceSetPowerManagementLimit );
	SYM_LOAD( nvmlDeviceGetPowerManagementLimitConstraints );
	SYM_LOAD( nvmlDeviceGetPowerManagementMode );
	SYM_LOAD( nvmlErrorString );

	return true;
}

} // namespace devices
