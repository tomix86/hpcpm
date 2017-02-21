#include <gtest/gtest.h>

class TestEnvironment : public ::testing::Environment {
public:
	virtual ~TestEnvironment() {}
	// Override this to define how to set up the environment.
	virtual void SetUp();
	// Override this to define how to tear down the environment.
	virtual void TearDown();
};

void TestEnvironment::SetUp() {
}

void TestEnvironment::TearDown() {
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new TestEnvironment());
	return RUN_ALL_TESTS();
}
