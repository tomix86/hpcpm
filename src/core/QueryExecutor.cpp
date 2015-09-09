#include "QueryExecutor.hpp"
#include "query_handlers/GetNodeInformationQueryHandler.hpp"
#include "query_handlers/GetPowerLimitPercentageQueryHandler.hpp"
#include "query_handlers/GetPowerLimitQueryHandler.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

namespace core {
QueryExecutor::QueryExecutor( std::shared_ptr<devices::DevicesManager> devicesManager ) {
	using type = Query::Type;
	queryHandlers.emplace( type::GetNodeInformation, utility::make_unique<GetNodeInformationQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::GetPowerLimit, utility::make_unique<GetPowerLimitQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::GetPowerLimitPercentage, utility::make_unique<GetPowerLimitPercentageQueryHandler>( devicesManager ) );
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
	catch ( utility::RuntimeError& ex ) {
		LOG ( ERROR ) << "Query execution failed, exception thrown: " << ex.info();
		return std::make_shared<QueryHandler::ResultWithError>( ex.info() );
	}
}

} // namespace core
