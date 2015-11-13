#pragma once
#include "Query.hpp"
#include "utility/Functional.hpp"

namespace core {

class GetPowerLimitPercentageQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			object[ "PowerLimitPercentage" ] = web::json::value::parse( utility::ftos( powerLimit, 3 ) );
			return object.serialize();
		}

	private:
		friend class GetPowerLimitPercentageQuery;

		devices::DeviceIdentifier deviceIdentifier;
		devices::Percentage powerLimit;
	};

	std::string getTypeName( void ) const final { return "GetPowerLimitPercentage"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->powerLimit = dev.getCurrentPowerLimitPercentage();
		result->deviceIdentifier = dev.getInfo().identifier;

		return result;
	}
};

} // namespace core
