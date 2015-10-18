#include <gtest/gtest.h>
#include "devices/DevicesManager.hpp"
#include "devices/IntelXeon/IntelXeonDevice.hpp"
#include "devices/IntelXeon/MockNMPRKCommunicationProvider.hpp"

using namespace devices;

class DevicesManagerAccessor : public devices::DevicesManager {
public:
	DevicesManagerAccessor( void ) :
	devices::DevicesManager{ { false, false, false } } {
	}

	using devices::DevicesManager::devicesList;
};

class MockIntelXeonDevice: public IntelXeonDevice<MockNMPRKCommunicationProvider> {
public:
	MockIntelXeonDevice( DeviceIdentifier::idType id ) :
	IntelXeonDevice<MockNMPRKCommunicationProvider>{ id, {} } {
	}
};

TEST( DevicesManagerTestSuite, getDeviceByIdentifierTest ) {
	DevicesManagerAccessor devicesManager;

	DeviceIdentifier devId{ DeviceType::IntelXeon, "0" };
	ASSERT_THROW( devicesManager.getDeviceByIdentifier( devId ), DeviceNotFoundException );

	devicesManager.devicesList.push_back( std::make_shared<MockIntelXeonDevice>( devId.id ) );

	auto& dev = devicesManager.getDeviceByIdentifier( devId );
	ASSERT_EQ( devId.type, dev.getInfo().identifier.type );
	ASSERT_EQ( devId.id, dev.getInfo().identifier.id );
}

TEST( DeviceIdentifierTestSuite, typeName ) {
	DeviceIdentifier id;
	id.type = DeviceType::None;
	ASSERT_STREQ( "None", id.typeName().c_str() );
	id.type = DeviceType::IntelXeon;
	ASSERT_STREQ( "IntelXeon", id.typeName().c_str() );
	id.type = DeviceType::IntelXeonPhi;
	ASSERT_STREQ( "IntelXeonPhi", id.typeName().c_str() );
	id.type = DeviceType::NvidiaTesla;
	ASSERT_STREQ( "NvidiaTesla", id.typeName().c_str() );
}

TEST( DeviceIdentifierTestSuite, typeFromString ) {
	ASSERT_EQ( DeviceType::None, DeviceIdentifier::typeFromString( "None" ) );
	ASSERT_EQ( DeviceType::IntelXeon, DeviceIdentifier::typeFromString( "IntelXeon" ) );
	ASSERT_EQ( DeviceType::IntelXeonPhi, DeviceIdentifier::typeFromString( "IntelXeonPhi" ) );
	ASSERT_EQ( DeviceType::NvidiaTesla, DeviceIdentifier::typeFromString( "NvidiaTesla" ) );
	ASSERT_THROW( DeviceIdentifier::typeFromString( "IntelCore2Duo" ), utility::RuntimeError );
}
