#pragma once

namespace utility {

enum ApplicationExitCode {
	EXIT_SUCCESSFUL = 0,
	EXIT_UNKNOWN_EXCEPTION = 1,
	EXIT_UNHANDLED_EXCEPTION = 2,
	EXIT_ERROR_WHILE_PARSING_COMMAND_LINE = 3
};

} // namespace utility
