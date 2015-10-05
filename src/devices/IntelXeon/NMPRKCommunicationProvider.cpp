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
#include "utility/Logging.hpp"

namespace devices {

nmprk::ipmi::device NMPRKCommunicationProvider::d;

using namespace nmprk;

NMPRKCommunicationProvider::NMPRKCommunicationProvider( DeviceIdentifier::idType deviceId ) {
	(void)deviceId;
}
/*
NMPRKCommunicationProvider::~NMPRKCommunicationProvider( void ) {

}*/

void NMPRKCommunicationProvider::init( void ) {
	try {
		if ( translation::swSubSystemSetup( translation::initLibrary, nullptr ) ) {
			LOG( INFO ) << "NMPRK Library initialized.";
		}
		else {
			LOG( ERROR ) << "NMPRK Library initialization failed.";
		}

		d.type = ipmi::device_nm;
		d.address = "local";

		if ( translation::swSubSystemSetup( translation::initDevice, &d ) ) {
			LOG( INFO ) << "Device initialized.";
		}
		else {
			LOG( ERROR ) << "Device initialization failed.";
		}
	}
	catch ( nmprkException& e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::init", e };
	}
}

void NMPRKCommunicationProvider::shutdown( void ) {
	try {
		translation::swSubSystemSetup( translation::unInitDevice, &d );

		if ( translation::swSubSystemSetup( translation::unInitLibrary, nullptr ) ) {
			LOG( INFO ) << "NMPRK Library uninitialized.";
		}
		else {
			LOG( ERROR ) << "NMPRK Library uninitialization failed.";
		}
	}
	catch ( nmprkException& e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::shutdown", e };
	}
}

std::map<std::string, std::string> NMPRKCommunicationProvider::getInfo( void ) { //TODO: clean up
	std::map<std::string, std::string> info;

	try {
		auto capabilities = translation::getCapabilities( &d );
		std::shared_ptr<translation::capabilities_t> capabilitiesRAIIGuard{ capabilities };

		if( capabilities == nullptr ) {
			throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", "getCapabilities() returned a null pointer" };
		}

		auto version = translation::getNMVersion( &d );
		std::shared_ptr<translation::nmVersion_t> versionRAIIGuard{ version };
		if( version == nullptr ) {
			throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", "getNMVersion() returned a null pointer" };
		}

		if( version->nmVersion_2pt0 ) {
			info[ "NodeManagerVersion" ] = "2.0";
		}
		else if( version->nmVersion_1pt5 ) {
			info[ "NodeManagerVersion" ] = "1.5";
		}
		else {
			info[ "NodeManagerVersion" ] = "Unknown";
		}

		if( version->ipmiVersion_2pt0 ) {
			info[ "IPMIVersion" ] = "2.0";
		}
		else if( version->ipmiVersion_1pt0 ) {
			info[ "IPMIVersion" ] = "1.0";
		}
		else {
			info[ "IPMIVersion" ] = "Unknown";
		}

		if ( ipmi::connectDevice( &d ) ) {
			auto deviceId = ipmi::global::getDeviceId( &d );
			std::shared_ptr<ipmi::getDeviceIdRsp> versionRAIIGuard{ deviceId };
			if( deviceId  == nullptr ) {
				throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", "getDeviceId() returned a null pointer" };
			}

			info[ "DeviceId" ] = "0x" + helper::int2HexStr( deviceId->deviceId );
			info[ "DeviceRevision" ] = "0x" + helper::int2HexStr( deviceId->deviceRev );
			info[ "FirmwareRevision" ] = "0x" + helper::int2HexStr( deviceId->firmwareRev );
			info[ "FirmwareRevision2" ] = "0x" + helper::int2HexStr( deviceId->firmwareRev2 );

			if ( !ipmi::disconnectDevice( &d ) ) {
				throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", "Disconnection from device failed" };
			}
		}
		else {
			throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", "Connection attempt to device failed" };
		}

		info[ "MaxConcurentConnections" ] = std::to_string( capabilities->maxConSettings );
		info[ "MaxTriggerValue(for policies)" ] = std::to_string( capabilities->maxTriggerValue );
		info[ "MinTriggerValue(for policies)" ] = std::to_string( capabilities->minTriggerValue );
		info[ "MaxCorrectionTime(for policies)" ] = std::to_string( capabilities->maxCorrectionTime );
		info[ "MinCorrectionTime(for policies)" ] = std::to_string( capabilities->minCorrectionTime );
		info[ "MaxStatisticsReportingPeriod" ] = std::to_string( capabilities->maxStatReportPeriod );
		info[ "MinStatisticsReportingPeriod" ] = std::to_string( capabilities->minStatReportPeriod );
	}
	catch( nmprk::nmprkException& e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::getInfo", e };
	}

	return info;
}

unsigned NMPRKCommunicationProvider::getCurrentPowerLimit( void ) const {
	try {
		auto sample = translation::getSample( &d, translation::samplePower, translation::domainSystem, nullptr );
		std::shared_ptr<translation::sample_t> sampleRAIIGuard{ sample };
		if( sample == nullptr ) {
			throw NMPRKError{ "NMPRKCommunicationProvider::getCurrentPowerLimit", "getSample() returned a null pointer" };
		}

		auto reading = sample->avg;
		translation::resetStatistics( &d, translation::domainSystem, nullptr );
		return reading;
	}
	catch ( nmprkException& e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::getCurrentPowerLimit", e };
	}
}

void NMPRKCommunicationProvider::setPowerLimit( unsigned watts ) {
	translation::policy_t policy;
	policy.policyId = 0x02;
	policy.policyEnabled = true;
	policy.policyType = translation::policyPower; // power limit, not thermal limit
	policy.component = translation::domainSystem; // set a limit for the whole system TODO: zamienic na CPU?
	policy.sendAlert = true; // send alert if policy.policyLimit is breached for longer then policy.correctionTime
	policy.shutdown = false; // shutdown system if policy.policyLimit is breached for longer then policy.correctionTime
	policy.statReportingPeriod = 10; // how long values are averaged over
	policy.correctionTime = 10; // how long a policy can be over its limit before policy.sendAlert and policy.shutdown are performed if true
	policy.policyLimit = watts;

	try { //TODO: we may need to disable policy before changing it, see tips at the end of ImplementationGuide-NMPRK.docx
		if ( translation::setPolicy( &d, &policy ) ) {
			LOG( INFO ) << "Power cap set to " << policy.policyLimit << "W"; //TODO: wiecej logowania diagnostycznego we wszystkich comm providerach
		}
		else {
			LOG( ERROR ) << "Setting power cap failed.";
		}
	}
	catch ( nmprkException& e ) {
		throw NMPRKError{ "NMPRKCommunicationProvider::setPowerLimit", e };
	}
}

std::pair<unsigned, unsigned> NMPRKCommunicationProvider::getPowerLimitConstraints( void ) const {
	return { 0, 0 };
}

} // namespace devices
