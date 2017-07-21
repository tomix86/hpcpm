#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "devices/NvidiaTesla/NvidiaTeslaDevice.hpp"
#include "../utility/ConfigLoaderAccessor.hpp"

using namespace devices;

class MockTeslaCommunicationProvider {
public:
	MockTeslaCommunicationProvider( DeviceIdentifier::idType id  ) {
		(void)id;
	}

	MOCK_CONST_METHOD0( getCurrentPowerLimit, unsigned( void ) );

	MOCK_CONST_METHOD0( getPowerLimitConstraints, std::pair<unsigned, unsigned>( void ) );

	MOCK_METHOD1( setPowerLimit, void( unsigned ) );

	MOCK_CONST_METHOD0( getCurrentPowerUsage, unsigned( void ) );
};

class NvidiaTeslaDeviceAccessor : public NvidiaTeslaDevice<MockTeslaCommunicationProvider> {
public:
	NvidiaTeslaDeviceAccessor( void ) :
	NvidiaTeslaDevice<MockTeslaCommunicationProvider>{ "0", { {"PowerManagementCapable", "false"} } } {
	}

	using NvidiaTeslaDevice<MockTeslaCommunicationProvider>::communicationProvider;
};


class NvidiaTeslaTestSuite : public ::testing::Test {
protected:
	static void SetUpTestCase() {
		ConfigLoaderAccessor::setInitialized( false );
		ConfigLoaderAccessor::clearConfig();

		auto& config = ConfigLoaderAccessor::getConfig();
		config[ "NVML_history_size" ] = "100";
		config[ "NVML_interval" ] = "10";

		ConfigLoaderAccessor::setInitialized( true );
	}

	static void TearDownTestCase() {
		ConfigLoaderAccessor::setInitialized( false );
		ConfigLoaderAccessor::clearConfig();
	}
};

TEST_F( NvidiaTeslaTestSuite, setPowerLimitArgumentCorectnessTest ) {
	NvidiaTeslaDeviceAccessor device;

	EXPECT_CALL( device.communicationProvider, getPowerLimitConstraints() )
		.Times( 5 )
		.WillRepeatedly( ::testing::Return( std::make_pair( 100000u, 150000u ) ) );

	EXPECT_CALL( device.communicationProvider, setPowerLimit( ::testing::_ ) )
		.Times( 3 );

	ASSERT_THROW( device.setPowerLimit( devices::Power{ 99 } ), ArgumentOutOfBounds );
	ASSERT_THROW( device.setPowerLimit( devices::Power{ 151 } ), ArgumentOutOfBounds );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Power{ 100 } ) );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Power{ 125 } ) );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Power{ 150 } ) );
}

TEST_F( NvidiaTeslaTestSuite, setPowerLimitPercentageArgumentCorectnessTest ) {
	NvidiaTeslaDeviceAccessor device;

	EXPECT_CALL( device.communicationProvider, getPowerLimitConstraints() )
		.WillRepeatedly( ::testing::Return( std::make_pair( 0u, 0u ) ) );

	EXPECT_CALL( device.communicationProvider, setPowerLimit( ::testing::_ ) )
		.Times( 3 );

	ASSERT_THROW( device.setPowerLimit( devices::Percentage{ 1.1f } ), ArgumentOutOfBounds );
	ASSERT_THROW( device.setPowerLimit( devices::Percentage{ -.1f } ), ArgumentOutOfBounds );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Percentage{ 0.f } ) );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Percentage{ 1.f } ) );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Percentage{ .5f } ) );
}

TEST_F( NvidiaTeslaTestSuite, setPowerLimitPercentageTest ) {
	NvidiaTeslaDeviceAccessor device;

	EXPECT_CALL( device.communicationProvider, getPowerLimitConstraints() )
		.Times( 3 )
		.WillRepeatedly( ::testing::Return( std::make_pair( 100000u, 200000u ) ) );

	EXPECT_CALL( device.communicationProvider, setPowerLimit( 100000 ) );
	EXPECT_CALL( device.communicationProvider, setPowerLimit( 150000 ) );
	EXPECT_CALL( device.communicationProvider, setPowerLimit( 200000 ) );

	ASSERT_NO_THROW( device.setPowerLimit( devices::Percentage{ 0.f } ) );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Percentage{ 0.5f } ) );
	ASSERT_NO_THROW( device.setPowerLimit( devices::Percentage{ 1.f } ) );
}

TEST_F( NvidiaTeslaTestSuite, getCurrentPowerLimitPercentageTest ) {
	NvidiaTeslaDeviceAccessor device;

	EXPECT_CALL( device.communicationProvider, getPowerLimitConstraints() )
		.Times( 3 )
		.WillRepeatedly( ::testing::Return( std::make_pair( 100000u, 200000u ) ) );

	EXPECT_CALL( device.communicationProvider, getCurrentPowerLimit() )
		.WillOnce( ::testing::Return( 100000 ) );

	ASSERT_EQ( 0.f, device.getCurrentPowerLimitPercentage() );

	EXPECT_CALL( device.communicationProvider, getCurrentPowerLimit() )
		.WillOnce( ::testing::Return( 150000 ) );

	ASSERT_EQ( .5f, device.getCurrentPowerLimitPercentage() );

	EXPECT_CALL( device.communicationProvider, getCurrentPowerLimit() )
		.WillOnce( ::testing::Return( 200000 ) );

	ASSERT_EQ( 1.f, device.getCurrentPowerLimitPercentage() );
}
