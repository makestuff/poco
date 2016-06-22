#include "SybaseTestSuite.h"
#include "SybaseTest.h"


CppUnit::Test* SybaseTestSuite::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("SybaseTestSuite");
	addTest(suite, SybaseTest::suite());
	return suite;
}


void SybaseTestSuite::addTest(CppUnit::TestSuite* suite, CppUnit::Test* test)
{
	if ( suite && test )
		suite->addTest(test);
}
