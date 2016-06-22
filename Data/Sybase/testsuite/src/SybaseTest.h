#ifndef SybaseTest_INCLUDED
#define SybaseTest_INCLUDED


#include "Poco/Data/Sybase/Sybase.h"
#include "Poco/Data/Session.h"
#include "Poco/SharedPtr.h"
#include "Poco/CppUnit/TestCase.h"
#include "SQLExecutor.h"


class SybaseTest: public CppUnit::TestCase {
public:
	explicit SybaseTest(const std::string& name);
	~SybaseTest();

	void testSimpleAccess();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void dropTable(const std::string& tableName);
	void recreatePersonTable();

	static std::string _connString;
	static Poco::SharedPtr<Poco::Data::Session> _session;
	static Poco::SharedPtr<SQLExecutor> _executor;
};


#endif // SybaseTest_INCLUDED
