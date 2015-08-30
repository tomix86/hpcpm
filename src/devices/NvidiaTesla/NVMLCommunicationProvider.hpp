#pragma once
#include <nvml.h>
#include <vector>
#include <devices/DeviceInformation.hpp>
#include "utility/Exceptions.hpp"

namespace devices {

class NVMLError : public utility::RuntimeError {
public:
	NVMLError( std::string source, nvmlReturn_t code ) :
		utility::RuntimeError{ source, "error code: " + std::to_string( code ) },
		code( code ) {
	}

	nvmlReturn_t code;
};



class NVMLCommunicationProvider {
public:
	NVMLCommunicationProvider( DeviceIdentifier::idType deviceId );
	~NVMLCommunicationProvider( void );

	static std::vector<nvmlDevice_t> listDevices( void );

	unsigned getCurrentPowerLimit( void ) const;

private:
	nvmlDevice_t deviceHandle;

	// throws NVMLError if an error is detected (i.e. status != NVML_SUCCESS)
	static void checkNVMLErrors(  const char* source, nvmlReturn_t status );
};
} // namespace devices
