#pragma once
#include "Query.hpp"

namespace core {

class GetPowerLimitQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerLimit" ] = powerLimit;
			return object.serialize();
		}

	private:
		friend class GetPowerLimitQuery;

		devices::DeviceIdentifier deviceIdentifier;
		devices::Power powerLimit;
	};

	std::string getTypeName( void ) const final { return "GetPowerLimit"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->powerLimit = dev.getCurrentPowerLimit();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
