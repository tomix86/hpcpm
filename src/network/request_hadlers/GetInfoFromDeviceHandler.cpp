#include <boost/xpressive/xpressive.hpp>
#include "GetInfoFromDeviceHandler.hpp"

namespace network {
namespace handlers {

GetInfoFromDeviceHandler::GetInfoFromDeviceHandler( core::QueryExecutor::Ptr queryExecutor, core::QueryType queryType ) :
		Handler{ queryExecutor },
		queryType{ queryType } {
}

GetInfoFromDeviceHandler::~GetInfoFromDeviceHandler( void ) {
}

std::vector<core::Query::Ptr> GetInfoFromDeviceHandler::splitIntoQueries( http_request request ) {
	auto queryString = request.request_uri().query();
	if ( queryString.empty() ) {
		LOG ( WARNING ) << "Query string was empty.";
	}

	if ( !isQueryStringWellFormed( queryString ) ) {
		throw MalformedQueryException( "GetInfoFromDeviceHandler::splitIntoQueries", "malformed query string: " + queryString );
	}

	std::vector<core::Query::Ptr> queries;
	for( auto devString : utility::tokenizeString( queryString, '&' ) ) {
		auto tokenizedDevString = utility::tokenizeString( devString, '=' );
		auto query = core::QueryFactory::createQuery( queryType );
		query->setDeviceIdentifier( { tokenizedDevString.at( 0 ), tokenizedDevString.at( 1 ) } );
		queries.push_back( query );
	}

	return queries;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-overflow"
bool GetInfoFromDeviceHandler::isQueryStringWellFormed( std::string queryString ) {
	using namespace boost::xpressive;

	std::string singleEntry = std::string( "(IntelXeon=" ) + IntelXeonIdRegex + "|IntelXeonPhi=" + IntelXeonPhiIdRegex + "|NvidiaTesla=" + NvidiaTeslaIdRegex + ")";
	std::string matchEmptyOrOneDevice = "(" + singleEntry + "){0,1}";
	std::string matchMultipleDevices = singleEntry + "(&" + singleEntry + ")+";
	// The line below should use sregex::compile, but it doesn't work for GCC4.8 due to a bug so we need a simple workaround
	auto regex = sregex_compiler{}.compile( "^" + matchEmptyOrOneDevice + "|" + matchMultipleDevices + "$" );
	smatch match;
	return regex_match( queryString, match, regex );
}
#pragma GCC diagnostic pop

} // namespace handlers
} // namespace network
