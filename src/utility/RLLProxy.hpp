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
	void setLibraryName(std::string name); //TODO: adnotacja ze to ma sens jedynie przed initem? moze wpis do logu w przypadku wywolania po inicie lub rzucenie logic_error?

private:
	void* libHandle;
	std::string libraryName;

	static const int DL_SUCCESS = 0;
};

} // namespace utility
