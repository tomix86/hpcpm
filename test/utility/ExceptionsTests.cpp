#include <gtest/gtest.h>
#include "utility/Exceptions.hpp"

TEST(UtilityTestSuite, Exceptions_ExceptionTest) {
	try {
		try {
			try {
				throw utility::Exception("h1", "m1");
			}
			catch (utility::Exception &ex) {
				ASSERT_STREQ("h1: m1", ex.info().c_str());
				ex << utility::entry("h2", "m2");
				ASSERT_STREQ("h2: m2", ex.info().c_str());
				ASSERT_STREQ("h1 -> h2 -> [..]", ex.trace().c_str());
				ASSERT_STREQ("h1[m1] -> h2[m2] -> [..]", ex.traceWithMessages().c_str());
				ex.rethrow();
			}
		}
		catch (utility::Exception &ex) {
			ASSERT_STREQ("h2: m2", ex.info().c_str());
			utility::Exception x("h3", "m3");
			ASSERT_STREQ("h3: m3", x.info().c_str());
			x.appendExceptionInfo(ex);

			ASSERT_STREQ("[..]", ex.traceWithMessages().c_str());

			ASSERT_STREQ("h1 -> h2 -> h3 -> [..]", x.trace().c_str());
			ASSERT_STREQ("h1[m1] -> h2[m2] -> h3[m3] -> [..]", x.traceWithMessages().c_str());

			throw x;
		}
	}
	catch (utility::Exception &ex) {
		ex << utility::entry("h4", "m4");

		ASSERT_STREQ("h1 -> h2 -> h3 -> h4 -> [..]", ex.trace().c_str());
		ASSERT_STREQ("h1[m1] -> h2[m2] -> h3[m3] -> h4[m4] -> [..]", ex.traceWithMessages().c_str());
	}
}

TEST(UtilityTestSuite, Exceptions_RuntimeErrorTest) {
	utility::RuntimeError x("h1", "m1");

	ASSERT_STREQ("h1 -> [..]", x.trace().c_str());
	ASSERT_STREQ("h1[m1] -> [..]", x.traceWithMessages().c_str());

	x << utility::entry("h2", "m2");

	ASSERT_STREQ("h1 -> h2 -> [..]", x.trace().c_str());
	ASSERT_STREQ("h1[m1] -> h2[m2] -> [..]", x.traceWithMessages().c_str());

	utility::RuntimeError x2("h3", "m3");
	x2.appendExceptionInfo(x);

	ASSERT_STREQ("h1 -> h2 -> h3 -> [..]", x2.trace().c_str());
	ASSERT_STREQ("h1[m1] -> h2[m2] -> h3[m3] -> [..]", x2.traceWithMessages().c_str());
}

TEST(UtilityTestSuite, Exceptions_LogicErrorTest) {
	utility::LogicError x("h1", "m1");

	ASSERT_STREQ("h1 -> [..]", x.trace().c_str());
	ASSERT_STREQ("h1[m1] -> [..]", x.traceWithMessages().c_str());

	x << utility::entry("h2", "m2");

	ASSERT_STREQ("h1 -> h2 -> [..]", x.trace().c_str());
	ASSERT_STREQ("h1[m1] -> h2[m2] -> [..]", x.traceWithMessages().c_str());

	utility::LogicError x2("h3", "m3");
	x2.appendExceptionInfo(x);

	ASSERT_STREQ("h1 -> h2 -> h3 -> [..]", x2.trace().c_str());
	ASSERT_STREQ("h1[m1] -> h2[m2] -> h3[m3] -> [..]", x2.traceWithMessages().c_str());
}
