#pragma once

#include <string>

namespace utility {
namespace validators {
bool fileExists( std::string arg );

bool isPositiveInt( std::string arg );

bool isBool( std::string arg );
} // namespace validators
} // namespace utility
