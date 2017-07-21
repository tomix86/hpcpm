#pragma once
// use this header to include easylogging++
// https://github.com/easylogging/easyloggingpp

//#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING
#define ELPP_THREAD_SAFE
#define ELPP_STACKTRACE_ON_CRASH
#define ELPP_LOGGING_FLAGS_FROM_ARG
#ifdef DEBUG
	#define ELPP_DEBUG_ERRORS
#endif

#pragma GCC diagnostic push
// currently disabling -Wundef does not work due to a bug in GCC, thus -Wundef must be excluded from GCC compilation flags
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53431
#pragma GCC diagnostic ignored "-Wundef"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Winline"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wswitch-default"
	#include <easylogging++.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <cstring>
#include <iterator>
#include <sstream>
#include <vector>
#include "utility/ConfigLoader.hpp"

namespace utility {
namespace logging {

void setDefaultConfiguration( void );
void loadConfiguration( void );

namespace internal {

class CommandLineParser {
public:
	CommandLineParser( std::string commandLine );

	~CommandLineParser( void );

	char** argv( void ) { return commands.data(); }

	int argc( void ) const { return commands.size(); }

private:
	std::vector<char*> commands;
};
}
// namespace internal

} // namespace logging
} // namespace utility
