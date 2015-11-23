#pragma once
#include "Query.hpp"

namespace core {

class RemovePowerLimitQuery : public Query {
public:
	class Result : public Query::Result {
	public:
		std::string serialize( void ) const final {
			auto object = serializeDeviceIdentifierToJsonObject( deviceIdentifier );
			return object.serialize();
		}

	private:
		friend class RemovePowerLimitQuery;

		devices::DeviceIdentifier deviceIdentifier;
	};

	std::string getTypeName( void ) const final { return "RemovePowerLimit"; }

	Query::Result::Ptr execute( std::shared_ptr<devices::DevicesManager> devicesManager ) const final {
		auto& dev = devicesManager->getDeviceByIdentifier( getDeviceIdentifier() );
		auto result = std::make_shared<Result>();
		result->deviceIdentifier = dev.getInfo().identifier;

		auto constraints = dev.getPowerLimitConstraints();
		dev.setPowerLimit( constraints.upper );

		return result;
	}
};

} // namespace core
