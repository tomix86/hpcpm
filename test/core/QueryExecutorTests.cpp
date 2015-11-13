#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/QueryExecutor.hpp"
#include "core/queries/QueryFactory.hpp"
#include "devices/IntelXeon/IntelXeonDevice.hpp"
#include "devices/IntelXeon/MockNMPRKCommunicationProvider.hpp"
#include "devices/IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "devices/IntelXeonPhi/MockMPSSCommunicationProvider.hpp"
#include "devices/NvidiaTesla/MockNVMLCommunicationProvider.hpp"
#include "devices/NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "utility/make_unique.hpp"

using namespace core;

class MockDevicesManager : public devices::DevicesManager {
public:
	MockDevicesManager( void ) :
	devices::DevicesManager{ { false, false, false } } {
	}

	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
	MOCK_METHOD1( getDeviceByIdentifier, devices::Device& ( devices::DeviceIdentifier ) );
};

class MockDevice : public devices::Device {
public:
	MockDevice( void ) : devices::Device{ devices::DeviceInformation{ {}, {} } } {}
	MOCK_CONST_METHOD0( getInfo, const devices::DeviceInformation& ( void ) );
	MOCK_METHOD1( setPowerLimit, void ( devices::Power ) );
	MOCK_METHOD1( setPowerLimit, void ( devices::Percentage ) );
	MOCK_CONST_METHOD0( getCurrentPowerLimit, devices::Power ( void ) );
	MOCK_CONST_METHOD0( getCurrentPowerLimitPercentage, devices::Percentage ( void ) );
	MOCK_CONST_METHOD0( getPowerLimitConstraints, devices::PowerLimitConstraints ( void ) );
};

class MockIntelXeonDevice: public devices::IntelXeonDevice<devices::MockNMPRKCommunicationProvider> {
public:
	MockIntelXeonDevice( devices::DeviceIdentifier::idType id ) :
	IntelXeonDevice<devices::MockNMPRKCommunicationProvider>{ id, {} } {
	}
};

//This class is needed for ResponseTest and MultipleQueries test
//those tests had been crashing due to an issue with gmock (EXPECT_CALL), which I was unable to track down
class MockDevicesManager2 : public devices::DevicesManager {
public:
	MockDevicesManager2( void ) :
	devices::DevicesManager{ { false, false, false } } {
	}

	devices::Device& getDeviceByIdentifier( devices::DeviceIdentifier deviceIdentifier ) {
		(void)deviceIdentifier;
		return deviceMock;
	}

	MockDevice deviceMock;
};


class QueryExecutorTestSuite : public ::testing::Test {
public:
	QueryExecutorTestSuite( void ) :
	devicesManager{ std::make_shared<MockDevicesManager>() },
	devicesManager2{ std::make_shared<MockDevicesManager2>() } {
	}

protected:
	std::shared_ptr<MockDevicesManager> devicesManager;
	std::shared_ptr<MockDevicesManager2> devicesManager2;
};

TEST_F( QueryExecutorTestSuite, GetNodeInformation ) {
	std::vector<devices::Device::Ptr> devs;
	devs.push_back( std::make_shared<MockIntelXeonDevice>( "123" ) );
	devs.push_back( std::make_shared<MockIntelXeonDevice>( "456" ) );

	EXPECT_CALL( *devicesManager, getDevicesList() )
		.WillOnce( ::testing::ReturnRef( devs ) );

	auto query = core::QueryFactory::createQuery( QueryType::GetNodeInformation );
	GetNodeInformationQuery::Result::Ptr result;
	ASSERT_NO_THROW( result = query->execute( devicesManager ) );
	ASSERT_TRUE( result->serialize().find( "\"Type\":\"IntelXeon\",\"id\":\"123\"" ) != std::string::npos );
	ASSERT_TRUE( result->serialize().find( "\"Type\":\"IntelXeon\",\"id\":\"456\"" ) != std::string::npos );
}

TEST_F( QueryExecutorTestSuite, GetPowerLimit ) {
	EXPECT_CALL( devicesManager2->deviceMock, getCurrentPowerLimit() )
		.WillOnce( ::testing::Return( devices::Power{ 1000 } ) );

	devices::DeviceIdentifier deviceIdentifier{ devices::DeviceType::IntelXeon, "0" };
	devices::DeviceInformation info;
	info.identifier = deviceIdentifier;
	EXPECT_CALL( devicesManager2->deviceMock, getInfo() )
		.WillOnce( ::testing::ReturnRef( info ) );

	auto query = core::QueryFactory::createQuery( QueryType::GetPowerLimit );
	query->setDeviceIdentifier( deviceIdentifier );
	ASSERT_STREQ( "{\"PowerLimit\":1000,\"Type\":\"IntelXeon\",\"id\":\"0\"}", query->execute( devicesManager2 )->serialize().c_str() );
}
