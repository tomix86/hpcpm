#include "Exceptions.hpp"

namespace utility {
	Exception::Exception( std::string src, std::string msg ) {
		exceptionInfo.emplace_back( src, msg );
	}

	std::string Exception::source( void ) const {
		return exceptionInfo.back().first;
	}

	std::string Exception::message( void ) const {
		return exceptionInfo.back().second;
	}

	std::string Exception::info( void ) const {
		return source() + ": " + message();
	}

	std::string Exception::trace( void ) const {
		std::string result;
		for ( unsigned i = 0; i < exceptionInfo.size(); ++i ) {
			result += exceptionInfo[ i ].first;
			if ( i + 1 != exceptionInfo.size() ) {
				result += " -> ";
			}
		}
		return result;
	}

	std::string Exception::traceWithMessages( void ) const {
		std::string result;
		for ( unsigned i = 0; i < exceptionInfo.size(); ++i ) {
			result += exceptionInfo[ i ].first + '[' + exceptionInfo[ i ].second + ']';
			if ( i + 1 != exceptionInfo.size() ) {
				result += " -> ";
			}
		}
		return result;
	}

	void Exception::appendExceptionInfo( const Exception& other ) {
		exceptionInfo.insert( exceptionInfo.begin(), other.exceptionInfo.begin(), other.exceptionInfo.end() );
	}

	RuntimeError::RuntimeError( std::string src, std::string msg ) :
		Exception( src, msg ) {
	}

	LogicError::LogicError( std::string src, std::string msg ) :
		Exception( src, msg ) {
	}

	InvalidArgument::InvalidArgument( std::string src, std::string msg ) :
		LogicError( src, msg ) {

	}
} // namespace utility
