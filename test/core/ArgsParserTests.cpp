#include <gtest/gtest.h>
#include "core/ArgsParser.hpp"

void mockCallback(std::string) { }
bool mockFailingValidator(std::string) { return false; }

TEST(ArgsParserTestSuite, ParsingSuccessful) {
	core::ArgsParser argsParser;

	argsParser.addOption("tt", "test", 't')
		.setCallback(mockCallback);

	const char* argv[] = { "", "--tt=123" };
	const char* argv2[] = { "", "-t=123" };
	const char* argv3[] = { "", "-t" };
	const char* argv4[] = { "", "--tt" };
	ASSERT_NO_THROW(argsParser.parse(2, argv));
	ASSERT_NO_THROW(argsParser.parse(2, argv2));
	ASSERT_NO_THROW(argsParser.parse(2, argv3));
	ASSERT_NO_THROW(argsParser.parse(2, argv4));
}

TEST(ArgsParserTestSuite, NonexistentOption) {
	core::ArgsParser argsParser;

	argsParser.addOption("tt", "test", 't');

	const char* argv[] = { "", "--tu=123" };
	const char* argv2[] = { "", "-s=123" };

	std::streambuf* oldcerr = std::cerr.rdbuf(nullptr);
	std::streambuf* oldcout = std::cout.rdbuf(nullptr);
	ASSERT_THROW(argsParser.parse(2, argv), core::InvalidOptionException);
	ASSERT_THROW(argsParser.parse(2, argv2), core::InvalidOptionException);
	std::cerr.rdbuf(oldcerr);
	std::cout.rdbuf(oldcout);
}

TEST(ArgsParserTestSuite, InvalidCommandLineSyntax) {
	core::ArgsParser argsParser;

	argsParser.addOption("tt", "test", 't');

	const char* argv[] = { "", "--t=123" };
	const char* argv2[] = { "", "-ts=123" };
	const char* argv3[] = { "", "--tst=" };

	std::streambuf* oldcerr = std::cerr.rdbuf(nullptr);
	std::streambuf* oldcout = std::cout.rdbuf(nullptr);
	ASSERT_THROW(argsParser.parse(2, argv), core::InvalidOptionSyntaxException);
	ASSERT_THROW(argsParser.parse(2, argv2), core::InvalidOptionSyntaxException);
	ASSERT_THROW(argsParser.parse(2, argv3), core::InvalidOptionSyntaxException);
	std::cerr.rdbuf(oldcerr);
	std::cout.rdbuf(oldcout);
}

TEST(ArgsParserTestSuite, OptionValueValidationFailed) {
	core::ArgsParser argsParser;

	argsParser.addOption("tt", "test", 't')
		.setValidator(mockFailingValidator);

	const char* argv[] = { "", "--tt==123" };

	std::streambuf* oldcerr = std::cerr.rdbuf(nullptr);
	std::streambuf* oldcout = std::cout.rdbuf(nullptr);
	ASSERT_THROW(argsParser.parse(2, argv), core::ValidatorException);
	std::cerr.rdbuf(oldcerr);
	std::cout.rdbuf(oldcout);
}

TEST(ArgsParserTestSuite, RequiredOptionNotSupplied) {
	core::ArgsParser argsParser;

	argsParser.addOption("tt", "test", 't')
		.setRequired(true);

	argsParser.addOption("ts", "test", 'p')
		.setRequired(false);

	const char* argv[] = { "", "--ts=123" };

	std::streambuf* oldcerr = std::cerr.rdbuf(nullptr);
	std::streambuf* oldcout = std::cout.rdbuf(nullptr);
	ASSERT_THROW(argsParser.parse(2, argv), core::RequiredOptionOmittedException);
	std::cerr.rdbuf(oldcerr);
	std::cout.rdbuf(oldcout);
}

TEST(ArgsParserTestSuite, NameRedefinition) {
	core::ArgsParser argsParser;

	argsParser.addOption("tt", "test", 't');

	ASSERT_THROW(argsParser.addOption("tt", "test", 'p'), core::OptionRedefinitionException);
	ASSERT_THROW(argsParser.addOption("ts", "test", 't'), core::OptionRedefinitionException);
}
