#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/QueryExecutor.hpp"
#include "devices/IntelXeon/IntelXeonDevice.hpp"
#include "devices/IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "devices/NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "utility/make_unique.hpp"

class MockDevicesManager : public devices::DevicesManager {
public:
	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
};

using core::Query;
struct QueryExecutorAccessor : public core::QueryExecutor {
	QueryExecutorAccessor( void ) :
	core::QueryExecutor{ utility::make_unique<MockDevicesManager>() } {
	}

	Query::Result handleGetNodeInformation( void ) {
		return core::QueryExecutor::handleGetNodeInformation();
	}

	MockDevicesManager& getDevicesManager( void ) {
		return dynamic_cast<MockDevicesManager&>( *devicesManager );
	}
};

TEST( QueryExecutorTestSuite, GetNodeInformation ) {
	QueryExecutorAccessor queryExecutor;

	std::vector<devices::Device::Ptr> devs;
	devs.push_back( std::make_shared<devices::IntelXeonDevice>( "123" ) );
	devs.push_back( std::make_shared<devices::IntelXeonPhiDevice>( "456" ) );
	devs.push_back( std::make_shared<devices::NvidiaTeslaDevice>( "789" ) );

	EXPECT_CALL( queryExecutor.getDevicesManager(), getDevicesList() )
		.WillOnce( ::testing::ReturnRef( devs ) );

	Query::Result result;
	ASSERT_NO_THROW( result = queryExecutor.handleGetNodeInformation() );
	ASSERT_STREQ( "[ { \"IntelXeon\": \"123\" }, { \"IntelXeonPhi\": \"456\" }, { \"NvidiaTesla\": \"789\" } ]", result.c_str() );
}
