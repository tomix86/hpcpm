#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
	#include <nmprk/nmprk_defines.h>
	#include <nmprk/nmprk_ipmi_global.h>
	#include <nmprk/nmprk_translation.h>
	#include <nmprk/nmprk_translation_defines.h>
	#include <nmprk/nmprk_helper.h>
#pragma GCC diagnostic pop

#include <memory>
#include "NMPRKCommunicationProvider.hpp"
#include "utility/Functional.hpp"
#include "utility/Logging.hpp"

// Documentation states that NMPRK functions will never return false or nullptr as
// it would be preceeded by exception being thrown inside function body.
// Nevertheless it's better to check the return value just to make sure.
#define NMPRK_CHECK_RETURN_VAL( x ) if( !x ) { throw NMPRKError{ UTILITY_STRINGIFY( x ), "Return value check failed at line:" UTILITY_STRINGIFY( __LINE__ ) };  }

//TODO: should delPolicy() be called when the application exits?

namespace devices {

nmprk::ipmi::device NMPRKCommunicationProvider::d;
std::string NMPRKCommunicationProvider::deviceId;
const unsigned NMPRKCommunicationProvider::POLICY_ID;
unsigned NMPRKCommunicationProvider::correctionTime;
unsigned NMPRKCommunicationProvider::statReportingPeriod;
unsigned NMPRKCommunicationProvider::minValue;
unsigned NMPRKCommunicationProvider::maxValue;

using namespace nmprk;

bool NMPRKCommunicationProvider::init( void ) {
	try {
		NMPRK_CHECK_RETURN_VAL( translation::swSubSystemSetup( translation::initLibrary, nullptr ) );
		LOG( INFO ) << "NMPRK - library initialized";

		d.type = ipmi::device_nm;
		d.address = "local";

		NMPRK_CHECK_RETURN_VAL( translation::swSubSystemSetup( translation::initDevice, &d ) );
		LOG( INFO ) << "NMPRK - device initialized";

		NMPRK_CHECK_RETURN_VAL( ipmi::connectDevice( &d ) );
		LOG( INFO ) << "NMPRK - connection to device succeeded";

		NMPRK_CHECK_RETURN_VAL( ipmi::initSystemForLocal() );
		LOG( INFO ) << "NMPRK - initSystemForLocal() succeeded";

		minValue = 0;

		try {
			maxValue = getCurrentPowerUsage();
		}
		catch ( ... ) {
			LOG ( WARNING ) << "Failed to get upper power limit, upper constraints set to 0";
			maxValue = 0;
		}

		return true;
	}
	catch ( const NMPRKError& e ) {
		LOG( ERROR ) << e.info();
		return false;
	}
	catch ( const nmprkException* e ) {
		LOG( ERROR ) << NMPRKError::nmprkExceptionToString( e );
		return false;
	}
}

bool NMPRKCommunicationProvider::shutdown( void ) {
	try {
		NMPRK_CHECK_RETURN_VAL( !ipmi::disconnectDevice( &d ) );

		NMPRK_CHECK_RETURN_VAL( translation::swSubSystemSetup( translation::unInitDevice, &d ) );

		NMPRK_CHECK_RETURN_VAL( translation::swSubSystemSetup( translation::unInitLibrary, nullptr ) );
		LOG( INFO ) << "NMPRK Library uninitialized.";

		return true;
	}
	catch ( const NMPRKError& e ) {
		LOG( ERROR ) << e.info();
		return false;
	}
	catch ( const nmprkException* e ) {
		LOG( ERROR ) << NMPRKError::nmprkExceptionToString( e );
		return false;
	}
}

DeviceInformation::InfoContainer NMPRKCommunicationProvider::getInfo( void ) {
	DeviceInformation::InfoContainer info;

	try {
		fillCapabilitiesInfo( info );
		fillVersionInfo( info );

		auto devId = ipmi::global::getDeviceId( &d );
		NMPRK_CHECK_RETURN_VAL( devId );
		std::shared_ptr<ipmi::getDeviceIdRsp> deviceRAIIGuard{ devId };

		deviceId = info[ "DeviceId" ] = "0x" + helper::int2HexStr( devId->deviceId );
		info[ "DeviceRevision" ] = "0x" + helper::int2HexStr( devId->deviceRev );
		info[ "FirmwareRevision" ] = "0x" + helper::int2HexStr( devId->firmwareRev );
		info[ "FirmwareRevision2" ] = "0x" + helper::int2HexStr( devId->firmwareRev2 );
	}
	catch ( const nmprkException* e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", e };
	}

	return info;
}

unsigned NMPRKCommunicationProvider::getCurrentPowerLimit( void ) {
	translation::policy_t policy;
	NMPRK_CHECK_RETURN_VAL( translation::getPolicy( &d, &policy ) );
	return policy.policyLimit;
}

void NMPRKCommunicationProvider::setPowerLimit( unsigned watts ) {
	translation::policy_t policy;
	policy.policyId = POLICY_ID;
	policy.policyType = translation::policyPower;
	policy.component = translation::domainSystem;
	//policy.sendAlert = false;
	policy.shutdown = false;
	policy.policyLimit = watts;
	policy.statReportingPeriod = statReportingPeriod;
	policy.correctionTime = correctionTime;
	policy.policyEnabled = true;

	try { //TODO: we may need to disable policy before changing it, see tips at the end of ImplementationGuide-NMPRK.docx
		NMPRK_CHECK_RETURN_VAL( translation::setPolicy( &d, &policy ) );
	}
	catch ( const nmprkException* e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::setPowerLimit", e };
	}
}

std::pair<unsigned, unsigned> NMPRKCommunicationProvider::getPowerLimitConstraints( void ) {
	return { minValue, maxValue };
}

unsigned NMPRKCommunicationProvider::getCurrentPowerUsage( void ) {
	try {
		auto sample = translation::getSample( &d, translation::samplePower, translation::domainSystem, nullptr );
		NMPRK_CHECK_RETURN_VAL( sample );
		std::shared_ptr<translation::sample_t> sampleRAIIGuard{ sample };

		auto reading = sample->avg;

		NMPRK_CHECK_RETURN_VAL( translation::resetStatistics( &d, translation::domainSystem, nullptr ) ); //czy to w ogole jest potrzebne?

		return reading;
	}
	catch ( const nmprkException* e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::getCurrentPowerLimit", e };
	}
}

void NMPRKCommunicationProvider::fillCapabilitiesInfo( DeviceInformation::InfoContainer& container ) {
	try {
		auto capabilities = translation::getCapabilities( &d );
		NMPRK_CHECK_RETURN_VAL( capabilities );
		std::shared_ptr<translation::capabilities_t> capabilitiesRAIIGuard{ capabilities };

		container[ "SuportsPowerManagement" ] = utility::toString( capabilities->supportsPowerManagement );
		container[ "MaxTriggerValue(for policies)" ] = std::to_string( capabilities->maxTriggerValue );
		container[ "MinTriggerValue(for policies)" ] = std::to_string( capabilities->minTriggerValue );
		container[ "MaxCorrectionTime(for policies)" ] = std::to_string( capabilities->maxCorrectionTime );
		container[ "MinCorrectionTime(for policies)" ] = std::to_string( capabilities->minCorrectionTime );
		container[ "MaxStatisticsReportingPeriod" ] = std::to_string( capabilities->maxStatReportPeriod );
		container[ "MinStatisticsReportingPeriod" ] = std::to_string( capabilities->minStatReportPeriod );

		// both of those two below can be manually set to 0 to force using the default value
		correctionTime = capabilities->minCorrectionTime;
		statReportingPeriod = capabilities->minStatReportPeriod;
	}
	catch ( const nmprkException* e ) {
		container[ "SuportsPowerManagement" ] = "false";
	}
}

void NMPRKCommunicationProvider::fillVersionInfo( DeviceInformation::InfoContainer& container ) {
	try {
		auto version = translation::getNMVersion( &d );
		NMPRK_CHECK_RETURN_VAL( version );
		std::shared_ptr<translation::nmVersion_t> versionRAIIGuard{ version };

		if( version->nmVersion_2pt0 ) {
			container[ "NodeManagerVersion" ] = "2.0";
		}
		else if( version->nmVersion_1pt5 ) {
			container[ "NodeManagerVersion" ] = "1.5";
		}
		else {
			container[ "NodeManagerVersion" ] = "Unknown";
		}

		if( version->ipmiVersion_2pt0 ) {
			container[ "IPMIVersion" ] = "2.0";
		}
		else if( version->ipmiVersion_1pt0 ) {
			container[ "IPMIVersion" ] = "1.0";
		}
		else {
			container[ "IPMIVersion" ] = "Unknown";
		}
	}
	catch ( const nmprkException* e ) {
		container[ "NodeManagerVersion" ] = "N/A";
		container[ "IPMIVersion" ] = "N/A";
	}
}

} // namespace devices
