#include "SybaseTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Environment.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Tuple.h"
#include "Poco/NamedTuple.h"
#include "Poco/Exception.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/Sybase/Connector.h"
#include "Poco/Data/Sybase/SybaseException.h"
#include "Poco/Nullable.h"
#include "Poco/Data/DataException.h"
#include <iostream>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using Poco::Data::Sybase::ConnectionException;
using Poco::Data::Sybase::StatementException;
using Poco::format;
using Poco::NotFoundException;
using Poco::Int32;
using Poco::Nullable;
using Poco::Tuple;
using Poco::NamedTuple;
using Poco::Environment;

Poco::SharedPtr<Poco::Data::Session> SybaseTest::_session = 0;
Poco::SharedPtr<SQLExecutor> SybaseTest::_executor = 0;
std::string SybaseTest::_connString;


SybaseTest::SybaseTest(const std::string& name): 
	CppUnit::TestCase(name)
{ }


SybaseTest::~SybaseTest()
{ }


void SybaseTest::testSimpleAccess()
{
	recreatePersonTable();
	_executor->simpleAccess();
}


void SybaseTest::dropTable(const std::string& tableName)
{
	try { *_session << format("if exists (select 1 from sysobjects where name='%s') drop table %s", tableName, tableName), now; }
	catch ( const ConnectionException& ce ) { std::cout << ce.displayText() << std::endl; fail("dropTable()"); }
	catch ( const StatementException& se ) { std::cout << se.displayText() << std::endl; fail("dropTable()"); }
}


void SybaseTest::recreatePersonTable()
{
	dropTable("person");
	try { *_session << "create table person (lastName varchar(30), firstName varchar(30), address varchar(30), age int)", now; }
	catch ( const ConnectionException& ce) { std::cout << ce.displayText() << std::endl; fail ("recreatePersonTable()"); }
	catch ( const StatementException& se) { std::cout << se.displayText() << std::endl; fail ("recreatePersonTable()"); }
}


void SybaseTest::setUp()
{ }


void SybaseTest::tearDown()
{
	dropTable("person");
}


CppUnit::Test* SybaseTest::suite()
{
	const char* const connStr = std::getenv("SYBASE_CONN");
	if ( connStr )
	{
		try
		{
			_session = new Session(Sybase::Connector::KEY, connStr);
		}
		catch ( const ConnectionFailedException& ex )
		{
			std::cout << ex.displayText() << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "You need to set the SYBASE_CONN environment variable\n";
		return 0;
	}
	std::cout << "*** Connected to [" << Sybase::Connector::KEY << "] test database." << std::endl;
	_executor = new SQLExecutor("Sybase SQL Executor", _session);
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("SybaseTest");
	CppUnit_addTest(suite, SybaseTest, testSimpleAccess);
	return suite;
}
