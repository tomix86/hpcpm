#pragma once
extern "C" {
	#include <sys/utsname.h>
}
#include "network/cpprest.hpp"
#include "Query.hpp"

namespace core {

class GetNodeInformationQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			web::json::value mainObject;
			mainObject[ "sysname" ] = web::json::value( sysInfo.sysname );
			mainObject[ "nodename" ] = web::json::value( sysInfo.nodename );
			mainObject[ "release" ] = web::json::value( sysInfo.release );
			mainObject[ "version" ] = web::json::value( sysInfo.version );
			mainObject[ "machine" ] = web::json::value( sysInfo.machine );
			mainObject[ "supportsNVML" ] = web::json::value( supportedLibraries.NVML );
			mainObject[ "supportsNMPRK" ] = web::json::value( supportedLibraries.NMPRK );
			mainObject[ "supportsMPSS" ] = web::json::value( supportedLibraries.MPSS );
			mainObject[ "supportsOpenCL (KernelHive)" ] = web::json::value( supportedLibraries.OpenCL ); //TODO: przy wsparciu dla opencl nalezy uwzglednic sprawdzenie rozszerzen platformy CL czy wspiera to specyficzne query, jezeli nie wspeira to nie uda sie zmatchowac i wtedy nie wspieramy niestety, mozna dodac fallback, ze po samej nazwie urzadzenia ale bedzie on zawody

			web::json::value devicesArray;
			for ( auto device : devices ) {
				auto deviceObject = serializeDeviceIdentifierToJsonObject( device->getInfo().identifier );

				web::json::value infoObject;
				for ( auto entry : device->getInfo().entries ) {
					infoObject[ entry.first ] = web::json::value( entry.second );
				}
				deviceObject[ "info" ] = infoObject;

				devicesArray[ devicesArray.size() ] = deviceObject;
			}
			mainObject[ "devices" ] = devicesArray;

			return mainObject.serialize();
		}

	private:
		friend class GetNodeInformationQuery;

		utsname sysInfo;
		std::vector<devices::Device::Ptr> devices;
		devices::SupportedLibraries supportedLibraries;
	};

	std::string getTypeName( void ) const final { return "GetNodeInformation"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto result = std::make_shared<Result>();
		uname( &result->sysInfo );

		result->devices = devicesManager->getDevicesList();

		result->supportedLibraries = devicesManager->getSupportedLibraries();

		return result;
	}
};

} // namespace core
