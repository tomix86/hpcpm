#pragma once
#include <boost/noncopyable.hpp>
#include <vector>
#include "Device.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

DEFINE_RUNTIME_ERROR_DERIVATIVE( DeviceNotFoundException );

struct SupportedLibraries {
	bool NVML;
	bool NMPRK;
	bool MPSS;
	bool OpenCL;
};

class DevicesManager : public boost::noncopyable {
public:
	DevicesManager( SupportedLibraries supportedLibraries );
	virtual ~DevicesManager( void );

	void init( void );

	virtual devices::Device& getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier );

	virtual const std::vector<devices::Device::Ptr>& getDevicesList( void ) const;

	void updateDevicesList( void );

	SupportedLibraries getSupportedLibraries( void ) const;

protected:
	std::vector<devices::Device::Ptr> devicesList;

private:
	SupportedLibraries supportedLibraries;
};
} // namespace devices
