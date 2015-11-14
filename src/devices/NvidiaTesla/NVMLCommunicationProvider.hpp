#pragma once
#include <map>
#include <string>
#include <vector>
#include <devices/DeviceInformation.hpp>
#include "NVMLProxy.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

DEFINE_RUNTIME_ERROR_DERIVATIVE( NVMLError );

class NVMLCommunicationProvider {
public:
	NVMLCommunicationProvider( DeviceIdentifier::idType deviceId );

	static bool init( void );

	static bool shutdown( void );

	static std::vector<nvmlDevice_t> listDevices( void );

	static DeviceIdentifier::idType getPrimaryId( nvmlDevice_t deviceHandle );

	static DeviceInformation::InfoContainer getInfo( nvmlDevice_t deviceHandle );

	unsigned getCurrentPowerLimit( void ) const;

	void setPowerLimit( unsigned milliwatts );

	std::pair<unsigned, unsigned> getPowerLimitConstraints( void ) const;

	unsigned getCurrentPowerUsage( void ) const;

private:
	nvmlDevice_t deviceHandle;
	static NVMLProxy proxy;

	// throws NVMLError if an error is detected (i.e. status != NVML_SUCCESS)
	static void checkNVMLErrors(  const char* source, nvmlReturn_t status );

	static std::string computeModeToString( nvmlComputeMode_t computeMode );

	static std::string gpuOperationModeToString( nvmlGpuOperationMode_t gpuOperationMode );

	static bool isDevicePowerManagementCapable( nvmlDevice_t deviceHandle );

	using ValueType = DeviceInformation::InfoContainer::value_type::second_type;
	static void setIfSupported( nvmlReturn_t returnCode, ValueType& field, ValueType value );
};
} // namespace devices
