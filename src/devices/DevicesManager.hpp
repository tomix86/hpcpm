#pragma once
#include <boost/noncopyable.hpp>
#include <vector>
#include "Device.hpp"
#include "utility/Exceptions.hpp"

namespace devices {

DEFINE_RUNTIME_ERROR_DERIVATIVE( DeviceNotFoundException )

class DevicesManager : public boost::noncopyable {
public:
	void init( void );

	virtual ~DevicesManager( void );

	virtual devices::Device& getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier );

	virtual const std::vector<devices::Device::Ptr>& getDevicesList( void ) const;

	void updateDevicesList( void );

protected:
	std::vector<devices::Device::Ptr> devicesList;
};
} // namespace devices
