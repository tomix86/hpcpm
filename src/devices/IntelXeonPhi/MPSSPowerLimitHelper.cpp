#include <cstring>
#include <sstream>
#include "MPSSPowerLimitHelper.hpp"
#include "utility/ConfigLoader.hpp"
#include "utility/Logging.hpp"

namespace devices {

const unsigned MPSSPowerLimitHelper::TIME_WINDOW_0;
const unsigned MPSSPowerLimitHelper::TIME_WINDOW_1;
const unsigned MPSSPowerLimitHelper::DIFF_BETWEEN_PL0_AND_PL1;
const unsigned MPSSPowerLimitHelper::POWER_LIMIT_ADJUSTMENT_STEP;
const unsigned MPSSPowerLimitHelper::RUNNING_AVERAGE_DISCREPANCY_TOLERANCE;
MPSSProxy* MPSSPowerLimitHelper::proxy;

MPSSPowerLimitHelper::MPSSPowerLimitHelper( MicDevicePtr dev ) :
		deviceHandle{ dev },
		powerReadings( utility::ConfigLoader::getUnsignedParam( "MPSS_history_size" ), 0 ),
		pos{ 0 },
		watcher{ &MPSSPowerLimitHelper::watcherFunction, this },
		running{ true },
		timeBetweenReads{ utility::ConfigLoader::getUnsignedParam( "MPSS_interval" ) },
		gracePeriodDuration{ utility::ConfigLoader::getUnsignedParam( "MPSS_grace_period_duration" ) },
		gracePeriodCounter{ 0 },
		powerLimit{ 0 },
		expectedPowerLimit0{ 0 },
		expectedPowerLimit1{ 0 } {
}

MPSSPowerLimitHelper::~MPSSPowerLimitHelper( void ) {
	running = false;
	watcher.join();
}

void MPSSPowerLimitHelper::init( MPSSProxy& proxy ) {
	MPSSPowerLimitHelper::proxy = &proxy;
}

unsigned MPSSPowerLimitHelper::getPowerLimitLowerConstraint( void ) const {
	// From documentation: "If PL0 or PL1 is less than 50 then an E_MIC_RAS_ERROR will be thrown
	return 50 + DIFF_BETWEEN_PL0_AND_PL1;
}

unsigned MPSSPowerLimitHelper::getPowerLimitUpperConstraint( void ) const {
	// Although documentation says that max limit that can be set is 400 I think that using phys limit as max value is better choice
	mic_power_limit* powerLimit;
	proxy->mic_get_power_limit( deviceHandle.get(), &powerLimit );
	std::shared_ptr<mic_power_limit> powerLimitRAIIGuard{ powerLimit, [this]( mic_power_limit* ptr ) { proxy->mic_free_power_limit( ptr ); } };

	std::uint32_t limit;
	MPSS_ERROR_CHECK( proxy->mic_get_power_phys_limit( powerLimit, &limit ) );
	LOG( INFO ) << "Phys limit: " << std::to_string( limit ); //TOOD: usunac ten print

	return limit;
}

unsigned MPSSPowerLimitHelper::getPowerLimit( void ) const {
	return powerLimit;
}

void MPSSPowerLimitHelper::setPowerLimit( unsigned watts ) {
	Lock lock{ powerLimitMutex };
	setPL1andPL0( watts );

	powerLimit = watts;
	gracePeriodCounter = gracePeriodDuration;
}

unsigned MPSSPowerLimitHelper::getCurrentPowerUsage( void ) const {
	return getPowerRunningAverage();
}

void MPSSPowerLimitHelper::watcherFunction( void ) {
	try {
		while( running ) {
			try {
				Lock lock{ powerLimitMutex };
				if( gracePeriodCounter == 0 ) {
					addSample( _getCurrentPowerUsage() );
					checkPowerRunningAverage();
				}
				else {
					gracePeriodCounter--;
				}
			}
			catch ( const MPSSError& err ) {
				LOG( ERROR ) << err.info();
			}

			std::this_thread::sleep_for( timeBetweenReads );
		}
	}
	catch ( const std::exception& ex ) {
		LOG( FATAL ) << "Unexpected exception thrown in MPSSPowerLimitHelper::watcherFunction, the thread will terminate: " << ex.what();
	}
	catch ( ... ) {
		LOG( FATAL ) << "Unknown exception thrown in MPSSPowerLimitHelper::watcherFunction, the thread will terminate";
	}
}

unsigned MPSSPowerLimitHelper::_getCurrentPowerUsage( void ) const {
	mic_power_util_info* powerInfo;
	MPSS_ERROR_CHECK( proxy->mic_get_power_utilization_info( deviceHandle.get(), &powerInfo ) );
	std::shared_ptr<mic_power_util_info> powerInfoRAIIGuard{ powerInfo, [this]( mic_power_util_info* ptr ) { proxy->mic_free_power_utilization_info( ptr ); } };

	std::uint32_t power;
	MPSS_ERROR_CHECK( proxy->mic_get_inst_power_readings( powerInfo, &power ) );

	return power;
}

void MPSSPowerLimitHelper::checkPowerRunningAverage( void ) {
	auto powerUsageRunningAverage = getPowerRunningAverage();

	if ( powerUsageRunningAverage - RUNNING_AVERAGE_DISCREPANCY_TOLERANCE > powerLimit ) {
		if ( powerLimit - POWER_LIMIT_ADJUSTMENT_STEP >= getPowerLimitLowerConstraint() ) {
			setPL1andPL0( powerLimit - POWER_LIMIT_ADJUSTMENT_STEP );
			gracePeriodCounter = gracePeriodDuration; // Enable grace period so the running average can stabilize before next check
		}
	}
	else if ( powerUsageRunningAverage + RUNNING_AVERAGE_DISCREPANCY_TOLERANCE < powerLimit ) {
		if ( powerLimit + POWER_LIMIT_ADJUSTMENT_STEP <= getPowerLimitUpperConstraint() ) {
			setPL1andPL0( powerLimit + POWER_LIMIT_ADJUSTMENT_STEP );
			gracePeriodCounter = gracePeriodDuration; // Enable grace period so the running average can stabilize before next check
		}
	}
}

unsigned MPSSPowerLimitHelper::getPowerRunningAverage( void ) const {
	return std::accumulate( powerReadings.begin(), powerReadings.end(), 0 ) / powerReadings.size();
}

void MPSSPowerLimitHelper::addSample( unsigned powerReading ) {
		pos = ( pos + 1 ) % powerReadings.size();
		const_cast<std::vector<unsigned>&>( powerReadings )[ pos ] = powerReading;
}

void MPSSPowerLimitHelper::checkForExternalModifications( void ) const {
	mic_power_limit* powerLimit;
	proxy->mic_get_power_limit( deviceHandle.get(), &powerLimit );
	std::shared_ptr<mic_power_limit> powerLimitRAIIGuard{ powerLimit, [this]( mic_power_limit* ptr ) { proxy->mic_free_power_limit( ptr ); } };

	std::uint32_t TW0;
	MPSS_ERROR_CHECK( proxy->mic_get_time_window0( powerLimit, &TW0 ) );
	if ( TW0 != TIME_WINDOW_0 ) {
		LOG( INFO ) << "TW0 was changed by external process, setting it back to desired value: " << TIME_WINDOW_0;
	}

	std::uint32_t TW1;
	MPSS_ERROR_CHECK( proxy->mic_get_time_window1( powerLimit, &TW1 ) );
	if ( TW1 != TIME_WINDOW_1 ) {
		LOG( INFO ) << "TW1 was changed by external process, setting it back to desired value: " << TIME_WINDOW_1;
	}

	std::uint32_t PL1;
	MPSS_ERROR_CHECK( proxy->mic_get_power_lmrk( powerLimit, &PL1 ) );
	if ( PL1 != expectedPowerLimit1 ) {
		LOG( INFO ) << "PL1 was changed by external process, setting it back to desired value: " << expectedPowerLimit1;
	}

	std::uint32_t PL0;
	MPSS_ERROR_CHECK( proxy->mic_get_power_hmrk( powerLimit, &PL0 ) );
	if ( PL0 != expectedPowerLimit0 ) {
		LOG( INFO ) << "PL0 was changed by external process, setting it back to desired value: " << expectedPowerLimit0;
	}

	if ( TW0 != TIME_WINDOW_0 || PL0 != expectedPowerLimit0 ) {
		MPSS_ERROR_CHECK( proxy->mic_set_power_limit0( deviceHandle.get(), expectedPowerLimit0, TIME_WINDOW_0 ) );
	}
	if ( TW1 != TIME_WINDOW_1 || PL1 != expectedPowerLimit1 ) {
		MPSS_ERROR_CHECK( proxy->mic_set_power_limit1( deviceHandle.get(), expectedPowerLimit1, TIME_WINDOW_1 ) );
	}

	LOG( INFO ) << "TW0: " << std::to_string( TW0 ); //TOOD: remove prints
	LOG( INFO ) << "TW1: " << std::to_string( TW1 );
	LOG( INFO ) << "PL1: " << std::to_string( PL1 );
	LOG( INFO ) << "PL0: " << std::to_string( PL0 );
}

void MPSSPowerLimitHelper::setPL1andPL0( unsigned watts ) {
	expectedPowerLimit1 = watts;
	MPSS_ERROR_CHECK( proxy->mic_set_power_limit1( deviceHandle.get(), expectedPowerLimit1, TIME_WINDOW_1 ) );

	expectedPowerLimit0 = expectedPowerLimit1 - DIFF_BETWEEN_PL0_AND_PL1;
	MPSS_ERROR_CHECK( proxy->mic_set_power_limit0( deviceHandle.get(), expectedPowerLimit0, TIME_WINDOW_0 ) );
}

void MPSSPowerLimitHelper::checkMPSSErrors( const char* source, int status ) {
	if ( status != E_MIC_SUCCESS ) {
		std::ostringstream err;
		err << proxy->mic_get_error_string() << ": " << strerror( errno );
		throw MPSSError( source, err.str());
	}
}

} // namespace devices
