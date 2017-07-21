#pragma once
// use this header to include cpprest
// https://casablanca.codeplex.com/

#define _TURN_OFF_PLATFORM_STRING

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wshadow"
	#include <cpprest/http_listener.h>
#pragma GCC diagnostic pop
