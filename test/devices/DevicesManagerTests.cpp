#include <gtest/gtest.h>
#include "devices/DevicesManager.hpp"
#include "devices/IntelXeon/IntelXeonDevice.hpp"

using namespace devices;

class DevicesManagerAccessor : public devices::DevicesManager {
public:
	std::vector<devices::Device::Ptr>& getDevicesListRef( void ) {
		return devicesList;
	}
};

TEST( DevicesManagerTestSuite, getDeviceByIdentifierTest ) {
	DevicesManagerAccessor devicesManager;

	DeviceIdentifier devId{ DeviceType::IntelXeon, "0" };
	ASSERT_THROW( devicesManager.getDeviceByIdentifier( devId ), DeviceNotFoundException );

	devicesManager.getDevicesListRef().push_back( std::make_shared<devices::IntelXeonDevice>( devId.id ) );

	auto& dev = devicesManager.getDeviceByIdentifier( devId );
	ASSERT_EQ( devId.type, dev.getInfo().identifier.type );
	ASSERT_EQ( devId.id, dev.getInfo().identifier.id );
}
