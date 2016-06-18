#pragma once
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include "MPSSProxy.hpp"
#include "utility/CircuralBuffer.hpp"
#include "utility/Exceptions.hpp"
#include "utility/Functional.hpp"

/*
The Intel® Xeon PhiTM coprocessor supports 2 power threshold levels, PL0 and PL1,
which determine coprocessor power throttling points. These are not to be confused
with setting coprocessor power limits, that is, they do not change the absolute TDP of
the product.
PL1 is defined as the first power threshold. When the coprocessor detects that the
power consumption stays above PL1 for a specified time period, the coprocessor will
begin power throttling. By default, the card's PL1 power threshold is set to 105% of the
TDP, and the time duration allowed before throttling starts is 300 ms. When the SMC
detects that these conditions have been met it will assert power throttling, causing the
frequency to drop by about 100 MHz. Throttling will stop once the power consumption
drops 15 W or 20 W (depending on card TDP) below PL1. The difference in the
throttling assertion and deassertion thresholds will help prevent the coprocessor from
continually cycling between throttling and running normally. For cards with a TDP of
250 W or less, the deassertion point is 15 W lower than PL1. For 300 W cards the
deassertion point is 20 W below PL1.
PL0 is normally set to a higher power threshold than PL1. By default, it is set to 125%
of TDP, and the time duration allowed at this power level is 50 ms. If these conditions
are met, the SMC will use the thermal throttling mechanism to force the coprocessor to
the lowest operating frequency which is around 600 MHz. The power reduction from
PL0 is expected to be much greater than PL1. The thermal throttling state will continue
until the total coprocessor power has dropped 40 W below PL0. When PL0 is exceeded,
the initial change in power consumption is a result of the lower operating frequency.
The coprocessor will also reduce the CPU core voltage to a value that is appropriate for
the lower frequency and this will provide additional power savings. The voltage
reduction takes place 3-400 msec after PL0 throttling is asserted.
*/

namespace devices {

typedef std::shared_ptr<mic_device> MicDevicePtr;

DEFINE_RUNTIME_ERROR_DERIVATIVE( MPSSError );

#define MPSS_ERROR_CHECK( statusCode ) checkMPSSErrors( "MPSS error at line:" UTILITY_STRINGIFY( __LINE__ ), statusCode )

class MPSSPowerLimitHelper {
public:
	MPSSPowerLimitHelper( MicDevicePtr dev );
	~MPSSPowerLimitHelper( void );

	static void init( MPSSProxy& proxy );

	unsigned getPowerLimitLowerConstraint( void ) const;
	unsigned getPowerLimitUpperConstraint( void ) const;
	unsigned getPowerLimit( void ) const;
	void setPowerLimit( unsigned watts );
	unsigned getCurrentPowerUsage( void ) const;

private:
	// Documentation states:
	// It is recommended that power limit 1 be greater than power limit 0, and time window 1 be less than time window 0 as well as a multiple of 50.
	// If time window 0/1 is not a multiple of 50, it will be rounded down to the closest 50 decrement.
	// Also, if time window 0/1 is not greater than 50 and less than 1000 or power limit 0/1 is less than 50 or greater than 400 then an E_MIC_RAS_ERROR will be thrown.
	static const unsigned TIME_WINDOW_0 = 300; //TODO: should we use constant values or fetch them from config file?
	static const unsigned TIME_WINDOW_1 = 50;
	static const unsigned DIFF_BETWEEN_PL0_AND_PL1 = 30; // For our purpose, we assume that PL0 must be greater than PL1
	static const unsigned POWER_LIMIT_ADJUSTMENT_STEP = 5;
	static const unsigned RUNNING_AVERAGE_DISCREPANCY_TOLERANCE = 4;

	using Lock = std::lock_guard<std::mutex>;

	static MPSSProxy* proxy;
	MicDevicePtr deviceHandle;

	utility::CircuralBuffer powerReadings;

	std::atomic<bool> running;
	std::thread watcher;
	const std::chrono::milliseconds timeBetweenReads;
	const unsigned gracePeriodDuration; //in cycles
	int gracePeriodCounter;

	std::mutex powerLimitMutex;

	// From documentation: "If PL0 or PL1 is less than 50 or greater than 400 then an E_MIC_RAS_ERROR will be thrown (when setting the limits)
	unsigned powerLimit;
	unsigned expectedPowerLimit0;
	unsigned expectedPowerLimit1;

	void watcherFunction( void );
	unsigned _getCurrentPowerUsage( void ) const;
	void checkPowerRunningAverage( void );

	// Check if TW0, TW1, PL0 or PL1 was changed by external process
	void checkForExternalModifications( void ) const;

	void setPL1andPL0( unsigned watts );

	static void checkMPSSErrors( const char* source, int status ); //TODO: remove duplication
};

} // namespace devices
