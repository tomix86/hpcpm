#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/QueryExecutor.hpp"
#include "core/query_handlers/GetNodeInformationHandler.hpp"
#include "devices/IntelXeon/IntelXeonDevice.hpp"
#include "devices/IntelXeonPhi/IntelXeonPhiDevice.hpp"
#include "devices/NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "utility/make_unique.hpp"

using namespace core;

class MockDevicesManager : public devices::DevicesManager {
public:
	MOCK_CONST_METHOD0( getDevicesList, const std::vector<devices::Device::Ptr>& ( void ) );
};

struct MockQueryExecutor : public QueryExecutor {
	MockQueryExecutor( void ) :
	QueryExecutor{ nullptr } {
	}
};

TEST( QueryExecutorTestSuite, InvalidQueryTypeExecutionTest ) {
	MockQueryExecutor queryExecutor;
	std::ostringstream sink;
	auto oldBuf = std::cout.rdbuf( sink.rdbuf() );

	core::QueryHandler::NullResult null;
	Query q( Query::Type::None );
	ASSERT_STREQ( null.serialize().c_str(), queryExecutor.execute( q )->serialize().c_str() );
	ASSERT_TRUE( sink.str().find( "ERROR: Invalid query type" ) != std::string::npos );

	std::cout.rdbuf( oldBuf );
}

TEST( QueryExecutorTestSuite, GetNodeInformation ) {
	auto devMgr = std::make_shared<MockDevicesManager>();
	GetNodeInformationQueryHandler handler{ devMgr };

	std::vector<devices::Device::Ptr> devs;
	devs.push_back( std::make_shared<devices::IntelXeonDevice>( "123" ) );
	devs.push_back( std::make_shared<devices::IntelXeonPhiDevice>( "456" ) );
	devs.push_back( std::make_shared<devices::NvidiaTeslaDevice>( "789" ) );

	EXPECT_CALL( *devMgr, getDevicesList() )
		.WillOnce( ::testing::ReturnRef( devs ) );

	GetNodeInformationQueryHandler::Result::Ptr result;
	ASSERT_NO_THROW( result = handler.handle( Query{ Query::Type::GetNodeInformation } ) );
	ASSERT_TRUE( result->serialize().find( "[{\"Type\":\"IntelXeon\",\"id\":\"123\"},{\"Type\":\"IntelXeonPhi\",\"id\":\"456\"},{\"Type\":\"NvidiaTesla\",\"id\":\"789\"}]" ) != std::string::npos );
}
