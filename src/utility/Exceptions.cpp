#include "Exceptions.hpp"

namespace utility {
	Exception::Exception(std::string src, std::string msg) {
		exceptionInfo.emplace_back( src, msg );
	}

	std::string Exception::source(void) const {
		return exceptionInfo.back().first;
	}

	std::string Exception::message(void) const {
		return exceptionInfo.back().second;
	}

	std::string Exception::info(void) const {
		return source() + ": " + message();
	}

	std::string Exception::trace(void) const {
		std::string result;
		for (auto entry : exceptionInfo)
			result += entry.first + " -> ";
		result += "[..]";
		return result;
	}

	std::string Exception::traceWithMessages(void) const {
		std::string result;
		for (auto entry : exceptionInfo)
			result += entry.first + '[' + entry.second + ']' + " -> ";
		result += "[..]";
		return result;
	}

	Exception& Exception::operator<<(infoEntry entry) {
		exceptionInfo.push_back(entry);
		return *this;
	}

	void Exception::rethrow(void) const {
		throw *this;
	}

	void Exception::appendExceptionInfo(Exception& other) {
		exceptionInfo.splice(exceptionInfo.begin(), std::move(other.exceptionInfo));
	}

	RuntimeError::RuntimeError(std::string src, std::string msg) :
		Exception(src, msg) {
	}

	LogicError::LogicError(std::string src, std::string msg) :
		Exception(src, msg) {
	}

	InvalidArgument::InvalidArgument(std::string src, std::string msg) :
		LogicError(src, msg) {

	}

	std::pair<std::string, std::string> entry(std::string handlerName, std::string message) {
		return std::make_pair(handlerName, message);
	}
} // namespace utility
