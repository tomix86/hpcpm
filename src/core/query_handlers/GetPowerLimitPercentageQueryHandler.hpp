#pragma once
#include "QueryHandler.hpp"
#include "utility/Functional.hpp"

namespace core {

class GetPowerLimitPercentageQueryHandler : public QueryHandler {
public:
	class Result : public QueryHandler::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerLimitPercentage" ] = web::json::value::parse( utility::ftos( powerLimit, 3 ) );
			return object.serialize();
		}

	private:
		friend class GetPowerLimitPercentageQueryHandler;

		devices::DeviceIdentifier deviceIdentifier;
		devices::Percentage powerLimit;
	};

	GetPowerLimitPercentageQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
			QueryHandler( devicesManager ) {
	}

	QueryHandler::Result::Ptr handle( Query query ) final {
		auto& dev = devicesManager->getDeviceByIdentifier( query.getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->powerLimit = dev.getCurrentPowerLimitPercentage();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
