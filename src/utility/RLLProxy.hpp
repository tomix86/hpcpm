#pragma once
#include <string>

namespace utility {

typedef void( *VoidFuncPtr )( void );

#define SYM_LOAD( x ) if( !loadSymbol( reinterpret_cast<utility::VoidFuncPtr&>( x ), #x ) ) { return false; }

class RLLProxy {
public:
	RLLProxy( std::string libraryName );
	virtual ~RLLProxy( void );

	virtual bool init( void );
	virtual bool shutdown( void );

protected:
	virtual bool loadSymbols( void ) = 0;
	bool loadSymbol( VoidFuncPtr& symbol, const char* name ) const;

	// must be called before init()
	void setLibraryName(std::string name);

private:
	void* libHandle;
	std::string libraryName;

	static const int DL_SUCCESS = 0;
};

} // namespace utility
