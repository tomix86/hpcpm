#pragma once
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

#define DEFINE_RUNTIME_ERROR_DERIVATIVE( ExceptionName ) \
class ExceptionName: public utility::RuntimeError {\
public:\
	ExceptionName( std::string src, std::string msg ) : utility::RuntimeError( src, msg) { }\
};

#define DEFINE_LOGIC_ERROR_DERIVATIVE ( ExceptionName ) \
class ExceptionName: public utility::RuntimeError {\
public:\
	ExceptionName( std::string src, std::string msg ) : utility::RuntimeError( src, msg) { }\
};

namespace utility {
	class Exception : public std::exception {
	protected:
		typedef std::pair<std::string, std::string> infoEntry;

	public:
		Exception(std::string src, std::string msg);

		virtual ~Exception( void ) {}

		virtual std::string source(void) const;

		virtual std::string message(void) const;

		// Returns exception source + message
		virtual std::string info(void) const;

		// Returns exception trace ie. the trail of handlers it passed through
		// for example: Source -> H1 -> H2 -> [..]
		// [..] indicates handler that called trace()
		virtual std::string trace(void) const;

		// Returns exception trace ie. the trail of handlers it passed through
		// along with message for each handler
		// for example: Source[msg1] -> H1[msg2] -> H2[msg3] -> [..]
		// [..] indicates handler that called info()
		virtual std::string traceWithMessages(void) const;

		// Adds handler and message associated with it to exception's trace
		// for example: MyException << utility::entry(MyHandler, MyMessage);
		virtual Exception& operator<<(infoEntry entry);

		// Appends info from another exception(by moving it, so other.exceptionInfo becomes invalidated)
		// to the current one for example: current info {(H3, M3)} other info: {(H1, M1), (H2, M2)}
		// after calling current.appendExceptionInfo(other) current info: {(H1, M1), (H2, M2), (H3, M3)} other info: {}
		virtual void appendExceptionInfo(Exception& other);

		// Rethrows the exception
		void rethrow(void) const;

	protected:
		std::list<infoEntry> exceptionInfo;

	private:
		void what( void ) = delete;
	};



	class RuntimeError : public Exception {
	public:
		RuntimeError(std::string src, std::string msg);
	};



	class LogicError : public Exception {
	public:
		LogicError(std::string src, std::string msg);
	};



	class InvalidArgument : public LogicError {
	public:
		InvalidArgument(std::string src, std::string msg);
	};

	// Returns exceptionInfo entry built out of handlerName and message
	// for example: entry("MyHandler", "MyMessage")
	std::pair<std::string, std::string> entry(std::string handlerName, std::string message);
} // namespace utility
