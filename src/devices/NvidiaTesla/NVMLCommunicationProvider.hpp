#pragma once
#include <map>
#include <string>
#include <vector>
#include <devices/DeviceInformation.hpp>
#include "NVMLProxy.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

class NVMLError : public utility::RuntimeError {
public:
	NVMLError( std::string source, nvmlReturn_t code ) ://TODO: re-enable
		utility::RuntimeError{ source, "error code: " + std::to_string( code ) + ", description: "/* + proxy.nvmlErrorString( code )*/ },
		code( code ) {
	}

	nvmlReturn_t code;
};

class NVMLCommunicationProvider {
public:
	NVMLCommunicationProvider( DeviceIdentifier::idType deviceId );

	static bool init( void );

	static bool shutdown( void );

	static std::vector<nvmlDevice_t> listDevices( void );

	static devices::DeviceIdentifier::idType getPrimaryId( nvmlDevice_t deviceHandle );

	static std::map<std::string, std::string> getInfo( nvmlDevice_t deviceHandle );

	unsigned getCurrentPowerLimit( void ) const;

	void setPowerLimit( unsigned milliwatts );

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const;

private:
	nvmlDevice_t deviceHandle;
	static NVMLProxy proxy;

	// throws NVMLError if an error is detected (i.e. status != NVML_SUCCESS)
	static void checkNVMLErrors(  const char* source, nvmlReturn_t status );

	static std::string computeModeToString( nvmlComputeMode_t computeMode );

	static std::string gpuOperationModeToString( nvmlGpuOperationMode_t gpuOperationMode );

	static bool isDevicePowerManagementCapable( nvmlDevice_t deviceHandle );
};
} // namespace devices
