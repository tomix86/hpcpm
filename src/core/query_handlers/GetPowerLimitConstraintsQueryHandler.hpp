#pragma once
#include "QueryHandler.hpp"

namespace core {

class GetPowerLimitConstraintsQueryHandler : public QueryHandler {
public:
	class Result : public QueryHandler::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerLimitConstraints" ] = serializePowerLimitConstraintsToJsonObject( constraints );
			return object.serialize();
		}

	private:
		friend class GetPowerLimitConstraintsQueryHandler;

		devices::DeviceIdentifier deviceIdentifier;
		devices::PowerLimitConstraints constraints;

		web::json::value serializePowerLimitConstraintsToJsonObject( devices::PowerLimitConstraints constraints ) const {
			web::json::value object;
			object[ "lower" ] = constraints.lower;
			object[ "upper" ] = constraints.upper;
			return object;
		}
	};

	GetPowerLimitConstraintsQueryHandler( std::shared_ptr<devices::DevicesManager> devicesManager ) :
			QueryHandler( devicesManager ) {
	}

	QueryHandler::Result::Ptr handle( Query query ) final {
		auto& dev = devicesManager->getDeviceByIdentifier( query.getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->constraints = dev.getPowerLimitConstraints();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
