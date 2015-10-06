#pragma once
#include <boost/noncopyable.hpp>
#include <vector>
#include "Device.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

DEFINE_RUNTIME_ERROR_DERIVATIVE( DeviceNotFoundException );

class DevicesManager : public boost::noncopyable {
public:
	DevicesManager( bool hasNVML, bool hasNMPRK, bool hasMPSS );
	virtual ~DevicesManager( void );

	void init( void );

	virtual devices::Device& getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier );

	virtual const std::vector<devices::Device::Ptr>& getDevicesList( void ) const;

	void updateDevicesList( void );

protected:
	std::vector<devices::Device::Ptr> devicesList;

private:
	 const bool hasNVML;
	 const bool hasNMPRK;
	 const bool hasMPSS;
};
} // namespace devices
