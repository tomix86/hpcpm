#include <gtest/gtest.h>
#include "utility/ConfigLoader.hpp"

using utility::ConfigLoader;

class ConfigLoaderAccessor : public ConfigLoader {
public:
	static void setInitialized( bool val ) {
		ConfigLoader::configFileLoaded = val;
	}

	static void clearConfig( void ) {
		ConfigLoader::config.clear();
	}

	static std::map<std::string, std::string> getConfig( void ) {
		return config;
	}
};

class ConfigLoaderTestSuite : public ::testing::Test {
protected:
	static void SetUpTestCase() {
		oldCoutBuf = std::cout.rdbuf( nullptr );
	}

	static void TearDownTestCase() {
		std::cout.rdbuf( oldCoutBuf );
	}

	virtual void SetUp() {
		ConfigLoaderAccessor::setInitialized( false );
		ConfigLoaderAccessor::clearConfig();
		filename = std::tmpnam( nullptr );
		file.open( filename, std::ios_base::out | std::ios_base::trunc );
	}

	virtual void TearDown() {
		file.close();
		std::remove( filename.c_str() );
	}

	void setFileData( std::string data ) {
		file << data;
		file.close();
	}

	static std::ofstream file;
	static std::string filename;
	static std::streambuf* oldCoutBuf;
};
std::ofstream ConfigLoaderTestSuite::file;
std::string ConfigLoaderTestSuite::filename;
std::streambuf* ConfigLoaderTestSuite::oldCoutBuf;

TEST_F( ConfigLoaderTestSuite, SuccessfulInitializationTest ) {
	setFileData(	"India Pale Ale\n"
					"American Wheat\n"
					"Baltic Porter" );

	ASSERT_NO_THROW( ConfigLoader::initialize( filename ) );
}

TEST_F( ConfigLoaderTestSuite, NonexistentFileInitializationTest ) {
	ASSERT_THROW( ConfigLoader::initialize( "" ), utility::RuntimeError );
}

TEST_F( ConfigLoaderTestSuite, ReinitializationTest  ) {
	ConfigLoaderAccessor::setInitialized( true );
	ASSERT_THROW( ConfigLoader::initialize( filename ), utility::LogicError );
}

TEST_F( ConfigLoaderTestSuite, ParserTest ) {
	setFileData(	"India Pale Ale\n"
					"American Wheat\n"
					"Baltic Porter" );

	ASSERT_NO_THROW( ConfigLoader::initialize( filename ) );

	auto config = ConfigLoaderAccessor::getConfig();
	ASSERT_STREQ( "Pale Ale", config.at("India").c_str() );
	ASSERT_STREQ( "Wheat", config.at("American").c_str() );
	ASSERT_STREQ( "Porter", config.at("Baltic").c_str() );
}

TEST_F( ConfigLoaderTestSuite, GetParamTest ) {
	setFileData (	"p1 90\n"
					"p2 80\n"
					"p3 1.5\n"
					"p4 0.3\n"
					"p5 John Galt\n"
					"p6 true" );

	ConfigLoader::initialize( filename );

	ASSERT_EQ( 90, ConfigLoader::getIntParam( "p1" ) );
	ASSERT_EQ( 80, ConfigLoader::getUnsignedParam( "p2" ) );
	ASSERT_FLOAT_EQ( 1.5f, ConfigLoader::getFloatParam( "p3" ) );
	ASSERT_DOUBLE_EQ( .3, ConfigLoader::getDoubleParam( "p4" ) );
	ASSERT_STREQ( "John Galt", ConfigLoader::getStringParam( "p5" ).c_str() );
	ASSERT_EQ ( true, ConfigLoader::getBoolParam( "p6" ) );
}

TEST_F( ConfigLoaderTestSuite, GetNonexistentParamTest ) {
	setFileData( "NiceParam :)" );

	ConfigLoader::initialize( filename );

	ASSERT_THROW( ConfigLoader::getIntParam( "NaughtyParam" ), utility::InvalidKeyException );
}

TEST_F( ConfigLoaderTestSuite, GetParamConversionFailedTest ) {
	setFileData( "StringParam obviously" );

	ConfigLoader::initialize( filename );

	ASSERT_THROW( ConfigLoader::getIntParam( "StringParam" ), utility::InvalidKeyException );
	ASSERT_THROW( ConfigLoader::getUnsignedParam( "StringParam" ), utility::InvalidKeyException );
	ASSERT_THROW( ConfigLoader::getFloatParam( "StringParam" ), utility::InvalidKeyException );
	ASSERT_THROW( ConfigLoader::getDoubleParam( "StringParam" ), utility::InvalidKeyException );
	ASSERT_THROW( ConfigLoader::getBoolParam( "StringParam" ), utility::InvalidKeyException );
}

TEST_F( ConfigLoaderTestSuite, GetParamUnitializedTest  ) {
	ASSERT_THROW( ConfigLoader::getStringParam( "NaughtyParam" ), utility::LogicError );
}
