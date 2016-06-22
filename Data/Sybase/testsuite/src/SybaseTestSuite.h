#ifndef SybaseTestSuite_INCLUDED
#define SybaseTestSuite_INCLUDED


#include "Poco/CppUnit/TestSuite.h"

class SybaseTestSuite {
public:
	static CppUnit::Test* suite();

private:
	static void addTest(CppUnit::TestSuite* suite, CppUnit::Test* test);
};


#endif // SybaseTestSuite_INCLUDED
