#pragma once
#include "Query.hpp"

namespace core {

class GetCurrentPowerUsageQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerUsage" ] = powerUsage;
			return object.serialize();
		}

	private:
		friend class GetCurrentPowerUsageQuery;

		devices::DeviceIdentifier deviceIdentifier;
		devices::Power powerUsage;
	};

	std::string getTypeName( void ) const final { return "GetCurrentPowerUsage"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->powerUsage = dev.getCurrentPowerUsage();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
