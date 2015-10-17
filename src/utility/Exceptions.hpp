#pragma once
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define DEFINE_RUNTIME_ERROR_DERIVATIVE( ExceptionName ) \
class ExceptionName: public utility::RuntimeError {\
public:\
	ExceptionName( std::string src, std::string msg ) : utility::RuntimeError( src, msg) { }\
}

#define DEFINE_LOGIC_ERROR_DERIVATIVE ( ExceptionName ) \
class ExceptionName: public utility::RuntimeError {\
public:\
	ExceptionName( std::string src, std::string msg ) : utility::RuntimeError( src, msg) { }\
}

namespace utility {
	class Exception : public std::exception {
	protected:
		typedef std::pair<std::string, std::string> infoEntry;

	public:
		Exception(std::string src, std::string msg);

		virtual ~Exception( void ) {}

		std::string source( void ) const;

		std::string message( void ) const;

		// Returns exception source + message
		std::string info( void ) const;

		// Returns exception trace ie. the trail of handlers it passed through
		// for example: Source -> H1 -> H2
		std::string trace( void ) const;

		// Returns exception trace ie. the trail of handlers it passed through
		// along with message for each handler
		// for example: Source[msg1] -> H1[msg2] -> H2[msg3]
		std::string traceWithMessages( void ) const;

		// Appends info from another exception
		// to the current one for example: current info {(H3, M3)} other info: {(H1, M1), (H2, M2)}
		// after calling current.appendExceptionInfo(other) current info: {(H1, M1), (H2, M2), (H3, M3)}
		void appendExceptionInfo( const Exception& other );

	protected:
		std::vector<infoEntry> exceptionInfo;

	private:
		void what( void ) = delete;
	};

	class RuntimeError : public Exception {
	public:
		RuntimeError( std::string src, std::string msg );
	};

	class LogicError : public Exception {
	public:
		LogicError( std::string src, std::string msg );
	};

	class InvalidArgument : public LogicError {
	public:
		InvalidArgument( std::string src, std::string msg );
	};
} // namespace utility
