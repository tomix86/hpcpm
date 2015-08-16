#include "QueryExecutor.hpp"
#include "utility/Logging.hpp"

namespace core {
QueryExecutor::QueryExecutor( void ) {
	devicesManager.init();
	devicesManager.updateDevicesList();
}

Query::Result QueryExecutor::execute( Query query ) {
	LOG ( INFO ) << "Executing query \"" << Query::typeName( query.getType() )
				 << "\" [" << query.getDeviceIdentifier() << "]: "
				 << ( query.getArgument().empty() ? "NO_ARG" : query.getArgument() );

	using t = Query::Type;
	switch ( query.getType() ) {
	case t::GetNodeInformation:
		return handleGetNodeInformation();
	case t::GetPowerLimit:
		return handleGetPowerLimit( query );
	case t::GetPowerLimitConstraints:
		return Query::Result();
	case t::SetPowerLimitAbsolute:
		return Query::Result();
	case t::SetPowerLimitPercentage:
		return Query::Result();
	case t::GetPowerLimitPercentage:
		return Query::Result();
	default:
		LOG ( ERROR ) << "Invalid query type";
		return Query::Result();
	}
}

Query::Result QueryExecutor::handleGetNodeInformation( void ) {
	// should return list of devices and information about the host(node) itself (like for example OS version)
	return Query::Result("It seems that it works\n");
}

Query::Result QueryExecutor::handleGetPowerLimit( Query query ) {
	Query::Result result;

	devicesManager.getDeviceByIdentifier( query.getDeviceIdentifier() );

	return result;
}
} // namespace core
