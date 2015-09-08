#include <gtest/gtest.h>
#include "network/cpprest.hpp"

using web::http::http_response;
using web::http::status_codes;
using namespace web;

// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
TEST( CpprestTestSuite, http_response_StatusCodesTest ) {
	http_response response( status_codes::OK );
	ASSERT_EQ( 200, response.status_code() );

	response.set_status_code( status_codes::BadRequest );
	ASSERT_EQ( 400, response.status_code() );

	response.set_status_code( status_codes::Forbidden );
	ASSERT_EQ( 403, response.status_code() );

	response.set_status_code( status_codes::InternalError );
	ASSERT_EQ( 500, response.status_code() );
}

TEST( CpprestTestSuite, http_response_PlaintextBodyTest ) {
	http_response response;
	response.set_body( U( "Plaintext response body" ) );

	ASSERT_STREQ( U( "Plaintext response body" ), response.extract_string().get().c_str() );
}

TEST( CpprestTestSuite, JSONValuesTest ) {
	json::value stringVal( U( "Device1" ) );
	ASSERT_STREQ( U( "Device1" ), stringVal.as_string().c_str() );

	json::value integerVal( 225000 );
	ASSERT_EQ( 225000, integerVal.as_integer() );

	ASSERT_THROW( integerVal.as_string(), web::json::json_exception );
}

TEST( CpprestTestSuite, JSONStringParsingTest ) {
	json::value val;
	ASSERT_THROW( json::value::parse( U( "test" ) ), json::json_exception );
	ASSERT_NO_THROW( val = json::value::parse( U( "\"test\"" ) ) );
	ASSERT_STREQ( U( "test" ), val.as_string().c_str() );

	ASSERT_THROW( json::value::parse( U( "" ) ), json::json_exception );
	ASSERT_NO_THROW( val = json::value::parse( U( "\"\"" ) ) );
}

TEST( CpprestTestSuite, JSONObjectParsingTest ) {
	auto stringsToParse = {	U( "{ \"DeviceID\" : \"0:0:0:1\" }" ),
							U( "{ \"DeviceID\"     :  \"0:0:0:1\" }" ),
							U( "{    \"DeviceID\" : \"0:0:0:1\"   }    " ),
							U( "      { \"DeviceID\" : \"0:0:0:1\" }" ) };

	for ( auto string : stringsToParse ) {
		json::value object;
		ASSERT_NO_THROW( object = json::value::parse( string ) );
		ASSERT_EQ( 1, object.size() );
		ASSERT_STREQ( U( "0:0:0:1" ), object.at( U( "DeviceID" ) ).as_string().c_str() );
	}
}

TEST( CpprestTestSuite, JSONObjectTest ) {
	json::value object = json::value::parse( U( "{ \"DeviceID\" : \"0:0:0:1\" }" ) );
	ASSERT_EQ( 1, object.size() );
	ASSERT_STREQ( U( "0:0:0:1" ), object.at( U( "DeviceID" ) ).as_string().c_str() );

	object[ U( "PowerLimit" ) ] = json::value( 225000 );
	ASSERT_EQ( 2, object.size() );
	ASSERT_EQ( 225000, object.at( U( "PowerLimit" ) ).as_integer() );

	ASSERT_THROW( object.at( ( "PowerUsage" ) ), json::json_exception );
	ASSERT_NO_THROW( object[ U( "PowerUsage" ) ] );

	ASSERT_TRUE( object[ U( "PowerUsage" ) ].is_null() );
}

TEST( CpprestTestSuite, JSONArrayTest ) {
	json::value array = json::value::parse( U( "[ 1, 2, 3, 4, { \"a\":true } ]" ) );

	ASSERT_EQ( 5, array.size() );
	ASSERT_EQ( 1, array.at( 0 ).as_integer() );
	ASSERT_TRUE( array.at( 4 ).as_object().at( U( "a" ) ).as_bool() );

	ASSERT_NO_THROW( array[ 4 ] = 123 );
	ASSERT_EQ( 123, array.at( 4 ).as_integer() );

	ASSERT_NO_THROW( array[ 5 ] = 456 );
	ASSERT_EQ( 456, array.at( 5 ).as_integer() );
}

TEST( CpprestTestSuite, http_response_JSONBodyTest ) {
	json::value object = json::value::parse( U( "{ \"DeviceID\" : \"0:0:0:1\" }" ) );
	json::value array = json::value::parse( U( "[ 1, 2, 3, 4, { \"a\":true } ]" ) );
	object[ U("Values") ] = array;

	http_response response;
	response.set_body( object );

	ASSERT_STREQ( U( "{\"DeviceID\":\"0:0:0:1\",\"Values\":[1,2,3,4,{\"a\":true}]}" ), response.extract_string().get().c_str() );
}
