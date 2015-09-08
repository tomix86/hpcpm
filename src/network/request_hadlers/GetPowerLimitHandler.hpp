#pragma once
#include <boost/xpressive/xpressive.hpp>
#include "Handler.hpp"

namespace network {
namespace handlers {
class GetPowerLimitHandler : public Handler {
public:
	GetPowerLimitHandler( core::QueryExecutor::Ptr queryExecutor ) :
	Handler{ queryExecutor } {
	}

protected:
	// Query string syntax: should contain list of devices that are being queried in form:
	// ?DevType=DevId&Dev2Type=Dev2Id&Dev3Type=Dev3Id
	std::vector<core::Query> splitIntoQueries( http_request request ) final {
		auto queryString = request.request_uri().query();
		if ( queryString.empty() ) {
			LOG ( WARNING ) << "Query string was empty.";
		}

		if ( !isQueryStringWellFormed( queryString ) ) {
			throw MalformedQueryException( "GetPowerLimitHandler::splitIntoQueries", "malformed query string: " + queryString );
		}

		std::vector<core::Query> queries;
		for( auto devString : utility::tokenizeString( queryString, '&' ) ) {
			auto tokenizedDevString = utility::tokenizeString( devString, '=' );
			core::Query query{ core::Query::Type::GetPowerLimit };
			query.setDeviceIdentifier( { tokenizedDevString.at( 0 ), tokenizedDevString.at( 1 ) } );
			queries.push_back( query );
		}

		return queries;
	}

	http_response serializeQueriesResults( std::vector<core::QueryHandler::Result::Ptr> result ) final {
		web::json::value array;
		for ( auto element : result ) {
			array[ array.size() ] = web::json::value::parse( U( element->serialize() ) );
		}

		http_response response;
		response.set_body( array.serialize(), JSON_RESPONSE_TYPE );
		return response;
	}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-overflow"
	bool isQueryStringWellFormed( std::string queryString ) {
		using namespace boost::xpressive;

		//TODO: change ID pattern when we have determined format of primary id for each of the devices
		std::string singleEntry = "(IntelXeon|IntelXeonPhi|NvidiaTesla)=[A-Za-z0-9:]+";
		std::string matchEmptyOrOneDevice = "(" + singleEntry + "){0,1}";
		std::string matchMultipleDevices = singleEntry + "(&" + singleEntry + ")+";
		// The line below should use sregex::compile, but it doesn't work for GCC4.8 due to a bug so we need a simple workaround
		auto regex = sregex_compiler{}.compile( "^" + matchEmptyOrOneDevice + "|" + matchMultipleDevices + "$" );
		smatch match;
		return regex_match( queryString, match, regex );
	}
#pragma GCC diagnostic pop
};
} // namespace handlers
} // namespace network
