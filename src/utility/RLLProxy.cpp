#include <dlfcn.h>
#include "RLLProxy.hpp"
#include "utility/Logging.hpp"

namespace utility {

//Redeclaration of dlsym function as a workaround for GCC warning regarding casting from pointer-to-object to pointer-to-function
extern "C" VoidFuncPtr dlsym( void *handle, const char *symbol );
static_assert( sizeof( void* ) == sizeof( VoidFuncPtr ),
	"Sizes of pointer-to-function and pointer-to-object doesn't match, proxy code will fail due to casts between those pointer being performed" );

RLLProxy::RLLProxy( const char* libraryName ) :
libHandle{ nullptr },
libraryName{ libraryName }{
}

RLLProxy::~RLLProxy( void ) {
	if ( libHandle != nullptr ) {
		shutdown();
	}

}

bool RLLProxy::init( void ) {
	LOG( INFO ) << "Loading " << libraryName << "...";

	libHandle = dlopen( libraryName, RTLD_LAZY );
	if ( libHandle == nullptr ) {
		LOG( ERROR ) << "Failure: " << dlerror();
		return false;
	}

	LOG( INFO ) << "Success, loading symbols...";
	if( !loadSymbols() ) {
		LOG( ERROR ) << "Symbols loading failed.";
		return false;
	}

	LOG( INFO ) << "Symbols loaded successfully";
	return true;
}

bool RLLProxy::shutdown( void ) {
	if( dlclose( libHandle ) != DL_SUCCESS ) {
		LOG( ERROR ) << "Error while closing " << libraryName << ": " << dlerror();
		return false;
	}
	return true;
}

bool RLLProxy::loadSymbol( VoidFuncPtr& symbol, const char* name ) const {
	symbol = reinterpret_cast<VoidFuncPtr>( dlsym( libHandle, name ) );
	if( symbol != nullptr ) {
		LOG( TRACE ) << "Loading of symbol \"" << name << "\" succeeded";
		return true;
	}
	else {
		LOG( ERROR ) << "Loading of symbol \"" << name << "\" failed: " << dlerror();
		return false;
	}
}

} // namespace utility
