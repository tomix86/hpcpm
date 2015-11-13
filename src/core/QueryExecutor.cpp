#include "QueryExecutor.hpp"
#include "utility/Logging.hpp"

namespace core {

QueryExecutor::QueryExecutor( std::shared_ptr<devices::DevicesManager> devicesManager ) :
		m_devicesManager{ devicesManager } {
}

Query::Result::Ptr QueryExecutor::execute( Query::Ptr query ) {
	LOG ( INFO ) << "Executing query \"" << query->getTypeName()
				 << "\" [" << query->getDeviceIdentifier() << "]: "
				 << ( query->getArgument().empty() ? "NO_ARG" : query->getArgument() );

	try {
		return query->execute( m_devicesManager );
	}
	catch ( const utility::RuntimeError& ex ) {
		LOG ( ERROR ) << "Query execution failed, exception thrown: " << ex.info();
		return std::make_shared<Query::ResultWithError>( ex.info() );
	}
}

} // namespace core
