#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/QueryExecutor.hpp"
#include "core/query_handlers/GetNodeInformationQueryHandler.hpp"
#include "core/query_handlers/GetPowerLimitQueryHandler.hpp"
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
	devices::DevicesManager{ false, false, false } {
	}

	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
	MOCK_METHOD1( getDeviceByIdentifier, devices::Device& ( devices::DeviceIdentifier ) );
};

class MockDevice : public devices::Device {
public:
	MOCK_CONST_METHOD0( getInfo, const devices::DeviceInformation& ( void ) );
	MOCK_METHOD1( setPowerLimit, void ( devices::Power ) );
	MOCK_METHOD1( setPowerLimit, void ( devices::Percentage ) );
	MOCK_CONST_METHOD0( getCurrentPowerLimit, devices::Power ( void ) );
	MOCK_CONST_METHOD0( getCurrentPowerLimitPercentage, devices::Percentage ( void ) );
	MOCK_CONST_METHOD0( getPowerLimitConstraints, devices::PowerLimitConstraints ( void ) );
};

//This class is needed for ResponseTest and MultipleQueries test
//those tests had been crashing due to an issue with gmock (EXPECT_CALL), which I was unable to track down
class MockDevicesManager2 : public devices::DevicesManager {
public:
	MockDevicesManager2( void ) :
	devices::DevicesManager{ false, false, false } {
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

TEST_F( QueryExecutorTestSuite, InvalidQueryTypeExecutionTest ) {
	std::ostringstream sink;
	auto oldBuf = std::cout.rdbuf( sink.rdbuf() );

	QueryExecutor queryExecutor{ nullptr };
	core::QueryHandler::NullResult null;
	Query q( Query::Type::None );
	ASSERT_STREQ( null.serialize().c_str(), queryExecutor.execute( q )->serialize().c_str() );
	ASSERT_TRUE( sink.str().find( "ERROR: Invalid query type" ) != std::string::npos );

	std::cout.rdbuf( oldBuf );
}

TEST_F( QueryExecutorTestSuite, GetNodeInformation ) {
	GetNodeInformationQueryHandler handler{ devicesManager };

	std::vector<devices::Device::Ptr> devs;
	devs.push_back( std::make_shared<devices::IntelXeonDevice<devices::MockMPSSCommunicationProvider>>( "123" ) );
	devs.push_back( std::make_shared<devices::IntelXeonPhiDevice<devices::MockNMPRKCommunicationProvider>>( "456" ) );
	devs.push_back( std::make_shared<devices::NvidiaTeslaDevice<devices::MockNVMLCommunicationProvider>>( "789" ) );

	EXPECT_CALL( *devicesManager, getDevicesList() )
		.WillOnce( ::testing::ReturnRef( devs ) );

	GetNodeInformationQueryHandler::Result::Ptr result;
	ASSERT_NO_THROW( result = handler.handle( Query{ Query::Type::GetNodeInformation } ) );
	ASSERT_TRUE( result->serialize().find( "\"Type\":\"IntelXeon\",\"id\":\"123\"" ) != std::string::npos );
	ASSERT_TRUE( result->serialize().find( "\"Type\":\"IntelXeonPhi\",\"id\":\"456\"" ) != std::string::npos );
	ASSERT_TRUE( result->serialize().find( "\"Type\":\"NvidiaTesla\",\"id\":\"789\"" ) != std::string::npos );
}

TEST_F( QueryExecutorTestSuite, GetPowerLimit ) {
	GetPowerLimitQueryHandler handler{ devicesManager2 };

	EXPECT_CALL( devicesManager2->deviceMock, getCurrentPowerLimit() )
		.WillOnce( ::testing::Return( devices::Power{ 1000 } ) );

	devices::DeviceIdentifier deviceIdentifier{ devices::DeviceType::IntelXeon, "0" };
	devices::DeviceInformation info;
	info.identifier = deviceIdentifier;
	EXPECT_CALL( devicesManager2->deviceMock, getInfo() )
		.WillOnce( ::testing::ReturnRef( info ) );

	Query query{ Query::Type::GetPowerLimit };
	query.setDeviceIdentifier( deviceIdentifier );
	ASSERT_STREQ( "{\"PowerLimit\":1000,\"Type\":\"IntelXeon\",\"id\":\"0\"}", handler.handle( query )->serialize().c_str() );
}
