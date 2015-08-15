#pragma once
#include "utility/Exceptions.hpp"

namespace devices {
class DeviceNotFoundException : public utility::RuntimeError {
public:
	DeviceNotFoundException( std::string src, std::string msg ) : utility::RuntimeError( src, msg) { }
};
} // namespace devices
