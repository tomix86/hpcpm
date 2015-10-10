#pragma once

namespace utility {

typedef void( *VoidFuncPtr )( void );

#define SYM_LOAD( x ) if( !loadSymbol( reinterpret_cast<utility::VoidFuncPtr&>( x ), #x ) ) { return false; }

class RLLProxy {
public:
	RLLProxy( const char* libraryName );
	virtual ~RLLProxy( void );

	bool init( void );
	bool shutdown( void );

protected:
	virtual bool loadSymbols( void ) = 0;
	bool loadSymbol( VoidFuncPtr& symbol, const char* name ) const;

private:
	void* libHandle;
	const char* libraryName;

	static const int DL_SUCCESS = 0;
};

} // namespace utility
