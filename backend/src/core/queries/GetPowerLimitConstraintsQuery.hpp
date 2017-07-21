#pragma once
#include "Query.hpp"

namespace core {

class GetPowerLimitConstraintsQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerLimitConstraints" ] = serializePowerLimitConstraintsToJsonObject( constraints );
			return object.serialize();
		}

	private:
		friend class GetPowerLimitConstraintsQuery;

		devices::DeviceIdentifier deviceIdentifier;
		devices::PowerLimitConstraints constraints;

		web::json::value serializePowerLimitConstraintsToJsonObject( devices::PowerLimitConstraints constraints ) const {
			web::json::value object;
			object[ "lower" ] = constraints.lower;
			object[ "upper" ] = constraints.upper;
			return object;
		}
	};

	std::string getTypeName( void ) const final { return "GetPowerLimitConstraints"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->constraints = dev.getPowerLimitConstraints();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
