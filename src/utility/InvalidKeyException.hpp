#pragma once
#include "Exceptions.hpp"

namespace utility {
	class InvalidKeyException : public utility::RuntimeError {
	public:
		InvalidKeyException(std::string src, std::string msg) : RuntimeError(src, msg) { }
	};
} // namespace utility
