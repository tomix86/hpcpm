#include "QueryExecutor.hpp"
#include "query_handlers/GetNodeInformationHandler.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

namespace core {
QueryExecutor::QueryExecutor( std::shared_ptr<devices::DevicesManager> devicesManager ) {
	using type = Query::Type;
	queryHandlers.emplace( type::GetNodeInformation, utility::make_unique<GetNodeInformationQueryHandler>( devicesManager ) );
//	queryHandlers[ type::GetPowerLimit ] = ;
//	queryHandlers[ type::GetPowerLimitConstraints ] = ;
//	queryHandlers[ type::SetPowerLimitAbsolute ] = ;
//	queryHandlers[ type::SetPowerLimitPercentage ] = ;
//	queryHandlers[ type::GetPowerLimitPercentage ] = ;
}

QueryHandler::Result::Ptr QueryExecutor::execute( Query query ) {
	LOG ( INFO ) << "Executing query \"" << query.getTypeName()
				 << "\" [" << query.getDeviceIdentifier() << "]: "
				 << ( query.getArgument().empty() ? "NO_ARG" : query.getArgument() );

	try {
		return queryHandlers.at( query.getType() )->handle( query );
	}
	catch ( std::out_of_range& ex ) {
		LOG ( ERROR ) << "Invalid query type";
		return std::make_shared<QueryHandler::NullResult>();
	}
}

} // namespace core
