#include "QueryExecutor.hpp"
#include "query_handlers/GetNodeInformationQueryHandler.hpp"
#include "query_handlers/GetPowerLimitPercentageQueryHandler.hpp"
#include "query_handlers/GetPowerLimitQueryHandler.hpp"
#include "query_handlers/GetPowerLimitConstraintsQueryHandler.hpp"
#include "query_handlers/SetPowerLimitPercentageQueryHandler.hpp"
#include "query_handlers/SetPowerLimitQueryHandler.hpp"
#include "utility/Logging.hpp"
#include "utility/make_unique.hpp"

namespace core {
QueryExecutor::QueryExecutor( std::shared_ptr<devices::DevicesManager> devicesManager ) {
	using type = Query::Type;
	queryHandlers.emplace( type::GetNodeInformation, utility::make_unique<GetNodeInformationQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::GetPowerLimit, utility::make_unique<GetPowerLimitQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::GetPowerLimitPercentage, utility::make_unique<GetPowerLimitPercentageQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::GetPowerLimitConstraints, utility::make_unique<GetPowerLimitConstraintsQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::SetPowerLimit, utility::make_unique<SetPowerLimitQueryHandler>( devicesManager ) );
	queryHandlers.emplace( type::SetPowerLimitPercentage, utility::make_unique<SetPowerLimitPercentageQueryHandler>( devicesManager ) );
}

QueryHandler::Result::Ptr QueryExecutor::execute( Query query ) {
	LOG ( INFO ) << "Executing query \"" << query.getTypeName()
				 << "\" [" << query.getDeviceIdentifier() << "]: "
				 << ( query.getArgument().empty() ? "NO_ARG" : query.getArgument() );

	try {
		return queryHandlers.at( query.getType() )->handle( query );
	}
	catch ( const std::out_of_range& ex ) {
		LOG ( ERROR ) << "Invalid query type";
		return std::make_shared<QueryHandler::NullResult>();
	}
	catch ( const utility::RuntimeError& ex ) {
		LOG ( ERROR ) << "Query execution failed, exception thrown: " << ex.info();
		return std::make_shared<QueryHandler::ResultWithError>( ex.info() );
	}
}

} // namespace core
