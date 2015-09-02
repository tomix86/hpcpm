#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/QueryExecutor.hpp"
#include "core/query_handlers/GetNodeInformationQueryHandler.hpp"
#include "core/query_handlers/GetPowerLimitQueryHandler.hpp"
#include "devices/IntelXeon/IntelXeonDevice.hpp"
#include "devices/IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "devices/NvidiaTesla/MockNVMLCommunicationProvider.hpp"
#include "devices/NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "utility/make_unique.hpp"

using namespace core;

class MockDevicesManager : public devices::DevicesManager {
public:
	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
	MOCK_METHOD1( getDeviceByIdentifier, devices::Device& ( devices::DeviceIdentifier ) );
};

class MockQueryExecutor : public QueryExecutor {
public:
	MockQueryExecutor( void ) :
	QueryExecutor{ nullptr } {
	}
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

class QueryExecutorTestSuite : public ::testing::Test {
public:
	QueryExecutorTestSuite( void ) :
		devicesManager{ std::make_shared<MockDevicesManager>() } {
	}

protected:
	std::shared_ptr<MockDevicesManager> devicesManager;
	MockDevice device;
};

TEST_F( QueryExecutorTestSuite, DISABLED_InvalidQueryTypeExecutionTest ) {
	MockQueryExecutor queryExecutor;
	std::ostringstream sink;
	auto oldBuf = std::cout.rdbuf( sink.rdbuf() );

	core::QueryHandler::NullResult null;
	Query q( Query::Type::None );
	ASSERT_STREQ( null.serialize().c_str(), queryExecutor.execute( q )->serialize().c_str() );
	ASSERT_TRUE( sink.str().find( "ERROR: Invalid query type" ) != std::string::npos );

	std::cout.rdbuf( oldBuf );
}

TEST_F( QueryExecutorTestSuite, GetNodeInformation ) {
	GetNodeInformationQueryHandler handler{ devicesManager };

	std::vector<devices::Device::Ptr> devs;
	devs.push_back( std::make_shared<devices::IntelXeonDevice>( "123" ) );
	devs.push_back( std::make_shared<devices::IntelXeonPhiDevice>( "456" ) );
	devs.push_back( std::make_shared<devices::NvidiaTeslaDevice<devices::MockNVMLCommunicationProvider>>( "789" ) );

	EXPECT_CALL( *devicesManager, getDevicesList() )
		.WillOnce( ::testing::ReturnRef( devs ) );

	GetNodeInformationQueryHandler::Result::Ptr result;
	ASSERT_NO_THROW( result = handler.handle( Query{ Query::Type::GetNodeInformation } ) );
	ASSERT_TRUE( result->serialize().find( "[{\"Type\":\"IntelXeon\",\"id\":\"123\"},{\"Type\":\"IntelXeonPhi\",\"id\":\"456\"},{\"Type\":\"NvidiaTesla\",\"id\":\"789\"}]" ) != std::string::npos );
}

TEST_F( QueryExecutorTestSuite, DISABLED_GetPowerLimit ) {
	GetPowerLimitQueryHandler handler{ devicesManager };

	devices::DeviceIdentifier deviceIdentifier{ devices::DeviceType::IntelXeon, "0" };
	EXPECT_CALL( *devicesManager, getDeviceByIdentifier( deviceIdentifier ) )
		.WillOnce( ::testing::ReturnRef( device ) );

	EXPECT_CALL( device, getCurrentPowerLimit() )
		.WillOnce( ::testing::Return( devices::Power{ 1000 } ) );

	devices::DeviceInformation info;
	info.identifier = deviceIdentifier;
	EXPECT_CALL( device, getInfo() )
		.WillOnce( ::testing::ReturnRef( info ) );

	Query query{ Query::Type::GetPowerLimit };
	query.setDeviceIdentifier( deviceIdentifier );
	ASSERT_STREQ( "{\"PowerLimit\":1000,\"Type\":\"IntelXeon\",\"id\":\"0\"}", handler.handle( query )->serialize().c_str() );
}
